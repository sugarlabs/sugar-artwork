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

#ifndef __SUGAR_INFO_H
#define __SUGAR_INFO_H

#include <gtk/gtkwidget.h>
#include "sugar-style.h"
#include "sugar-rc-style.h"

typedef enum
{
    CORNER_NONE        = 0,
    CORNER_TOPLEFT     = 1,
    CORNER_TOPRIGHT    = 2,
    CORNER_BOTTOMLEFT  = 4,
    CORNER_BOTTOMRIGHT = 8,
    CORNER_ALL         = 15
} SugarCorners;

typedef enum {
    EDGE_NONE      = 0,
    EDGE_TOP       = 1 << 0,
    EDGE_BOTTOM    = 1 << 1,
    EDGE_LEFT      = 1 << 2,
    EDGE_RIGHT     = 1 << 3,
    EDGE_ALL       = 0xf
} SugarEdges;

typedef struct {
    GtkWidget *widget;
    GtkStyle *style;
    SugarRcStyle *rc_style;
    GtkStateType  state;
    GtkShadowType shadow;
    gboolean  ltr;
    GdkRectangle pos;
    SugarCorners corners;
    const gchar *detail;
    gdouble max_radius;
    SugarEdges  cont_edges;
} SugarInfo;


typedef struct {
    SugarInfo info;
    GtkOrientation orientation;
    gboolean focused;
    gboolean trough_fill;
} SugarRangeInfo;

typedef struct {
    SugarInfo info;
    GtkArrowType dir;
} SugarArrowInfo;

G_GNUC_INTERNAL void sugar_info_get_style_property (SugarInfo *info, const gchar *property, gpointer dest);
G_GNUC_INTERNAL void sugar_fill_generic_info (SugarInfo *info, GtkStyle *style, GtkStateType state_type, GtkShadowType shadow_type, GtkWidget *widget, const gchar *detail, gint x, gint y, gint width, gint height);
G_GNUC_INTERNAL void sugar_fill_range_info (SugarRangeInfo *range_info, gboolean trough);
G_GNUC_INTERNAL void sugar_remove_corners (SugarCorners *corners, SugarEdges edge);

#endif /* __SUGAR_INFO_H */
