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

#include <gmodule.h>
#include "sugar-rc-style.h"
#include "sugar-style.h"

G_MODULE_EXPORT void
theme_init(GTypeModule *module)
{
    sugar_rc_style_register_type(module);
    sugar_style_register_type(module);
}

G_MODULE_EXPORT void
theme_exit(void)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style(void)
{
    return GTK_RC_STYLE(g_object_new(SUGAR_TYPE_RC_STYLE, NULL));
}
