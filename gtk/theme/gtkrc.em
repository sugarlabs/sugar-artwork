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


# Colors:

black = '"#000000"'
toolbar_grey = '"#404040"'
button_grey = '"#808080"'
selection_grey = '"#A6A6A6"'
panel_grey = '"#C0C0C0"'
text_field_grey = '"#E5E5E5"'
white = '"#FFFFFF"'


}
style "default"
{
    # Will be overriden in some widgets
    color["focus_line"] = $white

    bg[NORMAL] = $button_grey
    bg[PRELIGHT] = $button_grey
    bg[ACTIVE] = $white
    bg[SELECTED] = $text_field_grey

    fg[NORMAL]        = $black
    fg[ACTIVE]        = $black
    fg[SELECTED]      = $white
    fg[PRELIGHT]      = $white

    text[NORMAL] = $black # button_grey? 1. need to patch GTK+ to use ACTIVE when focused, 2. what color should this really be?
    text[ACTIVE] = $black
    text[SELECTED] = $black
    text[PRELIGHT] = $black

    base[NORMAL] = $text_field_grey
    base[ACTIVE] = $white
    base[SELECTED] = $selection_grey
    base[PRELIGHT] = $white

    fg[INSENSITIVE] = $button_grey
    text[INSENSITIVE] = $button_grey

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

    GtkToolbar::shadow-type = GTK_SHADOW_NONE

    GtkProgressBar::min-horizontal-bar-height = $subcell_size
    GtkProgressBar::min-vertical-bar-width = $subcell_size

    engine "sugar" {
        line_width = $line_width
        thick_line_width = $thick_line_width

        # Not sure about this one, but it is not that important
        max_radius = $( 2*subcell_size )
    }
}

style "window-child"
{
    bg[INSENSITIVE] = $panel_grey
    base[INSENSITIVE] = $panel_grey
    
    engine "sugar" {
        label_fg_color = $black
    }
}

style "window"
{
    bg[NORMAL] = $panel_grey
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
        scrollbar_border = $thickness
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
    bg[NORMAL]  = $button_grey
    bg[ACTIVE]  = $white
    fg[NORMAL]  = $white
    fg[ACTIVE]  = $black

    engine "sugar" {
        hint = "spinbutton"
    }
}

style "comboboxentry"
{
    # Copied from the "default" style. Part of the workaround for bug #382646.
    text[NORMAL] = $black
    text[ACTIVE] = $black
    text[SELECTED] = $black
    text[PRELIGHT] = $black

    engine "sugar" {
        hint = "comboboxentry"
    }
}


style "frame"
{
    bg[NORMAL] = $toolbar_grey
}

style "notebook"
{
    GtkNotebook::tab-overlap = -2
    
    xthickness = 0
    ythickness = 0

    bg[ACTIVE] = $button_grey
}

style "toolbutton"
{
    color["focus_line"] = $black

    bg[PRELIGHT] = $black
    bg[ACTIVE] = $button_grey

    fg[NORMAL] = $white
    fg[ACTIVE] = $white
    fg[PRELIGHT] = $white

    engine "sugar" {
        max_radius = 5.0
    }
}

style "toolbox"
{
    fg[NORMAL] = $white
    fg[ACTIVE] = $white

    bg[NORMAL] = $toolbar_grey
    bg[INSENSITIVE] = $toolbar_grey
    base[INSENSITIVE] = $toolbar_grey
}

style "panel"
{
    bg[NORMAL] = $panel_grey
    fg[NORMAL] = $black
}

style "entry"
{
    # small inner border and a large x/ythickness for entries
    # to reduce the number of hacks needed :-)
    xthickness = $(thickness*3)
    ythickness = $(thickness*3)

    # This tries to get a height of exactly 45 pixel for the entry.
    GtkEntry::inner-border = { $(max(subcell_size - thickness*3, 0)), $(max(subcell_size - thickness*3, 0)),
                               $(max(my_floor((3*subcell_size - font_height - thickness*3*2)/2),0)), $(max(my_ceil((3*subcell_size - font_height - thickness*3*2)/2), 0)) }

    GtkWidget::focus-line-width = 0
}

style "button"
{
    fg[NORMAL]  = $white

    bg[NORMAL] = $button_grey
}

style "combobox"
{
    # Work around GTK+ bug #382646
    text[NORMAL]      = $white
    text[ACTIVE]      = $white
    text[PRELIGHT]    = $white
}

style "checkbutton"
{
    base[NORMAL]      = $white
    base[PRELIGHT]    = $white
    base[ACTIVE]      = $text_field_grey

    text[PRELIGHT]    = $toolbar_grey
    text[NORMAL]      = $toolbar_grey
    text[ACTIVE]      = $toolbar_grey

    # I won't get the exact sizing, but this should be pretty close
    GtkCheckButton::indicator-size = $radio_size
    GtkCheckButton::indicator-spacing = 3
    GtkWidget::focus-padding = 3

    engine "sugar" {
        # Reset the colors to the normal color again
        # as they were overriden by the button style
        fg[NORMAL]        = label_fg_color
        fg[PRELIGHT]      = label_fg_color
        fg[ACTIVE]        = label_fg_color
    }
}

style "progressbar"
{
    xthickness = 0
    ythickness = 0
}

style "palette"
{
    bg[NORMAL] = $black
}

style "palette-child"
{
    fg[NORMAL] = $white
}

style "menu"
{
    color["bg_color"] = $black

    fg[NORMAL]   = $white
    fg[PRELIGHT] = $white
    bg[NORMAL] = $button_grey

    bg[INSENSITIVE] = $black
    base[INSENSITIVE] = $black
    
    # The following colors are for the check and radio menu items
    base[NORMAL]      = $white
    base[PRELIGHT]    = $white
    base[ACTIVE]      = $text_field_grey

    text[PRELIGHT]    = $toolbar_grey
    text[NORMAL]      = $toolbar_grey
    text[ACTIVE]      = $toolbar_grey
    
    GtkMenu::horizontal-padding = 0
    GtkMenu::vertical-padding   = 0

    xthickness = 0
    ythickness = 0
}

style "menuitem"
{
    GtkCheckMenuItem::indicator-size = $radio_size

    bg[PRELIGHT] = $button_grey

    # For check/radio menu items
    text[NORMAL] = $white
    text[PRELIGHT] = $white

    GtkMenuItem::horizontal-padding = $line_width
    xthickness = $(my_ceil(line_width * 2))
    ythickness = $thickness
}

style "separatormenuitem"
{
    GtkMenuItem::horizontal-padding = 0
    xthickness = 0
}

####################################################################
# Default style, setting some generic options and style properties
####################################################################
class "GtkWidget" style "default"


####################################################################
# Styles that apply the different background (and foreground) colors
####################################################################

# This one should probably be the default (ie. no window-child style)
widget_class "<GtkWindow>"               style "window"
widget_class "<GtkWindow>*"              style "window-child"
widget_class "<GtkWindow>*<GtkEventBox>" style "window"

# SugarToolbox
#widget_class "*<SugarToolbox>" style "toolbox"
#widget_class "*<SugarToolbox>*" style "toolbox-child"
#widget_class "*<SugarToolbox>*<GtkEventBox>" style "toolbox"
widget_class "*<SugarToolbox>*"        style "toolbox"

# SugarPalette
widget_class "*<SugarPalette>"         style "palette"
widget_class "*<SugarPalette>*"        style "palette-child"

# SugarFrameWindow
widget_class "*<SugarFrameWindow>*"    style "frame"

# SugarPanel
widget_class "*<SugarPanel>*"          style "panel"


####################################################################
# Normal widget styles, using the above things
####################################################################
# The following only uses widget_class matches to get the priority
# right. An alternative would be to lower the priority of the
# background matches (which need widget_class) to eg. "application"

# Toolbar
widget_class "*<GtkToolButton>*"          style "toolbutton"

# Menu
widget_class "*<GtkMenuShell>*"           style "menu"               # Why is this menu shell?
widget_class "*<GtkMenuItem>*"            style "menuitem"
widget_class "*<GtkSeparatorMenuItem>*"   style "separatormenuitem"

# Buttons and Combos
widget_class "*<GtkComboBox>*"         style "combobox"
widget_class "*<GtkComboBoxEntry>*"    style "comboboxentry"
widget_class "*<GtkCombo>*"            style "comboboxentry"
widget_class "*<GtkButton>*"           style "button"
widget_class "*<GtkCheckButton>*"      style "checkbutton"

# Entries
widget_class "*<GtkEntry>"       style "entry"
widget_class "*<GtkSpinButton>"  style "spinbutton"


# Misc widgets
widget_class "*<GtkNotebook>"    style "notebook"
widget_class "*<GtkScrollbar>"   style "scrollbar"
widget_class "*<GtkHScale>"      style "hscale"
widget_class "*<GtkVScale>"      style "vscale"
widget_class "*<GtkProgressBar>" style "progressbar"



