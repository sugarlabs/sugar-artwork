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

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <cairo.h>
#include <cairo-xlib.h>
#include <gdk/gdkx.h>
#include <gtk/gtkwindow.h>

#include "olpc.h"
#include "olpc-style.h"
#include "olpc-rc-style.h"

static GtkStyleClass *olpc_style_parent_class;

enum {
	CORNER_TOPLEFT = 1,
	CORNER_TOPRIGHT = 2,
	CORNER_BOTTOMLEFT = 4,
	CORNER_BOTTOMRIGHT = 8
};

/******* Drawing functions, split them out at some point **********/

static void
olpc_rounded_rectangle (cairo_t *cr,
                        double x, double y, double w, double h,
                        double radius, int corners)
{
	if (corners & CORNER_TOPLEFT)
		cairo_move_to (cr, x+radius, y);
	else
		cairo_move_to (cr, x, y);
	
	if (corners & CORNER_TOPRIGHT)
		cairo_arc (cr, x+w-radius, y+radius, radius, M_PI * 1.5, M_PI * 2);
	else
		cairo_line_to (cr, x+w, y);
	
	if (corners & CORNER_BOTTOMRIGHT)
		cairo_arc (cr, x+w-radius, y+h-radius, radius, 0, M_PI * 0.5);
	else
		cairo_line_to (cr, x+w, y+h);
	
	if (corners & CORNER_BOTTOMLEFT)
		cairo_arc (cr, x+radius,   y+h-radius, radius, M_PI * 0.5, M_PI);
	else
		cairo_line_to (cr, x, y+h);
	
	if (corners & CORNER_TOPLEFT)
		cairo_arc (cr, x+radius,   y+radius,   radius, M_PI, M_PI * 1.5);
	else
		cairo_line_to (cr, x, y);
}

/************************************************************************/

static void
set_cairo_color (cairo_t *cr, GdkColor color)
{
	double r, g, b;

	r = (double)color.red / (double)65535;
	g = (double)color.green / (double)65535;
	b = (double)color.blue / (double)65535;
	
	cairo_set_source_rgb (cr, r, g, b);
}

static cairo_pattern_t *
create_linear_pattern(GdkColor *top, GdkColor *bottom, int height)
{
	cairo_pattern_t *pattern;

	pattern = cairo_pattern_create_linear(0, 0, 0, height);
	
	cairo_pattern_add_color_stop_rgb(pattern, 0.0,
									 top->red / 65535.,
					    			 top->green / 65535.,
					    			 top->blue / 65535.);
	cairo_pattern_add_color_stop_rgb(pattern, 1.0,
					      			 bottom->red / 65535.,
					      			 bottom->green / 65535.,
					      			 bottom->blue / 65535.);

	return pattern;
}

static void
draw_button (GtkStyle     *style,
			 GtkStateType  state,
			 GtkWidget    *widget,
			 GdkWindow    *window,
			 GdkRectangle *area)
{
    OlpcRcStyle *rc_style = OLPC_RC_STYLE (style->rc_style);
	cairo_t *cr;
	cairo_pattern_t *pattern;
	
	cr = gdk_cairo_create(window);

	cairo_translate(cr, area->x, area->y);

	cairo_rectangle(cr, 0, 0, area->width, area->height);
	
	pattern = create_linear_pattern(&rc_style->window_top_color,
									&rc_style->window_bottom_color,
									area->height);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);
	cairo_fill_preserve(cr);
	
	set_cairo_color(cr, style->fg[state]);
	cairo_stroke(cr);
	
	cairo_destroy(cr);
}

static void
draw_entry (GtkStyle     *style,
			GtkWidget    *widget,
			GdkWindow    *window,
			GdkRectangle *area)
{
	cairo_t *cr;
	const float RADIUS = 10.0;

	cr = gdk_cairo_create (window);
	
	cairo_translate(cr, area->x + 0.5, area->y + 0.5);
	cairo_set_line_width(cr, 1.0);
	
	/* Fill the entry's base color */
	cairo_rectangle(cr, 1.5, 1.5, area->width - 4, area->height - 4);
	set_cairo_color(cr, style->base[0]);
	cairo_fill(cr);
	
	/* Draw the border */
	set_cairo_color(cr, style->fg[0]);
	olpc_rounded_rectangle(cr, 1, 1, area->width - 3, area->height - 3,
						   RADIUS, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_stroke (cr);
	cairo_destroy (cr);
}

static void
draw_window_background (GtkStyle     *style,
						GtkWidget    *widget,
						GdkWindow    *window,
						GdkRectangle *area)
{
    OlpcRcStyle *rc_style = OLPC_RC_STYLE (style->rc_style);
    cairo_t *cr;
    GdkColor top_color;
    GdkColor bottom_color;

    /* GtkWindow paints out of realize() to allow the
     * theme to set the window background pixmap
     */
    if (!GTK_WIDGET_MAPPED (widget))
	return;

    if (!rc_style->window_top_color.pixel)
	top_color = style->bg[widget->state];
    else
      top_color = rc_style->window_top_color;

    if (!rc_style->window_bottom_color.pixel)
	bottom_color = style->bg[widget->state];
    else
	bottom_color = rc_style->window_bottom_color;

	cr = gdk_cairo_create (window);

    if (top_color.red == bottom_color.red &&
	top_color.green == bottom_color.green &&
	top_color.blue == bottom_color.blue) {

	cairo_set_source_rgb (cr,
			      top_color.red / 65535.,
			      top_color.green / 65535.,
			      top_color.blue / 65535.);
	
    } else {
	cairo_pattern_t *pattern;
	int height;
	
	gdk_drawable_get_size (window, 0, &height);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	if (pattern) {
	  cairo_pattern_add_color_stop_rgb (pattern, 0.0,
					    top_color.red / 65535.,
					    top_color.green / 65535.,
					    top_color.blue / 65535.);
	    cairo_pattern_add_color_stop_rgb (pattern, 1.0,
					      bottom_color.red / 65535.,
					      bottom_color.green / 65535.,
					      bottom_color.blue / 65535.);
	    cairo_set_source (cr, pattern);
	    cairo_pattern_destroy (pattern);
	}
    }

    cairo_paint (cr);
	cairo_destroy (cr);    
}

static void
olpc_draw_box (GtkStyle        *style,
			   GdkWindow       *window,
			   GtkStateType     state_type,
			   GtkShadowType    shadow_type,
			   GdkRectangle    *area,
			   GtkWidget       *widget,
			   const gchar     *detail,
			   int              x,
			   int              y,
			   int              width,
			   int              height)
{
    if (strcmp(detail, "button") == 0 ||
    	strcmp(detail, "buttondefault") == 0) {
		draw_button(style, state_type, widget, window, area);
    } else {
		olpc_style_parent_class->draw_flat_box (style, window,
							    state_type, shadow_type,
							    area, widget, detail,
							    x, y, width, height);
    }
}

static void
olpc_draw_flat_box (GtkStyle        *style,
					GdkWindow       *window,
					GtkStateType     state_type,
					GtkShadowType    shadow_type,
					GdkRectangle    *area,
					GtkWidget       *widget,
					const gchar     *detail,
					int              x,
					int              y,
					int              width,
					int              height)
{
    if (widget && GTK_IS_WINDOW (widget)) {
		draw_window_background(style, widget, window, area);
    } else {
		olpc_style_parent_class->draw_flat_box (style, window,
							    state_type, shadow_type,
							    area, widget, detail,
							    x, y, width, height);
    }
}

static void
olpc_draw_shadow (GtkStyle      *style,
                  GdkWindow     *window,
                  GtkStateType   state_type,
                  GtkShadowType  shadow_type,
                  GdkRectangle  *area,
                  GtkWidget     *widget,
                  const gchar   *detail,
                  int            x,
                  int            y,
                  int            width,
                  int            height)
{
    if (strcmp(detail, "entrylll") == 0) {
		draw_entry(style, widget, window, area);
    } else {
		olpc_style_parent_class->draw_shadow (style, window,
							    state_type, shadow_type,
							    area, widget, detail,
							    x, y, width, height);
    }
}

static void
olpc_style_init (OlpcStyle *style)
{
}

static void
olpc_style_class_init (OlpcStyleClass *klass)
{
	GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

	style_class->draw_flat_box = olpc_draw_flat_box;
	style_class->draw_box = olpc_draw_box;
	style_class->draw_shadow = olpc_draw_shadow;
        
	olpc_style_parent_class = g_type_class_peek_parent (klass);
}

GType olpc_type_style = 0;

void
olpc_style_register_type (GTypeModule *module)
{
    static const GTypeInfo object_info = {
	sizeof (OlpcStyleClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) olpc_style_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data */
	sizeof (OlpcStyle),
	0,              /* n_preallocs */
	(GInstanceInitFunc) olpc_style_init,
  };
  
  olpc_type_style = g_type_module_register_type (module,
						 GTK_TYPE_STYLE,
						 "OlpcStyle",
						 &object_info, 0);
}
