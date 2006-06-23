/* olpc - cairo based gtk+ theme
 *
 * Copyright © 2005 Red Hat, Inc.
 * Based on CGE by Julien Boulnois.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "olpc-style.h"
#include "olpc-rc-style.h"

#include <gtk/gtk.h>
#include <string.h>

static void	olpc_rc_style_init			(OlpcRcStyle *style);
static void	olpc_rc_style_class_init	(OlpcRcStyleClass *klass);
static void	olpc_rc_style_finalize		(GObject *object);

enum
{
    TOKEN_TOP_COLOR = G_TOKEN_LAST + 1,
    TOKEN_BOTTOM_COLOR,
    TOKEN_ACTIVE,
    TOKEN_PRELIGHT,
    TOKEN_SELECTED,
    TOKEN_INSENSITIVE,
    TOKEN_NORMAL
};

static struct
{
    gchar *name;
    guint token;
}
theme_symbols[] =
{
    { "top-color",	TOKEN_TOP_COLOR },
    { "bottom-color",	TOKEN_BOTTOM_COLOR },
    { "NORMAL",		TOKEN_NORMAL },
    { "ACTIVE",		TOKEN_ACTIVE },
    { "PRELIGHT",	TOKEN_PRELIGHT },
    { "SELECTED",	TOKEN_SELECTED },
    { "INSENSITIVE",	TOKEN_INSENSITIVE }
};

static GtkRcStyleClass *parent_class;

GType olpc_type_rc_style = 0;

void
olpc_rc_style_register_type (GTypeModule *module)
{
    static const GTypeInfo object_info = {
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

static GtkStyle *
olpc_rc_style_create_style (GtkRcStyle *style)
{
    return g_object_new (OLPC_TYPE_STYLE, NULL);
}

static guint
parse_state (GScanner     *scanner,
             GtkStateType *state)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_BRACE)
    return G_TOKEN_LEFT_BRACE;

  token = g_scanner_get_next_token (scanner);
  switch (token)
    {
    case TOKEN_ACTIVE:
      *state = GTK_STATE_ACTIVE;
      break;
    case TOKEN_INSENSITIVE:
      *state = GTK_STATE_INSENSITIVE;
      break;
    case TOKEN_NORMAL:
      *state = GTK_STATE_NORMAL;
      break;
    case TOKEN_PRELIGHT:
      *state = GTK_STATE_PRELIGHT;
      break;
    case TOKEN_SELECTED:
      *state = GTK_STATE_SELECTED;
      break;
    default:
      *state = TOKEN_NORMAL;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_RIGHT_BRACE)
    return G_TOKEN_RIGHT_BRACE;

  return G_TOKEN_NONE;
}

static GTokenType
olpc_rc_style_parse_top_color (OlpcRcStyle	*rc_style,
				               GtkSettings	*settings,
				               GScanner		*scanner)
{
    GTokenType token;
    GtkStateType state;

    token = g_scanner_get_next_token (scanner);
    if (token != TOKEN_TOP_COLOR)
	return TOKEN_TOP_COLOR;

    token = parse_state(scanner, &state);
    if (token != G_TOKEN_NONE)
        return token;

    token = g_scanner_get_next_token (scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
	return G_TOKEN_EQUAL_SIGN;

    token = gtk_rc_parse_color (scanner,
				&(rc_style->top_color[state]));
    if (token != G_TOKEN_NONE)
	return token;

    rc_style->top_color[state].pixel = TRUE;

    return G_TOKEN_NONE;
}

static GTokenType
olpc_rc_style_parse_bottom_color (OlpcRcStyle	*rc_style,
				  GtkSettings	*settings,
				  GScanner	*scanner)
{
    GTokenType token;
    GtkStateType state;

    token = g_scanner_get_next_token (scanner);
    if (token != TOKEN_BOTTOM_COLOR)
	return TOKEN_BOTTOM_COLOR;

    token = parse_state(scanner, &state);
    if (token != G_TOKEN_NONE)
        return token;

    token = g_scanner_get_next_token (scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
	return G_TOKEN_EQUAL_SIGN;

    token = gtk_rc_parse_color (scanner, &(rc_style->bottom_color[state]));
    if (token != G_TOKEN_NONE)
	return token;

    rc_style->bottom_color[state].pixel = TRUE;

    return G_TOKEN_NONE;
}

static GTokenType
olpc_rc_style_parse (GtkRcStyle  *rc_style,
			 		 GtkSettings *settings,
			 		 GScanner    *scanner)
{
  static GQuark scope_id = 0;
  OlpcRcStyle *olpc_style = OLPC_RC_STYLE (rc_style);

  guint old_scope;
  GTokenType token, expected;
  gint i;

  /* Set up a new scope in this scanner. */
  if (!scope_id)
      scope_id = g_quark_from_string ("olpc_theme_engine");

  old_scope = g_scanner_set_scope (scanner, scope_id);

  /* Add symbols for this scope, (if not present already) */
  if (!g_scanner_lookup_symbol(scanner, theme_symbols[0].name)) {
      for (i = 0; i < G_N_ELEMENTS (theme_symbols); i++)
	  g_scanner_scope_add_symbol(scanner, scope_id,
				     theme_symbols[i].name,
				     GINT_TO_POINTER(theme_symbols[i].token));
  }

  /* Begin parsing top-level. Apparently a left curly brace has
   * already been consumed for use, but we're expected to consume the
   * final right closing brace. Weird. */
  while (1) {
      token = g_scanner_peek_next_token(scanner);
      if (token == G_TOKEN_RIGHT_CURLY) {
	  g_scanner_get_next_token (scanner);
	  expected = G_TOKEN_NONE;
	  break;
      }

      switch (token)
      {
      case TOKEN_TOP_COLOR:
	  expected = olpc_rc_style_parse_top_color (olpc_style, settings, scanner);
	  break;
      case TOKEN_BOTTOM_COLOR:
	  expected = olpc_rc_style_parse_bottom_color (olpc_style, settings, scanner);
	  break;
      default:
	  g_scanner_get_next_token (scanner);
	  expected = G_TOKEN_RIGHT_CURLY;
	  break;
      }

      if (expected != G_TOKEN_NONE)
	  break;
  }

  /* On success, restore the previous scope. (For errors, we need the
   * current scope to format a proper error message.) */
  if (expected == G_TOKEN_NONE)
      g_scanner_set_scope(scanner, old_scope);

  return expected;
}

static void
olpc_rc_style_merge (GtkRcStyle *dest, GtkRcStyle *src)
{
    if (OLPC_IS_RC_STYLE (src)) {
        int i;
	OlpcRcStyle *olpc_dest = OLPC_RC_STYLE (dest);
	OlpcRcStyle *olpc_src = OLPC_RC_STYLE (src);

        for (i = 0; i < 5; i++) {
	    if (olpc_src->top_color[i].pixel) {
	        olpc_dest->top_color[i] = olpc_src->top_color[i];
            }
	    if (olpc_src->bottom_color[i].pixel) {
	        olpc_dest->bottom_color[i] = olpc_src->bottom_color[i];
            }
        }
    }

    parent_class->merge (dest, src);
}

static void
olpc_rc_style_init (OlpcRcStyle *rc_style)
{
}

static void
olpc_rc_style_class_init (OlpcRcStyleClass *klass)
{
    GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    parent_class = g_type_class_peek_parent (klass);

    rc_style_class->parse = olpc_rc_style_parse;
    rc_style_class->merge = olpc_rc_style_merge;
    rc_style_class->create_style = olpc_rc_style_create_style;

    object_class->finalize = olpc_rc_style_finalize;
}

static void
olpc_rc_style_finalize (GObject *object)
{
    /* OlpcRcStyle *rc_style = OLPC_RC_STYLE (object);
       We would free anything necessary in rc_style here. */
 
    G_OBJECT_CLASS (parent_class)->finalize (object);
}


