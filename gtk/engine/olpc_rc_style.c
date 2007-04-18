#include "olpc_rc_style.h"
#include "olpc_style.h"

static void      olpc_rc_style_init         (OlpcRcStyle      *style);
static void      olpc_rc_style_class_init   (OlpcRcStyleClass *klass);
static guint     olpc_rc_style_parse        (GtkRcStyle          *rc_style,
					       GtkSettings          *settings,
					       GScanner             *scanner);
static void      olpc_rc_style_merge       (GtkRcStyle           *dest,
					       GtkRcStyle           *src);

static GtkStyle *olpc_rc_style_create_style (GtkRcStyle          *rc_style);


static struct
  {
    gchar       *name;
    guint        token;
  }
olpc_rc_symbols[] =
{
  { "rect_scrollbar",      TOKEN_RECTSCROLLBAR },
  { "scrollbar_marks",     TOKEN_SCROLLBARMARKS },
  { "scroll_button_marks", TOKEN_SCROLLBUTTONMARKS },
  { "handlebox_marks",     TOKEN_HANDLEBOXMARKS },
  { "mark_type1",          TOKEN_MARKTYPE1 },
  { "mark_type2",          TOKEN_MARKTYPE2 },
  { "paned_dots",          TOKEN_PANEDDOTS },

  { "TRUE",                TOKEN_TRUE },
  { "FALSE",               TOKEN_FALSE },

  { "NOTHING",             TOKEN_NOTHING },
  { "SLASH",               TOKEN_SLASH },
  { "INVSLASH",            TOKEN_INVSLASH },
  { "DOT",                 TOKEN_DOT },
  { "INVDOT",              TOKEN_INVDOT },
  { "ARROW",               TOKEN_ARROW },

  { "FULL",                TOKEN_FULL },
  { "SOME",                TOKEN_SOME },
  { "NONE",                TOKEN_NONE },

};

static guint n_olpc_rc_symbols = sizeof(olpc_rc_symbols) / sizeof(olpc_rc_symbols[0]);

static GtkRcStyleClass *olpc_parent_rc_style_class;

GType olpc_type_rc_style = 0;

void
olpc_rc_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (OlpcRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) olpc_rc_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (OlpcRcStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) olpc_rc_style_init,
  };
  
  olpc_type_rc_style = g_type_module_register_type (module,
						      GTK_TYPE_RC_STYLE,
						      "OlpcRcStyle",
						      &object_info, 0);
}

static void
olpc_rc_style_init (OlpcRcStyle *style)
{
}

static void
olpc_rc_style_class_init (OlpcRcStyleClass *klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);

  olpc_parent_rc_style_class = g_type_class_peek_parent (klass);

  rc_style_class->parse = olpc_rc_style_parse;
  rc_style_class->merge = olpc_rc_style_merge;
  rc_style_class->create_style = olpc_rc_style_create_style;
}

#if 0
static guint
olpc_rc_parse_int(GScanner *scanner,
                GTokenType wanted_token,
                guint *retval)
{
  guint token;
  
  token = g_scanner_get_next_token(scanner);
  if (token != wanted_token)
    return wanted_token;

  token = g_scanner_get_next_token(scanner);
  if (token != G_TOKEN_EQUAL_SIGN)
    return wanted_token;
  
  token = g_scanner_get_next_token(scanner);
  if (token != G_TOKEN_INT)
    return G_TOKEN_INT;
  
  *retval = g_scanner_cur_value(scanner).v_int;

  return G_TOKEN_NONE;
}
#endif

static guint
olpc_rc_parse_boolean(GScanner *scanner,
                    GTokenType wanted_token,
                    guint *retval)
{
  guint token;
  
  token = g_scanner_get_next_token(scanner);
  if (token != wanted_token)
    return wanted_token;
  
  token = g_scanner_get_next_token(scanner);
  if (token != G_TOKEN_EQUAL_SIGN)
    return G_TOKEN_EQUAL_SIGN;
  
  token = g_scanner_get_next_token(scanner);
  if (token == TOKEN_TRUE)
    *retval = TRUE;
  else if (token == TOKEN_FALSE)
    *retval = FALSE;
  else
    return TOKEN_TRUE;

  return G_TOKEN_NONE;
}

static guint
olpc_rc_parse_marktype(GScanner *scanner,
		     GTokenType wanted_token,
		     guint *retval)
{
  guint token;

  token = g_scanner_get_next_token(scanner);
  if (token != wanted_token)
    return wanted_token;

  token = g_scanner_get_next_token(scanner);
  if (token != G_TOKEN_EQUAL_SIGN)
    return G_TOKEN_EQUAL_SIGN;
  
  token = g_scanner_get_next_token(scanner);
  switch (token)
    {
    case TOKEN_NOTHING:
      *retval = MARKS_NOTHING;
      break;
    case TOKEN_SLASH:
      *retval = MARKS_SLASH;
      break;
    case TOKEN_INVSLASH:
      *retval = MARKS_INVSLASH;
      break;
    case TOKEN_DOT:
      *retval = MARKS_DOT;
      break;
    case TOKEN_INVDOT:
      *retval = MARKS_INVDOT;
      break;
    case TOKEN_ARROW:
      *retval = MARKS_ARROW;
      break;
    default:
      return TOKEN_NOTHING;
    }

  return G_TOKEN_NONE;
}

static guint
olpc_rc_parse_paned(GScanner *scanner,
		  GTokenType wanted_token,
		  guint *retval)
{
  guint token;

  token = g_scanner_get_next_token(scanner);
  if (token != wanted_token)
    return wanted_token;

  token = g_scanner_get_next_token(scanner);
  if (token != G_TOKEN_EQUAL_SIGN)
    return G_TOKEN_EQUAL_SIGN;
  
  token = g_scanner_get_next_token(scanner);
  switch (token)
    {
    case TOKEN_NONE:
      *retval = PANED_DOTSNONE;
      break;
    case TOKEN_SOME:
      *retval = PANED_DOTSSOME;
      break;
    case TOKEN_FULL:
      *retval = PANED_DOTSFULL;
      break;
    default:
      return TOKEN_NOTHING;
    }

  return G_TOKEN_NONE;
}

static guint
olpc_rc_style_parse (GtkRcStyle *rc_style,
			GtkSettings  *settings,
			GScanner   *scanner)
{
  static GQuark       scope_id = 0;
  OlpcRcStyle     *theme_data = OLPC_RC_STYLE (rc_style);
  guint               old_scope;
  guint               token;
  guint               i;

  /* Set up a new scope in this scanner. */

  /*
  g_print("olpc_rc_parse_rc_style(\"%s\")\n", rc_style->name);
  */
  if (!scope_id)
    scope_id = g_quark_from_string("theme_engine");

  /* If we bail out due to errors, we *don't* reset the scope, so the
   * error messaging code can make sense of our tokens.
   */
  old_scope = g_scanner_set_scope(scanner, scope_id);
  
  /* Now check if we already added our symbols to this scope
   * (in some previous call to olpc_rc_parse_rc_style for the
   * same scanner.
   */

  if (!g_scanner_lookup_symbol(scanner, olpc_rc_symbols[0].name))
    {
      for (i = 0; i < n_olpc_rc_symbols; i++)
        {
          g_scanner_scope_add_symbol(scanner, scope_id,
              olpc_rc_symbols[i].name,
              GINT_TO_POINTER(olpc_rc_symbols[i].token));
        }
    }

  /* We're ready to go, now parse the top level */

  /* theme_data = g_new0(OlpcRcStyle, 1); */
  theme_data->scrollbar_type = DEFAULT_SCROLLSHAPE;
  theme_data->scrollbar_marks = DEFAULT_SCROLLBARMARKS;
  theme_data->scroll_button_marks = DEFAULT_SCROLLBUTTONMARKS;
  theme_data->handlebox_marks = DEFAULT_HANDLEBOXMARKS;
  theme_data->mark_type1 = DEFAULT_MARKTYPE1;
  theme_data->mark_type2 = DEFAULT_MARKTYPE2;

  token = g_scanner_peek_next_token(scanner);
  while (token != G_TOKEN_RIGHT_CURLY)
    {
      switch (token)
	{
        case TOKEN_RECTSCROLLBAR:
          token = olpc_rc_parse_boolean(scanner, TOKEN_RECTSCROLLBAR, &i);
          if (token != G_TOKEN_NONE)
            break;
          if (i == FALSE)
            theme_data->scrollbar_type = SCROLL_SHAPED;
          else
            theme_data->scrollbar_type = SCROLL_RECT;
          break;

        case TOKEN_SCROLLBUTTONMARKS:
          token = olpc_rc_parse_boolean(scanner, TOKEN_SCROLLBUTTONMARKS, &i);
          if (token != G_TOKEN_NONE)
            break;
          if (i == TRUE)
            theme_data->mark_type2 = MARKS_SLASH;
          else
            theme_data->mark_type2 = MARKS_NOTHING;
          /*
          if (i == TRUE)
            theme_data->scroll_button_marks = MARKS_ON;
          else
            theme_data->scroll_button_marks = MARKS_OFF;
            */
          break;

        case TOKEN_SCROLLBARMARKS:
          token = olpc_rc_parse_boolean(scanner, TOKEN_SCROLLBARMARKS, &i);
          if (token != G_TOKEN_NONE)
            break;
          if (i == TRUE)
            theme_data->mark_type1 = MARKS_SLASH;
          else
            theme_data->mark_type1 = MARKS_NOTHING;
          /*
          if (i == TRUE)
            theme_data->scrollbar_marks = MARKS_ON;
          else
            theme_data->scrollbar_marks = MARKS_OFF;
            */
          break;

        case TOKEN_HANDLEBOXMARKS:
          token = olpc_rc_parse_boolean(scanner, TOKEN_HANDLEBOXMARKS, &i);
          if (token != G_TOKEN_NONE)
            break;
          if (i == TRUE)
            theme_data->handlebox_marks = MARKS_ON;
          else
            theme_data->handlebox_marks = MARKS_OFF;
          break;

        case TOKEN_MARKTYPE1:
          token = olpc_rc_parse_marktype(scanner, TOKEN_MARKTYPE1, &i);
          if (token != G_TOKEN_NONE)
            break;
          theme_data->mark_type1 = i;
          break;

        case TOKEN_MARKTYPE2:
          token = olpc_rc_parse_marktype(scanner, TOKEN_MARKTYPE2, &i);
          if (token != G_TOKEN_NONE)
            break;
          theme_data->mark_type2 = i;
          break;

        case TOKEN_PANEDDOTS:
          token = olpc_rc_parse_paned(scanner, TOKEN_PANEDDOTS, &i);
          if (token != G_TOKEN_NONE)
            break;
          theme_data->paned_dots = i;
          break;

	default:
	  g_scanner_get_next_token(scanner);
	  token = G_TOKEN_RIGHT_CURLY;
	  break;
	}

      if (token != G_TOKEN_NONE)
	{
//	  g_free(theme_data);
	  return token;
	}
      token = g_scanner_peek_next_token(scanner);
    }

  g_scanner_get_next_token(scanner);

  g_scanner_set_scope(scanner, old_scope);

  return G_TOKEN_NONE;
}

static void
olpc_rc_style_merge (GtkRcStyle * dest,
			GtkRcStyle * src)
{
  if (OLPC_IS_RC_STYLE (src)) {
    OlpcRcStyle        *src_data = OLPC_RC_STYLE (src);
    OlpcRcStyle        *dest_data = OLPC_RC_STYLE (dest);

    /*
      g_print("theme_merge_rc_style(\"%s\", \"%s\")\n", dest->name, src->name);
    */
    
    dest_data->scrollbar_type = src_data->scrollbar_type;
    dest_data->scrollbar_marks = src_data->scrollbar_marks;
    dest_data->scroll_button_marks = src_data->scroll_button_marks;
    dest_data->handlebox_marks = src_data->handlebox_marks;
    dest_data->mark_type1 = src_data->mark_type1;
    dest_data->mark_type2 = src_data->mark_type2;
    dest_data->paned_dots = src_data->paned_dots;
  }
  
  olpc_parent_rc_style_class->merge (dest, src);
}

/* Create an empty style suitable to this RC style
 */
static GtkStyle *
olpc_rc_style_create_style (GtkRcStyle *rc_style)
{
  return GTK_STYLE (g_object_new (OLPC_TYPE_STYLE, NULL));
}
