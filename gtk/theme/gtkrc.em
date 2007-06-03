${
import math

def my_floor(num):
    return int(math.floor(num))

def my_ceil(num):
    return int(math.ceil(num))


# Should we set the line width in the engine to 2.25, and draw non pixel aligned lines?
# Are these already the correct sizes for the XO?

# These sizes need to be sanity checked ...
if theme == "sugar-xo":
    xo = True
    line_width = 2.25       # 2.25
    thick_line_width = 3.5  # 3.5
    subcell_size = 15
    bullet_size = 9.5
    font_height = 24
else: # About 50% smaller
    xo = False
    line_width = 1.125
    thick_line_width = 1.75
    subcell_size = 7
    bullet_size = 5
    font_height = 12


radio_size = my_floor(subcell_size + bullet_size + line_width)
scale_slider_width = my_floor(2 * subcell_size + line_width)
thickness = my_ceil(line_width)

}
style "default"
{
    # Will be overriden in some widgets
    color["focus_line"] = "#FFFFFF"

    bg[NORMAL] = "#808080"
    bg[PRELIGHT] = "#808080"
    bg[ACTIVE] = "#ffffff"
    bg[SELECTED] = "#e7e7e7"

    fg[NORMAL]        = "#000000"
    fg[ACTIVE]        = "#000000"
    fg[SELECTED]      = "#ffffff"
    fg[PRELIGHT]      = "#ffffff"

    text[NORMAL] = "#000000" # "#7f7f7f" 1. need to patch GTK+ to use ACTIVE when focused, 2. what color should this really be?
    text[ACTIVE] = "#000000"
    text[SELECTED] = "#000000"
    text[PRELIGHT] = "#000000"

    base[NORMAL] = "#E6E6E6"
    base[ACTIVE] = "#ffffff"
    base[SELECTED] = "#a6a6a6"
    base[PRELIGHT] = "#ffffff"

    fg[INSENSITIVE] = "#808080"
    text[INSENSITIVE] = "#808080"

    xthickness = $thickness
    ythickness = $thickness

    # A lot of these will probably need to be changed, but this has to
    # be done when the exact sizes are known
    GtkWidget::interior-focus = 0
    GtkWidget::focus-line-width = 1   # This does not do anything really
    GtkWidget::focus-padding = 0
    # 0.05 works good for both the sugar and sugar-xo themes
    GtkWidget::cursor-aspect-ratio = 0.05

    GtkWidget::wide-separators = 0
    GtkWidget::separator-height = $thickness
    GtkWidget::separator-width = $thickness

    GtkRange::activate-slider = 1
    GtkButton::inner-border = { 6, 6, 6, 6 }           # ??

    GtkButton::default-border = { 1, 1, 1, 1 }         # line_width - focus-line-width
    GtkButton::default-outside-border = { 2, 2, 2, 2 } # focus-line-width

    GtkScrolledWindow::scrollbar-spacing = 0

    GtkExpander::expander-size = 24
    GtkExpander::expander-spacing = 2     # XXX

    GtkTreeView::expander-size = 24
    
    GtkArrow::arrow-size = 1.0

    engine "sugar" {
        # Is this correct? Should we make sure it is pixel aligned?
        # 6 pixel for the XO version, and 3 for the "normal" one
        line_width = $line_width
        # 9-10 pixel for the XO version, and 4 for the "normal" one
        thick_line_width = $thick_line_width

        max_radius = $( xo ? 25 : 15)
    }
}

style "window-content"
{
    color["bg_color"] = "#C0C0C0"

    bg[INSENSITIVE] = @bg_color
    base[INSENSITIVE] = @bg_color
}

style "window"
{
    color["bg_color"] = "#C0C0C0"

    bg[NORMAL] = @bg_color
}

style "scrollbar"
{
    GtkRange::slider-width = 16    # ?
    GtkRange::trough-border = 0
    GtkRange::stepper-spacing = 0
    GtkScrollbar::min-slider-length = 30    # ?
    GtkScrollbar::has-forward-stepper = 0
    GtkScrollbar::has-backward-stepper = 0
    GtkScrollbar::has-secondary-forward-stepper = 0
    GtkScrollbar::has-secondary-backward-stepper = 0

    # Hack for mozilla as it does not obey the above style properties ...
    # Really, mozilla should be fixed ...
    GtkRange::stepper-size = 1

    engine "sugar" {
        hint = "scrollbar"
        scrollbar_border = 3
    }
}

style "scale"
{
    GtkWidget::focus-line-width = 0

    # "square" slider (really round of course)
    # Same as indicator-size?
    GtkRange::slider-width = $scale_slider_width
    GtkScale::slider-length = $scale_slider_width
    GtkRange::trough-border = 0
    GtkRange::stepper-spacing = 0
    GtkRange::trough-side-details = 1
}

# hints for the trough sizing hacks
style "hscale" = "scale"
{
    engine "sugar" {
        hint = "hscale"
    }
}

style "vscale" = "scale"
{
    engine "sugar" {
        hint = "vscale"
    }
}

style "spinbutton"
{
    fg[NORMAL]        = "#ffffff"
    engine "sugar" {
        hint = "spinbutton"
    }
}

style "comboboxentry"
{
    # Copied from the "default" style. Part of the workaround for bug #382646.
    text[NORMAL] = "#000000"
    text[ACTIVE] = "#000000"
    text[SELECTED] = "#000000"
    text[PRELIGHT] = "#000000"

    engine "sugar" {
        hint = "comboboxentry"
    }
}


style "frame"
{
    bg[NORMAL] = "#404040"
}

style "notebook"
{
    GtkNotebook::tab-overlap = -2
    
    xthickness = 0
    ythickness = 0
}

style "toolbutton"
{
    color["focus_line"] = "#000000"

    #bg[NORMAL] = "#404040"
    bg[PRELIGHT] = "#000000"
    bg[ACTIVE] = "#808080"

    fg[NORMAL] = "#FFFFFF"
    fg[ACTIVE] = "#FFFFFF"
    fg[PRELIGHT] = "#ffffff"

    engine "sugar" {
        max_radius = 5.0
    }
}

style "toolbox"
{
    color["bg_color"] = "#404040"

    fg[NORMAL] = "#FFFFFF"
    fg[ACTIVE] = "#FFFFFF"

    bg[NORMAL] = @bg_color
}

style "toolbox-content"
{
    GtkToolbar::shadow-type = GTK_SHADOW_NONE

    color["bg_color"] = "#404040"

    bg[INSENSITIVE] = @bg_color
}


style "panel"
{
    bg[NORMAL] = "#C0C0C0"
    fg[NORMAL] = "#000000"
}

style "entry"
{
    # small inner border and a large x/ythickness for entries
    # to reduce the number of hacks needed :-)
    xthickness = $(thickness*3)
    ythickness = $(thickness*3)

    # This tries to get a height of exactly 45 pixel for the entry.
    GtkEntry::inner-border = { $(subcell_size - thickness*3), $(subcell_size - thickness*3),
                               $(my_floor((3*subcell_size - font_height - thickness*3*2)/2)), $(my_ceil((3*subcell_size - font_height - thickness*3*2)/2)) }

    GtkWidget::focus-line-width = 0
}

style "button"
{
    fg[NORMAL]        = "#ffffff"
}

style "combobox"
{
    # Work around GTK+ bug #382646
    text[NORMAL]      = "#FFFFFF"
    text[ACTIVE]      = "#FFFFFF"
    text[PRELIGHT]    = "#ffffff"
}

style "checkbutton"
{
    fg[NORMAL]        = "#000000"
    fg[PRELIGHT]      = "#000000"
    fg[ACTIVE]        = "#000000"
    
    base[NORMAL]      = "#ffffff"
    base[PRELIGHT]    = "#ffffff"
    base[ACTIVE]      = "#E6E6E6"

    text[PRELIGHT]    = "#5d5d5d"
    text[NORMAL]      = "#5d5d5d"
    text[ACTIVE]      = "#5d5d5d"

    # I won't get the exact sizing, but this should be pretty close
    GtkCheckButton::indicator-size = $radio_size
    GtkCheckButton::indicator-spacing = 3
    GtkWidget::focus-padding = 3
}

style "progressbar"
{
    xthickness = 0
    ythickness = 0

    # Make the bar visible (only until the engine implements it!)
    bg[PRELIGHT] = "#ffffff"
}

style "menu"
{
    color["bg_color"] = "#000000"

    fg[NORMAL]   = "#ffffff"
    fg[PRELIGHT] = "#ffffff"
    bg[NORMAL] = "#808080"
    
    # The following colors are for the check and radio menu items
    base[NORMAL]      = "#ffffff"
    base[PRELIGHT]    = "#ffffff"
    base[ACTIVE]      = "#E6E6E6"

    text[PRELIGHT]    = "#5d5d5d"
    text[NORMAL]      = "#5d5d5d"
    text[ACTIVE]      = "#5d5d5d"
    
    GtkMenu::horizontal-padding = 0
    GtkMenu::vertical-padding   = 0

    xthickness = 0
    ythickness = 0
}

style "menuitem"
{
    GtkCheckMenuItem::indicator-size = $radio_size

    bg[PRELIGHT] = "#808080"

    # For check/radio menu items
    text[NORMAL] = "#FFFFFF"
    text[PRELIGHT] = "#FFFFFF"

    GtkMenuItem::horizontal-padding = $line_width
    xthickness = $(my_ceil(line_width * 2))
    ythickness = $thickness
}

style "separatormenuitem"
{
    GtkMenuItem::horizontal-padding = 0
    xthickness = 0
}


class "GtkWidget"      style "default"
class "GtkWindow"      style "window"
class "GtkEventBox"    style "window"
class "GtkNotebook"    style "notebook"
class "GtkEntry"       style "entry"
class "GtkSpinButton"  style "spinbutton"
class "GtkScrollbar"   style "scrollbar"
class "GtkHScale"      style "hscale"
class "GtkVScale"      style "vscale"
class "GtkProgressBar" style "progressbar"

widget_class "<GtkWindow>.*"                style "window-content"

widget_class "*<SugarToolbox>*"             style "toolbox"
widget_class "*<SugarToolbox>.GtkNotebook"  style "toolbox"
widget_class "*<SugarToolbox>*"             style "toolbox-content"
widget_class "*<GtkToolButton>*"            style "toolbutton"

widget_class "*<GtkMenu>*"                style "menu"
widget_class "*<GtkMenuItem>*"            style "menuitem"
widget_class "*<GtkSeparatorMenuItem>*"   style "menuitem"

widget_class "*<GtkComboBox>*"         style "combobox"
widget_class "*<GtkComboBoxEntry>*"    style "comboboxentry"
widget_class "*<GtkCombo>*"            style "comboboxentry"
widget_class "*<GtkButton>*"           style "button"
widget_class "*<GtkCheckButton>*"      style "checkbutton"

widget_class "*<SugarPanel>"           style "panel"

widget_class "*<SugarFrameWindow>*"    style "frame"
