/*
 * Copyright (C) 2007, Red Hat, Inc.
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

#include <string.h>

#include "sugar-style.h"
#include "sugar-rc-style.h"

static void sugar_style_init       (SugarStyle      *style);
static void sugar_style_class_init (SugarStyleClass *klass);

GType sugar_type_style = 0;

static GtkStyleClass *parent_class;

void
sugar_style_register_type (GTypeModule *module)
{
    static const GTypeInfo object_info =
    {
        sizeof (SugarStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) sugar_style_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (SugarStyle),
        0,              /* n_preallocs */
        (GInstanceInitFunc) sugar_style_init,
    };
  
    sugar_type_style = g_type_module_register_type (module,
						                            GTK_TYPE_STYLE,
						                            "SugarStyle",
						                            &object_info, 0);
}

static void
sugar_style_init (SugarStyle *style)
{
}

static void
sugar_style_draw_extension(GtkStyle        *style,
                           GdkWindow       *window,
                           GtkStateType     state_type,
                           GtkShadowType    shadow_type,
                           GdkRectangle    *area,
                           GtkWidget       *widget,
                           const char      *detail,
                           int              x,
                           int              y,
                           int              width,
                           int              height,
                           GtkPositionType  gap_side)
{
    cairo_t *cr;

    cr = gdk_cairo_create (window);

    if (strcmp(detail, "tab") == 0) {
        gdk_cairo_set_source_color(cr, &style->bg[state_type]);
        cairo_rectangle(cr, x, y, width, height);
        cairo_fill(cr);
    } else {
        parent_class->draw_extension(style, window, state_type,
		                             shadow_type, area, widget, detail,
		                             x, y, width, height, gap_side);
    }
    
    cairo_destroy(cr);
}

static void
sugar_style_class_init (SugarStyleClass *klass)
{
    GtkStyleClass *style_class = GTK_STYLE_CLASS(klass);
    
    parent_class = g_type_class_peek_parent(klass);
    
    style_class->draw_extension = sugar_style_draw_extension;
}


