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

#ifndef OLPC_RC_STYLE_H
#define OLPC_RC_STYLE_H

#include "olpc.h"

#include <gtk/gtkrc.h>

extern GType olpc_type_rc_style;

#define OLPC_TYPE_RC_STYLE				olpc_type_rc_style
#define OLPC_RC_STYLE(obj)				(G_TYPE_CHECK_INSTANCE_CAST ((obj), OLPC_TYPE_RC_STYLE, OlpcRcStyle))
#define OLPC_RC_STYLE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), OLPC_TYPE_RC_STYLE, OlpcRcStyleClass))
#define OLPC_IS_RC_STYLE(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), OLPC_TYPE_RC_STYLE))
#define OLPC_IS_RC_STYLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), OLPC_TYPE_RC_STYLE))
#define OLPC_RC_STYLE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), OLPC_TYPE_RC_STYLE, OlpcRcStyleClass))

typedef struct _OlpcRcStyle {
    GtkRcStyle parent_instance;

    GdkColor top_color;
    GdkColor bottom_color;
} OlpcRcStyle;

typedef struct _OlpcRcStyleClass {
    GtkRcStyleClass parent_class;
} OlpcRcStyleClass;;

void
olpc_rc_style_register_type (GTypeModule *module);

#endif
