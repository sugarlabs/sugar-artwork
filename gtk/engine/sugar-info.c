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
        range_info->focused = GTK_WIDGET_HAS_FOCUS (info->widget);
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

    /* The scale trough is drawn larger than it should be. Subtract from its width/height. */
    if (trough && (HINT ("vscale") || HINT ("hscale"))) {
        gdouble width;

        switch (range_info->orientation) {
            case GTK_ORIENTATION_VERTICAL:
                width = (info->pos.width - line_width) / 2.0 + line_width;

                info->pos.x += (info->pos.width - width) / 2;
                info->pos.width = width;
                break;
            case GTK_ORIENTATION_HORIZONTAL:
                width = (info->pos.height - line_width) / 2.0 + line_width;

                info->pos.y += (info->pos.height - width) / 2;
                info->pos.height = width;
                break;
        }

        range_info->trough_fill = DETAIL ("trough-lower");

        /* I don't like the ACTIVE state there ... */
        info->state = info->state == GTK_STATE_ACTIVE ? GTK_STATE_NORMAL : GTK_STATE_INSENSITIVE;
    }
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
}
