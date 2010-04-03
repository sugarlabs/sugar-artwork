/*
 * Copyright (C) 2007, Benjamin Berg
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>
#include <math.h>
#include "sugar-info.h"

#define HINT(str) (info->rc_style->hint && g_str_equal (info->rc_style->hint, str))
#define DETAIL(str) (info->detail && g_str_equal (info->detail, str))

static gboolean
sugar_widget_is_ltr (GtkWidget *widget)
{
    GtkTextDirection direction = GTK_TEXT_DIR_NONE;

    if (widget)
        direction = gtk_widget_get_direction (widget);
    
    if (direction == GTK_TEXT_DIR_NONE)
        direction = gtk_widget_get_default_direction ();

    return (direction == GTK_TEXT_DIR_LTR);
}


void
sugar_info_get_style_property (SugarInfo   *info,
                               const gchar *property,
                               gpointer     dest)
{
    if (info->widget) {
        gtk_widget_style_get (info->widget, property, dest, NULL);
    }
}

void
sugar_remove_corners (SugarCorners *corners,
                      SugarEdges    edges)
{
    if (edges & EDGE_TOP)
        *corners &= ~(CORNER_TOPRIGHT | CORNER_TOPLEFT);
    if (edges & EDGE_BOTTOM)
        *corners &= ~(CORNER_BOTTOMRIGHT | CORNER_BOTTOMLEFT);
    if (edges & EDGE_LEFT)
        *corners &= ~(CORNER_TOPLEFT | CORNER_BOTTOMLEFT);
    if (edges & EDGE_RIGHT)
        *corners &= ~(CORNER_TOPRIGHT | CORNER_BOTTOMRIGHT);
}

void
sugar_fill_range_info (SugarRangeInfo *range_info, gboolean trough)
{
    SugarInfo *info = &range_info->info;
    gdouble border_size;
    gdouble line_width = info->rc_style->line_width;

    if (info->widget) {
        range_info->focused = gtk_widget_has_focus (info->widget);
    } else {
        /* Fall back to unfocused and orientation from the width/height. */
        range_info->focused = FALSE;
    }

    if (HINT ("vscale")) {
        range_info->orientation = GTK_ORIENTATION_VERTICAL;
    } else if (HINT ("hscale")) {
        range_info->orientation = GTK_ORIENTATION_HORIZONTAL;
    } else {
        /* use size for the scrollbars, orientation just does not matter ... */
        range_info->orientation = info->pos.width >= info->pos.height ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL;
    }

    border_size = 2*line_width;

    /* The scale trough is drawn larger than it should be. Subtract from its width/height.
     * Also set the corners accordingly, and in the future we may want do some other things. */
    if (trough && (HINT ("vscale") || HINT ("hscale"))) {
        gdouble width;
        gboolean inverted = FALSE;

        switch (range_info->orientation) {
            case GTK_ORIENTATION_VERTICAL:
                width = (info->pos.width - line_width) / 2.0 + line_width;

                info->pos.x += (info->pos.width - width) / 2;
                info->pos.width = width;

                info->pos.y += floor(width/2.0);
                info->pos.height -= 2*floor(width/2.0);

                if (DETAIL ("trough-lower") || DETAIL ("trough-upper")) {
                    /* If there is no real scale, assume that it is not inverted. */
                    if (info->widget && GTK_IS_RANGE (info->widget) && gtk_range_get_inverted (GTK_RANGE (info->widget)))
                        inverted = TRUE;
                    
                    if (DETAIL ("trough-upper"))
                        inverted ^= TRUE;  
                    
                    if (inverted)
                        sugar_remove_corners (&info->corners, EDGE_TOP);
                    else
                        sugar_remove_corners (&info->corners, EDGE_BOTTOM);
                }

                break;
            case GTK_ORIENTATION_HORIZONTAL:
                width = (info->pos.height - line_width) / 2.0 + line_width;

                info->pos.y += (info->pos.height - width) / 2;
                info->pos.height = width;

                info->pos.x += floor(width/2.0);
                info->pos.width -= 2*floor(width/2.0);

                if (DETAIL ("trough-lower") || DETAIL ("trough-upper")) {
                    /* If there is no real scale, assume that it is not inverted. */
                    if (info->widget && GTK_IS_RANGE (info->widget) && gtk_range_get_inverted (GTK_RANGE (info->widget)))
                        inverted = TRUE;
                    
                    if (DETAIL ("trough-upper"))
                        inverted ^= TRUE;  
                    
                    if (inverted)
                        sugar_remove_corners (&info->corners, EDGE_LEFT);
                    else
                        sugar_remove_corners (&info->corners, EDGE_RIGHT);
                }

                break;
        }
        
        range_info->trough_fill = DETAIL ("trough-lower");

        /* I don't like the ACTIVE state there ... */
        info->state = info->state == GTK_STATE_ACTIVE ? GTK_STATE_NORMAL : GTK_STATE_INSENSITIVE;
    }

    if (!trough && (HINT ("vscrollbar") || HINT ("hscrollbar"))) {
        if (HINT ("hscrollbar")) {
            info->pos.y += info->rc_style->scrollbar_border;
            info->pos.height -= 2 * info->rc_style->scrollbar_border;
        } else {
            info->pos.x += info->rc_style->scrollbar_border;
            info->pos.width -= 2 * info->rc_style->scrollbar_border;
        }
    } else if (!trough && (HINT ("vscrollbar-scrolled-window") ||
                           HINT ("hscrollbar-scrolled-window"))) {
        if (HINT ("hscrollbar-scrolled-window")) {
            info->pos.height -= info->rc_style->scrollbar_border;
        } else {
            if (!info->ltr)
                info->pos.x += info->rc_style->scrollbar_border;
            info->pos.width -= info->rc_style->scrollbar_border;
        }
    } else if (trough && (HINT ("vscrollbar-scrolled-window") ||
                          HINT ("hscrollbar-scrolled-window"))) {
        if (HINT ("hscrollbar-scrolled-window")) {
            info->pos.x -= info->rc_style->scrollbar_border;
            info->pos.width += 2*info->rc_style->scrollbar_border;
        } else {
            info->pos.y -= info->rc_style->scrollbar_border;
            info->pos.height += 2*info->rc_style->scrollbar_border;
        }
    }
}

void
sugar_fill_arrow_info (SugarArrowInfo *arrow_info,
                       GtkArrowType    arrow_type)
{
    SugarInfo *info = &arrow_info->info;
    GdkColor *color;

    arrow_info->dir = arrow_type;
    arrow_info->filled_triangle = FALSE;

    /* If the background is black, then we draw a solid triangle. */
    if (info->rc_style->color_flags & SUGAR_COLOR_BG)
        color = &info->rc_style->colors[SUGAR_COLOR_BG];
    else
        color = &info->style->bg[GTK_STATE_NORMAL];
    if (color->red == 0 && color->green == 0 && color->blue == 0)
        arrow_info->filled_triangle = TRUE;
}

void
sugar_fill_generic_info (SugarInfo     *info,
                         GtkStyle      *style,
                         GtkStateType   state_type,
                         GtkShadowType  shadow_type,
                         GtkWidget     *widget,
                         const gchar   *detail,
                         gint           x,
                         gint           y,
                         gint           width,
                         gint           height)
{
    info->style = style;
    info->rc_style = SUGAR_RC_STYLE (style->rc_style);
    info->widget = widget;
    info->state = state_type;
    info->shadow = shadow_type;
    info->corners = CORNER_ALL;
    info->detail = detail;    
    info->pos.x = x;
    info->pos.y = y;
    info->pos.width = width;
    info->pos.height = height;
    /* copy the radius setting so we can modify it for eg. the focus */
    info->max_radius = info->rc_style->max_radius;
    info->cont_edges = EDGE_NONE;

    info->ltr = sugar_widget_is_ltr (widget);

    /* nasty little special cases ... */
    if (!DETAIL ("palette-invoker") && !DETAIL ("toolbutton-prelight")) {
        info->pos.x += info->rc_style->fake_padding;
        info->pos.y += info->rc_style->fake_padding;
        info->pos.width -= 2*info->rc_style->fake_padding;
        info->pos.height -= 2*info->rc_style->fake_padding;
    }

    /* Ignore the prelight state in some cases. */
    if (info->state == GTK_STATE_PRELIGHT &&
	gdk_color_equal(&style->bg[GTK_STATE_PRELIGHT],
			&style->bg[GTK_STATE_NORMAL])) {
        if (DETAIL ("button") || DETAIL ("buttondefault") ||
            DETAIL ("spinbutton_down") || DETAIL ("spinbutton_up")) {

            if (info->shadow == GTK_SHADOW_IN) {
                info->state = GTK_STATE_ACTIVE;
            } else {
                info->state = GTK_STATE_NORMAL;

                /* Set the shadow to NONE if the normal state is invisible. */
                if (widget && GTK_IS_BUTTON (widget)) {
                    GtkReliefStyle relief;

                    relief = gtk_button_get_relief (GTK_BUTTON (widget));

                    if (relief == GTK_RELIEF_NONE)
                        info->shadow = GTK_SHADOW_NONE;
                }
            }
        }
    }
}

void
sugar_clip_gap (cairo_t *cr,
                SugarInfo *info,
                SugarGapInfo *gap,
                gdouble padding,
                gdouble size)
{
    SugarRectangle *pos = &info->pos;

    /* Don't clip anything, if there is no gap. */
    if (gap == NULL)
        return;

    /* Nothing clipped away. */
    if (2*padding >= gap->size)
        return;

    /* The whole area, then subtract the correct region. */
    cairo_rectangle (cr, pos->x, pos->y, pos->width, pos->height);
    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);

    /* This assumes that there is enough space for size ... */
    switch (gap->side) {
        case GTK_POS_TOP:
            cairo_rectangle (cr, pos->x + gap->start + padding, pos->y, gap->size - 2*padding, size);
            break;
        case GTK_POS_BOTTOM:
            cairo_rectangle (cr, pos->x + gap->start + padding, pos->y + pos->height - size, gap->size - 2*padding, size);
            break;
        case GTK_POS_LEFT:
            cairo_rectangle (cr, pos->x, pos->y + gap->start + padding, size, gap->size - 2*padding);
            break;
        case GTK_POS_RIGHT:
            cairo_rectangle (cr, pos->x + pos->width - size, pos->y + gap->start + padding, size, gap->size - 2*padding);
            break;
    }
    cairo_clip (cr);
}

