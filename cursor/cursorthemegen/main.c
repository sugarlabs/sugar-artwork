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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cursortheme.h"

static CursorImage *
cursor_source_find_image (CursorSource *source,
			  int           use)
{
  GSList *tmp_list;

  for (tmp_list = source->images; tmp_list; tmp_list = tmp_list->next)
    {
      CursorImage *image = tmp_list->data;

      if (image->use == use)
	return image;
    }

  return NULL;
}

static gboolean
cursor_find_hotspot (Cursor       *cursor,
		     CursorSource *source,
		     int          *hot_x,
		     int          *hot_y)
{
  CursorImage *image;
  int x = cursor->column * source->gridsize;
  int y = cursor->row * source->gridsize;

  *hot_x = -1;
  *hot_y = -1;
  
  image = cursor_source_find_image (source, -1);
  if (!image)
    {
      g_printerr ("Source at size %d doesn't have a hotspot image\n",
		  source->size);
      return FALSE;
    }

  if (!gdk_pixbuf_get_has_alpha (image->image) ||
      gdk_pixbuf_get_colorspace (image->image) != GDK_COLORSPACE_RGB)
    {
      g_printerr ("Invalid format for hotspot file\n");
      return FALSE;
    }
  
  if (x + source->gridsize <= gdk_pixbuf_get_width (image->image) &&
      y + source->gridsize <= gdk_pixbuf_get_height (image->image))
    {
      int rowstride;
      guchar *pixels;
      int i, j;

      rowstride = gdk_pixbuf_get_rowstride (image->image);
      pixels = gdk_pixbuf_get_pixels (image->image) + y * rowstride + x * 4;
      for (j = 0; j < source->gridsize; j++)
	{
	  for (i = 0; i < source->gridsize; i++)
	    {
	      if (pixels[4*i + 3] > 0x80)
		{
		  if (*hot_x >=0 || *hot_y  >= 0)
		    {
		      g_printerr ("Multiple hotspots for cursor %s at size %d\n",
				  cursor->name, source->size);
		      return FALSE;
		    }
		  *hot_x = i;
		  *hot_y = j;
		}
	    }
	  pixels += rowstride;
	}
    }
  
  if (*hot_x == -1 || *hot_y == -1)
    {
      g_printerr ("Cannot find hotspot for cursor %s at size %d\n",
		  cursor->name, source->size);
      return FALSE;
    }

  return TRUE;
}

static gboolean
cursor_is_present (Cursor       *cursor,
		   CursorSource *source,
		   int           frame)
{
  CursorImage *image = cursor_source_find_image (source, frame);
  int x = cursor->column * source->gridsize;
  int y = cursor->row * source->gridsize;
  int i, j;
  
  const guchar *pixels;
  int rowstride;
  int n_channels;

  if (!image)
    return FALSE;

  if (gdk_pixbuf_get_colorspace (image->image) != GDK_COLORSPACE_RGB)
    {
      g_printerr ("Invalid format for image file\n");
      return FALSE;
    }

  if (x + source->gridsize > gdk_pixbuf_get_width (image->image) ||
      y + source->gridsize > gdk_pixbuf_get_height (image->image))
    return FALSE;

  rowstride = gdk_pixbuf_get_rowstride (image->image);
  n_channels = gdk_pixbuf_get_n_channels (image->image);

  if (n_channels == 3)
    return TRUE;
  
  pixels = gdk_pixbuf_get_pixels (image->image) + y * rowstride + x * 4;

  for (j = 0; j < source->gridsize; j++)
    {
      for (i = 0; i < source->gridsize; i++)
	{
	  if (pixels[4*i + 3] != 0)
	    return TRUE;
	}
      pixels += rowstride;
    }

  return FALSE;
}

static void
cursor_frame_find_bounds (Cursor       *cursor,
			  CursorFrame  *frame,
			  CursorSource *source,
			  int           frame_index,
			  int          *x,
			  int          *y,
			  int          *width,
			  int          *height)
{
  CursorImage *image = cursor_source_find_image (source, frame_index);
  int start_x = cursor->column * source->gridsize;
  int start_y = cursor->row * source->gridsize;
  int i, j;
  
  const guchar *pixels;
  int rowstride;
  int n_channels;

  rowstride = gdk_pixbuf_get_rowstride (image->image);
  n_channels = gdk_pixbuf_get_n_channels (image->image);

  if (n_channels == 3)
    {
      *x = start_x;
      *y = start_x;
      *width = source->gridsize;
      *height = source->gridsize;
      
      return;
    }
  else
    {
      int min_x = start_x + frame->hot_x;
      int max_x = start_x + frame->hot_x + 1;
      int min_y = start_y + frame->hot_y;
      int max_y = start_y + frame->hot_y + 1;
      
      pixels = gdk_pixbuf_get_pixels (image->image) + start_y * rowstride + start_x * 4;

      for (j = 0; j < source->gridsize; j++)
	{
	  for (i = 0; i < source->gridsize; i++)
	    {
	      if (pixels[4*i + 3] != 0)
		{
		  if (start_x + i < min_x)
		    min_x = start_x + i;
		  if (start_x + i >= max_x)
		    max_x = start_x + i + 1;
		  if (start_y + j < min_y)
		    min_y = start_y + j;
		  if (start_y + j >= max_y)
		    max_y = start_y + j + 1;
		}
	    }
	  
	  pixels += rowstride;
	}
	  
      *x = min_x;
      *y = min_y;
      *width = max_x - min_x;
      *height = max_y - min_y;
    }
}

static void
cursor_get_image (Cursor       *cursor,
		  CursorFrame  *frame,
		  CursorSource *source,
		  int           frame_index,
                  int		x,
                  int		y,
                  int           width,
                  int		height)
{
  CursorImage *image = cursor_source_find_image (source, frame_index);
  GdkPixbuf *tmp_pixbuf;
  
  /*cursor_frame_find_bounds (cursor, frame, source, frame_index,
			    &x, &y, &width, &height);
*/
  tmp_pixbuf = gdk_pixbuf_new_subpixbuf (image->image,
					 x, y, width, height);

  frame->hot_x -= x - cursor->column * source->gridsize;
  frame->hot_y -= y - cursor->row * source->gridsize;
  
  frame->image = gdk_pixbuf_copy (tmp_pixbuf);
  g_object_unref (tmp_pixbuf);
}

static gboolean
cursor_add_source (Cursor       *cursor,
		   CursorSource *source)
{
  GSList *tmp_list;
  int hot_x, hot_y;
  int i;
  int x, y, x2, y2, height, width;

  /* If the first frame is missing we silently treat
   * it as OK
   */
  if (!cursor_is_present (cursor, source, 0))
    return TRUE;
  
  if (!cursor_find_hotspot (cursor, source, &hot_x, &hot_y))
    return FALSE;

  /* generate a single hight width for all cursors in a
     given animation*/

  for (tmp_list = cursor->frame_configs, i = 0;
       tmp_list;
       tmp_list = tmp_list->next, i++)
    {
      CursorFrameConfig *frame_config = tmp_list->data;
      CursorFrame *frame;
      int tx, ty, tx2, ty2, twidth, theight;

      if (i != 0 && !cursor_is_present (cursor, source, i))
        continue;

      frame = g_new0 (CursorFrame, 1);
      frame->size = source->size;
      frame->hot_x = hot_x;
      frame->hot_y = hot_y;
      frame->delay = frame_config->delay;

      cursor_frame_find_bounds (cursor, frame, source, i,
                                &tx, &ty, &twidth, &theight);

      tx2 = tx + twidth;
      ty2 = ty + theight;

      if (i == 0)
        {
          x = tx;
          y = ty;
          x2 = tx2;
          y2 = ty2;
        } 
      else 
        {
          if (tx < x)
            x = tx;

          if (ty < y)
            y = ty;

          if (tx2 > x2)
            x2 = tx2;
	
          if (ty2 > y2)
            y2 = ty2;
        }
        
      g_free (frame);
    }

  width = x2 - x;
  height = y2 - y;

  for (tmp_list = cursor->frame_configs, i = 0;
       tmp_list;
       tmp_list = tmp_list->next, i++)
    {
      CursorFrameConfig *frame_config = tmp_list->data;
      CursorFrame *frame;

      if (i != 0 && !cursor_is_present (cursor, source, i))
	{
	  g_printerr ("Frame %d missing for cursor '%s' at size %d\n",
		      i, cursor->name, source->size);
	  frame = g_new0 (CursorFrame, 1);
	  frame->size = -1;
	  cursor->frames = g_slist_append (cursor->frames, frame);
	  continue;
	}

      frame = g_new0 (CursorFrame, 1);
      frame->size = source->size;
      frame->hot_x = hot_x;
      frame->hot_y = hot_y;
      frame->delay = frame_config->delay;

      cursor_get_image (cursor, frame, source, i, x, y, width, height);

      cursor->frames = g_slist_append (cursor->frames, frame);
    }
  
  return TRUE;
}

static gboolean
cursor_theme_read_cursor (CursorTheme *theme,
			  Cursor      *cursor)
{
  GSList *tmp_list;

  for (tmp_list = theme->sources; tmp_list; tmp_list = tmp_list->next)
    {
      if (!cursor_add_source (cursor, tmp_list->data))
	return FALSE;
    }

  return TRUE;
}

static void
cursor_theme_write_cursor (CursorTheme *theme,
			   Cursor      *cursor)
{
  GSList *tmp_list;
  char *config_filename;
  char *command;
  FILE *config_file;
  GError *error = NULL;
  int status;
  int i;

  if (g_hash_table_lookup (theme->aliases, cursor->name))
    {
      g_printerr ("Warning: cursor '%s' overridden by alias\n", cursor->name);
      return;
    }
  
  if (!cursor->frames)
    return;
  
  config_filename = g_strconcat (cursor->name, ".cfg", NULL);
  config_file = fopen (config_filename, "w");
  
  if (!config_file)
    {
      g_printerr ("Cannot open config file '%s'\n", config_filename);
      return;
    }

  for (tmp_list = cursor->frames, i = 0; tmp_list; tmp_list = tmp_list->next, i++)
    {
      CursorFrame *frame = tmp_list->data;
      char *filename;
      
      if (frame->size == -1)
	  continue;
      filename = g_strdup_printf ("%s-%d.png", cursor->name, i);
      if (gdk_pixbuf_save (frame->image, filename, "png", &error, NULL))
	{
	  if (frame->delay > 0)
	    fprintf (config_file, "%d %d %d %s %d\n",
		     frame->size, frame->hot_x, frame->hot_y, filename, frame->delay);
	  else
	    fprintf (config_file, "%d %d %d %s\n",
		     frame->size, frame->hot_x, frame->hot_y, filename);
	}
      else
	{
	  g_printerr ("Error saving image file: %s\n", error->message);
	  g_error_free (error);
	}
      g_free (filename);
    }

  fclose (config_file);

  command = g_strdup_printf ("sh -c 'xcursorgen %s > %s'\n",
			     config_filename, cursor->name);
  if (!g_spawn_command_line_sync (command, NULL, NULL, &status, &error))
    {
      g_printerr ("Error running xcursorgen for %s: %s\n",
		  cursor->name, error->message);
      g_error_free (error);
    }
  else if (status)
    {
      g_printerr ("Error running xcursorgen for %s\n",
		  cursor->name);
    }
  else
    {
      /* Only delete temporary files if no error occurred
       */
      unlink (config_filename);
      g_free (config_filename);
      
      for (tmp_list = cursor->frames, i = 0; tmp_list; tmp_list = tmp_list->next, i++)
	{
	  char *filename;
	  
	  filename = g_strdup_printf ("%s-%d.png", cursor->name, i);
	  unlink (filename);
	  g_free (filename);
	}
    }
}

static const char *
cursor_theme_check_alias (CursorTheme *theme,
			  CursorAlias *alias)
{
  /* Dereference, using tortoise-and-hare checkign for circular aliases
   */
  CursorAlias *tortoise = alias;
  CursorAlias *hare = alias;
  Cursor *target;

  while (TRUE)
    {
      CursorAlias *next;

      next = g_hash_table_lookup (theme->aliases, hare->target);
      if (!next)
	break;
      hare = next;

      if (hare == tortoise)
	goto found_loop;
      
      next = g_hash_table_lookup (theme->aliases, hare->target);
      if (!next)
	break;
      hare = next;

      if (hare == tortoise)
	goto found_loop;
      
      tortoise = g_hash_table_lookup (theme->aliases, tortoise->target);
    }

  target = g_hash_table_lookup (theme->cursors, hare->target);

  if (!target || !target->frames)
    {
      g_printerr ("Cursor '%s', which is the target of alias '%s', is not in theme\n",
		  hare->target, alias->name);
      return NULL;
    }

  return hare->target;

 found_loop:
  g_printerr ("Circular looop detected when dereferencing alias '%s'\n",
	      alias->name);
  return NULL;
}

static void
cursor_theme_write_alias (CursorTheme *theme,
			  CursorAlias *alias)
{
  const char *target = cursor_theme_check_alias (theme, alias);
  if (!target)
    return;

  if (symlink (target, alias->name) < 0)
    {
      g_printerr ("Error creating symlink for alias '%s' to '%s': %s\n",
		  alias->name, target, g_strerror (errno));
    }
}

static void
write_cursor_foreach (gpointer key,
		      gpointer value,
		      gpointer data)
{
  cursor_theme_write_cursor (data, value);
}

static void
write_alias_foreach (gpointer key,
		     gpointer value,
		     gpointer data)
{
  cursor_theme_write_alias (data, value);
}

static gboolean
cursor_theme_write (CursorTheme *theme,
		    const char  *output_dir)
{
  char *curdir;
  
  if (mkdir (output_dir, 0755) < 0 && errno != EEXIST)
    {
      g_printerr ("Error creating output directory '%s'\n: %s",
		  output_dir, g_strerror (errno));
      return FALSE;
    }

  curdir = g_get_current_dir ();
  if (chdir (output_dir) < 0)
    {
      g_printerr ("Could not change to output directory '%s'\n", output_dir);
      return FALSE;
    }

  g_hash_table_foreach (theme->cursors,
			write_cursor_foreach,
			theme);

  g_hash_table_foreach (theme->aliases,
			write_alias_foreach,
			theme);

  chdir (curdir);

  return TRUE;
}

void
usage (void)
{
  g_printerr ("Usage: cursorthemegen CONFIG_FILE OUTPUT_DIR\n");
  exit (1);
}

static void
read_cursor_foreach (gpointer key,
		     gpointer value,
		     gpointer data)
{
  if (!cursor_theme_read_cursor (data, value))
    exit (1);
}

int
main (int argc, char **argv)
{
  CursorTheme *theme;

  g_type_init ();
  
  if (argc != 3)
    usage ();

  theme = cursor_theme_read (argv[1]);
  if (!theme)
    exit (1);

  g_hash_table_foreach (theme->cursors,
			read_cursor_foreach,
			theme);

  if (!cursor_theme_write (theme, argv[2]))
    exit (1);

  return 0;
}
