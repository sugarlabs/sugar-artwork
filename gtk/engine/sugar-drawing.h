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

#include <glib.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include "sugar-info.h"

G_GNUC_INTERNAL void sugar_fill_background (cairo_t *cr, SugarInfo *info);

G_GNUC_INTERNAL void sugar_draw_exterior_focus (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_scale_trough (cairo_t *cr, SugarRangeInfo *range_info);
G_GNUC_INTERNAL void sugar_draw_scale_slider (cairo_t *cr, SugarRangeInfo *range_info);
G_GNUC_INTERNAL void sugar_draw_scrollbar_slider (cairo_t *cr, SugarRangeInfo *range_info);
G_GNUC_INTERNAL void sugar_draw_scrollbar_trough (cairo_t *cr, SugarRangeInfo *range_info);
G_GNUC_INTERNAL void sugar_draw_entry (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_button (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_button_default (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_arrow (cairo_t *cr, SugarArrowInfo *arrow_info);
G_GNUC_INTERNAL void sugar_draw_radio_button (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_check_button (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_progressbar_trough (cairo_t *cr, SugarInfo *info);
G_GNUC_INTERNAL void sugar_draw_progressbar_bar (cairo_t *cr, SugarInfo *info, GtkProgressBarOrientation orientation);

