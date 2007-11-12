/*
 * Copyright (C) 2007, Red Hat, Inc.
 * Copyright (C) 2007, Benjamin Berg
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
#include <gtk/gtk.h>

#include "sugar-style.h"
#include "sugar-rc-style.h"
#include "sugar-info.h"
#include "sugar-drawing.h"
#include "sugar-utils.h"

#define SANITIZE_SIZE g_return_if_fail (width >= -1 && height >= -1);    \
    if (width == -1 && height == -1) {                          \
        gdk_drawable_get_size (GDK_DRAWABLE (window), &width, &height);          \
    } else if (width == -1) {                                   \
        gdk_drawable_get_size (GDK_DRAWABLE (window), &width, NULL);             \
    } else if (height == -1) {                                  \
        gdk_drawable_get_size (GDK_DRAWABLE (window), NULL, &height);            \
    }

#define HINT(str) (SUGAR_RC_STYLE (style->rc_style)->hint && g_str_equal (SUGAR_RC_STYLE (style->rc_style)->hint, str))
#define DETAIL(str) (detail && g_str_equal (detail, str))


static void sugar_style_init       (SugarStyle      *style);
static void sugar_style_class_init (SugarStyleClass *klass);

GType sugar_type_style = 0;

static GtkStyleClass *parent_class;

void
sugar_style_register_type (GTypeModule *module)
{
    static const GTypeInfo object_info =
    {
        sizeof (SugarStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) sugar_style_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (SugarStyle),
        0,              /* n_preallocs */
        (GInstanceInitFunc) sugar_style_init,
    };
  
    sugar_type_style = g_type_module_register_type (module,
                                                    GTK_TYPE_STYLE,
                                                    "SugarStyle",
                                                    &object_info, 0);
}

static void
sugar_style_init (SugarStyle *style)
{
}

static void
sugar_style_init_from_rc (GtkStyle *style, GtkRcStyle *rc_style)
{
    SugarRcStyle *sugar_rc_style = SUGAR_RC_STYLE (rc_style);
    SugarStyle *sugar_style = SUGAR_STYLE (style);
    GtkStateType state;

    parent_class->init_from_rc(GTK_STYLE (style), GTK_RC_STYLE (rc_style));

    /* Copy the label fg color. */
    if (sugar_rc_style->flags & OPTION_LABEL_FG_COLOR) {
        /* Now copy colors around if wanted. If any color has been set. */
        for (state = 0; state < 5; state++) {
            if (sugar_rc_style->apply_label_color.bg & (1 << state))
                style->bg[state] = sugar_rc_style->label_fg_color;
            if (sugar_rc_style->apply_label_color.fg & (1 << state))
                style->fg[state] = sugar_rc_style->label_fg_color;
            if (sugar_rc_style->apply_label_color.base & (1 << state))
                style->base[state] = sugar_rc_style->label_fg_color;
            if (sugar_rc_style->apply_label_color.text & (1 << state))
                style->text[state] = sugar_rc_style->label_fg_color;
        }
    }
}

static cairo_t*
sugar_cairo_create (GdkWindow *window, GdkRectangle *area)
{
    cairo_t *cr;
    
    cr = gdk_cairo_create (GDK_DRAWABLE (window));
    
    if (area) {
        gdk_cairo_rectangle (cr, area);
        cairo_clip (cr);
    }
    return cr;
}


static void
sugar_style_draw_hline(GtkStyle       *style,
                       GdkWindow      *window,
                       GtkStateType    state_type,
                       GdkRectangle   *area,
                       GtkWidget      *widget,
                       const gchar    *detail,
                       gint            x1,
                       gint            x2,
                       gint            y)
{
    cairo_t *cr;
    gdouble width;
    gdouble height;

    cr = sugar_cairo_create (window, area);

    gdk_cairo_set_source_color (cr, &style->bg[state_type]);

    g_assert (x1 < x2);

    width = x2 - x1 + 1;
    height = SUGAR_RC_STYLE (style->rc_style)->line_width;

    /* Probably never used. Just in case someone wants a vertical toolbar. */
    if (DETAIL ("toolbar")) {
        /* First of all, reverse engineer the real size. Only works if no
         * rounding was done, but luckily this is not the case for anything
         * we should ever see here. */
        gint subcell_size = SUGAR_RC_STYLE (style->rc_style)->subcell_size;
        gint toolbar_width = (width - 1) * 10 / 6;
        
        x1 = x1 - (toolbar_width - width) / 2 + subcell_size;
        width = toolbar_width - 2*subcell_size;
    }

    cairo_rectangle (cr, x1, y, width, height);
    cairo_fill (cr);

    cairo_destroy (cr);
}

static void
sugar_style_draw_vline(GtkStyle       *style,
                       GdkWindow      *window,
                       GtkStateType    state_type,
                       GdkRectangle   *area,
                       GtkWidget      *widget,
                       const gchar    *detail,
                       gint            y1,
                       gint            y2,
                       gint            x)
{
    cairo_t *cr;
    gdouble width;
    gdouble height;

    cr = sugar_cairo_create (window, area);

    gdk_cairo_set_source_color (cr, &style->bg[state_type]);

    g_assert (y1 < y2);

    height = y2 - y1 + 1;
    width = SUGAR_RC_STYLE (style->rc_style)->line_width;

    if (DETAIL ("toolbar")) {
        /* First of all, reverse engineer the real size. Only works if no
         * rounding was done, but luckily this is not the case for anything
         * we should ever see here. */
        gint subcell_size = SUGAR_RC_STYLE (style->rc_style)->subcell_size;
        gint toolbar_height = (height - 1) * 10 / 6;
        
        y1 = y1 - (toolbar_height - height) / 2 + subcell_size;
        height = toolbar_height - 2*subcell_size;
    }

    cairo_rectangle (cr, x, y1, width, height);
    cairo_fill (cr);

    cairo_destroy (cr);
}


static void
sugar_style_draw_focus (GtkStyle       *style,
                       GdkWindow      *window,
                       GtkStateType    state_type,
                       GdkRectangle   *area,
                       GtkWidget      *widget,
                       const gchar    *detail,
                       gint            x,
                       gint            y,
                       gint            width,
                       gint            height)
{
    SugarInfo info;
    cairo_t *cr;
    gboolean interior_focus = TRUE;

    sugar_fill_generic_info (&info, style, state_type, GTK_SHADOW_NONE, widget, detail, x, y, width, height);
    sugar_info_get_style_property (&info, "interior-focus", &interior_focus);

    if (!interior_focus) {
        cr = sugar_cairo_create (window, area);

        if (DETAIL ("button") && HINT ("comboboxentry")) {
            sugar_remove_corners (&info.corners, info.ltr ? EDGE_LEFT : EDGE_RIGHT);
        } else if (DETAIL ("entry") && (HINT ("spinbutton") || HINT ("comboboxentry"))) {
            /* Focus for the entry of the combobox and spinbutton. The Combobox
             * focus just disappears into nothingness ... */
            info.cont_edges |= info.ltr ? EDGE_RIGHT : EDGE_LEFT;

            sugar_remove_corners (&info.corners, info.cont_edges);

            info.pos.width += info.rc_style->thick_line_width;
            if (!info.ltr)
                info.pos.x -= info.rc_style->thick_line_width;
        } else if (DETAIL ("spinbutton_up") || DETAIL ("spinbutton_down")) {
            /* spinbutton button focus hack -- this gets called from draw_box */

            /* duplicated from draw_box */
            if (DETAIL ("spinbutton_up"))
                info.cont_edges |= EDGE_BOTTOM;
            else
                info.cont_edges |= EDGE_TOP;

            info.cont_edges |= info.ltr ? EDGE_LEFT : EDGE_RIGHT;

            sugar_remove_corners (&info.corners, info.cont_edges);
        } else if (DETAIL ("trough")) {
            /* Must be scale?!? */
            SugarRangeInfo range_info;

            /* This will decrease the size of the focus as neccessary. */
            range_info.info = info;
            sugar_fill_range_info (&range_info, TRUE);
            info = range_info.info;
        }

        sugar_draw_exterior_focus (cr, &info);
        cairo_destroy (cr);
    } else {
        parent_class->draw_focus (style, window, state_type, area, widget, detail, x, y, width, height);
    }
}

static void
sugar_style_draw_slider (GtkStyle       *style,
                        GdkWindow      *window,
                        GtkStateType    state_type,
                        GtkShadowType   shadow_type,
                        GdkRectangle   *area,
                        GtkWidget      *widget,
                        const gchar    *detail,
                        gint            x,
                        gint            y,
                        gint            width,
                        gint            height,
                        GtkOrientation  orientation)
{
    cairo_t *cr;

    SANITIZE_SIZE
    
    cr = sugar_cairo_create (window, area);
    
    if (DETAIL ("hscale") || DETAIL ("vscale")) {
        SugarRangeInfo range_info;
        sugar_fill_generic_info (&range_info.info, style, state_type, shadow_type, widget, detail, x, y, width, height);
        sugar_fill_range_info (&range_info, FALSE);

        sugar_draw_scale_slider (cr, &range_info);
    } else if (HINT ("hscrollbar") || HINT ("vscrollbar") ||
               HINT ("hscrollbar-scrolled-window") ||
               HINT ("vscrollbar-scrolled-window")) {
        SugarRangeInfo range_info;

        sugar_fill_generic_info (&range_info.info, style, state_type, shadow_type, widget, detail,
                                 x, y, width, height);

        sugar_fill_range_info (&range_info, FALSE);

        sugar_draw_scrollbar_slider (cr, &range_info);
    } else {
        parent_class->draw_box (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
    }
    cairo_destroy (cr);
}

static void
sugar_style_draw_box (GtkStyle       *style,
                     GdkWindow      *window,
                     GtkStateType    state_type,
                     GtkShadowType   shadow_type,
                     GdkRectangle   *area,
                     GtkWidget      *widget,
                     const gchar    *detail,
                     gint            x,
                     gint            y,
                     gint            width,
                     gint            height)
{
    cairo_t *cr;

    SANITIZE_SIZE
    
    cr = sugar_cairo_create (window, area);
    
    if (DETAIL ("button") || DETAIL ("optionmenu") || DETAIL ("buttondefault")) {
        SugarInfo info;
        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

        /* Ignore shadow of NONE (happens for prelighted toolbuttons.
         * The prelight is handeled separately. */
        if (info.shadow == GTK_SHADOW_NONE) {
            cairo_destroy (cr);
            return;
        }

        if (HINT ("comboboxentry")) {
            info.cont_edges = info.ltr ? EDGE_LEFT : EDGE_RIGHT;
            sugar_remove_corners (&info.corners, info.cont_edges);
        }

        if (DETAIL ("buttondefault"))
            sugar_draw_button_default (cr, &info);
        else
            sugar_draw_button (cr, &info);

    } else if (DETAIL ("spinbutton")) {
        SugarInfo info;
        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

        /* Fill the background with bg_color. */
        sugar_fill_background (cr, &info);

        info.cont_edges = info.ltr ? EDGE_LEFT : EDGE_RIGHT;
        sugar_remove_corners (&info.corners, info.cont_edges);

        sugar_draw_button (cr, &info);
    } else if (DETAIL ("spinbutton_up") || DETAIL ("spinbutton_down")) {
        SugarInfo info;
        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

        info.cont_edges = info.ltr ? EDGE_LEFT : EDGE_RIGHT;

        if (DETAIL ("spinbutton_up"))
            info.cont_edges |= EDGE_BOTTOM;
        else
            info.cont_edges |= EDGE_TOP;

        sugar_remove_corners (&info.corners, info.cont_edges);
        sugar_draw_button (cr, &info);

        /* Spinbutton focus hack. */
        if (widget && GTK_WIDGET_HAS_FOCUS (widget)) {
            /* Draw a focus for the spinbutton */
            sugar_style_draw_focus (style, window, GTK_STATE_NORMAL, area, widget, detail, x, y, width, height);
        }

    } else if (DETAIL ("trough") || DETAIL ("trough-upper") || DETAIL ("trough-lower")) {
        /* scale or progress bar trough */
        if (HINT ("hscale") || HINT ("vscale")) {
            SugarRangeInfo range_info;
            sugar_fill_generic_info (&range_info.info, style, state_type, shadow_type, widget, detail, x, y, width, height);
            sugar_fill_range_info (&range_info, TRUE);

            sugar_draw_scale_trough (cr, &range_info);
        } else if (HINT ("hscrollbar") || HINT ("vscrollbar") ||
                   HINT ("hscrollbar-scrolled-window") ||
                   HINT ("vscrollbar-scrolled-window")) {
            SugarRangeInfo range_info;

            sugar_fill_generic_info (&range_info.info, style, state_type, shadow_type, widget, detail,
                                     x, y, width, height);

            sugar_fill_range_info (&range_info, TRUE);

            sugar_draw_scrollbar_trough (cr, &range_info);
        } else { /* Progress bar trough */
            SugarInfo info;
            gboolean fill;

            sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);
            
            if (widget && !GTK_WIDGET_IS_SENSITIVE (widget))
                info.state = GTK_STATE_INSENSITIVE;

            fill = (gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (widget)) != 0.0);
            
            /* Needed because the trough and bar are cached in a buffer inside GtkProgress. */
            sugar_fill_background (cr, &info);
            sugar_draw_progressbar_trough (cr, &info, fill);
        }
    } else if (DETAIL ("bar")) {
            SugarInfo info;
            GtkProgressBarOrientation orientation;

            sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);
            
            if (widget && !GTK_WIDGET_IS_SENSITIVE (widget))
                info.state = GTK_STATE_INSENSITIVE;
            
            if (widget && GTK_IS_PROGRESS_BAR (widget)) {
                orientation = gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (widget));
            } else {
                orientation = info.ltr ? GTK_PROGRESS_LEFT_TO_RIGHT : GTK_PROGRESS_RIGHT_TO_LEFT;
            }
            
            sugar_draw_progressbar_bar (cr, &info, orientation);
    } else if (DETAIL ("hseparator")) {
            /* Draw a normal separator centered in the area */
            gtk_paint_hline (style, window, state_type, area, widget, "separator",
                             x, x + width - 1,
                             y + (height - SUGAR_RC_STYLE (style->rc_style)->line_width) / 2);
    } else if (DETAIL ("vseparator")) {
            /* Draw a normal separator centered in the area */
            gtk_paint_vline (style, window, state_type, area, widget, "separator",
                             y, y + height - 1,
                             x + (width - SUGAR_RC_STYLE (style->rc_style)->line_width) / 2);
    } else if (DETAIL ("menu") || DETAIL ("palette")) {
        SugarInfo info;
        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);
        
        /* This code is to detect the up/down arrow buttons, as GTK+ unfortunately
         * does not have any special detail string :-/
         * The reason we need this hack is that there is no other way to get the
         * correct padding on the top/bottom menu item. */
        if (widget && widget->allocation.x == 0) {
            /* Just make the area larger, it does not matter really how
             * we do this ... note that this ignores GtkMenu::horizontal-padding
             * and GtkMenu::vertical-padding ... */
            if (y == style->ythickness) {
                info.pos.x -= style->xthickness;
                info.pos.width += 2*style->xthickness;
                info.pos.y -= style->ythickness;
                info.pos.height += style->ythickness;
            }
            if (y + height == widget->allocation.height - style->ythickness) {
                info.pos.x -= style->xthickness;
                info.pos.width += 2*style->xthickness;
                info.pos.height += style->ythickness;
            }
        }

        sugar_draw_menu (cr, &info, NULL);
    } else if (DETAIL ("palette-invoker") || DETAIL ("toolbutton-prelight")) {
        SugarInfo info;

        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

        sugar_draw_palette_invoker (cr, &info, NULL);
    } else {
        parent_class->draw_box (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
    }
    cairo_destroy (cr);
}

static void
sugar_style_draw_box_gap (GtkStyle       *style,
                          GdkWindow      *window,
                          GtkStateType    state_type,
                          GtkShadowType   shadow_type,
                          GdkRectangle   *area,
                          GtkWidget      *widget,
                          const gchar    *detail,
                          gint            x,
                          gint            y,
                          gint            width,
                          gint            height,
                          GtkPositionType gap_side,
                          gint            gap_x,
                          gint            gap_width)
{
    cairo_t *cr;

    SANITIZE_SIZE
    
    cr = sugar_cairo_create (window, area);

    if (DETAIL ("palette-invoker")) {
        SugarInfo info;
        SugarGapInfo gap;

        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);
        
        gap.side = gap_side;
        gap.start = gap_x;
        gap.size = gap_width;

        sugar_draw_palette_invoker (cr, &info, &gap);
    } else if (DETAIL ("palette")) {
        SugarInfo info;
        SugarGapInfo gap;

        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);
        
        gap.side = gap_side;
        gap.start = gap_x;
        gap.size = gap_width;

        sugar_draw_menu (cr, &info, &gap);
    } else {
        parent_class->draw_box_gap (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height, gap_side, gap_x, gap_width);
    }

    cairo_destroy (cr);
}

static void
sugar_style_draw_flat_box (GtkStyle       *style,
                          GdkWindow      *window,
                          GtkStateType    state_type,
                          GtkShadowType   shadow_type,
                          GdkRectangle   *area,
                          GtkWidget      *widget,
                          const gchar    *detail,
                          gint            x,
                          gint            y,
                          gint            width,
                          gint            height)
{
    /* Hack to change the entries background when it has the focus. */
    if (DETAIL ("entry_bg")) {
        if (widget && GTK_WIDGET_HAS_FOCUS (widget)) {
            state_type = GTK_STATE_ACTIVE;
        }
    } else if (DETAIL ("radiobutton") || DETAIL ("checkbutton")) {
        /* No background drawing for radio and check buttons. */
        return;
    } else if (DETAIL ("groupbox")) {
        if (state_type == GTK_STATE_NORMAL)
            state_type = GTK_STATE_INSENSITIVE;
    }

    parent_class->draw_flat_box (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
}

static void
sugar_style_draw_shadow (GtkStyle       *style,
                        GdkWindow      *window,
                        GtkStateType    state_type,
                        GtkShadowType   shadow_type,
                        GdkRectangle   *area,
                        GtkWidget      *widget,
                        const gchar    *detail,
                        gint            x,
                        gint            y,
                        gint            width,
                        gint            height)
{
    cairo_t *cr;

    SANITIZE_SIZE
    
    cr = sugar_cairo_create (window, area);
    
    if (DETAIL ("entry")) {
        SugarInfo info;

        sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

        /* Corner detection. */
        if (HINT ("comboboxentry") || HINT("spinbutton")) {
            info.cont_edges = info.ltr ? EDGE_RIGHT : EDGE_LEFT;
            sugar_remove_corners (&info.corners, info.cont_edges);

            /* Remove the padding on one side. */
            width += info.rc_style->thick_line_width;
            if (!info.ltr) {
                x -= info.rc_style->thick_line_width;
            }
        }
        
        /* XXX: This fakes an ACTIVE state for the focused entry.
         *      Getting this changed in GTK+ with a style property would be cleaner
         *      as that also works for the font colors. (see also draw_flat_box) */
        if (widget && GTK_WIDGET_HAS_FOCUS (widget)) {
            info.state = GTK_STATE_ACTIVE;
        }
        if (widget && !GTK_WIDGET_IS_SENSITIVE (widget)) {
            info.state = GTK_STATE_INSENSITIVE;
        }

        /* Fill the background with bg_color. */
        sugar_fill_background (cr, &info);
        sugar_draw_entry (cr, &info);
    } else {
        gdouble line_width;

        line_width = SUGAR_RC_STYLE (style->rc_style)->line_width;

        gdk_cairo_set_source_color (cr, &style->bg[state_type]);
        cairo_set_line_width (cr, line_width);
        cairo_rectangle (cr, x + line_width / 2.0, y + line_width / 2.0, width - line_width, height - line_width);
        cairo_stroke (cr);
    }
    
    cairo_destroy (cr);    
}

static void
sugar_style_draw_arrow (GtkStyle       *style,
                        GdkWindow      *window,
                        GtkStateType    state_type,
                        GtkShadowType   shadow_type,
                        GdkRectangle   *area,
                        GtkWidget      *widget,
                        const gchar    *detail,
                        GtkArrowType    arrow_type,
                        gboolean        fill,
                        gint            x,
                        gint            y,
                        gint            width,
                        gint            height)
{
    SugarArrowInfo arrow_info;
    SugarInfo *info = &arrow_info.info;
    cairo_t *cr;

    if (arrow_type == GTK_ARROW_NONE)
        return;

    if (width < 0 || height < 0)
        return;
    
    cr = sugar_cairo_create (window, area);

    sugar_fill_generic_info (info, style, state_type, shadow_type, widget, detail, x, y, width, height);

    arrow_info.dir = arrow_type;

    sugar_draw_arrow (cr, &arrow_info);

    cairo_destroy (cr);
}

static void
sugar_style_draw_extension(GtkStyle        *style,
                           GdkWindow       *window,
                           GtkStateType     state_type,
                           GtkShadowType    shadow_type,
                           GdkRectangle    *area,
                           GtkWidget       *widget,
                           const char      *detail,
                           int              x,
                           int              y,
                           int              width,
                           int              height,
                           GtkPositionType  gap_side)
{
    cairo_t *cr;

    cr = gdk_cairo_create (window);

    if (DETAIL("tab")) {
        gdk_cairo_set_source_color(cr, &style->bg[state_type]);
        cairo_rectangle(cr, x, y, width, height);
        cairo_fill(cr);
    } else {
        parent_class->draw_extension(style, window, state_type,
                                     shadow_type, area, widget, detail,
                                     x, y, width, height, gap_side);
    }
    
    cairo_destroy(cr);
}

static void
sugar_style_draw_option(GtkStyle        *style,
                        GdkWindow       *window,
                        GtkStateType     state_type,
                        GtkShadowType    shadow_type,
                        GdkRectangle    *area,
                        GtkWidget       *widget,
                        const char      *detail,
                        int              x,
                        int              y,
                        int              width,
                        int              height)
{
    SugarInfo info;
    cairo_t *cr;

    cr = sugar_cairo_create (window, area);

    sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

    sugar_draw_radio_button (cr, &info);

    cairo_destroy (cr);    
}

static void
sugar_style_draw_check (GtkStyle        *style,
                        GdkWindow       *window,
                        GtkStateType     state_type,
                        GtkShadowType    shadow_type,
                        GdkRectangle    *area,
                        GtkWidget       *widget,
                        const char      *detail,
                        int              x,
                        int              y,
                        int              width,
                        int              height)
{
    SugarInfo info;
    cairo_t *cr;

    cr = sugar_cairo_create (window, area);

    sugar_fill_generic_info (&info, style, state_type, shadow_type, widget, detail, x, y, width, height);

    sugar_draw_check_button (cr, &info);

    cairo_destroy (cr);    
}

static void
sugar_style_draw_layout(GtkStyle        *style,
                        GdkWindow       *window,
                        GtkStateType     state_type,
                        gboolean         use_text,
                        GdkRectangle    *area,
                        GtkWidget       *widget,
                        const char      *detail,
                        int              x,
                        int              y,
                        PangoLayout     *layout)
{
    GdkGC *gc;
    GtkStateType sugar_state = state_type;
    GtkWidget *btn = NULL;

    /* XXX: This is ugly, better patch GTK+ somehow. */
    if (widget)
        btn = widget->parent;
    
    if (btn && !GTK_IS_BUTTON (btn)) {
        if (btn->parent && GTK_IS_BUTTON (btn->parent))
            btn = btn->parent;
        else
            btn = NULL;
    }
    if (state_type != GTK_STATE_INSENSITIVE && btn) {
        /* Access private information ... */
        sugar_state = GTK_BUTTON (btn)->depressed ? GTK_STATE_ACTIVE : GTK_STATE_NORMAL;
    }
    

    /* We don't want embossed text. */
    gc = use_text ? style->text_gc[sugar_state] : style->fg_gc[sugar_state];

    if (area)
        gdk_gc_set_clip_rectangle (gc, area);

    gdk_draw_layout (window, gc, x, y, layout);

    if (area)
        gdk_gc_set_clip_rectangle (gc, NULL);
}

/* Based on the GTK+ implementation. */
static GdkPixbuf*
sugar_style_render_icon (GtkStyle               *style,
                         const GtkIconSource    *source,
                         GtkTextDirection        direction,
                         GtkStateType            state,
                         GtkIconSize             size,
                         GtkWidget              *widget,
                         const gchar            *detail)
{
    gint width = 1;
    gint height = 1;
    GdkPixbuf *scaled;
    GdkPixbuf *stated;
    GdkPixbuf *base_pixbuf;
    GdkScreen *screen;
    GtkSettings *settings;

    /* Oddly, style can be NULL in this function, because
     * GtkIconSet can be used without a style and if so
     * it uses this function.
     */

    base_pixbuf = gtk_icon_source_get_pixbuf (source);

    g_return_val_if_fail (base_pixbuf != NULL, NULL);

    if (widget && gtk_widget_has_screen (widget)) {
        screen = gtk_widget_get_screen (widget);
        settings = gtk_settings_get_for_screen (screen);
    } else if (style && style->colormap) {
        screen = gdk_colormap_get_screen (style->colormap);
        settings = gtk_settings_get_for_screen (screen);
    } else {
        settings = gtk_settings_get_default ();
        g_warning ("Using the default screen to get the icon sizes");
    }
  
    if (size != (GtkIconSize) -1 && !gtk_icon_size_lookup_for_settings (settings, size, &width, &height)) {
        g_warning (G_STRLOC ": invalid icon size '%d'", size);
        return NULL;
    }

    /* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
     * leave it alone.
     */
    if (size != (GtkIconSize)-1 && gtk_icon_source_get_size_wildcarded (source))
        scaled = sugar_pixbuf_scale_or_ref (base_pixbuf, width, height);
    else
        scaled = g_object_ref (base_pixbuf);

    /* If the state was wildcarded, then generate a state. */
    if (gtk_icon_source_get_state_wildcarded (source)) {
        if (state == GTK_STATE_INSENSITIVE) {
            guint base = 127;

            if (style) {
                GdkColor *color = &style->bg[GTK_STATE_INSENSITIVE];
                base = (color->red >> 8) + (color->green >> 8) + (color->blue >> 8);
                base = base / 3;
            }

            stated = sugar_get_insensitive_icon (scaled, 25, 127);  

            /* Unref the scaled one. */
            g_object_unref (scaled);
        } else {
            stated = scaled;
        }
    } else
      stated = scaled;

    return stated;
}

static void
sugar_style_class_init (SugarStyleClass *klass)
{
    GtkStyleClass *style_class = GTK_STYLE_CLASS(klass);
    
    parent_class = g_type_class_peek_parent(klass);

    style_class->init_from_rc = sugar_style_init_from_rc;

    style_class->draw_hline = sugar_style_draw_hline;
    style_class->draw_vline = sugar_style_draw_vline;
    style_class->draw_extension = sugar_style_draw_extension;
    style_class->draw_box = sugar_style_draw_box;
    style_class->draw_box_gap = sugar_style_draw_box_gap;
    style_class->draw_flat_box = sugar_style_draw_flat_box;
    style_class->draw_shadow = sugar_style_draw_shadow;
    style_class->draw_focus = sugar_style_draw_focus;
    style_class->draw_slider = sugar_style_draw_slider;
    style_class->draw_arrow = sugar_style_draw_arrow;
    style_class->draw_option = sugar_style_draw_option;
    style_class->draw_check = sugar_style_draw_check;
    style_class->draw_layout = sugar_style_draw_layout;

    style_class->render_icon = sugar_style_render_icon;
}


