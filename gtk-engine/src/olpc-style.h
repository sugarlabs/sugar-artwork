/* olpc - cairo based gtk+ theme
 *
 * Copyright © 2005 Red Hat, Inc.
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

#ifndef OLPC_STYLE_H
#define OLPC_STYLE_H

#include <gtk/gtkstyle.h>

extern GType olpc_type_style;

#define OLPC_TYPE_STYLE			olpc_type_style
#define OLPC_STYLE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), OLPC_TYPE_STYLE, OlpcStyle))
#define OLPC_STYLE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), OLPC_TYPE_STYLE, OlpcStyleClass))
#define OLPC_IS_STYLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), OLPC_TYPE_STYLE))
#define OLPC_IS_STYLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), OLPC_TYPE_STYLE))
#define OLPC_STYLE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), OLPC_TYPE_STYLE, OlpcStyleClass))

typedef struct _OlpcStyle
{
    GtkStyle parent_instance;
} OlpcStyle;

typedef struct _OlpcStyleClass
{
    GtkStyleClass parent_class;
} OlpcStyleClass;

void
olpc_style_register_type (GTypeModule *module);

#endif
