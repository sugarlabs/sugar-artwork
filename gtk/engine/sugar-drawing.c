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
sugar_cairo_rectangle (cairo_t        *cr,
                       SugarRectangle *rect)
{
    cairo_rectangle (cr, rect->x, rect->y, rect->width, rect->height);
}

static void
sugar_rounded_rectangle (cairo_t      *cr,
                         SugarRectangle *pos,
                         gdouble       padding,
                         gdouble       radius,
                         SugarCorners  corners)
{
    gdouble x, y, width, height;
    
    x = pos->x + padding;
    y = pos->y + padding;
    width  = pos->width  - 2*padding;
    height = pos->height - 2*padding;
    radius = MAX (0, radius - padding);
    
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

    if (corners & CORNER_TOPRIGHT) {
        cairo_move_to (cr, x + width - radius, y);
        cairo_arc (cr, x + width - radius, y + radius, radius, -G_PI_2, 0);
    } else {
        cairo_move_to (cr, x + width, y);
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
sugar_rounded_inner_stroke (cairo_t      *cr,
                            SugarRectangle *pos,
                            gdouble      outline_width,
                            gdouble      radius,
                            SugarCorners corners,
                            SugarEdges   cont_edges)
{
    SugarRectangle real_pos = *pos;

    cairo_save (cr);

    cairo_set_line_width (cr, outline_width);

    sugar_remove_corners (&corners, cont_edges);

    sugar_cairo_rectangle (cr, pos);
    cairo_clip (cr);

    /* This is not great, duplicated from above, because otherwise the radius may be too large. */
    radius = MAX (0, radius - outline_width / 2.0);
    
    /* Make sure the radius is sane. */
    if ((corners & (CORNER_TOPRIGHT | CORNER_TOPLEFT)) && (corners & (CORNER_BOTTOMRIGHT | CORNER_BOTTOMLEFT)))
        radius = MIN (radius, pos->height / 2.0);
    else
        radius = MIN (radius, pos->height);
    
    if ((corners & (CORNER_TOPRIGHT | CORNER_BOTTOMRIGHT)) && (corners & (CORNER_TOPLEFT | CORNER_BOTTOMLEFT)))
        radius = MIN (radius, pos->width / 2.0);
    else
        radius = MIN (radius, pos->width);


    if (cont_edges & EDGE_TOP) {
        real_pos.y -= outline_width;
        real_pos.height += outline_width;
    }
    if (cont_edges & EDGE_BOTTOM) {
        real_pos.height += outline_width;
    }
    if (cont_edges & EDGE_LEFT) {
        real_pos.x -= outline_width;
        real_pos.width += outline_width;
    }
    if (cont_edges & EDGE_RIGHT) {
        real_pos.width += outline_width;
    }

    sugar_rounded_rectangle (cr, &real_pos, outline_width / 2.0, radius, corners);
    cairo_stroke (cr);

    cairo_restore (cr);
}

static void
sugar_draw_insensitive_outline (cairo_t *cr, SugarInfo *info)
{
    if (info->state == GTK_STATE_INSENSITIVE) {
        gdk_cairo_set_source_color (cr, &info->style->fg[GTK_STATE_INSENSITIVE]);
        sugar_rounded_inner_stroke (cr, &info->pos, info->rc_style->line_width, info->max_radius, info->corners, info->cont_edges);
    }
}



void
sugar_fill_background (cairo_t *cr, SugarInfo *info)
{
    GdkColor bg_color = info->style->bg[GTK_STATE_INSENSITIVE];

    /* bg[INSENSITIVE] (and base[INSENSITIVE]) should be set to the window background. */
    gdk_cairo_set_source_color (cr, &bg_color);
    cairo_paint (cr);
}

void
sugar_draw_exterior_focus (cairo_t *cr, SugarInfo *info)
{
    gdouble line_width = info->rc_style->line_width;
    SugarRectangle *pos = &info->pos;
    /* Fallback to fg[NORMAL] */
    GdkColor line_color = info->style->fg[GTK_STATE_NORMAL];

    gtk_style_lookup_color (info->style, "focus_line", &line_color);

    gdk_cairo_set_source_color (cr, &line_color);
    sugar_rounded_inner_stroke (cr, pos, line_width, info->max_radius, info->corners, info->cont_edges);
}

void
sugar_draw_scale_trough (cairo_t *cr, SugarRangeInfo *range_info)
{
    SugarInfo *info = &range_info->info;
    SugarRectangle *pos = &info->pos;

    if (info->state == GTK_STATE_INSENSITIVE) {
        gdouble outline_width = info->rc_style->line_width;
        gdk_cairo_set_source_color (cr, &info->style->fg[info->state]);
        sugar_rounded_inner_stroke (cr, pos, outline_width, info->max_radius, info->corners, info->cont_edges);
        return;
    }

    gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);
    sugar_rounded_rectangle (cr, pos, 0, info->max_radius, info->corners);
    cairo_fill (cr);

    if (range_info->trough_fill) {
        gdouble outline_width = info->rc_style->line_width;
        /* Draw the fill for the trough. */
        gdk_cairo_set_source_color (cr, &info->style->bg[GTK_STATE_ACTIVE]);

        /* Prevent drawing glitches on focused scales. */
        if (!range_info->focused)
            sugar_rounded_rectangle (cr, pos, outline_width, info->max_radius, info->corners);
        else
            sugar_rounded_rectangle (cr, pos, outline_width - info->rc_style->line_width / 2.0, info->max_radius, info->corners);

        cairo_fill (cr);
    }
}

void
sugar_draw_scale_slider (cairo_t *cr, SugarRangeInfo *range_info)
{
    SugarInfo *info = &range_info->info;
    SugarRectangle *pos = &info->pos;
    GdkColor *fill, *outline, *line;
    gdouble max_radius = info->max_radius;
    gdouble line_width = info->rc_style->line_width;
    
    fill = line = outline = &info->style->bg[GTK_STATE_NORMAL];

    if (info->state == GTK_STATE_INSENSITIVE) {
        fill = NULL;
    } else if ((info->state == GTK_STATE_ACTIVE) || range_info->focused) {
        line = &info->style->fg[GTK_STATE_PRELIGHT];
    } else {
        line = &info->style->bg[GTK_STATE_SELECTED];
    }

    /* Draw the center fill first. */
    if (fill) {
        gdk_cairo_set_source_color (cr, fill);
        sugar_rounded_rectangle (cr, pos, line_width/2.0 + (MIN(pos->width, pos->height) - line_width)/4.0, max_radius, info->corners);
        cairo_fill (cr);
    }


    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
    gdk_cairo_set_source_color (cr, outline);
    sugar_rounded_rectangle (cr, pos, 0, max_radius, info->corners);
    sugar_rounded_rectangle (cr, pos, line_width + (MIN(pos->width, pos->height) - line_width) / 4.0, max_radius, info->corners);
    
    cairo_fill (cr);

    gdk_cairo_set_source_color (cr, line);
    sugar_rounded_rectangle (cr, pos, line_width, max_radius, info->corners);
    sugar_rounded_rectangle (cr, pos, (MIN(pos->width, pos->height) - line_width) / 4.0, max_radius, info->corners);
    
    cairo_fill (cr);
}

void
sugar_draw_scrollbar_slider (cairo_t       *cr,
                             SugarRangeInfo *range_info)
{
    SugarInfo *info = &range_info->info;
    SugarRectangle *pos = &info->pos;
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
    sugar_rounded_rectangle (cr, pos, 0, info->max_radius, info->corners);
    cairo_fill (cr);
}

void
sugar_draw_entry (cairo_t *cr, SugarInfo *info)
{
    SugarRectangle *pos = &info->pos;

    gdk_cairo_set_source_color (cr, &info->style->base[info->state]);
    sugar_rounded_rectangle (cr, pos, 0, info->max_radius, info->corners);
    cairo_fill (cr);

    sugar_draw_insensitive_outline (cr, info);
}

void
sugar_draw_button (cairo_t *cr, SugarInfo *info)
{
    SugarRectangle *pos = &info->pos;
    guint radius;

    gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);

    radius = info->max_radius;

    sugar_rounded_rectangle (cr, pos, 0, radius, info->corners);
    cairo_fill (cr);

    sugar_draw_insensitive_outline (cr, info);
}

void
sugar_draw_button_default (cairo_t *cr, SugarInfo *info)
{
    SugarRectangle *pos = &info->pos;
    gdouble line_width = info->rc_style->line_width;
    guint radius;

    gdk_cairo_set_source_color (cr, &info->style->bg[info->state]);

    /* This is broken! */
    radius = info->max_radius + info->rc_style->line_width + line_width;

    sugar_rounded_inner_stroke (cr, pos, line_width, info->max_radius, info->corners, info->cont_edges);
}

void
sugar_draw_arrow (cairo_t *cr, SugarArrowInfo *arrow_info)
{
    SugarInfo *info = &arrow_info->info;
    SugarRectangle *pos = &info->pos;
    gdouble line_width = info->rc_style->thick_line_width;
    gdouble run_length;

    cairo_save (cr);

    /* Center, so that rotation, etc. is easier. */
    cairo_translate (cr, pos->x + pos->width / 2.0, pos->y + pos->height / 2.0);

    switch (arrow_info->dir) {
        case GTK_ARROW_DOWN:
            run_length = MIN ((pos->width - line_width) / 2.0, pos->height - line_width);
            break;
        case GTK_ARROW_UP:
            cairo_rotate (cr, G_PI);
            run_length = MIN ((pos->width - line_width) / 2.0, pos->height - line_width);
            break;
        case GTK_ARROW_LEFT:
            cairo_rotate (cr, G_PI_2);
            run_length = MIN (pos->width - line_width, (pos->height - line_width) / 2.0);
            break;
        case GTK_ARROW_RIGHT:
            cairo_rotate (cr, -G_PI_2);
            run_length = MIN (pos->width, pos->height / 2.0);
            break;
        default:
            g_assert_not_reached ();
    }

    cairo_set_line_width (cr, line_width);
    cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
    gdk_cairo_set_source_color (cr, &info->style->fg[info->state]);

    cairo_move_to (cr, -run_length, -run_length / 2.0);
    cairo_line_to (cr, 0, run_length / 2.0);
    cairo_line_to (cr, run_length, -run_length / 2.0);

    cairo_stroke (cr);

    cairo_restore (cr);
}

void
sugar_draw_radio_button (cairo_t *cr, SugarInfo *info)
{
    SugarRectangle *pos = &info->pos;
    gdouble radius = MIN (pos->width, pos->height) / 2.0;
    gdouble line_width = info->rc_style->line_width;
    gdouble outer_stroke_radius;

    cairo_save (cr);

    cairo_translate (cr, pos->x + radius, pos->y + radius);

    outer_stroke_radius = radius - line_width / 2.0;

    /* Fill the background first (if the state is not insensitive) */
    if (info->state == GTK_STATE_ACTIVE || info->shadow != GTK_SHADOW_OUT) {
        gdk_cairo_set_source_color (cr, &info->style->base[info->state]);
        cairo_arc (cr, 0, 0, outer_stroke_radius, 0, 2*G_PI);
        cairo_fill (cr);
    }

    /* Draw the outline (always with fg[INSENSITIVE]) */
    gdk_cairo_set_source_color (cr, &info->style->fg[GTK_STATE_INSENSITIVE]);
    cairo_arc (cr, 0, 0, outer_stroke_radius, 0, 2*G_PI);
    cairo_set_line_width (cr, line_width);
    cairo_stroke (cr);

    /* Draw the bullet, if shadow is IN */
    if (info->shadow == GTK_SHADOW_IN) {
        if (info->state == GTK_STATE_INSENSITIVE)
            gdk_cairo_set_source_color (cr, &info->style->fg[GTK_STATE_INSENSITIVE]);
        else
            gdk_cairo_set_source_color (cr, &info->style->text[info->state]);

        /* Just used a factor of 2.64 here ... */
        cairo_arc (cr, 0, 0, (radius - line_width) * 9.0/22.0, 0, 2*G_PI);
        cairo_fill (cr);
    } else if (info->shadow == GTK_SHADOW_ETCHED_IN) {
        /* Sorry, not implemented :-) */
        cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
        cairo_arc (cr, 0, 0, radius, 0, 2*G_PI);
        cairo_fill (cr);
    }

    cairo_restore (cr);
}

void
sugar_draw_check_button (cairo_t *cr, SugarInfo *info)
{
    SugarRectangle *pos = &info->pos;
    gdouble line_width = info->rc_style->line_width;

    /* Fill the background */
    if (info->state == GTK_STATE_ACTIVE || info->shadow != GTK_SHADOW_OUT) {
        gdk_cairo_set_source_color (cr, &info->style->base[info->state]);
        sugar_cairo_rectangle (cr, pos);
        cairo_fill (cr);
    }
    
    cairo_set_line_width (cr, line_width);
    gdk_cairo_set_source_color (cr, &info->style->fg[GTK_STATE_INSENSITIVE]);
    cairo_rectangle (cr, pos->x + line_width / 2.0, pos->y + line_width / 2.0,
                         pos->width - line_width, pos->height - line_width);
    cairo_stroke (cr);

    if (info->shadow == GTK_SHADOW_IN) {
        /* Try to draw a nice check :-) */
        gdouble width, height;
        gdouble thick_line_width = info->rc_style->thick_line_width;

        width = info->pos.width - line_width * 2.0 - thick_line_width * 2.3;
        height = info->pos.height - line_width * 2.0 - thick_line_width * 2.3;

        cairo_save (cr);
        gdk_cairo_set_source_color (cr, &info->style->text[info->state]);

        cairo_translate (cr, (double)info->pos.x + line_width + thick_line_width * 1.25, (double)info->pos.y + line_width + thick_line_width*1.15);
        cairo_set_line_width (cr, info->rc_style->thick_line_width);
        cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
        cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);

        cairo_move_to (cr, 0.0, height / 2.0);
        cairo_line_to (cr, width / 3, height - thick_line_width / 2.0);
        cairo_line_to (cr, width, thick_line_width / 2.0);

        cairo_stroke (cr);

        cairo_restore (cr);
    } else if (info->shadow == GTK_SHADOW_ETCHED_IN) {
        cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
        sugar_cairo_rectangle (cr, &info->pos);
        cairo_fill (cr);
    }
}


void
sugar_draw_progressbar_trough (cairo_t *cr, SugarInfo *info)
{
    if (info->state == GTK_STATE_INSENSITIVE) {
        gdk_cairo_set_source_color (cr, &info->style->fg[GTK_STATE_INSENSITIVE]);
        sugar_rounded_inner_stroke (cr, &info->pos, info->rc_style->line_width, info->max_radius, info->corners, info->cont_edges);
    } else {
        gdk_cairo_set_source_color (cr, &info->style->fg[GTK_STATE_INSENSITIVE]);
        sugar_rounded_rectangle (cr, &info->pos, 0, info->max_radius, info->corners);
        cairo_fill (cr);
    }
}

void
sugar_draw_progressbar_bar (cairo_t *cr, SugarInfo *info, GtkProgressBarOrientation orientation)
{
    /* Invisible bar for insensitive progress bars. */
    if (info->state != GTK_STATE_INSENSITIVE) {
        SugarRectangle pos = info->pos;

        /* Clip from both sides to get the correct region. */
        if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT || orientation == GTK_PROGRESS_RIGHT_TO_LEFT) {
            pos.width += 2*info->max_radius;
            sugar_rounded_rectangle (cr, &pos, 0, info->max_radius, info->corners);
            cairo_clip(cr);

            pos.x -= 2*info->max_radius;
            sugar_rounded_rectangle (cr, &pos, 0, info->max_radius, info->corners);
            cairo_clip(cr);
        } else {
            pos.height += 2*info->max_radius;
            sugar_rounded_rectangle (cr, &pos, 0, info->max_radius, info->corners);
            cairo_clip(cr);

            pos.y -= 2*info->max_radius;
            sugar_rounded_rectangle (cr, &pos, 0, info->max_radius, info->corners);
            cairo_clip(cr);
        }

        gdk_cairo_set_source_color (cr, &info->style->base[info->state]);
        cairo_paint(cr);
    }
}

