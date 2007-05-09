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

#include <gtk/gtkrc.h>

typedef struct _SugarRcStyle SugarRcStyle;
typedef struct _SugarRcStyleClass SugarRcStyleClass;

G_GNUC_INTERNAL extern GType sugar_type_rc_style;

/* Bit field for the engine options. */
typedef enum {
	OPTION_LINE_WIDTH = 1 << 0,
	OPTION_THICK_LINE_WIDTH = 1 << 1,
	OPTION_HINT = 1 << 2
} SugarRcStyleOptions;


#define SUGAR_TYPE_RC_STYLE              sugar_type_rc_style
#define SUGAR_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), SUGAR_TYPE_RC_STYLE, SugarRcStyle))
#define SUGAR_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SUGAR_TYPE_RC_STYLE, SugarRcStyleClass))
#define SUGAR_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), SUGAR_TYPE_RC_STYLE))
#define SUGAR_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SUGAR_TYPE_RC_STYLE))
#define SUGAR_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SUGAR_TYPE_RC_STYLE, SugarRcStyleClass))

struct _SugarRcStyle {
    GtkRcStyle parent_instance;

    SugarRcStyleOptions flags;

    gchar *hint;
    gdouble line_width;
    gdouble thick_line_width;
};

struct _SugarRcStyleClass {
    GtkRcStyleClass parent_class;
};

G_GNUC_INTERNAL void sugar_rc_style_register_type (GTypeModule *module);
