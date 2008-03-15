/*
 * Copyright (C) 2007, Red Hat, Inc.
 * Copyright (C) 2008, Benjamin Berg
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
    "max_radius\0"
    "scrollbar_border\0"
    "fake_padding\0"
    "subcell_size\0"
    "parent_fg_color\0"
    "parent_bg_color\0"
    "bg\0"
    "fg\0"
    "base\0"
    "text\0";

typedef enum {
    TOKEN_INVALID = G_TOKEN_LAST,
    TOKEN_HINT,
    TOKEN_LINE_WIDTH,
    TOKEN_THICK_LINE_WIDTH,
    TOKEN_MAX_RADIUS,
    TOKEN_SCROLLBAR_BORDER,
    TOKEN_FAKE_PADDING,
    TOKEN_SUBCELL_SIZE,
    TOKEN_PARENT_FG_COLOR,
    TOKEN_PARENT_BG_COLOR,
    TOKEN_BG,
    TOKEN_FG,
    TOKEN_BASE,
    TOKEN_TEXT,
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
    gint i;

    /* Initilize the RC style. */
    rc_style->line_width = 3;
    rc_style->thick_line_width = 4;
    rc_style->max_radius = 5;
    rc_style->scrollbar_border = 5;
    rc_style->fake_padding = 0;
    rc_style->subcell_size = 15;
    rc_style->hint = NULL;

    rc_style->color_flags = 0;

    for (i = 0; i < 5; i++) {
        rc_style->color_mapping.fg[i] = SUGAR_COLOR_ORIGINAL;
        rc_style->color_mapping.bg[i] = SUGAR_COLOR_ORIGINAL;
        rc_style->color_mapping.base[i] = SUGAR_COLOR_ORIGINAL;
        rc_style->color_mapping.text[i] = SUGAR_COLOR_ORIGINAL;
    }
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
sugar_rc_parse_color (GScanner *scanner, GdkColor *color)
{
    GTokenType token;

    token = g_scanner_get_next_token(scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
        return G_TOKEN_EQUAL_SIGN;

    token = gtk_rc_parse_color(scanner, color);

    return token;
}

static guint
sugar_rc_parse_color_assignment (GScanner *scanner, SugarRcStyle *rc_style)
{
    GTokenType type;
    GTokenType token;
    GtkStateType state;
    SugarRCColor color;

    type = g_scanner_cur_token(scanner);
    
    token = gtk_rc_parse_state (scanner, &state);

    /* In theory this is not perfect, because color may be invalid.
     * Can't happen here though. */
    if (token != G_TOKEN_NONE)
        return token;

    token = g_scanner_get_next_token(scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
        return G_TOKEN_EQUAL_SIGN;

    token = g_scanner_get_next_token(scanner);
    if (token == TOKEN_PARENT_FG_COLOR)
        color = SUGAR_COLOR_FG;
    else if (token == TOKEN_PARENT_BG_COLOR)
        color = SUGAR_COLOR_BG;
    else
        return TOKEN_PARENT_FG_COLOR;

    switch (type) {
        case TOKEN_BG:
            rc_style->color_mapping.bg[state] = color;
        break;
        case TOKEN_FG:
            rc_style->color_mapping.fg[state] = color;
        break;
        case TOKEN_BASE:
            rc_style->color_mapping.base[state] = color;
        break;
        case TOKEN_TEXT:
            rc_style->color_mapping.text[state] = color;
        break;
        default:
            return G_TOKEN_IDENTIFIER;
    }
    
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
            case TOKEN_SCROLLBAR_BORDER:
                token = sugar_rc_parse_float(scanner, &sugar_rc_style->scrollbar_border);
                sugar_rc_style->flags |= OPTION_SCROLLBAR_BORDER;
                break;
            case TOKEN_FAKE_PADDING:
                token = sugar_rc_parse_float(scanner, &sugar_rc_style->fake_padding);
                sugar_rc_style->flags |= OPTION_FAKE_PADDING;
                break;
            case TOKEN_SUBCELL_SIZE:
                token = sugar_rc_parse_float(scanner, &sugar_rc_style->subcell_size);
                sugar_rc_style->flags |= OPTION_SUBCELL_SIZE;
                break;
            case TOKEN_HINT:
                token = sugar_rc_parse_string(scanner, &sugar_rc_style->hint);
                sugar_rc_style->flags |= OPTION_HINT;
                break;
            case TOKEN_PARENT_FG_COLOR:
                token = sugar_rc_parse_color(scanner, &sugar_rc_style->colors[SUGAR_COLOR_FG]);
                sugar_rc_style->color_flags |= 1 << SUGAR_COLOR_FG;
                break;
            case TOKEN_PARENT_BG_COLOR:
                token = sugar_rc_parse_color(scanner, &sugar_rc_style->colors[SUGAR_COLOR_BG]);
                sugar_rc_style->color_flags |= 1 << SUGAR_COLOR_BG;
                break;

            case TOKEN_FG:
            case TOKEN_BG:
            case TOKEN_BASE:
            case TOKEN_TEXT:
                token = sugar_rc_parse_color_assignment(scanner, sugar_rc_style);
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
    GtkStateType state;
    gint i;

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
    if (flags & OPTION_SCROLLBAR_BORDER)
        sugar_dest->scrollbar_border = sugar_src->scrollbar_border;
    if (flags & OPTION_FAKE_PADDING)
        sugar_dest->fake_padding = sugar_src->fake_padding;
    if (flags & OPTION_SUBCELL_SIZE)
        sugar_dest->subcell_size = sugar_src->subcell_size;
    if (flags & OPTION_HINT) {
        g_free (sugar_dest->hint);
        sugar_dest->hint = g_strdup (sugar_src->hint);
    }

    sugar_dest->flags |= flags;


    flags = (~sugar_dest->color_flags) & sugar_src->color_flags;
    for (i = 0; i < SUGAR_COLOR_COUNT; i++) {
        if (flags & (1 << i))
            sugar_dest->colors[i] = sugar_src->colors[i];
        
    }
    sugar_dest->color_flags |= flags;

    for (i = 0; i < 5; i++) {
        if (sugar_dest->color_mapping.bg[i] == SUGAR_COLOR_ORIGINAL)
            sugar_dest->color_mapping.bg[i] = sugar_src->color_mapping.bg[i];

        if (sugar_dest->color_mapping.fg[i] == SUGAR_COLOR_ORIGINAL)
            sugar_dest->color_mapping.fg[i] = sugar_src->color_mapping.fg[i];

        if (sugar_dest->color_mapping.base[i] == SUGAR_COLOR_ORIGINAL)
            sugar_dest->color_mapping.base[i] = sugar_src->color_mapping.base[i];

        if (sugar_dest->color_mapping.text[i] == SUGAR_COLOR_ORIGINAL)
            sugar_dest->color_mapping.text[i] = sugar_src->color_mapping.text[i];
    }
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
