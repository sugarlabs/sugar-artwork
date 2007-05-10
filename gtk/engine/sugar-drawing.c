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

#include "sugar-drawing.h"

static void
sugar_rounded_rectangle (cairo_t     *cr,
                         gdouble      x,
                         gdouble      y,
                         gdouble      width,
                         gdouble      height,
                         gdouble      radius,
                         SugarCorners corners)
{
    /* Make sure the radius is sane. */
    if ((corners & (CORNER_TOPRIGHT | CORNER_TOPLEFT)) && (corners & (CORNER_BOTTOMRIGHT | CORNER_BOTTOMLEFT)))
        radius = MIN (radius, height / 2.0);
    else
        radius = MIN (radius, height);
    
    if ((corners & (CORNER_TOPRIGHT | CORNER_BOTTOMRIGHT)) && (corners & (CORNER_TOPLEFT | CORNER_BOTTOMLEFT)))
        radius = MIN (radius, width / 2.0);
    else
        radius = MIN (radius, width);

    if (corners == CORNER_NONE || radius < 0.001) {
        cairo_rectangle (cr, x, y, width, height);
        return;
    }

    /* cairo_move_to (cr, x + radius, y); */
    if (corners & CORNER_TOPRIGHT) {
        cairo_line_to (cr, x + width - radius, y);
        cairo_arc (cr, x + width - radius, y + radius, radius, -G_PI_2, 0);
    } else {
        cairo_line_to (cr, x + width, y);
    }
    if (corners & CORNER_BOTTOMRIGHT) {
        cairo_line_to (cr, x + width, y + height - radius);
        cairo_arc (cr, x + width - radius, y + height - radius, radius,  0, G_PI_2);
    } else {
        cairo_line_to (cr, x + width, y + height);
    }
    if (corners & CORNER_BOTTOMLEFT) {
        cairo_line_to (cr, x + radius, y + height);
        cairo_arc (cr, x + radius, y + height - radius, radius, G_PI_2, G_PI);
    } else {
        cairo_line_to (cr, x, y + height);
    }
    if (corners & CORNER_TOPLEFT) {
        cairo_line_to (cr, x, y + radius);
        cairo_arc (cr, x + radius, y + radius, radius, G_PI, G_PI + G_PI_2);
    } else {
        cairo_line_to (cr, x, y);
    }

    cairo_close_path (cr);
}

/* Helper function to stroke a box so that the outside of the stroke is on the given path. */
static void
sugar_rounded_inner_stroke (cairo_t     *cr,
                            gdouble      x,
                            gdouble      y,
                            gdouble      width,
                            gdouble      height,
                            gdouble      outline_width,
                            gdouble      radius,
                            SugarCorners corners)
{
    cairo_set_line_width (cr, outline_width);
    sugar_rounded_rectangle (cr, x + outline_width / 2.0, y + outline_width / 2.0, width - outline_width, height - outline_width, MAX (0, radius - outline_width), corners);
    cairo_stroke (cr);
}

void
sugar_fill_background (cairo_t *cr, SugarInfo *info)
{
    GdkColor bg_color = info->style->bg[GTK_STATE_NORMAL];

    /* try to lookup the symbolic color bg_color */
    gtk_style_lookup_color (info->style, "bg_color", &bg_color);

    gdk_cairo_set_source_color (cr, &bg_color);
    cairo_paint (cr);
}



void
sugar_draw_exterior_focus (cairo_t *cr, SugarInfo *info)
{
    gdouble line_width = info->rc_style->line_width;
    GdkRectangle *pos = &info->pos;
    /* Fallback to fg[NORMAL] */
    GdkColor line_color = info->style->fg[GTK_STATE_NORMAL];

    gtk_style_lookup_color (info->style, "focus_line", &line_color);

    gdk_cairo_set_source_color (cr, &line_color);
    sugar_rounded_inner_stroke (cr, pos->x, pos->y, pos->width, pos->height, line_width, info->max_radius, info->corners);
}

void
sugar_draw_scale_trough (cairo_t *cr, SugarRangeInfo *range_info)
{
    SugarInfo *info = &range_info->info;
    GdkRectangle *pos = &info->pos;

    if (info->state == GTK_STATE_INSENSITIVE) {
        gdouble outline_width = info->rc_style->line_width;
        /* XXX: Needs testing. */
        gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);
        sugar_rounded_inner_stroke (cr, pos->x, pos->y, pos->width, pos->height, outline_width, info->max_radius, info->corners);
        return;
    }

    gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);
    sugar_rounded_rectangle (cr, pos->x, pos->y, pos->width, pos->height, info->max_radius, info->corners);
    cairo_fill (cr);

    if (range_info->trough_fill) {
        gdouble outline_width = info->rc_style->line_width;
        /* Draw the fill for the trough. */
        gdk_cairo_set_source_color (cr, &info->style->bg[GTK_STATE_ACTIVE]);
        sugar_rounded_rectangle (cr, pos->x + outline_width, pos->y + outline_width, pos->width - 2*outline_width, pos->height - 2*outline_width, MAX (0, info->max_radius - outline_width), info->corners);
        cairo_fill (cr);
    }
}

void
sugar_draw_scale_slider (cairo_t *cr, SugarRangeInfo *range_info)
{
    SugarInfo *info = &range_info->info;
    GdkRectangle pos = info->pos;
    GdkColor *fill, *outline, *line;
    gdouble max_radius = info->max_radius;
    gdouble line_width = info->rc_style->line_width;
    
    fill = line = outline = &info->style->bg[GTK_STATE_NORMAL];

    if (info->state == GTK_STATE_INSENSITIVE) {
        fill = NULL;
    } else if ((info->state == GTK_STATE_ACTIVE) ||
               (range_info->focused && info->state == GTK_STATE_NORMAL) ||
               (info->state == GTK_STATE_PRELIGHT)) {
        line = &info->style->fg[GTK_STATE_PRELIGHT];
    } else {
        line = &info->style->bg[GTK_STATE_SELECTED];
    }

    /* from outside to inside */
    /* XXX: There are some artifacts. To fix this the fill should be drawn first -- slightly larger. */
    gdk_cairo_set_source_color (cr, outline);
    sugar_rounded_inner_stroke (cr, pos.x, pos.y, pos.width, pos.height, line_width, max_radius, info->corners);

    max_radius -= line_width;
    pos.x += line_width;
    pos.y += line_width;
    pos.width -= 2*line_width;
    pos.height -= 2*line_width;

    gdk_cairo_set_source_color (cr, line);
    sugar_rounded_inner_stroke (cr, pos.x, pos.y, pos.width, pos.height, line_width, max_radius, info->corners);
    
    max_radius -= line_width;
    pos.x += line_width;
    pos.y += line_width;
    pos.width -= 2*line_width;
    pos.height -= 2*line_width;

    gdk_cairo_set_source_color (cr, outline);
    sugar_rounded_inner_stroke (cr, pos.x, pos.y, pos.width, pos.height, line_width, max_radius, info->corners);

    max_radius -= line_width;
    pos.x += line_width;
    pos.y += line_width;
    pos.width -= 2*line_width;
    pos.height -= 2*line_width;

    if (fill) {
        gdk_cairo_set_source_color (cr, fill);
        sugar_rounded_rectangle (cr, pos.x, pos.y, pos.width, pos.height, max_radius, info->corners);
        cairo_fill (cr);
    }
}

void
sugar_draw_scrollbar_slider (cairo_t       *cr,
                             SugarRangeInfo *range_info)
{
    SugarInfo *info = &range_info->info;
    GdkRectangle *pos = &info->pos;
    GtkStateType state;

    /* This color selection completely bogus. */
    if (info->state == GTK_STATE_NORMAL)
        state = GTK_STATE_ACTIVE;
    else if (info->state == GTK_STATE_ACTIVE)
        state = GTK_STATE_SELECTED;
    else if (info->state == GTK_STATE_PRELIGHT)
        state = GTK_STATE_ACTIVE;
    else
        state = info->state;
    
    gdk_cairo_set_source_color (cr, &info->style->bg[state]);
    sugar_rounded_rectangle (cr, pos->x, pos->y, pos->width, pos->height, info->max_radius, info->corners);
    cairo_fill (cr);
}

void
sugar_draw_entry (cairo_t *cr, SugarInfo *info)
{
    GdkRectangle *pos = &info->pos;

    gdk_cairo_set_source_color (cr, &info->style->base[info->state]);
    sugar_rounded_rectangle (cr, pos->x, pos->y, pos->width, pos->height,
                             info->max_radius, info->corners);
    cairo_fill (cr);
}

void
sugar_draw_button (cairo_t *cr, SugarInfo *info)
{
    GdkRectangle *pos = &info->pos;
    guint radius;

    gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);

    radius = info->max_radius;

    sugar_rounded_rectangle (cr, pos->x, pos->y, pos->width, pos->height, radius, info->corners);
    cairo_fill (cr);
}

void
sugar_draw_button_default (cairo_t *cr, SugarInfo *info)
{
    GdkRectangle *pos = &info->pos;
    gdouble line_width = info->rc_style->line_width;
    guint radius;

    gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);

    /* This is broken! */
    radius = info->max_radius + info->rc_style->line_width + line_width;

    sugar_rounded_inner_stroke (cr, pos->x, pos->y, pos->width, pos->height, line_width, info->max_radius, info->corners);
}



