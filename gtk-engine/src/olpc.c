/* olpc - cairo based gtk+ theme
 *
 * Copyright © 2006 Red Hat, Inc.
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

#include "olpc.h"
#include "olpc-style.h"
#include "olpc-rc-style.h"

G_MODULE_EXPORT void
theme_init (GTypeModule * module)
{
    olpc_rc_style_register_type (module);
    olpc_style_register_type (module);
}

G_MODULE_EXPORT void
theme_exit (GTypeModule * module)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
    return g_object_new (OLPC_TYPE_RC_STYLE, NULL);
}
