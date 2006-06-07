/*
 * Copyright © 2003 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Red Hat not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Red Hat makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL RED HAT
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Owen Taylor, Red Hat, Inc.
 */
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "cursortheme.h"

typedef struct _ParseData ParseData;

typedef enum {
  OUTSIDE,
  IN_THEME,
  IN_SOURCE,
  IN_IMAGE,
  IN_ALIAS,
  IN_LAYOUT,
  IN_ROW,
  IN_CURSOR,
  IN_FRAME
} ParseState;

struct _ParseData {
  CursorTheme *theme;
  ParseState state;
  gboolean seen_theme;
  gboolean seen_layout;
  int row;
  int column;
  Cursor *current_cursor;
};

static gboolean
expect_tag (GMarkupParseContext *context,
	    const gchar *element_name,
	    const gchar *expected_name,	/* Null for expected empty */
	    GError             **error)
{
  if (!expected_name || strcmp (element_name, expected_name) != 0)
    {
      g_set_error (error,
		   G_MARKUP_ERROR,
		   G_MARKUP_ERROR_INVALID_CONTENT,
		   "Unexpected tag '%s'",
		   element_name);
      return FALSE;
    }
  return TRUE;
}

static gboolean
extract_attrs (GMarkupParseContext *context,
	       const gchar        **attribute_names,
	       const gchar        **attribute_values,
	       GError             **error,
	       ...)
{
  va_list vap;
  const char *name;
  gboolean *attr_map;
  gboolean nattrs = 0;
  int i;

  for (i = 0; attribute_names[i]; i++)
    nattrs++;

  attr_map = g_new0 (gboolean, nattrs);

  va_start (vap, error);
  name = va_arg (vap, const char *);
  while (name)
    {
      gboolean mandatory = va_arg (vap, gboolean);
      const char **loc = va_arg (vap, const char **);
      gboolean found = FALSE;

      for (i = 0; attribute_names[i]; i++)
	{
	  if (!attr_map[i] && strcmp (attribute_names[i], name) == 0)
	    {
	      if (found)
		{
		  g_set_error (error,
			       G_MARKUP_ERROR,
			       G_MARKUP_ERROR_INVALID_CONTENT,
			       "Duplicate attribute '%s'", name);
		  return FALSE;
		}
	  
	      *loc = attribute_values[i];
	      found = TRUE;
	      attr_map[i] = TRUE;
	    }
	}
      
      if (!found && mandatory)
	{
	  g_set_error (error,
		       G_MARKUP_ERROR,
		       G_MARKUP_ERROR_INVALID_CONTENT,
		       "Missing attribute '%s'", name);
	  return FALSE;
	}
      
      name = va_arg (vap, const char *);
    }

  for (i = 0; i < nattrs; i++)
    if (!attr_map[i])
      {
	g_set_error (error,
		     G_MARKUP_ERROR,
		     G_MARKUP_ERROR_UNKNOWN_ATTRIBUTE,
		     "Unknown attribute '%s'", attribute_names[i]);
	      return FALSE;
      }

  return TRUE;
}

static gboolean
get_int (const char *str,
	 int        *result)
{
  long val;
  char *p;

  val = strtol (str, &p, 0);
  if (*str == '\0' || *p != '\0' ||
      val < G_MININT || val > G_MAXINT)
    return FALSE;

  *result = val;

  return TRUE;
}

static void
add_source (ParseData *parse_data,
	    int        size,
	    int        gridsize)
{
  CursorSource *source = g_new0 (CursorSource, 1);
  source->size = size;
  source->gridsize = gridsize;
  parse_data->theme->sources = g_slist_prepend (parse_data->theme->sources, source);
}

static void
add_alias (ParseData  *parse_data,
	   const char *name,
	   const char *target)
{
  CursorAlias *alias = g_new0 (CursorAlias, 1);
  alias->name = g_strdup (name);
  alias->target = g_strdup (target);

  g_hash_table_insert (parse_data->theme->aliases, alias->name, alias);
}

static gboolean
add_image (ParseData  *parse_data,
	   const char *file,
	   int         use,
	   GError    **error)
{
  GdkPixbuf *pixbuf;
  CursorSource *source;
  CursorImage *image;

  pixbuf = gdk_pixbuf_new_from_file (file, error);
  if (!pixbuf)
    return FALSE;

  source = parse_data->theme->sources->data;
  image = g_new0 (CursorImage, 1);
  image->image = pixbuf;
  image->use = use;
  source->images = g_slist_prepend (source->images, image);

  return TRUE;
}

static void
add_cursor (ParseData  *parse_data,
	    const char *name)
{
  Cursor *cursor = g_new0 (Cursor, 1);
  cursor->name = g_strdup (name);
  cursor->row = parse_data->row;
  cursor->column = parse_data->column;
  parse_data->current_cursor = cursor;
  g_hash_table_insert (parse_data->theme->cursors, cursor->name, cursor);
}

static void
add_frame_config (ParseData  *parse_data,
		  int         delay)
{
  Cursor *cursor = parse_data->current_cursor;
  CursorFrameConfig *frame_config = g_new0 (CursorFrameConfig, 1);
  frame_config->delay = delay;
  cursor->frame_configs = g_slist_prepend (cursor->frame_configs, frame_config);
}

/* Called for open tags <foo bar="baz"> */
static void
cursor_theme_start_element (GMarkupParseContext *context,
			    const gchar         *element_name,
			    const gchar        **attribute_names,
			    const gchar        **attribute_values,
			    gpointer             user_data,
			    GError             **error)
{
  ParseData *parse_data = user_data;

  switch (parse_data->state)
    {
    case OUTSIDE:
      {
	const char *name;
	
	if (!expect_tag (context, element_name, "theme", error))
	  return;
	if (parse_data->seen_theme)
	  {
	    g_set_error (error,
			 G_MARKUP_ERROR,
			 G_MARKUP_ERROR_INVALID_CONTENT,
			 "Multiple occurrences of <theme>");
	    return;
	  }
	parse_data->seen_theme = TRUE;
	parse_data->state = IN_THEME;

	if (!extract_attrs (context, attribute_names, attribute_values, error,
			    "name", TRUE, &name,
			    NULL))
	  return;

	parse_data->theme->name = g_strdup (name);
      }
      break;
    case IN_THEME:
      if (strcmp (element_name, "source") == 0)
	{
	  const char *size_str;
	  const char *gridsize_str;
	  int size, gridsize;
	  
	  parse_data->state = IN_SOURCE;
	  
	  if (!extract_attrs (context, attribute_names, attribute_values, error,
			      "size", TRUE, &size_str,
			      "gridsize", FALSE, &gridsize_str,
			      NULL))
	    return;
	  
	  if (!get_int (size_str, &size) || size < 0)
	    {
	      g_set_error (error,
			   G_MARKUP_ERROR,
			   G_MARKUP_ERROR_INVALID_CONTENT,
			   "Invalid size %s", size_str);
	      return;
	    }
	  
	  if (gridsize_str)
	    {
	      if (!get_int (gridsize_str, &gridsize) || size < 0)
		{
		  g_set_error (error,
			       G_MARKUP_ERROR,
			       G_MARKUP_ERROR_INVALID_CONTENT,
			       "Invalid size %s", size_str);
		  return;
		}
	    }
	  else
	    gridsize = size;
	  
	  add_source (parse_data, size, gridsize);
	}
      else if (strcmp (element_name, "alias") == 0)
	{
	  const char *name;
	  const char *target;

	  parse_data->state = IN_ALIAS;

	  if (!extract_attrs (context, attribute_names, attribute_values, error,
			      "name", TRUE, &name,
			      "target", FALSE, &target,
			      NULL))
	    return;

	  add_alias (parse_data, name, target);
	}
      else if (strcmp (element_name, "layout") == 0)
	{
	  if (parse_data->seen_layout)
	    {
	      g_set_error (error,
			   G_MARKUP_ERROR,
			   G_MARKUP_ERROR_INVALID_CONTENT,
			   "Multiple occurrences of <layout>");
	      return;
	    }
	  parse_data->seen_layout = TRUE;
	  parse_data->state = IN_LAYOUT;
	  
	  if (!extract_attrs (context, attribute_names, attribute_values, error,
			      NULL))
	    return;
	}
      else
	expect_tag (context, element_name, NULL, error);
      break;
    case IN_SOURCE:
      {
	const char *file;
	const char *use_str;
	int use;
	
	if (!expect_tag (context, element_name, "image", error))
	  return;
	parse_data->state = IN_IMAGE;
	
	if (!extract_attrs (context, attribute_names, attribute_values, error,
			    "file", TRUE, &file,
			    "use", TRUE, &use_str,
			    NULL))
	  return;

	if (strcmp (use_str, "hotspot") == 0)
	  use = -1;
	else
	  {
	    if (!get_int (use_str, &use) || use < 0)
	      {
		g_set_error (error,
			     G_MARKUP_ERROR,
			     G_MARKUP_ERROR_INVALID_CONTENT,
			     "Invalid use value %s", use_str);
		return;
	      }
	  }

	add_image (parse_data, file, use, error);
      }

      break;
    case IN_LAYOUT:
      if (!expect_tag (context, element_name, "row", error))
	return;
      parse_data->state = IN_ROW;
      parse_data->column = 0;
      break;
    case IN_ROW:
      {
	const char *name;
	
	if (!expect_tag (context, element_name, "cursor", error))
	  return;
	parse_data->state = IN_CURSOR;
	
	if (!extract_attrs (context, attribute_names, attribute_values, error,
			    "name", TRUE, &name,
			    NULL))
	  return;

	add_cursor (parse_data, name);
      }
      break;
    case IN_CURSOR:
      {
	const char *delay_str = NULL;
	int delay = -1;
      
	if (!expect_tag (context, element_name, "frame", error))
	  return;
	parse_data->state = IN_FRAME;
	
	if (!extract_attrs (context, attribute_names, attribute_values, error,
			    "delay", FALSE, &delay_str,
			    NULL))
	  return;

	if (delay_str)
	  {
	    if (!get_int (delay_str, &delay) || delay < 0)
	      {
		g_set_error (error,
			     G_MARKUP_ERROR,
			     G_MARKUP_ERROR_INVALID_CONTENT,
			     "Invalid delay value '%s'", delay_str);
		return;
	      }
	  }

	add_frame_config (parse_data, delay);
      }
      break;
    case IN_ALIAS:
    case IN_IMAGE:
    case IN_FRAME:
      expect_tag (context, element_name, NULL, error);
      break;
    }
}
  
/* Called for close tags </foo> */
static void
cursor_theme_end_element (GMarkupParseContext *context,
                          const gchar         *element_name,
                          gpointer             user_data,
                          GError             **error)
{
  ParseData *parse_data = user_data;
  
  switch (parse_data->state)
    {
    case OUTSIDE:
      g_assert_not_reached ();
      break;
    case IN_THEME:
      parse_data->state = OUTSIDE;
      parse_data->theme->sources = g_slist_reverse (parse_data->theme->sources);
      break;
    case IN_SOURCE:
      {
	CursorSource *source = parse_data->theme->sources->data;
	source->images = g_slist_reverse (source->images);
	parse_data->state = IN_THEME;
      }
      break;
    case IN_ALIAS:
      parse_data->state = IN_THEME;
      break;
    case IN_IMAGE:
      parse_data->state = IN_SOURCE;
      break;
    case IN_LAYOUT:
      parse_data->state = IN_THEME;
      break;
    case IN_ROW:
      parse_data->state = IN_LAYOUT;
      parse_data->row++;
      break;
    case IN_CURSOR:
      {
	Cursor *cursor = parse_data->current_cursor;
	if (!cursor->frame_configs)
	  add_frame_config (parse_data, -1);
	cursor->frame_configs = g_slist_reverse (cursor->frame_configs);
	parse_data->state = IN_ROW;
	parse_data->column++;
	parse_data->current_cursor = NULL;
      }
      break;
    case IN_FRAME:
      parse_data->state = IN_CURSOR;
      break;
    }
}

/* Called for character data */
/* text is not nul-terminated */
static void
cursor_theme_text (GMarkupParseContext *context,
		   const gchar         *text,
		   gsize                text_len,  
		   gpointer             user_data,
		   GError             **error)
{
  int i;

  for (i = 0; i < text_len; i++)
    if (!g_ascii_isspace (text[i]))
      {
	g_set_error (error,
		     G_MARKUP_ERROR,
		     G_MARKUP_ERROR_INVALID_CONTENT,
		     "Unexpected text in theme file");
	return;
      }
}

/* Called for strings that should be re-saved verbatim in this same
 * position, but are not otherwise interpretable.  At the moment
 * this includes comments and processing instructions.
 */
/* text is not nul-terminated. */
static void
cursor_theme_passthrough (GMarkupParseContext *context,
                          const gchar         *passthrough_text,
                          gsize                text_len,  
                          gpointer             user_data,
                          GError             **error)
{
  /* do nothing */
}

/* Called on error, including one set by other
 * methods in the vtable. The GError should not be freed.
 */
static void
cursor_theme_error (GMarkupParseContext *context,
		    GError              *error,
		    gpointer             user_data)
{
}

static GMarkupParser cursor_theme_parse = {
  cursor_theme_start_element,
  cursor_theme_end_element,
  cursor_theme_text,
  cursor_theme_passthrough,
  cursor_theme_error
};

static void
cursor_theme_free (CursorTheme *theme)
{
}

CursorTheme *
cursor_theme_read (const char *filename)
{
  ParseData parse_data;
  GMarkupParseContext *context;
  GError *error = NULL;
  char *text;
  gboolean have_error = FALSE;
  size_t len;

  if (!g_file_get_contents (filename, &text, &len, &error))
    {
      g_printerr ("Cannot read theme definition file: %s\n", error->message);
      g_error_free (error);
      return NULL;
    }

  parse_data.theme = g_new0 (CursorTheme, 1);
  parse_data.theme->cursors = g_hash_table_new (g_str_hash, g_str_equal);
  parse_data.theme->aliases = g_hash_table_new (g_str_hash, g_str_equal);
  
  parse_data.state = OUTSIDE;
  parse_data.seen_theme = FALSE;
  parse_data.seen_layout = FALSE;
  parse_data.row = 0;
  parse_data.column = 0;
  
  context = g_markup_parse_context_new (&cursor_theme_parse, 0,
					&parse_data, NULL);

  if (!g_markup_parse_context_parse (context, text, len, &error) ||
      !g_markup_parse_context_end_parse (context, &error))
    {
      g_printerr ("Error parsing theme definition file: %s\n", error->message);
      have_error = TRUE;
      g_error_free (error);
    }
  else if (!parse_data.seen_theme)
    {
      g_printerr ("Did not find <theme> element in theme file\n");
      have_error = TRUE;
    }
  else if (!parse_data.seen_layout)
    {
      g_printerr ("Did not find <layout> element in theme file\n");
      have_error = TRUE;
    }
  else if (!parse_data.theme->sources)
    {
      g_printerr ("No <source> element in theme file\n");
      have_error = TRUE;
    }
    
  g_markup_parse_context_free (context);

  if (!have_error)
    return parse_data.theme;
  else
    {
      cursor_theme_free (parse_data.theme);
      return NULL;
    }
}
