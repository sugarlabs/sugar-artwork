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

static void
olpc_rounded_rectangle_corner (cairo_t *cr,
                        double x, double y, double w, double h,
                        double radius, int corner)
{
	switch (corner) {
	case CORNER_TOPLEFT:
		cairo_arc (cr, x+radius, y+radius, radius, M_PI, M_PI * 1.5);
		cairo_line_to (cr, x+radius, y);
		cairo_line_to (cr, x, y);
		cairo_line_to (cr, x, y+radius);
		cairo_close_path (cr);
		break;
	case CORNER_TOPRIGHT:
		cairo_arc (cr, x+w-radius, y+radius, radius, M_PI * 1.5, M_PI * 2);
		cairo_line_to (cr, x+w, y+radius);
		cairo_line_to (cr, x+w, y);
		cairo_line_to (cr, x+w-radius, y);
		cairo_close_path (cr);
		break;
	case CORNER_BOTTOMRIGHT:
		cairo_arc (cr, x+w-radius, y+h-radius, radius, 0, M_PI * 0.5);
		cairo_line_to (cr, x+w-radius, y+h);
		cairo_line_to (cr, x+w, y+h);
		cairo_line_to (cr, x+w, y+h-radius);
		cairo_close_path (cr);
		break;
	case CORNER_BOTTOMLEFT:
		cairo_arc (cr, x+radius, y+h-radius, radius, M_PI * 0.5, M_PI);
		cairo_line_to (cr, x, y+h-radius);
		cairo_line_to (cr, x, y+h);
		cairo_line_to (cr, x+radius, y+h);
		cairo_close_path (cr);
		break;
	default:
		break;
	}
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
create_linear_pattern(GdkColor *start, double start_a, GdkColor *stop, double stop_a, int width, int height)
{
	cairo_pattern_t *pattern;

	pattern = cairo_pattern_create_linear(0, 0, width, height);
	
	cairo_pattern_add_color_stop_rgb(pattern, start_a,
									 start->red / 65535.,
					    			 start->green / 65535.,
					    			 start->blue / 65535.);
	cairo_pattern_add_color_stop_rgb(pattern, stop_a,
					      			 stop->red / 65535.,
					      			 stop->green / 65535.,
					      			 stop->blue / 65535.);

	return pattern;
}

static void
draw_button (GtkStyle     *style,
			 GtkStateType  state,
			 GtkWidget    *widget,
			 GdkWindow    *window,
			 GdkRectangle *area,
			 int           x,
			 int		   y,
			 int           width,
			 int           height)

{
    OlpcRcStyle *rc_style = OLPC_RC_STYLE (style->rc_style);
	cairo_t *cr;
	cairo_pattern_t *pattern;
	
	cr = gdk_cairo_create(window);

	cairo_translate(cr, x, y);

	olpc_rounded_rectangle(cr, 0, 0, width, height,
						   5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	
	pattern = create_linear_pattern(&rc_style->top_color[state], 0.0,
					&rc_style->bottom_color[state], 1.0,
					0,
					height);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);
	cairo_fill_preserve(cr);
	
	set_cairo_color(cr, style->fg[state]);
	cairo_set_line_width(cr, 2);
	cairo_stroke(cr);
	
	cairo_destroy(cr);
}

static void
draw_scrolled_window (GtkStyle     *style,
			GtkStateType  state,
			GtkWidget    *widget,
			GdkWindow    *window,
			GdkRectangle *area,
			int           x,
			int			  y,
			int           width,
			int           height)
{
	cairo_t *cr;
	
	cr = gdk_cairo_create(window);
	cairo_translate(cr, x, y);

	set_cairo_color(cr, style->fg[state]);
	olpc_rounded_rectangle(cr, 0, 0, width, height,
			       5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
			       CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_set_line_width (cr, 1);
	cairo_stroke(cr);

	cairo_destroy(cr);
}

static void
draw_entry (GtkStyle     *style,
			GtkStateType  state,
			GtkWidget    *widget,
			GdkWindow    *window,
			GdkRectangle *area,
			int           x,
			int			  y,
			int           width,
			int           height)
{
	cairo_t *cr;
	const double radius = 5.0;
	
	cr = gdk_cairo_create(window);
	cairo_translate(cr, x, y);

	cairo_save(cr);
	cairo_reset_clip(cr);
	cairo_rectangle(cr, x, y, width, height);
	cairo_clip(cr);

	cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.10);
	cairo_arc (cr, x+radius+1, y+height-radius, radius, M_PI * 0.5, M_PI);
	cairo_arc (cr, x+radius+1, y+radius+1, radius, M_PI, M_PI * 1.5);
	cairo_line_to (cr, x+width, y+1);
	cairo_stroke(cr);

	set_cairo_color(cr, style->fg[state]);
	olpc_rounded_rectangle(cr, 0, 0, width, height,
						   5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_set_line_width (cr, 1);
	cairo_stroke(cr);

	olpc_rounded_rectangle_corner (cr, 0, 0, width, height,
							5.0, CORNER_TOPLEFT);
	cairo_fill (cr);
	olpc_rounded_rectangle_corner (cr, 0, 0, width, height,
							5.0, CORNER_TOPRIGHT);
	cairo_fill (cr);
	olpc_rounded_rectangle_corner (cr, 0, 0, width, height,
							5.0, CORNER_BOTTOMRIGHT);
	cairo_fill (cr);
	olpc_rounded_rectangle_corner (cr, 0, 0, width, height,
							5.0, CORNER_BOTTOMLEFT);
	cairo_fill (cr);

	cairo_restore(cr);

	cairo_destroy(cr);
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
    GtkStateType state = GTK_STATE_NORMAL;

    /* GtkWindow paints out of realize() to allow the
     * theme to set the window background pixmap
     */
    if (!GTK_WIDGET_MAPPED (widget))
	return;

    if (!rc_style->top_color[state].pixel)
	top_color = style->bg[widget->state];
    else
      top_color = rc_style->top_color[state];

    if (!rc_style->bottom_color[state].pixel)
	bottom_color = style->bg[widget->state];
    else
	bottom_color = rc_style->bottom_color[state];

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
olpc_draw_shadow (GtkStyle        *style,
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
    if (strcmp(detail, "entry") == 0) {
	draw_entry(style, state_type, widget, window, area,
		  x, y, width, height);
    } else if (strcmp(detail, "scrolled_window") == 0) {
	draw_scrolled_window(style, state_type, widget, window, area,
		             x, y, width, height);
    } else if (strcmp(detail, "toolbar") == 0) {
    } else {
		olpc_style_parent_class->draw_shadow (style, window,
							    state_type, shadow_type,
							    area, widget, detail,
							    x, y, width, height);
    }
}

static void
draw_trough (GtkStyle        *style,
	         GtkStateType     state_type,
			 GtkWidget       *widget,
			 GdkWindow       *window,
			 GdkRectangle    *area,
			 int              x,
			 int              y,
			 int              width,
			 int              height)
{
	cairo_t *cr;
	cairo_pattern_t *pattern;
	GdkColor left, right, border;

	cr = gdk_cairo_create(window);

	cairo_translate(cr, x, y);

	left.red = 65535 * 0.95;
	left.green = 65535 * 0.94;
	left.blue = 65535 * 0.92;
	right.red = 65535 * 0.99;
	right.green = 65535 * 0.99;
	right.blue = 65535 * 0.98;
	pattern = create_linear_pattern(&left, 1.0,
									&right, 1.0,
									width,
									0);
	cairo_set_source (cr, pattern);
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_fill(cr);
	cairo_pattern_destroy (pattern);

	border.red = 65535 * 0.93;
	border.green = 65535 * 0.93;
	border.blue = 65535 * 0.89;
	olpc_rounded_rectangle(cr, 0, 0, width, height,
						   5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_set_line_width (cr, 1);
	set_cairo_color(cr, border);
	cairo_stroke(cr);

	cairo_destroy(cr);
}

static void
draw_slider (GtkStyle        *style,
	         GtkStateType     state_type,
			 GtkWidget       *widget,
			 GdkWindow       *window,
			 GdkRectangle    *area,
			 int              x,
			 int              y,
			 int              width,
			 int              height)
{
	cairo_t *cr;
	cairo_pattern_t *pattern;
	GdkColor left, right, border;
	double grips_top, grips_left, grips_right, i;

	cr = gdk_cairo_create(window);

	cairo_translate(cr, x, y);

	left.red = 65535 * 0.68;
	left.green = 65535 * 0.93;
	left.blue = 65535 * 0.25;
	right.red = 65535 * 0.63;
	right.green = 65535 * 0.86;
	right.blue = 65535 * 0.23;
	pattern = create_linear_pattern(&left, 1.0,
									&right, 1.0,
									width,
									0);
	cairo_set_source (cr, pattern);
	olpc_rounded_rectangle(cr, 1, 1, width-2, height-2,
						   5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_fill(cr);
	cairo_pattern_destroy (pattern);

#if 0
	border.red = 65535 * 0.56;
	border.green = 65535 * 0.78;
	border.blue = 65535 * 0.20;
	olpc_rounded_rectangle(cr, 1, 1, width, height,
						   5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_set_line_width (cr, 1);
	set_cairo_color(cr, border);
	cairo_stroke(cr);
#endif

	border.red = 65535;
	border.green = 65535;
	border.blue = 65535;
	olpc_rounded_rectangle(cr, 0, 0, width-1, height-1,
						   5.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_set_line_width (cr, 1);
	set_cairo_color(cr, border);
	cairo_stroke(cr);

	/* Draw the grip */
	left.red = 65535 * 0.97;
	left.green = 65535 * 1.0;
	left.blue = 65535 * 0.93;
	right.red = 65535 * 0.56;
	right.green = 65535 * 0.78;
	right.blue = 65535 * 0.20;

	grips_left = (width / 2) - 4;
	grips_right = grips_left + 7;
	grips_top = height / 2 - 4;
	for (i = grips_top; i < grips_top + 8; i += 2)
	{
		cairo_move_to(cr, grips_left, i);
		cairo_line_to(cr, grips_right, i);
		cairo_set_line_width (cr, 1);
		set_cairo_color(cr, left);
		cairo_stroke(cr);

		cairo_move_to(cr, grips_left+1, i+1);
		cairo_line_to(cr, grips_right+1, i+1);
		cairo_set_line_width (cr, 1);
		set_cairo_color(cr, right);		
		cairo_stroke(cr);
	}

	cairo_destroy(cr);
}

static void
draw_stepper (GtkStyle        *style,
	         GtkStateType     state_type,
			 GtkWidget       *widget,
			 GdkWindow       *window,
			 GdkRectangle    *area,
			 int              x,
			 int              y,
			 int              width,
			 int              height)
{
	cairo_t *cr;
	cairo_pattern_t *pattern;
	GdkColor left, right, border;

	cr = gdk_cairo_create(window);

	cairo_translate(cr, x, y);

	left.red = 65535 * 0.68;
	left.green = 65535 * 0.93;
	left.blue = 65535 * 0.25;
	right.red = 65535 * 0.63;
	right.green = 65535 * 0.86;
	right.blue = 65535 * 0.23;
	pattern = create_linear_pattern(&left, 1.0,
									&right, 1.0,
									width,
									0);
	cairo_set_source (cr, pattern);
	olpc_rounded_rectangle(cr, 1, 1, width-2, height-2,
						   3.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_fill(cr);
	cairo_pattern_destroy (pattern);

	border.red = 65535;
	border.green = 65535;
	border.blue = 65535;
	olpc_rounded_rectangle(cr, 0, 0, width-1, height-1,
						   3.0, CORNER_TOPLEFT | CORNER_TOPRIGHT |
						   CORNER_BOTTOMLEFT | CORNER_BOTTOMRIGHT);
	cairo_set_line_width (cr, 1);
	set_cairo_color(cr, border);
	cairo_stroke(cr);

	cairo_destroy(cr);
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
    if ((strcmp(detail, "button") == 0 ||
    	 strcmp(detail, "buttondefault") == 0)) {
    	 draw_button(style, state_type, widget, window,
    	 	     area, x, y, width, height);
    } else if ((strcmp (detail, "trough") == 0) && widget && (GTK_IS_VSCROLLBAR (widget) || GTK_IS_HSCROLLBAR (widget))) {
    	draw_trough(style, state_type, widget, window, area,
    	 		x, y, width, height);
	} else if ((strcmp (detail, "slider") == 0)) {
		draw_slider(style, state_type, widget, window, area,
				x, y, width, height);
	} else if ((strcmp (detail, "vscrollbar") == 0) || (strcmp (detail, "hscrollbar") == 0)) {
		draw_stepper(style, state_type, widget, window, area,
				x, y, width, height);
	} else {
		olpc_style_parent_class->draw_box (style, window,
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
olpc_draw_arrow (GtkStyle        *style,
					GdkWindow       *window,
					GtkStateType     state_type,
					GtkShadowType    shadow_type,
					GdkRectangle    *area,
					GtkWidget       *widget,
					const gchar     *detail,
                    GtkArrowType     arrow_type,
                    gboolean         fill,
					int              x,
					int              y,
					int              width,
					int              height)
{
	if ((strcmp (detail, "vscrollbar") == 0) ||
	    (strcmp (detail, "hscrollbar") == 0) ||
	    (strcmp (detail, "arrow") == 0))  {
		cairo_t *cr;
		double ax = 0, ay = 0, bx = 0, by = 0, cx = 0, cy = 0;

		cr = gdk_cairo_create(window);

		cairo_set_line_width (cr, 2);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, .85);		

		switch (arrow_type)
		{
		case GTK_ARROW_UP:
			ax = x + 1;
			ay = y + height - 1;
			bx = x + (width / 2.0);
			by = y + 2;
			cx = x + width - 1;
			cy = ay;
			break;
		case GTK_ARROW_DOWN:
			ax = x + 1;
			ay = y + 1;
			bx = x + (width / 2.0);
			by = y + height - 2;
			cx = x + width - 1;
			cy = ay;
			break;
		case GTK_ARROW_LEFT:
			ax = x + width - 1.0;
			ay = y + 1.0;
			bx = x + 2.0;
			by = y + (height / 2.0);
			cx = ax;
			cy = y + height - 1;
			break;
		case GTK_ARROW_RIGHT:
			ax = x + 1;
			ay = y + 1.0;
			bx = x + width - 2.0;
			by = y + (height / 2.0);
			cx = ax;
			cy = y + height - 1;
			break;
		default:
			break;
		}
		cairo_move_to (cr, ax, ay);
		cairo_line_to (cr, bx, by);
		cairo_line_to (cr, cx, cy);
		cairo_stroke (cr);

		cairo_destroy(cr);
    } else {
		olpc_style_parent_class->draw_arrow (style, window,
						     state_type, shadow_type,
						     area, widget, detail,
						     arrow_type, fill,
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
	style_class->draw_arrow = olpc_draw_arrow;

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
