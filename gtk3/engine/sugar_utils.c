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

#include "sugar_utils.h"

void
sugar_trim_allocation_for_scale (GtkThemingEngine *engine,
				   gdouble *x,
				   gdouble *y,
				   gdouble *width,
				   gdouble *height)
{
  const GtkWidgetPath *path;

  path = gtk_theming_engine_get_path (engine);

  if (gtk_widget_path_is_type (path, GTK_TYPE_SCALE) &&
      (gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_TROUGH) ||
       gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_PROGRESSBAR)))
    {
      /* Render GtkScale trough thinner */
      if (!gtk_theming_engine_has_class (engine, GTK_STYLE_CLASS_VERTICAL))
        {
          *y += *height / 2.0 - 2.0;
          *height = 4;
        }
      else
        {
          *x += *width / 2.0 - 2.0;
          *width = 4;
        }
    }
}

void
_cairo_round_rectangle_sides (cairo_t          *cr,
                              gdouble           radius,
                              gdouble           x,
                              gdouble           y,
                              gdouble           width,
                              gdouble           height,
                              guint             sides,
                              GtkJunctionSides  junction)
{
  radius = CLAMP (radius, 0, MIN (width / 2, height / 2));

  if (sides & SIDE_RIGHT)
    {
      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_TOPRIGHT))
        cairo_move_to (cr, x + width, y);
      else
        {
          cairo_new_sub_path (cr);
          cairo_arc (cr, x + width - radius, y + radius, radius, - G_PI / 4, 0);
        }

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_BOTTOMRIGHT))
        cairo_line_to (cr, x + width, y + height);
      else
        cairo_arc (cr, x + width - radius, y + height - radius, radius, 0, G_PI / 4);
    }

  if (sides & SIDE_BOTTOM)
    {
      if (radius != 0 &&
          ! (junction & GTK_JUNCTION_CORNER_BOTTOMRIGHT))
        {
          if ((sides & SIDE_RIGHT) == 0)
            cairo_new_sub_path (cr);

          cairo_arc (cr, x + width - radius, y + height - radius, radius, G_PI / 4, G_PI / 2);
        }
      else if ((sides & SIDE_RIGHT) == 0)
        cairo_move_to (cr, x + width, y + height);

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_BOTTOMLEFT))
        cairo_line_to (cr, x, y + height);
      else
        cairo_arc (cr, x + radius, y + height - radius, radius, G_PI / 2, 3 * (G_PI / 4));
    }
  else
    cairo_move_to (cr, x, y + height);

  if (sides & SIDE_LEFT)
    {
      if (radius != 0 &&
          ! (junction & GTK_JUNCTION_CORNER_BOTTOMLEFT))
        {
          if ((sides & SIDE_BOTTOM) == 0)
            cairo_new_sub_path (cr);

          cairo_arc (cr, x + radius, y + height - radius, radius, 3 * (G_PI / 4), G_PI);
        }
      else if ((sides & SIDE_BOTTOM) == 0)
        cairo_move_to (cr, x, y + height);

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_TOPLEFT))
        cairo_line_to (cr, x, y);
      else
        cairo_arc (cr, x + radius, y + radius, radius, G_PI, G_PI + G_PI / 4);
    }

  if (sides & SIDE_TOP)
    {
      if (radius != 0 &&
          ! (junction & GTK_JUNCTION_CORNER_TOPLEFT))
        {
          if ((sides & SIDE_LEFT) == 0)
            cairo_new_sub_path (cr);

          cairo_arc (cr, x + radius, y + radius, radius, 5 * (G_PI / 4), 3 * (G_PI / 2));
        }
      else if ((sides & SIDE_LEFT) == 0)
        cairo_move_to (cr, x, y);

      if (radius == 0 ||
          (junction & GTK_JUNCTION_CORNER_TOPRIGHT))
        cairo_line_to (cr, x + width, y);
      else
        cairo_arc (cr, x + width - radius, y + radius, radius, 3 * (G_PI / 2), - G_PI / 4);
    }
}

void
_cairo_uneven_frame (cairo_t          *cr,
                     gdouble           radius,
                     gdouble           x,
                     gdouble           y,
                     gdouble           width,
                     gdouble           height,
                     GtkBorder        *border,
                     GtkJunctionSides  junction)
{
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_line_width (cr, 1);

  _cairo_round_rectangle_sides (cr, (gdouble) radius,
                                x, y,
                                width, height,
                                SIDE_ALL, junction);

  _cairo_round_rectangle_sides (cr, (gdouble) radius,
                                x + border->left,
                                y + border->top,
                                width - border->left - border->right,
                                height - border->top - border->bottom,
                                SIDE_ALL, junction);
}

/* Set the appropriate matrix for
 * patterns coming from the style context
 */
void
style_pattern_set_matrix (cairo_pattern_t *pattern,
                          gdouble          width,
                          gdouble          height,
			  gboolean         repeat)
{
  cairo_matrix_t matrix;
  gint w, h;

  if (cairo_pattern_get_type (pattern) == CAIRO_PATTERN_TYPE_SURFACE)
    {
      cairo_surface_t *surface;

      cairo_pattern_get_surface (pattern, &surface);
      w = cairo_image_surface_get_width (surface);
      h = cairo_image_surface_get_height (surface);
    }
  else
    w = h = 1;

  cairo_matrix_init_scale (&matrix, (gdouble) w / width, (gdouble) h / height);
  cairo_pattern_set_matrix (pattern, &matrix);

  if (repeat)
    cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
}

gboolean
sugar_render_from_assets_common (GtkThemingEngine *engine,
                                   cairo_t *cr,
                                   gdouble x,
                                   gdouble y,
                                   gdouble width,
                                   gdouble height)
{
  gboolean retval = FALSE;
  GtkStateFlags state;
  cairo_pattern_t *asset = NULL;
  cairo_surface_t *surface = NULL;

  state = gtk_theming_engine_get_state (engine);
  gtk_theming_engine_get (engine, state, 
                          "background-image", &asset,
                          NULL);

  if (asset != NULL)
    cairo_pattern_get_surface (asset, &surface);

  if (surface != NULL)
    {
      cairo_save (cr);

      cairo_set_source_surface (cr, surface, x, y);
      cairo_scale (cr,
                   width / cairo_image_surface_get_width (surface),
                   height / cairo_image_surface_get_height (surface));

      cairo_paint (cr);

      cairo_restore (cr);
      retval = TRUE;
    }

  if (asset != NULL)
    cairo_pattern_destroy (asset);

  return retval;
}
