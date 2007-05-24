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
#include "sugar-rc-style.h"
#include "sugar-style.h"

static void sugar_rc_style_class_init (SugarRcStyleClass *klass);
static void sugar_rc_style_init       (SugarRcStyle      *style);

GType sugar_type_rc_style = 0;
static GtkRcStyleClass *parent_class;

static gchar symbols[] =
    "hint\0"
    "line_width\0"
    "thick_line_width\0"
    "max_radius\0";

typedef enum {
    TOKEN_INVALID = G_TOKEN_LAST,
    TOKEN_HINT,
    TOKEN_LINE_WIDTH,
    TOKEN_THICK_LINE_WIDTH,
    TOKEN_MAX_RADIUS,
    TOKEN_LAST,
} SugarTokens;

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
sugar_rc_style_init (SugarRcStyle *rc_style)
{
    /* Initilize the RC style. */
    rc_style->line_width = 3;
    rc_style->thick_line_width = 4;
    rc_style->max_radius = 5;
    rc_style->hint = NULL;
}


static guint
sugar_rc_parse_float (GScanner *scanner, gdouble *dest)
{
    GTokenType token;

    token = g_scanner_get_next_token(scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
        return G_TOKEN_EQUAL_SIGN;

    token = g_scanner_get_next_token(scanner);
    if (token == G_TOKEN_FLOAT)
        *dest = scanner->value.v_float;
    else if (token == G_TOKEN_INT)
        *dest = scanner->value.v_int;
    else
        return G_TOKEN_FLOAT;

    return G_TOKEN_NONE;
}

static guint
sugar_rc_parse_string (GScanner *scanner, gchar **dest)
{
    GTokenType token;

    token = g_scanner_get_next_token(scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
        return G_TOKEN_EQUAL_SIGN;

    token = g_scanner_get_next_token(scanner);
    if (token != G_TOKEN_STRING)
        return G_TOKEN_STRING;

    *dest = g_strdup(scanner->value.v_string);
    return G_TOKEN_NONE;
}

static guint
sugar_rc_style_parse (GtkRcStyle   *rc_style,
                      GtkSettings  *settings,
                      GScanner     *scanner)
{
    static GQuark scope = 0;
    SugarRcStyle *sugar_rc_style;
    guint old_scope;
    GTokenType token;

    sugar_rc_style = SUGAR_RC_STYLE(rc_style);

    if (scope == 0)
        scope = g_quark_from_string("sugar_theme_engine");

    old_scope = g_scanner_set_scope(scanner, scope);

    /* Test if the scope has been set up already. */
    if (!g_scanner_lookup_symbol(scanner, symbols)) {
        gchar *current_symbol = symbols;
        gint i = TOKEN_INVALID + 1;

        /* Add our symbols */
        while ((current_symbol[0] != '\0') && (i < TOKEN_LAST)) {
            g_scanner_scope_add_symbol(scanner, scope, current_symbol, GINT_TO_POINTER (i));

            current_symbol += strlen(current_symbol) + 1;
            i++;
        }
        g_assert (i == TOKEN_LAST && current_symbol[0] == '\0');
    }

    token = g_scanner_get_next_token (scanner);
    while (token != G_TOKEN_RIGHT_CURLY && token != G_TOKEN_EOF) {
        switch (token) {
            case TOKEN_LINE_WIDTH:
                token = sugar_rc_parse_float(scanner, &sugar_rc_style->line_width);
                sugar_rc_style->flags |= OPTION_LINE_WIDTH;
                break;
            case TOKEN_THICK_LINE_WIDTH:
                token = sugar_rc_parse_float(scanner, &sugar_rc_style->thick_line_width);
                sugar_rc_style->flags |= OPTION_THICK_LINE_WIDTH;
                break;
            case TOKEN_MAX_RADIUS:
                token = sugar_rc_parse_float(scanner, &sugar_rc_style->max_radius);
                sugar_rc_style->flags |= OPTION_MAX_RADIUS;
                break;
            case TOKEN_HINT:
                token = sugar_rc_parse_string(scanner, &sugar_rc_style->hint);
                sugar_rc_style->flags |= OPTION_HINT;
                break;
            default:
                token = G_TOKEN_RIGHT_CURLY;
                break;
        }

        /* Don't reset the scope on failure, just return. */
        if (token != G_TOKEN_NONE)
            return token;
        
        token = g_scanner_get_next_token(scanner);
    }

    g_scanner_set_scope(scanner, old_scope);
    return G_TOKEN_NONE;
}


static void
sugar_rc_style_merge (GtkRcStyle *dest,
                      GtkRcStyle *src)
{
    SugarRcStyle *sugar_dest;
    SugarRcStyle *sugar_src;
    SugarRcStyleOptions flags;

    parent_class->merge (dest, src);

    if (!SUGAR_IS_RC_STYLE (src))
        return;

    sugar_dest = SUGAR_RC_STYLE (dest);
    sugar_src = SUGAR_RC_STYLE (src);

    flags = (~sugar_dest->flags) & sugar_src->flags;

    /* Merge all variables */
    if (flags & OPTION_LINE_WIDTH)
        sugar_dest->line_width = sugar_src->line_width;
    if (flags & OPTION_THICK_LINE_WIDTH)
        sugar_dest->thick_line_width = sugar_src->thick_line_width;
    if (flags & OPTION_MAX_RADIUS)
        sugar_dest->max_radius = sugar_src->max_radius;
    if (flags & OPTION_HINT) {
        g_free (sugar_dest->hint);
        sugar_dest->hint = g_strdup (sugar_src->hint);
    }

    sugar_dest->flags |= flags;
}


static GtkStyle *
sugar_rc_style_create_style (GtkRcStyle *rc_style)
{
    return GTK_STYLE(g_object_new(SUGAR_TYPE_STYLE, NULL));
}

static void
sugar_rc_style_finalize (GObject *object)
{
    SugarRcStyle *rc_style = SUGAR_RC_STYLE(object);
    g_free (rc_style->hint);

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
sugar_rc_style_class_init (SugarRcStyleClass *klass)
{
    GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
    parent_class = g_type_class_peek_parent(klass);

    rc_style_class->create_style = sugar_rc_style_create_style;
    rc_style_class->parse = sugar_rc_style_parse;
    rc_style_class->merge = sugar_rc_style_merge;

    G_OBJECT_CLASS (rc_style_class)->finalize = sugar_rc_style_finalize;
}
