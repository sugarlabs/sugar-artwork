/* Copyright Benjamin Berg 2007
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

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    STYLE_NORMAL,
    STYLE_VSCALE,
    STYLE_HSCALE,
    STYLE_SCROLLBAR,
    STYLE_COMBOBOXENTRY
} StyleType;
#define STYLE_COUNT (STYLE_COMBOBOXENTRY+1)

typedef enum {
    WIDGET_NULL,
    WIDGET_VSCALE,
    WIDGET_HSCALE,
    WIDGET_VSCROLLBAR,
    WIDGET_TOGGLE_BUTTON
} WidgetType;

#define WIDGET_COUNT (WIDGET_TOGGLE_BUTTON+1)
#define WIDGET_ALL (-1)


GType
widget_type_get_type (void)
{
    static GType type = 0;
    if (type == 0) {
        static const GEnumValue values[] = {
            { WIDGET_NULL, "WIDGET_NULL", "NULL" },
            { WIDGET_VSCALE, "WIDGET_VSCALE", "GtkVScale" },
            { WIDGET_HSCALE, "WIDGET_HSCALE", "GtkHScale" },
            { WIDGET_VSCROLLBAR, "WIDGET_VSCROLLBAR", "GtkVScrollBar" },
            { WIDGET_TOGGLE_BUTTON, "WIDGET_TOGGLE_BUTTON", "GtkToggleButton" },
            { 0, NULL, NULL }
        };
    type = g_enum_register_static ("WidgeType", values);
    }
    return type;
}

GType
bool_get_type (void)
{
    static GType type = 0;
    if (type == 0) {
        static const GEnumValue values[] = {
            { 0, "BOOL_FALSE", "False" },
            { 1, "BOOL_TRUE", "True" },
            { 0, NULL, NULL }
        };
        type = g_enum_register_static ("GtkBoolDummyType", values);
    }
    return type;
}


#define WIDGET_TYPE (widget_type_get_type ())
#define BOOL_TYPE (bool_get_type ())


typedef enum {
    FUNCTION_ARROW      = 1 << 0,
    FUNCTION_BOX        = 1 << 1,
    FUNCTION_SHADOW     = 1 << 2,
    FUNCTION_BOX_GAP    = 1 << 3,
    FUNCTION_SHADOW_GAP = 1 << 4,
    FUNCTION_CHECK      = 1 << 5,
    FUNCTION_EXPANDER   = 1 << 6,
    FUNCTION_EXTENSION  = 1 << 7,
    FUNCTION_FLAT_BOX   = 1 << 8,
    FUNCTION_FOCUS      = 1 << 9,
    FUNCTION_HANDLE     = 1 << 10,
    FUNCTION_HLINE      = 1 << 11,
    FUNCTION_VLINE      = 1 << 12,
    FUNCTION_OPTION     = 1 << 13,
    FUNCTION_RESIZE_GRIP = 1 << 14,
    FUNCTION_SLIDER     = 1 << 15,
    FUNCTION_TAB        = 1 << 16,

    FUNCTION_ALL        = -1
} FunctionType;
#define FUNCTION_LAST FUNCTION_BOX

GtkWidget *window;
GtkWidget *box;
GtkWidget *widgets[WIDGET_COUNT];
GtkStyle *styles[STYLE_COUNT];

typedef struct {
    FunctionType     function;
    WidgetType       widget;
    StyleType        style;
    gchar           *detail;
    GtkStateType     state;
    GtkShadowType    shadow;
    GtkArrowType     arrow_type;
    gboolean         fill;
    GtkPositionType  gap_side;
    GtkOrientation   orientation;
    GtkExpanderStyle expander_style;
    GdkWindowEdge    edge;
} Test;

#define ARROW_ALL (-1)
#define STATE_ALL (-1)
#define SHADOW_ALL (-1)
#define FILL_ALL (-1)
#define EXPANDER_ALL (-1)
#define EDGE_ALL (-1)
#define GAP_SIDE_ALL (-1)
#define ORIENTATION_ALL (-1)

#define CELL_DETAILS_SIDE(detail) detail ":" detail "_start" ":" detail "_middle" ":" detail "_end"
#define CELL_DETAILS_SORTED(detail) CELL_DETAILS_SIDE(detail) ":" CELL_DETAILS_SIDE(detail "_sorted")
#define CELL_DETAILS_RULED(detail) CELL_DETAILS_SORTED(detail) ":" CELL_DETAILS_SORTED(detail "_ruled")
#define CELL_DETAILS CELL_DETAILS_RULED("cell_even") ":" CELL_DETAILS_RULED("cell_odd")
#define CHECK_AND_RADIO_DETAILS "NULL:cellcheck:cellradio:check:checkbutton:radiobutton"
#define EXPANDER_DETAILS "NULL:expander" /* also in common details list ... */
#define ARROW_DETAILS "NULL:arrow:tab:optionmenutab"
#define COMMON_DETAILS "NULL:arrow:bar:base:button:buttondefault:combo_entry_button:dockitem:dockitem_bin:entry:entry_bg:frame:handle:handlebox:handlebox_bin:hpaned:hruler:hscale:hscrollbar:hseparator:label:menu:menubar:menuitem:metacity:notebook:option:optionmenu:optionmenutab:paned:scrolled_window:slider:spinbutton:spinbutton_arrow:spinbutton_down:spinbutton_up:stepper:tab:text:togglebutton:togglebuttondefault:toolbar:tooltip:treeitem:treeview:trough:viewport:viewportbin:vpaned:vruler:vscale:vscrollbar:vseparator:expander"

/* The following explicitly does not test EVERY combination, as that would be
 * overkill, and would take a lot longer. */
static Test tests[] = {
    {    /* ARROW */
        FUNCTION_ARROW,
        WIDGET_NULL,
        STYLE_NORMAL,
        ARROW_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        ARROW_ALL,
        FILL_ALL,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    {    /* things with gaps */
        FUNCTION_BOX_GAP | FUNCTION_SHADOW_GAP | FUNCTION_EXTENSION,
        WIDGET_NULL,
        STYLE_NORMAL,
        COMMON_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GAP_SIDE_ALL,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    {    /* expander */
        FUNCTION_EXPANDER,
        WIDGET_NULL,
        STYLE_NORMAL,
        EXPANDER_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        EXPANDER_ALL,
        GDK_WINDOW_EDGE_SOUTH
    },
    {    /* handle, slider (+orientation wildcarded) */
        FUNCTION_HANDLE | FUNCTION_SLIDER,
        WIDGET_NULL,
        STYLE_NORMAL,
        COMMON_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        ORIENTATION_ALL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    {    /* resize grip (+edge wildcarded) */
        FUNCTION_RESIZE_GRIP,
        WIDGET_NULL,
        STYLE_NORMAL,
        COMMON_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        EDGE_ALL
    },
    {    /* check and radio */
        FUNCTION_CHECK | FUNCTION_OPTION,
        WIDGET_NULL,
        STYLE_NORMAL,
        CHECK_AND_RADIO_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    {    /* cell flat box */
        FUNCTION_FLAT_BOX,
        WIDGET_NULL,
        STYLE_NORMAL,
        CELL_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    {    /* GENERIC STUFF (will be duplicated somewhat, but *shrug*) */
        FUNCTION_ALL ^ (FUNCTION_ARROW | FUNCTION_BOX_GAP | FUNCTION_SHADOW_GAP | FUNCTION_EXTENSION | FUNCTION_EXPANDER | FUNCTION_HANDLE | FUNCTION_SLIDER | FUNCTION_RESIZE_GRIP | FUNCTION_CHECK | FUNCTION_OPTION),
        WIDGET_NULL,
        STYLE_NORMAL,
        COMMON_DETAILS,
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },

    /* The above is just copied from gtk-engines. I am too lazy right now to clean it up ... */
    
    /* Scales */
    {
        FUNCTION_SLIDER | FUNCTION_BOX | FUNCTION_FOCUS | FUNCTION_ARROW,
        WIDGET_NULL,
        STYLE_VSCALE,
        "NULL:trough:trough-upper:trough-lower:hscale:vscale:stepper",
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    {
        FUNCTION_SLIDER | FUNCTION_BOX | FUNCTION_FOCUS | FUNCTION_ARROW,
        WIDGET_NULL,
        STYLE_VSCALE,
        "NULL:trough:trough-upper:trough-lower:hscale:vscale:stepper",
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
    
    /* scrollbar */
    {
        FUNCTION_SLIDER | FUNCTION_BOX | FUNCTION_FOCUS | FUNCTION_ARROW,
        WIDGET_NULL,
        STYLE_SCROLLBAR,
        "NULL:trough:trough-upper:trough-lower:hscrollbar:vscrollbar:slider",
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },

    /* comboboxentry */
    {
        FUNCTION_BOX | FUNCTION_FOCUS | FUNCTION_ARROW | FUNCTION_SHADOW,
        WIDGET_NULL,
        STYLE_SCROLLBAR,
        "NULL:button:entry:entry_bg:arrow",
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },

    /* spinbutton */
    {
        FUNCTION_BOX | FUNCTION_FOCUS | FUNCTION_ARROW | FUNCTION_SHADOW,
        WIDGET_NULL,
        STYLE_SCROLLBAR,
        "NULL:entry:entry_bg:arrow:spinbutton:spinbutton_up:spinbutton_down",
        STATE_ALL,
        SHADOW_ALL,
        GTK_ARROW_UP,
        TRUE,
        GTK_SIDE_TOP,
        GTK_ORIENTATION_HORIZONTAL,
        GTK_EXPANDER_EXPANDED,
        GDK_WINDOW_EDGE_SOUTH
    },
};

#define ENUM_NEXT(enum_type, ind, target)                \
    do {                                \
        GEnumClass *enum_class = g_type_class_ref (enum_type);    \
        if (initial || values[ind] >= enum_class->n_values - 1) { \
            values[ind] = 0;                \
        } else {                        \
            done = FALSE;                    \
            values[ind]++;                    \
        }                            \
                                    \
        target = enum_class->values[values[ind]].value;        \
        g_type_class_unref (enum_class);            \
        if (!done && !initial)                    \
            goto end;                    \
    } while(0)

static gboolean
get_next_combination (Test *wildcard_test, Test *test, gboolean initial)
{
    gint i;
    gchar *cur_pos, *col_pos;
    gboolean done = !initial;
    static gint values[10];
    
    test->function = wildcard_test->function;
    
    if (wildcard_test->widget == WIDGET_ALL)
        ENUM_NEXT (WIDGET_TYPE, 0, test->widget);
    else
        test->widget = wildcard_test->widget;

    if (wildcard_test->state == STATE_ALL)
        ENUM_NEXT (GTK_TYPE_STATE_TYPE, 1, test->state);
    else
        test->state = wildcard_test->state;

    if (wildcard_test->shadow == SHADOW_ALL)
        ENUM_NEXT (GTK_TYPE_SHADOW_TYPE, 2, test->shadow);
    else
        test->shadow = wildcard_test->shadow;

    if (wildcard_test->arrow_type == ARROW_ALL)
        ENUM_NEXT (GTK_TYPE_ARROW_TYPE, 3, test->arrow_type);
    else
        test->arrow_type = wildcard_test->arrow_type;

    if (wildcard_test->fill == FILL_ALL)
        ENUM_NEXT (BOOL_TYPE, 4, test->fill);
    else
        test->fill = wildcard_test->fill;

    if (wildcard_test->gap_side == GAP_SIDE_ALL)
        ENUM_NEXT (GTK_TYPE_POSITION_TYPE, 5, test->gap_side);
    else
        test->gap_side = wildcard_test->gap_side;

    if (wildcard_test->orientation == ORIENTATION_ALL)
        ENUM_NEXT (GTK_TYPE_ORIENTATION, 6, test->orientation);
    else
        test->orientation = wildcard_test->orientation;

    if (wildcard_test->expander_style == EXPANDER_ALL)
        ENUM_NEXT (GTK_TYPE_EXPANDER_STYLE, 7, test->expander_style);
    else
        test->expander_style = wildcard_test->expander_style;

    if (wildcard_test->edge == EDGE_ALL)
        ENUM_NEXT (GDK_TYPE_WINDOW_EDGE, 8, test->edge);
    else
        test->edge = wildcard_test->edge;
    
end:
    /* the detail is a : seperated list ... */
    if (initial) {
        values[9] = 0;
    } else {
        /* increase, if nothing else got changed. */
        if (done)
            values[9]++;
    }
    i = 0;
    cur_pos = wildcard_test->detail;
    
    while (i < values[9] && cur_pos) {
        cur_pos = strchr (cur_pos, ':');
        if (cur_pos != NULL) cur_pos++;
        i++;
    }
    if (cur_pos == NULL) {
        values[9] = 0;
        cur_pos = wildcard_test->detail;
        if (done)
            return TRUE;
    } else {
        /* not done, as we moved on */
        done = FALSE;
    }
    if (cur_pos != NULL) {
        col_pos = strchr (cur_pos, ':');
        if (col_pos == NULL)
            test->detail = g_strdup (cur_pos);
        else
            test->detail = g_strndup (cur_pos, col_pos - cur_pos);
    } else
        test->detail = NULL;
    
    /* replace "NULL" with a NULL pointer */
    if (test->detail && strcmp (test->detail, "NULL") == 0) {
        g_free (test->detail);
        test->detail = NULL;
    }
    
    return done;
}

static void
run_functions (Test *test)
{
    GtkStyle *style = styles[test->style];
    
    if (test->function & FUNCTION_ARROW)
        gtk_paint_arrow (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, test->arrow_type, test->fill, 0, 0, 10, 10);

    if (test->function & FUNCTION_BOX)
        gtk_paint_box (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);

    if (test->function & FUNCTION_SHADOW)
        gtk_paint_shadow (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);

    if (test->function & FUNCTION_BOX_GAP)
        gtk_paint_box_gap (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10, test->gap_side, 0, 100);

    if (test->function & FUNCTION_SHADOW_GAP)
        gtk_paint_shadow_gap (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10, test->gap_side, 0, 100);


    if (test->function & FUNCTION_CHECK)
        gtk_paint_check (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);
        
    if (test->function & FUNCTION_EXPANDER)
        gtk_paint_expander (style, window->window, test->state, NULL, widgets[test->widget], test->detail, 10, 10, test->expander_style);

    if (test->function & FUNCTION_EXTENSION)
        gtk_paint_extension (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10, test->gap_side);

    if (test->function & FUNCTION_FLAT_BOX)
        gtk_paint_flat_box (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);

    if (test->function & FUNCTION_FOCUS)
        gtk_paint_focus (style, window->window, test->state, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);

    if (test->function & FUNCTION_HANDLE)
        gtk_paint_handle (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10, test->orientation);

    if (test->function & FUNCTION_OPTION)
        gtk_paint_option (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);

    if (test->function & FUNCTION_RESIZE_GRIP)
        gtk_paint_resize_grip (style, window->window, test->state, NULL, widgets[test->widget], test->detail, test->edge, 0, 0, 10, 10);

    if (test->function & FUNCTION_SLIDER)
        gtk_paint_slider (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10, test->orientation);

    if (test->function & FUNCTION_TAB)
        gtk_paint_tab (style, window->window, test->state, test->shadow, NULL, widgets[test->widget], test->detail, 0, 0, 10, 10);


    if (test->function & FUNCTION_HLINE)
        gtk_paint_hline (style, window->window, test->state, NULL, widgets[test->widget], test->detail, 1, 10, 4);

    if (test->function & FUNCTION_VLINE)
        gtk_paint_vline (style, window->window, test->state, NULL, widgets[test->widget], test->detail, 1, 10, 4);
}

static void
run_test (gint test)
{
    Test current_test = {0};
    gint initial = TRUE;
    
    while (!get_next_combination (&tests[test], &current_test, initial)) {
        initial = FALSE;
        run_functions (&current_test);
        g_free (current_test.detail);
    }
}

static void
create_testwidgets ()
{
    widgets[WIDGET_VSCROLLBAR] = gtk_vscrollbar_new (NULL);
    widgets[WIDGET_TOGGLE_BUTTON] = gtk_toggle_button_new_with_label ("blah");
}

static GtkStyle*
get_attached_style (GtkStyle *fallback, GdkWindow *window, const gchar *path, GType type)
{
    GtkSettings *settings = gtk_settings_get_default ();
    GtkStyle *style;
    
    style = gtk_rc_get_style_by_paths (settings, path, path, type);
    if (!style)
        style = g_object_ref (fallback);
    else
        g_object_ref (style);

    return gtk_style_attach (style, window);
}

static void
create_teststyles ()
{
    styles[STYLE_NORMAL] = get_attached_style (window->style, window->window, "GtkWindow.GtkButton", GTK_TYPE_BUTTON);
    styles[STYLE_VSCALE] = get_attached_style (window->style, window->window, "GtkWindow.GtkVScale", GTK_TYPE_VSCALE);
    styles[STYLE_HSCALE] = get_attached_style (window->style, window->window, "GtkWindow.GtkHScale", GTK_TYPE_HSCALE);
    styles[STYLE_SCROLLBAR] = get_attached_style (window->style, window->window, "GtkWindow.GtkScrollbar", GTK_TYPE_SCROLLBAR);

    GTK_TYPE_COMBO_BOX_ENTRY; /* Initilize the combobox entry type (not really neccessary as GTK+ will fall back to string matching) */
    styles[STYLE_COMBOBOXENTRY] = get_attached_style (window->style, window->window, "GtkWindow.GtkComboBoxEntry.GtkButton", GTK_TYPE_BUTTON);
}

static void
destroy_teststyles ()
{
    gint i;
    
    for (i = 0; i < STYLE_COUNT; i++) {
        gtk_style_detach (styles[i]);
        g_object_unref (styles[i]);
        styles[i] = NULL;
    }
}

int
main (int argc, char **argv)
{
        gchar *new_default_files[1] = {NULL};
    GtkSettings *settings;
    gint i;
    gtk_init (&argc, &argv);
        
    /* we get a gtkrc from GTK2_RC_FILES, remove the users theme ... */
    settings = gtk_settings_get_default ();
    g_object_set (settings, "gtk-theme-name", "Raleigh", NULL);
        
    create_testwidgets ();
    
    window = gtk_window_new (GTK_WINDOW_POPUP);
    box = gtk_hbox_new (FALSE, 0);
    
    gtk_container_add ((GtkContainer*)window, box);
    
    for (i = 0; i < WIDGET_COUNT; i++) {
        if (widgets[i])
            gtk_box_pack_end (GTK_BOX (box), widgets[i], FALSE, FALSE, 0);
    }
    
    gtk_widget_show_all (window);

    while (gdk_events_pending ())
        gtk_main_iteration_do (FALSE);

    create_teststyles ();
    
    for (i = 0; i < G_N_ELEMENTS (tests); i++)
        run_test (i);

    destroy_teststyles ();
    
    /* remove the rc file again. This should cause an unload of the engine
     * and destruction of all engine objects. */
    gtk_rc_set_default_files (new_default_files);
    gtk_rc_reparse_all_for_settings (settings, TRUE);
    
    while (gdk_events_pending ())
        gtk_main_iteration_do (FALSE);

    /* TODO: It would be really cool to check if there is no
     * loaded engine at this point. */

    return 0;
}
