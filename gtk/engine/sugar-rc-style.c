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

#include "sugar-rc-style.h"
#include "sugar-style.h"

static void sugar_rc_style_class_init (SugarRcStyleClass *klass);
static void sugar_rc_style_init       (SugarRcStyle      *style);

GType sugar_type_rc_style = 0;

void
sugar_rc_style_register_type (GTypeModule *module)
{
    static const GTypeInfo object_info =
    {
        sizeof (SugarRcStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc)sugar_rc_style_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (SugarRcStyle),
        0,              /* n_preallocs */
        (GInstanceInitFunc)sugar_rc_style_init,
        NULL
    };

    sugar_type_rc_style = g_type_module_register_type (module,
        						                       GTK_TYPE_RC_STYLE,
    	    					                       "SugarRcStyle",
        						                       &object_info, 0);
}

static void
sugar_rc_style_init (SugarRcStyle *style)
{
}

static GtkStyle *
sugar_rc_style_create_style (GtkRcStyle *rc_style)
{
    return GTK_STYLE(g_object_new(SUGAR_TYPE_STYLE, NULL));
}

static void
sugar_rc_style_class_init (SugarRcStyleClass *klass)
{
    GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);

    rc_style_class->create_style = sugar_rc_style_create_style;
}
