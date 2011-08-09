/* Adwaita - a GTK+ engine
 *
 * Copyright (C) 2011 Carlos Garnacho <carlosg@gnome.org>
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Authors: Carlos Garnacho <carlosg@gnome.org>
 *          Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Project contact: <gnome-themes-list@gnome.org>
 */

#include <gtk/gtk.h>
#include <gmodule.h>
#include <math.h>
#include <cairo-gobject.h>

#include "sugar_utils.h"

#define ADWAITA_NAMESPACE "sugar"

typedef struct _AdwaitaEngine AdwaitaEngine;
typedef struct _AdwaitaEngineClass AdwaitaEngineClass;

struct _AdwaitaEngine
{
  GtkThemingEngine parent_object;
};

struct _AdwaitaEngineClass
{
  GtkThemingEngineClass parent_class;
};

#define ADWAITA_TYPE_ENGINE		 (sugar_engine_get_type ())
#define ADWAITA_ENGINE(object)		 (G_TYPE_CHECK_INSTANCE_CAST ((object), ADWAITA_TYPE_ENGINE, AdwaitaEngine))
#define ADWAITA_ENGINE_CLASS(klass)	 (G_TYPE_CHECK_CLASS_CAST ((klass), ADWAITA_TYPE_ENGINE, AdwaitaEngineClass))
#define ADWAITA_IS_ENGINE(object)	 (G_TYPE_CHECK_INSTANCE_TYPE ((object), ADWAITA_TYPE_ENGINE))
#define ADWAITA_IS_ENGINE_CLASS(klass)	 (G_TYPE_CHECK_CLASS_TYPE ((klass), ADWAITA_TYPE_ENGINE))
#define ADWAITA_ENGINE_GET_CLASS(obj)	 (G_TYPE_INSTANCE_GET_CLASS ((obj), ADWAITA_TYPE_ENGINE, AdwaitaEngineClass))

GType sugar_engine_get_type	    (void) G_GNUC_CONST;
void  sugar_engine_register_types (GTypeModule *module);

G_DEFINE_DYNAMIC_TYPE (AdwaitaEngine, sugar_engine, GTK_TYPE_THEMING_ENGINE)

void
sugar_engine_register_types (GTypeModule *module)
{
  sugar_engine_register_type (module);
}

static void
sugar_engine_init (AdwaitaEngine *self)
{
}

static void
sugar_engine_render_arrow (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           angle,
                             gdouble           x,
                             gdouble           y,
                             gdouble           size)
{
  double line_width;
  GtkStateFlags state;
  GdkRGBA color;

  cairo_save (cr);

  line_width = size / 3.0 / sqrt (2);
  cairo_set_line_width (cr, line_width);
  cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

  cairo_translate (cr, x + size / 2.0, y + size / 2.0);
  cairo_rotate (cr, angle - G_PI_2);
  cairo_translate (cr, size / 4.0, 0);
 
  cairo_scale (cr,
               (size / (size + line_width)),
               (size / (size + line_width)));

  cairo_move_to (cr, -size / 2.0, -size / 2.0);
  cairo_rel_line_to (cr, size / 2.0, size / 2.0);
  cairo_rel_line_to (cr, - size / 2.0, size / 2.0);

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_color (engine, state, &color);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM) &&
      !(state & GTK_STATE_FLAG_INSENSITIVE) && !(state & GTK_STATE_FLAG_PRELIGHT))
    {
      GdkRGBA *arrow_color;

      gtk_theming_engine_get (engine, state,
                              "-sugar-menuitem-arrow-color", &arrow_color,
                              NULL);

      if (arrow_color != NULL)
        color = *arrow_color;

      gdk_rgba_free (arrow_color);
    }

  gdk_cairo_set_source_rgba (cr, &color);
  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
sugar_engine_render_focus (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           x,
                             gdouble           y,
                             gdouble           width,
                             gdouble           height)
{
  GdkRGBA *fill_color, *border_color = NULL;
  cairo_pattern_t *pattern = NULL;
  GtkStateFlags state;
  gint line_width, focus_pad;
  gint border_radius;
  gboolean use_dashes;
  double dashes[2] = { 2.0, 0.2 };

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state,
                          "-sugar-focus-border-color", &border_color,
                          "-sugar-focus-fill-color", &fill_color,
                          "-sugar-focus-border-radius", &border_radius,
                          "-sugar-focus-border-gradient", &pattern,
                          "-sugar-focus-border-dashes", &use_dashes,
                          NULL);

  gtk_theming_engine_get_style (engine,
                                "focus-line-width", &line_width,
                                "focus-padding", &focus_pad,
                                NULL);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_NOTEBOOK) &&
      gtk_theming_engine_has_region (engine, GTK_STYLE_REGION_TAB, NULL))
    {
      /* as we render the tab smaller than the whole allocation, we need
       * to recenter and resize the focus on the tab.
       */
      y += 3.0;
      height -= 3.0;
    }

  cairo_save (cr);
  cairo_set_line_width (cr, line_width);

  if (line_width > 1)
    _cairo_round_rectangle_sides (cr, border_radius,
                                  x, y, width, height,
                                  SIDE_ALL, GTK_JUNCTION_NONE);
  else
    _cairo_round_rectangle_sides (cr, border_radius,
                                  x + 0.5, y + 0.5,
                                  width - 1, height - 1,
                                  SIDE_ALL, GTK_JUNCTION_NONE);

  /* if we have a fill color, draw the fill */
  if (fill_color != NULL)
    {
      gdk_cairo_set_source_rgba (cr, fill_color);
      cairo_fill_preserve (cr);
    }

  if (use_dashes)
    cairo_set_dash (cr, dashes, 1, 0.0);

  /* if we have a gradient, draw the gradient, otherwise
   * draw the line if we have a color for it.
   */
  if (pattern != NULL)
    {
      style_pattern_set_matrix (pattern, width, height, FALSE);
      cairo_set_source (cr, pattern);
    }
  else if (border_color != NULL)
    {
      gdk_cairo_set_source_rgba (cr, border_color);
    }

  cairo_stroke (cr);
  cairo_restore (cr);

  if (pattern != NULL)
    cairo_pattern_destroy (pattern);

  if (border_color != NULL)
    gdk_rgba_free (border_color);

  if (fill_color != NULL)
    gdk_rgba_free (fill_color);
}

static void
render_check_menuitem (GtkThemingEngine *engine,
                       cairo_t *cr,
                       gdouble x,
                       gdouble y,
                       gdouble width,
                       gdouble height)
{
  GdkRGBA color;
  GtkStateFlags state;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_color (engine, state, &color);

  if (!(state & GTK_STATE_FLAG_ACTIVE))
    return;

  cairo_save (cr);

  cairo_translate (cr, x, y);

  cairo_set_line_width (cr, 2.0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);

  cairo_move_to (cr, 0.5 + (width * 0.08), (height * 0.67));
  cairo_line_to (cr, 0.5 + (width * 0.32), (height * 0.90));
  cairo_line_to (cr, 0.5 + (width * 0.80), (height * 0.33));

  gdk_cairo_set_source_rgba (cr, &color);
  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
sugar_engine_render_check (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           x,
                             gdouble           y,
                             gdouble           width,
                             gdouble           height)
{
  gboolean res;

  if (gtk_theming_engine_has_class (engine,
                                    GTK_STYLE_CLASS_MENUITEM))
    {
      render_check_menuitem (engine, cr,
                             x, y, width, height);

      return;
    }

  res = sugar_render_from_assets_common (engine, cr,
                                           x, y + 2.0, width, height);

  if (!res)
    GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_check
      (engine, cr, x, y, width, height);
}

static void
render_radio_menuitem (GtkThemingEngine *engine,
                       cairo_t *cr,
                       gdouble x,
                       gdouble y,
                       gdouble width,
                       gdouble height)
{
  GdkRGBA color;
  GtkStateFlags state;
  double radius;

  state = gtk_theming_engine_get_state (engine);

  if (!(state & GTK_STATE_FLAG_ACTIVE))
    return;

  gtk_theming_engine_get_color (engine, state, &color);

  radius = MAX (height / 2.0, width / 2.0) * 0.58;

  cairo_save (cr);

  cairo_translate (cr, x + width / 2.0, y + height * 0.67);
  cairo_arc (cr,
             0, 0,
             radius,
             0, 4 * G_PI);

  gdk_cairo_set_source_rgba (cr, &color);
  cairo_fill (cr);

  cairo_restore (cr);
}

static void
sugar_engine_render_option (GtkThemingEngine *engine,
                              cairo_t          *cr,
                              gdouble           x,
                              gdouble           y,
                              gdouble           width,
                              gdouble           height)
{
  gboolean res;

  if (gtk_theming_engine_has_class (engine,
                                    GTK_STYLE_CLASS_MENUITEM))
    {
      render_radio_menuitem (engine, cr, x, y, width, height);
      return;
    }

  res = sugar_render_from_assets_common (engine, cr,
                                           x, y + 2.0, width, height);

  if (!res)
    GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_option
      (engine, cr, x, y, width, height);
}

static void
draw_tab_arcs (cairo_t *cr,
               gdouble curve_width,
               gdouble x,
               gdouble y,
               gdouble width,
               gdouble height)
{
  cairo_arc (cr, 
             curve_width, 6.0,
             2.5,
             G_PI, G_PI + G_PI_2);

  cairo_arc (cr,
             width - curve_width, 6.0,
             2.5,
             G_PI + G_PI_2, 2 * G_PI);
}

static void
draw_tab_shape_active (cairo_t *cr,
                       gdouble curve_width,
                       gdouble x,
                       gdouble y,
                       gdouble width,
                       gdouble height)
{
  cairo_move_to (cr, 0, height);

  draw_tab_arcs (cr, curve_width, x, y, width, height);

  cairo_line_to (cr, width, height);
}

static void
render_notebook_extension (GtkThemingEngine *engine,
                           cairo_t          *cr,
                           gdouble           x,
                           gdouble           y,
                           gdouble           width,
                           gdouble           height,
                           GtkPositionType   gap_side)
{
  gint tab_curvature;
  GdkRGBA *color, border_color, background_color;
  GtkStateFlags state;
  gdouble angle = 0;
  cairo_pattern_t *pattern = NULL, *background_pattern = NULL;
  cairo_matrix_t matrix;

  gtk_theming_engine_get_style (engine,
                                "tab-curvature", &tab_curvature,
                                NULL);
  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_background_color (engine, state, &background_color);
  gtk_theming_engine_get_border_color (engine, state, &border_color);
  gtk_theming_engine_get (engine, state,
                          "-sugar-selected-tab-color", &color,
                          "-sugar-border-gradient", &pattern,
                          "background-image", &background_pattern,
                          NULL);

  cairo_save (cr);
  cairo_set_line_width (cr, 1.0);

  if (gap_side == GTK_POS_TOP)
    {
      angle = G_PI;
      cairo_translate (cr, width, height);
    }

  if (gap_side == GTK_POS_BOTTOM)
    cairo_translate (cr,
                     x + 0.5,
                     (state & GTK_STATE_FLAG_ACTIVE) ?
                     y + 1.0 : y);
  else if (gap_side == GTK_POS_TOP)
    cairo_translate (cr,
                     x - 0.5,
                     (state & GTK_STATE_FLAG_ACTIVE) ?
                     y - 1.0 : y);

  cairo_rotate (cr, angle);

  width -= 1.0;
  draw_tab_shape_active (cr, tab_curvature, 0, 0, width, height);

  if (background_pattern != NULL)
    {
      cairo_matrix_init_scale (&matrix,
                               1. / width,
                               1. / height);
      cairo_pattern_set_matrix (background_pattern, &matrix);
      cairo_set_source (cr, background_pattern);
    }
  else
    {
      gdk_cairo_set_source_rgba (cr, &background_color);
    }

  cairo_fill (cr);

  if (state & GTK_STATE_FLAG_ACTIVE)
    {
      draw_tab_shape_active (cr, tab_curvature, 0, 0, width, 6.0);
      gdk_cairo_set_source_rgba (cr, color);
      cairo_fill (cr);
    }

  draw_tab_shape_active (cr, tab_curvature, 0, 0, width, height);

  if (state & GTK_STATE_FLAG_ACTIVE)
    {
      style_pattern_set_matrix (pattern, width, height - 6.0, FALSE);
      cairo_set_source (cr, pattern);
    }
  else
    {
      gdk_cairo_set_source_rgba (cr, &border_color);
    }

  cairo_stroke (cr);

  gdk_rgba_free (color);

  if (pattern != NULL)
    cairo_pattern_destroy (pattern);

  if (background_pattern != NULL)
    cairo_pattern_destroy (background_pattern);

  cairo_restore (cr);
}

static void
sugar_engine_render_extension (GtkThemingEngine *engine,
                                 cairo_t          *cr,
                                 gdouble           x,
                                 gdouble           y,
                                 gdouble           width,
                                 gdouble           height,
                                 GtkPositionType   gap_side)
{
  GtkStateFlags state;

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_NOTEBOOK) &&
      ((gap_side == GTK_POS_TOP) || (gap_side == GTK_POS_BOTTOM)))
    {
      render_notebook_extension (engine, cr, x, y, width, height, gap_side);
      return;
    }

  GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_extension
    (engine, cr,
     x, y, width, height,
     gap_side);

  state = gtk_theming_engine_get_state (engine);

  if (state & GTK_STATE_FLAG_ACTIVE)
    {
      GdkRGBA *fill;

      gtk_theming_engine_get (engine, state,
                              "-sugar-selected-tab-color", &fill,
                              NULL);

      switch (gap_side)
        {
        case GTK_POS_BOTTOM:
          cairo_rectangle (cr,
                           x + 1, y + 1,
                           width - 2, 3);
          break;
        case GTK_POS_TOP:
          cairo_rectangle (cr,
                           x + 1, y + height - 4,
                           width - 2, 3);
          break;
        case GTK_POS_RIGHT:
          cairo_rectangle (cr,
                           x + 1, y + 1,
                           3, height - 2);
          break;
        case GTK_POS_LEFT:
          cairo_rectangle (cr,
                           x + width - 4, y + 1,
                           3, height - 2);
          break;
        }

      gdk_cairo_set_source_rgba (cr, fill);
      cairo_fill (cr);

      gdk_rgba_free (fill);
    }
}

static void
draw_menu_bar_item_shape (cairo_t *cr,
                          gdouble radius,
                          gdouble x,
                          gdouble y,
                          gdouble w,
                          gdouble h,
                          gboolean for_fill)
{
  /* draw a round rectangle without the bottom side */
  cairo_move_to (cr, x+radius, y);
  cairo_arc (cr, x+w-radius, y+radius, radius, G_PI * 1.5, G_PI * 2);
  cairo_line_to (cr, x+w, y+h);

  if (for_fill)
    cairo_line_to (cr, x, y+h);
  else
    cairo_move_to (cr, x, y+h);

  cairo_arc (cr, x+radius, y+radius, radius, G_PI, G_PI * 1.5);
}

static void
render_menubar_active_frame (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           x,
                             gdouble           y,
                             gdouble           w,
                             gdouble           h)
{
  GtkStateFlags state;
  GdkRGBA color;
  gint radius, border_width;
  GtkBorder border;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_border_color (engine, state, &color);
  gtk_theming_engine_get_border (engine, state, &border);
  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          NULL);

  border_width = MIN (MIN (border.top, border.bottom),
                      MIN (border.left, border.right));

  if (border_width > 1)
    {
      x += (gdouble) border_width / 2;
      y += (gdouble) border_width / 2;
      w -= border_width;
      h -= border_width;
    }
  else if (border_width == 1)
    {
      x += 0.5;
      y += 0.5;
      w -= 1;
      h -= 1;
    }

  cairo_save (cr);

  cairo_set_line_width (cr, border_width);
  draw_menu_bar_item_shape (cr, radius, x, y, w, h, FALSE);

  gdk_cairo_set_source_rgba (cr, &color);
  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
render_frame_default (GtkThemingEngine *engine,
                      cairo_t *cr,
                      gdouble x,
                      gdouble y,
                      gdouble width,
                      gdouble height)
{
  cairo_pattern_t *pattern = NULL;
  GtkStateFlags state;
  GtkBorder border;
  gint border_radius;
  GtkBorderStyle border_style;
  GtkJunctionSides junctions;

  state = gtk_theming_engine_get_state (engine);

  gtk_theming_engine_get (engine, state,
                          "-sugar-border-gradient", &pattern,
                          "border-style", &border_style,
                          NULL);

  if (pattern == NULL || border_style == GTK_BORDER_STYLE_NONE)
    {
      GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_frame
        (engine, cr,
         x, y, width, height);

      return;
    }

  cairo_save (cr);

  gtk_theming_engine_get (engine, state,
                          "border-radius", &border_radius,
                          NULL);
  gtk_theming_engine_get_border (engine, state, &border);
  junctions = gtk_theming_engine_get_junction_sides (engine);

  style_pattern_set_matrix (pattern, width, height, TRUE);

  _cairo_uneven_frame (cr, border_radius,
                       x, y, width, height,
                       &border, junctions);
  cairo_set_source (cr, pattern);

  cairo_fill (cr);

  cairo_restore (cr);

  cairo_pattern_destroy (pattern);
}

static void
sugar_engine_render_frame (GtkThemingEngine *engine,
                             cairo_t          *cr,
                             gdouble           x,
                             gdouble           y,
                             gdouble           width,
                             gdouble           height)
{
  const GtkWidgetPath *path;
  GtkRegionFlags flags = 0;
  gint len;
  GtkStateFlags state;        

  state = gtk_theming_engine_get_state (engine);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM) &&
      gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUBAR))
    {
      render_menubar_active_frame (engine, cr, x, y, width, height);
      return;
    }
        
  path = gtk_theming_engine_get_path (engine);
  len = gtk_widget_path_length (path);

  cairo_save (cr);

  if (gtk_widget_path_iter_has_region (path, len - 2,
                                       GTK_STYLE_REGION_COLUMN_HEADER,
                                       &flags))
    {
      GdkRGBA color;

      if ((flags & GTK_REGION_LAST) != 0)
        goto out;

      /* Column header */
      if (gtk_theming_engine_get_direction (engine) == GTK_TEXT_DIR_RTL)
        {
          cairo_move_to (cr, x + 0.5, y + 2);
          cairo_line_to (cr, x + 0.5, y + height - 4);
        }
      else
        {
          cairo_move_to (cr, x + width - 0.5, y + 2);
          cairo_line_to (cr, x + width - 0.5, y + height - 4);
        }

      gtk_theming_engine_get_border_color (engine, state, &color);

      cairo_set_line_width (cr, 1);
      gdk_cairo_set_source_rgba (cr, &color);
      cairo_stroke (cr);
    }
  else
    {
      sugar_trim_allocation_for_scale (engine,
                                         &x, &y,
                                         &width, &height);
      render_frame_default (engine, cr, x, y, width, height);
    }

out:
  cairo_restore (cr);
}

static void
render_menubar_active_background (GtkThemingEngine *engine,
                                  cairo_t          *cr,
                                  gdouble           x,
                                  gdouble           y,
                                  gdouble           w,
                                  gdouble           h)
{
  GtkStateFlags state;
  GdkRGBA color;
  gint radius;
  GtkBorder border;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_border_color (engine, state, &color);
  gtk_theming_engine_get_border (engine, state, &border);
  gtk_theming_engine_get (engine, state,
                          "border-radius", &radius,
                          NULL);

  gtk_theming_engine_get_background_color (engine, state, &color);

  /* omit all the border but the bottom line */
  x += border.left;
  y += border.top;
  w -= border.left + border.right;
  h -= border.top;

  cairo_save (cr);
  cairo_translate (cr, x, y);

  draw_menu_bar_item_shape (cr, radius, 0, 0, w, h, TRUE);

  gdk_cairo_set_source_rgba (cr, &color);
  cairo_fill (cr);

  cairo_restore (cr);
}

static void
render_inset_lines (GtkThemingEngine *engine,
                    cairo_t *cr,
                    gdouble x,
                    gdouble y,
                    gdouble width,
                    gdouble height)
{
  GtkStateFlags state;
  GtkBorder border;
  GdkRGBA *inset_left, *inset_right, *inset_top, *inset_bottom;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_border (engine, state, &border);

  gtk_theming_engine_get (engine, state,
                          "-sugar-inset-left", &inset_left,
                          "-sugar-inset-right", &inset_right,
                          "-sugar-inset-top", &inset_top,
                          "-sugar-inset-bottom", &inset_bottom,
                          NULL);

  cairo_save (cr);
  cairo_set_line_width (cr, 1.0);

  if (inset_left != NULL)
    {
      cairo_move_to (cr,
                     x + border.left + 0.5,
                     y + border.top + 1.0);
      cairo_line_to (cr,
                     x + border.left + 1,
                     y + height - border.bottom - 1.0);

      gdk_cairo_set_source_rgba (cr, inset_left);
      cairo_stroke (cr);

      gdk_rgba_free (inset_left);
    }

  if (inset_right != NULL)
    {
      cairo_move_to (cr,
                     x + width - border.right - 0.5,
                     y + border.top + 1.0);
      cairo_line_to (cr,
                     x + width - border.right - 0.5,
                     y + height - border.bottom - 1.0);

      gdk_cairo_set_source_rgba (cr, inset_right);
      cairo_stroke (cr);

      gdk_rgba_free (inset_right);
    }

  if (inset_top != NULL)
    {
      cairo_move_to (cr,
                     x + border.left + 1.0,
                     y + border.top + 0.5);
      cairo_line_to (cr,
                     x + width - border.right - 1.0,
                     y + border.top + 0.5);

      gdk_cairo_set_source_rgba (cr, inset_top);
      cairo_stroke (cr);

      gdk_rgba_free (inset_top);
    }

  if (inset_bottom != NULL)
    {
      cairo_move_to (cr,
                     x + border.left + 1.0,
                     y + height - border.bottom - 0.5);
      cairo_line_to (cr,
                     x + width - border.right - 1.0,
                     y + height - border.bottom - 0.5);

      gdk_cairo_set_source_rgba (cr, inset_bottom);
      cairo_stroke (cr);

      gdk_rgba_free (inset_bottom);
    }

  cairo_restore (cr);
}

static void
sugar_engine_render_background (GtkThemingEngine *engine,
                                  cairo_t          *cr,
                                  gdouble           x,
                                  gdouble           y,
                                  gdouble           width,
                                  gdouble           height)
{
  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUITEM) &&
      gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MENUBAR))
    {
      render_menubar_active_background (engine, cr, x, y, width, height);
      return;
    }

  sugar_trim_allocation_for_scale (engine,
                                     &x, &y,
                                     &width, &height);

  GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_background
    (engine, cr, x, y,
     width, height);

  render_inset_lines (engine, cr, x, y, width, height);
}

static void
sugar_engine_render_expander (GtkThemingEngine *engine,
                                cairo_t          *cr,
                                gdouble           x,
                                gdouble           y,
                                gdouble           width,
                                gdouble           height)
{
  GdkRGBA fg;
  GtkStateFlags state;
  gdouble side, offset;
  gint line_width;
  GtkBorder border;

  side = floor (MIN (width, height));

  /* make sure the side length is always odd */
  if (((gint) side % 2) == 0)
    side -= 1.0;

  x += width / 2 - side / 2;
  y += height / 2 - side / 2;

  GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_background
    (engine, cr, x, y, side, side);
  GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_frame
    (engine, cr, x, y, side, side);

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get_color (engine, state, &fg);
  gtk_theming_engine_get_border (engine, state, &border);

  line_width = 1;
  offset = (1 + line_width / 2.0);

  cairo_save (cr);

  cairo_set_line_width (cr, line_width);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
  gdk_cairo_set_source_rgba (cr, &fg);

  cairo_move_to (cr,
                 x + border.left + offset,
                 y + side / 2);
  cairo_line_to (cr,
                 x + side - (border.right + offset),
                 y + side / 2);

  if ((state & GTK_STATE_FLAG_ACTIVE) == 0)
    {
      cairo_move_to (cr,
                     x + side / 2,
                     y + border.top + offset);
      cairo_line_to (cr,
                     x + side / 2,
                     y + side - (border.bottom + offset));
    }

  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
sugar_engine_render_activity (GtkThemingEngine *engine,
                                cairo_t          *cr,
                                gdouble           x,
                                gdouble           y,
                                gdouble           width,
                                gdouble           height)
{
  GtkStateFlags state;

  cairo_save (cr);
  state = gtk_theming_engine_get_state (engine);

  sugar_trim_allocation_for_scale (engine,
                                     &x, &y,
                                     &width, &height);

  GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_activity
    (engine, cr,
     x, y, width, height);

  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_PROGRESSBAR))
    {
      cairo_pattern_t *pattern = NULL;

      gtk_theming_engine_get (engine, state,
                              "-sugar-progressbar-pattern", &pattern,
                              NULL);

      if (pattern != NULL)
        {
          style_pattern_set_matrix (pattern, 20, 20, TRUE);
          cairo_rectangle (cr, x, y, width, height);
          cairo_set_source (cr, pattern);
          cairo_fill (cr);

          cairo_pattern_destroy (pattern);
        }
    }

  cairo_restore (cr);

  render_inset_lines (engine, cr, x, y, width, height);
}

static void
draw_round_slider (cairo_t *cr,
                   gdouble  width,
                   gdouble  height)
{
  cairo_arc (cr, (width) / 2.0, (height) / 2.0,
             MIN (height / 2.0, width / 2.0) - 0.5,
             0, 2 * G_PI);
  cairo_close_path (cr);        
}

static void
draw_mark_slider (cairo_t *cr,
                  gdouble  width,
                  gdouble  height,
                  gboolean marks_below,
                  GtkOrientation orientation)
{
  cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);

  if (marks_below)
    {
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        {
          cairo_move_to (cr, 0, height / 2.0 + 3.0);
          cairo_arc (cr, 2.5, 3.5,
                     2.0,
                     G_PI, G_PI + G_PI_2);
                        
          cairo_arc (cr, width - 2.5, 3.5,
                     2.0,
                     G_PI + G_PI_2, 2 * G_PI);

          cairo_line_to (cr, width, height / 2.0 + 3.0);
          cairo_line_to (cr, width / 2.0, height);
          cairo_line_to (cr, 0, height / 2.0 + 3.0);

          cairo_close_path (cr);
        }
      else
        {
          cairo_move_to (cr, width / 2.0, 0);
          cairo_arc (cr, width - 2.5, 2.5,
                     2.0,
                     G_PI + G_PI_2, 2 * G_PI);

          cairo_arc (cr, width - 2.5, height - 2.5,
                     2.0,
                     0, G_PI_2);

          cairo_line_to (cr, width / 2.0, height);
          cairo_line_to (cr, 0, height / 2.0);
          cairo_line_to (cr, width / 2.0, 0);

          cairo_close_path (cr);
        }
    }
  else
    {
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        {
          cairo_move_to (cr, width, height / 2.0 - 3.0);
          cairo_arc (cr, width - 2.5, height - 3.5,
                     2.0,
                     0, G_PI_2);

          cairo_arc (cr, 2.5, height - 3.5,
                     2.0,
                     G_PI_2, G_PI);

          cairo_line_to (cr, 0, height / 2.0 - 3.0);
          cairo_line_to (cr, width / 2.0, 0);
          cairo_line_to (cr, width, height / 2.0 - 3.0);

          cairo_close_path (cr);
        }
      else
        {
          cairo_move_to (cr, width / 2.0, height);
          cairo_arc (cr, 2.5, height - 2.5,
                     2.0,
                     G_PI_2, G_PI);

          cairo_arc (cr, 2.5, 2.5,
                     2.0,
                     G_PI, G_PI + G_PI_2);

          cairo_line_to (cr, width / 2.0, 0);
          cairo_line_to (cr, width, height / 2.0);
          cairo_line_to (cr, width / 2.0, height);

          cairo_close_path (cr);
        }
    }
}

static void
render_switch_lines (GtkThemingEngine *engine,
                     cairo_t *cr,
                     gdouble x,
                     gdouble y,
                     gdouble width,
                     gdouble height,
                     GtkOrientation orientation)
{
  GtkStateFlags state;
  GdkRGBA *lines_color;

  state = gtk_theming_engine_get_state (engine);

  if (state & GTK_STATE_FLAG_INSENSITIVE)
    return;

  gtk_theming_engine_get (engine, state,
                          "-sugar-switch-grip-color", &lines_color,
                          NULL);

  cairo_save (cr);

  cairo_translate (cr,
                   x + width / 2.0 - 4.0,
                   y + height / 2.0 - 3.0);

  cairo_move_to (cr, 0.0, 0.0);
  cairo_set_line_width (cr, 2.0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

  cairo_line_to (cr, 0.0, 6.0);
  cairo_move_to (cr, 4.0, 0.0);
  cairo_line_to (cr, 4.0, 6.0);
  cairo_move_to (cr, 8.0, 0.0);
  cairo_line_to (cr, 8.0, 6.0);

  gdk_cairo_set_source_rgba (cr, lines_color);
  cairo_stroke (cr);

  cairo_restore (cr);

  gdk_rgba_free (lines_color);
}

static void
sugar_engine_render_slider (GtkThemingEngine *engine,
                              cairo_t          *cr,
                              gdouble           x,
                              gdouble           y,
                              gdouble           width,
                              gdouble           height,
                              GtkOrientation    orientation)
{
  const GtkWidgetPath *path;

  path = gtk_theming_engine_get_path (engine);
  cairo_save (cr);

  if (gtk_widget_path_is_type (path, GTK_TYPE_SCALE))
    {
      cairo_pattern_t *pattern, *border_pattern;
      GtkStateFlags state;
      GdkRGBA color;
      gboolean marks_above = FALSE, marks_below = FALSE;

      if (gtk_theming_engine_has_class
          (engine, GTK_STYLE_CLASS_SCALE_HAS_MARKS_ABOVE))
        marks_above = TRUE;
      else if (gtk_theming_engine_has_class
               (engine, GTK_STYLE_CLASS_SCALE_HAS_MARKS_BELOW))
        marks_below = TRUE;

      cairo_translate (cr, x, y);

      if ((marks_above && marks_below) ||
          (!marks_above && !marks_below))
        draw_round_slider (cr, width, height);
      else
        draw_mark_slider (cr, width, height, marks_below, orientation);

      state = gtk_theming_engine_get_state (engine);
      cairo_set_line_width (cr, 1.0);

      gtk_theming_engine_get (engine, state,
                              "background-image", &pattern,
                              NULL);

      if (pattern != NULL)
        {
          style_pattern_set_matrix (pattern, width, height, FALSE);
          cairo_set_source (cr, pattern);
        }
      else
        {
          gtk_theming_engine_get_background_color (engine, state, &color);
          gdk_cairo_set_source_rgba (cr, &color);
        }

      cairo_fill_preserve (cr);

      gtk_theming_engine_get (engine, state,
                              "-sugar-border-gradient", &border_pattern,
                              NULL);

      if (border_pattern != NULL)
        {
          style_pattern_set_matrix (border_pattern, width, height, FALSE);
          cairo_set_source (cr, border_pattern);
        }
      else
        {
          gtk_theming_engine_get_border_color (engine, state, &color);
          gdk_cairo_set_source_rgba (cr, &color);
        }

      cairo_stroke (cr);

      if (pattern != NULL)
        cairo_pattern_destroy (pattern);

      if (border_pattern != NULL)
        cairo_pattern_destroy (border_pattern);
    }
  else
    {
      GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_slider
        (engine, cr,
         x, y, width, height,
         orientation);

      render_inset_lines (engine, cr, x, y, width, height);

      if (gtk_widget_path_is_type (path, GTK_TYPE_SWITCH))
        render_switch_lines (engine, cr, x, y, width, height, orientation);
    }

  cairo_restore (cr);
}

static void
sugar_engine_render_handle (GtkThemingEngine *engine,
                              cairo_t          *cr,
                              gdouble           x,
                              gdouble           y,
                              gdouble           width,
                              gdouble           height)
{
  if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_GRIP))
    {
      GdkRGBA bg;
      GtkJunctionSides sides;
      GtkStateFlags state;
      int lx, ly;
      int x_down;
      int y_down;
      int dots;

      state = gtk_theming_engine_get_state (engine);
      gtk_theming_engine_get_background_color (engine, state, &bg);

      /* The number of dots fitting into the area. Just hardcoded to 3 right now. */
      /* dots = MIN (width - 2, height - 2) / 3; */
      dots = 3;

      cairo_save (cr);

      sides = gtk_theming_engine_get_junction_sides (engine);

      switch (sides)
        {
        case GTK_JUNCTION_CORNER_TOPRIGHT:
          x_down = 0;
          y_down = 0;
          cairo_translate (cr, x + width - 4*dots, y + 1);
          break;
        case GTK_JUNCTION_CORNER_BOTTOMRIGHT:
          x_down = 0;
          y_down = 1;
          cairo_translate (cr, x + width - 4*dots, y + height + 1 - 4*dots);
          break;
        case GTK_JUNCTION_CORNER_BOTTOMLEFT:
          x_down = 1;
          y_down = 1;
          cairo_translate (cr, x + 2, y + height + 1 - 4*dots);
          break;
        case GTK_JUNCTION_CORNER_TOPLEFT:
          x_down = 1;
          y_down = 0;
          cairo_translate (cr, x + 2, y + 1);
          break;
        default:
          /* Not implemented. */
          return;
        }

      for (lx = 0; lx < dots; lx++) /* horizontally */
        {
          for (ly = 0; ly <= lx; ly++) /* vertically */
            {
              int mx, my;
              mx = x_down * dots + (1 - x_down * 2) * lx - x_down;
              my = y_down * dots + (1 - y_down * 2) * ly - y_down;

              gdk_cairo_set_source_rgba (cr, &bg);
              cairo_arc (cr,
                         mx * 4 - 1 + 1.5,
                         my * 4 - 1 + 1.5,
                         1.5,
                         0, G_PI * 2.0);

              cairo_fill (cr);
            }
        }

      cairo_restore (cr);
    }
  else if (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_PANE_SEPARATOR))
    {
      GdkRGBA fg;
      GtkStateFlags state;
      gdouble xx, yy;

      state = gtk_theming_engine_get_state (engine);
      gtk_theming_engine_get_color (engine, state, &fg);

      GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_background
        (engine, cr, x, y, width, height);
      GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_frame
        (engine, cr, x, y, width, height);

      cairo_save (cr);
      cairo_set_line_width (cr, 2.0);
      gdk_cairo_set_source_rgba (cr, &fg);

      if (width > height)
        {
          for (xx = x + width / 2 - 12; xx <= x + width / 2 + 12; xx += 6)
            {
              cairo_arc (cr, xx, y + height / 2.0,
                         1.0,
                         0, G_PI * 2.0);
              cairo_fill (cr);
            }
        }
      else
        {
          for (yy = y + height / 2 - 12; yy <= y + height / 2 + 12; yy += 6)
            {
              cairo_arc (cr, x + width / 2.0, yy,
                         1.0,
                         0, G_PI * 2.0);
              cairo_fill (cr);
            }
        }

      cairo_restore (cr);
    }
  else
    {
      GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_handle
        (engine, cr,
         x, y, width, height);
    }
}

/* taken from gtkthemingengine.c */
static GdkPixbuf *
scale_or_ref (GdkPixbuf *src,
              gint       width,
              gint       height)
{
  if (width == gdk_pixbuf_get_width (src) &&
      height == gdk_pixbuf_get_height (src))
    return g_object_ref (src);
  else
    return gdk_pixbuf_scale_simple (src,
                                    width, height,
                                    GDK_INTERP_BILINEAR);
}

static gboolean
lookup_icon_size (GtkThemingEngine *engine,
                  GtkIconSize       size,
                  gint             *width,
                  gint             *height)
{
  GdkScreen *screen;
  GtkSettings *settings;

  screen = gtk_theming_engine_get_screen (engine);
  settings = gtk_settings_get_for_screen (screen);

  return gtk_icon_size_lookup_for_settings (settings, size, width, height);
}

/* Kudos to the gnome-panel guys. */
static void
colorshift_pixbuf (GdkPixbuf *src,
                   GdkPixbuf *dest,
                   gint       shift)
{
  gint i, j;
  gint width, height, has_alpha, src_rowstride, dest_rowstride;
  guchar *target_pixels;
  guchar *original_pixels;
  guchar *pix_src;
  guchar *pix_dest;
  int val;
  guchar r, g, b;

  has_alpha       = gdk_pixbuf_get_has_alpha (src);
  width           = gdk_pixbuf_get_width (src);
  height          = gdk_pixbuf_get_height (src);
  src_rowstride   = gdk_pixbuf_get_rowstride (src);
  dest_rowstride  = gdk_pixbuf_get_rowstride (dest);
  original_pixels = gdk_pixbuf_get_pixels (src);
  target_pixels   = gdk_pixbuf_get_pixels (dest);

  for (i = 0; i < height; i++)
    {
      pix_dest = target_pixels   + i * dest_rowstride;
      pix_src  = original_pixels + i * src_rowstride;

      for (j = 0; j < width; j++)
        {
          r = *(pix_src++);
          g = *(pix_src++);
          b = *(pix_src++);

          val = r + shift;
          *(pix_dest++) = CLAMP (val, 0, 255);

          val = g + shift;
          *(pix_dest++) = CLAMP (val, 0, 255);

          val = b + shift;
          *(pix_dest++) = CLAMP (val, 0, 255);

          if (has_alpha)
            *(pix_dest++) = *(pix_src++);
        }
    }
}

static GdkPixbuf *
sugar_engine_render_icon_pixbuf (GtkThemingEngine    *engine,
                                   const GtkIconSource *source,
                                   GtkIconSize          size)
{
  GdkPixbuf *base_pixbuf;
  GdkPixbuf *scaled;
  GdkPixbuf *stated;
  GtkStateFlags state;
  gint width = 1;
  gint height = 1;

  cairo_surface_t *stated_surface;
  cairo_t *cr;

  base_pixbuf = gtk_icon_source_get_pixbuf (source);
  state = gtk_theming_engine_get_state (engine);

  g_return_val_if_fail (base_pixbuf != NULL, NULL);

  if (size != (GtkIconSize) -1 &&
      !lookup_icon_size (engine, size, &width, &height))
    {
      g_warning (G_STRLOC ": invalid icon size '%d'", size);
      return NULL;
    }

  /* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
   * leave it alone.
   */
  if (size != (GtkIconSize) -1 &&
      gtk_icon_source_get_size_wildcarded (source))
    scaled = scale_or_ref (base_pixbuf, width, height);
  else
    scaled = g_object_ref (base_pixbuf);

  /* If the state was wildcarded, then generate a state. */
  if (gtk_icon_source_get_state_wildcarded (source))
    {
      if (state & GTK_STATE_FLAG_INSENSITIVE)
        {
          /* dim the pixbuf with a 0.5 alpha black layer */
          stated_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                                       gdk_pixbuf_get_width (scaled),
                                                       gdk_pixbuf_get_height (scaled));
          cr = cairo_create (stated_surface);

          gdk_cairo_set_source_pixbuf (cr, scaled, 0, 0);
          cairo_paint (cr);

          cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.5);
          cairo_set_operator (cr, CAIRO_OPERATOR_DEST_IN);
          cairo_paint (cr);

          stated = gdk_pixbuf_get_from_surface (stated_surface,
                                                0, 0,
                                                cairo_image_surface_get_width (stated_surface),
                                                cairo_image_surface_get_height (stated_surface));

          g_object_unref (scaled);
          cairo_destroy (cr);
          cairo_surface_destroy (stated_surface);
        }
      else if (state & GTK_STATE_FLAG_PRELIGHT)
        {
          stated = gdk_pixbuf_copy (scaled);
          colorshift_pixbuf (scaled, stated, 30);
          g_object_unref (scaled);
        }
      else
        {
          stated = scaled;
        }
    }
  else
    {
      stated = scaled;
    }

  return stated;
}

static void
sugar_engine_render_line (GtkThemingEngine *engine,
                            cairo_t *cr,
                            gdouble x0,
                            gdouble y0,
                            gdouble x1,
                            gdouble y1)
{
  const GtkWidgetPath *path;

  path = gtk_theming_engine_get_path (engine);

  if ((gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_MARK) &&
       gtk_widget_path_is_type (path, GTK_TYPE_SCALE)) ||
      (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_SEPARATOR) &&
       gtk_widget_path_is_type (path, GTK_TYPE_TREE_VIEW)))
    {
      GtkStateFlags state;
      GdkRGBA bg;

      state = gtk_theming_engine_get_state (engine);
      gtk_theming_engine_get_background_color (engine, state, &bg);

      cairo_save (cr);

      cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);
      cairo_set_line_width (cr, 1);

      cairo_move_to (cr, x0 + 0.5, y0 + 0.5);
      cairo_line_to (cr, x1 + 0.5, y1 + 0.5);

      gdk_cairo_set_source_rgba (cr, &bg);
      cairo_stroke (cr);

      cairo_restore (cr);
    }
  else
    {
      GTK_THEMING_ENGINE_CLASS (sugar_engine_parent_class)->render_line
        (engine, cr,
         x0, y0, x1, y1);
    }
}

static void
sugar_engine_class_init (AdwaitaEngineClass *klass)
{
  GtkThemingEngineClass *engine_class = GTK_THEMING_ENGINE_CLASS (klass);

  engine_class->render_arrow = sugar_engine_render_arrow;
  engine_class->render_focus = sugar_engine_render_focus;
  engine_class->render_check = sugar_engine_render_check;
  engine_class->render_option = sugar_engine_render_option;
  engine_class->render_extension = sugar_engine_render_extension;
  engine_class->render_frame = sugar_engine_render_frame;
  engine_class->render_background = sugar_engine_render_background;
  engine_class->render_expander = sugar_engine_render_expander;
  engine_class->render_activity = sugar_engine_render_activity;
  engine_class->render_slider = sugar_engine_render_slider;
  engine_class->render_handle = sugar_engine_render_handle;
  engine_class->render_icon_pixbuf = sugar_engine_render_icon_pixbuf;
  engine_class->render_line = sugar_engine_render_line;

  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("focus-border-color",
                                                            "Focus border color",
                                                            "Focus border color",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_int ("focus-border-radius",
                                                          "Focus border radius",
                                                          "Focus border radius",
                                                          0, G_MAXINT, 0,
                                                          0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("focus-border-gradient",
                                                            "Focus border gradient",
                                                            "Focus border gradient",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("focus-fill-color",
                                                            "Focus fill color",
                                                            "Focus fill color",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("selected-tab-color",
                                                            "Selected tab color",
                                                            "Selected tab color",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("border-gradient",
                                                            "Border gradient",
                                                            "Border gradient",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boolean ("focus-border-dashes",
                                                              "Focus border uses dashes",
                                                              "Focus border uses dashes",
                                                              FALSE, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("menuitem-arrow-color",
                                                            "Menuitem arrow color",
                                                            "Menuitem arrow color",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("switch-grip-color",
                                                            "Switch grip color",
                                                            "Switch grip color",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("inset-left",
                                                            "Inset line left",
                                                            "Inset line left",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("inset-right",
                                                            "Inset line right",
                                                            "Inset line right",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("inset-top",
                                                            "Inset line top",
                                                            "Inset line top",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("inset-bottom",
                                                            "Inset line bottom",
                                                            "Inset line bottom",
                                                            GDK_TYPE_RGBA, 0));
  gtk_theming_engine_register_property (ADWAITA_NAMESPACE, NULL,
                                        g_param_spec_boxed ("progressbar-pattern",
                                                            "Progressbar pattern",
                                                            "Progressbar pattern",
                                                            CAIRO_GOBJECT_TYPE_PATTERN, 0));
}

static void
sugar_engine_class_finalize (AdwaitaEngineClass *klass)
{
}

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  sugar_engine_register_types (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkThemingEngine *
create_engine (void)
{
  return GTK_THEMING_ENGINE (g_object_new (ADWAITA_TYPE_ENGINE,
                                           "name", "sugar",
                                           NULL));
}
