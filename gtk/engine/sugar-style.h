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

#include <gtk/gtkstyle.h>

typedef struct _SugarStyle SugarStyle;
typedef struct _SugarStyleClass SugarStyleClass;

G_GNUC_INTERNAL extern GType sugar_type_style;

#define SUGAR_TYPE_STYLE              sugar_type_style
#define SUGAR_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), SUGAR_TYPE_STYLE, SugarStyle))
#define SUGAR_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SUGAR_TYPE_STYLE, SugarStyleClass))
#define SUGAR_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), SUGAR_TYPE_STYLE))
#define SUGAR_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SUGAR_TYPE_STYLE))
#define SUGAR_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SUGAR_TYPE_STYLE, SugarStyleClass))

struct _SugarStyle
{
  GtkStyle parent_instance;
};

struct _SugarStyleClass
{
  GtkStyleClass parent_class;
};

G_GNUC_INTERNAL void sugar_style_register_type (GTypeModule *module);
