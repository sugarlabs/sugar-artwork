${
import math

def my_floor(num):
    return int(math.floor(num))

def my_ceil(num):
    return int(math.ceil(num))


# Should we set the line width in the engine to 2.25, and draw non pixel aligned lines?
# Are these already the correct sizes for the XO?

# These sizes need to be sanity checked ...
if scaling == "100":
    xo = True
    line_width = 2.0        # 2.25px, rounded down
    thick_line_width = 3.5  # 3.5
    subcell_size = 15
    icon_base = 11
    bullet_size = 9.5
    font_height = 24
    default_padding = 6
    toolbutton_padding = 9
else: # About 72% of the XO size, adjusted so that eg. toolbuttons work
    xo = False
    line_width = 2.0            # 1.62 rounded up
    thick_line_width = 3.0      # 2.52
    subcell_size = 11           # 10.8
    icon_base = 8               # 7.92
    bullet_size = 6.5           # 6.84
    # This is a guess on the font size (Sans 10 at 96 DPI)
    font_height = 17
    default_padding = 4         # 4.32
    toolbutton_padding = 6      # 7.68


# Radio size used to be:
#
# radio_size = my_floor(subcell_size + bullet_size + line_width)
#
# But a screenshot shows that the graphic was actually rendered at 26px
# so the SVG displays at the correct size.
# FIXME this only works for 100, has not been tested in 75 resolution
radio_size = 26
scale_slider_width = my_floor(2 * subcell_size + line_width)
thickness = my_ceil(line_width)

icon_small = icon_base * 3
icon_large = icon_base * 5

}

* {
    -sugar-focus-line: @white;

    background-color: @button_grey;
    color: @black;

    border-color: none;
    border-radius: 0;
    border-style: none;

    padding: $thickness;

    /* A lot of these will probably need to be changed, but this has to
       be done when the exact sizes are known */
    -GtkWidget-interior-focus: 0;

    /* we have to disable focus border for GtkTreeView, see #1261 */
    -GtkTreeView-interior-focus: 1;
    -GtkTreeView-focus-line-width: 0;

    -GtkTextView-interior-focus: 1;

    -GtkWidget-focus-line-width: 0;  /* Prevents some drawing glitches */
    -GtkEntry-focus-line-width: 0;
    -GtkScale-focus-line-width: 0;
    -GtkScale-focus-line-width: 0;
    -GtkWidget-focus-padding: 3;
    /* 0.05 works good for both the sugar and sugar-xo themes */
    -GtkWidget-cursor-aspect-ratio: 0.05;

    -GtkWidget-wide-separators: false;
    -GtkWidget-separator-height: $thickness;
    -GtkWidget-separator-width: $thickness;

    -GtkWidget-scroll-arrow-hlength: $subcell_size;
    -GtkWidget-scroll-arrow-vlength: $subcell_size;

    -GtkRange-activate-slider: 1;

    /* We fake the default border in the theme */
    -GtkButton-default-border: 0 0 0 0;
    -GtkButton-default-outside-border: 0 0 0 0;
    -GtkButton-image-spacing: $subcell_size;

    -GtkEntry-progress-border: $thickness $thickness $thickness $thickness;

    -GtkScrolledWindow-scrollbar-spacing: 0;

    -GtkExpander-expander-size: $font_height;
    -GtkExpander-expander-spacing: 2;

    -GtkTreeView-expander-size: $font_height;

    -GtkArrow-arrow-size: 1.0;

    -GtkToolbar-space-size: $(2*subcell_size);

    -GtkProgressBar-min-horizontal-bar-height: $subcell_size;
    -GtkProgressBar-min-vertical-bar-width: $subcell_size;

    -GtkButtonBox-child-min-height: $(3*subcell_size);
    -GtkButtonBox-child-min-width: $(3*subcell_size);
    -GtkButtonBox-child-internal-pad-x: 0;
    -GtkButtonBox-child-internal-pad-y: 0;

    -GtkCheckButton-indicator-size: $radio_size;
    -GtkCheckButton-indicator-spacing: 3;
}

*:active {
    background-color: @white;
    color: @black;
}

/* Backgrounds and windows */

.background {
    padding: 0px;
    border-width: 0px;
    background-color: @panel_grey;
}

.window {
    background-color: @panel_grey;
    color: @black;
}

/* Handle this differently? */
.window *:insensitive {
    background-color: @panel_grey;
}

/* Buttons */

.button {
    ${ border = max(0, my_ceil((3*subcell_size/2.0 - icon_small / 2.0))) }
    /* It would be nicer to just set the inner-border, but that does not work
       for combo boxes ... The combobox ignores it, so set it to 0px
       See http://bugzilla.gnome.org/show_bug.cgi?id=485762 */

    -GtkButton-inner-border: 0 0 0 0;
    padding: $(border)px $(border)px $(border)px $(border)px;

    border-radius: $(2*subcell_size);
    background-color: @button_grey;
    color: @white;
}

.button * {
    color: @white;
}

.button:focused {
    border-width: $(thickness);
    border-color: @white;
    border-style: solid;
}

/* Spin buttons */

${ spin_ythickness = my_ceil(3*subcell_size - font_height) }
${ spin_xthickness = subcell_size }
${ spin_btn_ythickness = spin_ythickness }
${ spin_btn_xthickness = subcell_size * 2 }

.spinbutton.button,
.spinbutton.button:focused,
.spinbutton.button:active {
    padding: $(spin_ythickness)px $(spin_xthickness)px;
    border-color: @button_grey;
    border-width: $(thickness)px;
    background-color: @button_grey;
    color: @white;
}

.spinbutton.button:active {
    background-color: @white;
    color: @black;
    border-color: @white;
}

.spinbutton.button:insensitive {
    background-color: @selection_grey;
}

.spinbutton.button:focused,
.spinbutton.button:focused:prelight,
.spinbutton.button:focused:insensitive {
    border-color: @white;
}

/* Toggle buttons */

GtkToggleButton.button:active {
    background-color: @white;
}

GtkToggleButton.button:active GtkLabel {
    color: @black;
}

/* Entries and views */

.view {
    border-width: 0px;
    border-style: none;
    border-radius: 0;
    padding: 0px;
    background-color: @text_field_grey;
}

.entry {
    border-radius: $(2*subcell_size);
    border-width: $(thickness);
    border-color: @selection_grey;
    border-style: solid;
    background-color: @text_field_grey;
    color: @black;
    ${ entry_ythickness = my_ceil(0.2 * (subcell_size*3.0/2.0 - thickness) + thickness) }
    ${ entry_xthickness = my_ceil(0.32 * (subcell_size*3.0/2.0 - thickness) + thickness) }
    -GtkEntry-inner-border: $(2 * max(subcell_size - entry_xthickness, 0)), $(2 * max(subcell_size - entry_xthickness, 0)), $(2 * max(my_ceil((3*subcell_size - font_height - entry_ythickness*2)/2.0),0)), $(2 * max(my_floor((3*subcell_size - font_height - entry_ythickness*2)/2.0), 0))
}

.entry.progressbar {
    border-radius: $(2 * subcell_size);
    border-width: $(thickness);
    background-color: @selection_grey;
}

.entry:focused {
    background-color: @white;
}

.entry:insensitive, .view:insensitive {
    background-color: @button_grey;
}

.view:selected {
    background-color: @panel_grey;
    color: @black;
}

.entry:selected, .entry:selected:focused, 
.view:selected:focused {
    background-color: @selection_grey;
    color: @black;
}

/* Combo boxes */

GtkComboBox * {
    color: @white;
}

GtkComboBox .separator {
    /* Remove the separator turning it transparent */
    color: alpha(@theme_base_color, 0.0);
}

.toolbar GtkToggleButton.button:active,
SugarPaletteWindowWidget GtkToggleButton.button:active {
    background-color: @button_grey;
}

/* Notebooks */

.notebook {
    background-color: @selection_grey;
    border-width: 0;
    -GtkNotebook-tab-overlap: -2;
    -GtkNotebook-tab-curvature: $default_padding;
}

.notebook tab,
.notebook tab GtkLabel {
    background-color: @button_grey;
    color: @white;
}

.notebook tab:active {
    background-color: @selection_grey;
}

/* Browser notebook */

BrowserNotebook.notebook tab {
    background-color: @selection_grey;
}

BrowserNotebook.notebook tab .button {
    border-radius: $toolbutton_padding;
}

BrowserNotebook.notebook tab:active {
    background-color: @toolbar_grey;
}

BrowserNotebook.notebook tab:active *:active {
    color: @white;
}

/* Control panel */

SugarSectionView {
    background-color: @white;
    color: @black;
}

SugarSectionView *:insensitive {
    background-color: @white;
}

/* Alert */

SugarAlert {
    background-color: @black;
    color: @white;
}

SugarAlert *:insensitive {
    background-color: @black;
}

/* Tray */

SugarHTray * , SugarVTray * { background-color: @toolbar_grey;}

/* Menus and palettes */

SugarPaletteWindowWidget.background {
    background-color: @black;
}

SugarPaletteWindowWidget * {
    color: @white;
}

SugarPaletteMenuWidget.background {
    background-color: @black;
}

SugarPaletteMenuWidget * {
    color: @white;
}

SugarPaletteWindow SugarGroupBox {
    background-color: @toolbar_grey;
    color: @white;
}

SugarPaletteWindow SugarGroupBox *:insensitive {
    background-color: @toolbar_grey;
}

.menu,
.palette {
    background-color: @black;
    color: @white;

    -GtkMenu-scroll-arrow-vlength: $(my_floor(subcell_size/0.7 + 2*thickness));
    -GtkMenu-horizontal-padding: $thickness;
    -GtkMenu-vertical-padding  : $thickness;
    /* This means the outline of the submenu overlaps with a palette.
     * However in the case of two normal menus, they are next to each other.
     * It is not possible to be smarter about this, because the style comes from
     * the submenu. */
    -GtkMenu-horizontal-offset : 0;
    -GtkMenu-vertical-offset   : 0;

    padding: 0;
    border-width: 2;
    border-color: @button_grey;
    border-style: solid;
}

.menu :prelight, palette :prelight {
    color: @white
}

.menu :active, palette :active {
    background-color: @button_grey;
}

.palette {
    padding: $(thickness)px;
}

.palette .menu {
    -GtkMenu-horizontal-padding: 0;
    -GtkMenu-vertical-padding: 0;

    padding: 0px $(subcell_size)px;
}

.menu * {
    color: @white;
}

GtkMenuItem {
    padding: $subcell_size $((subcell_size * 3 - font_height) / 2);
}

/* Scrollbars */

.scrollbar {
    -GtkRange-slider-width: $subcell_size;
    -GtkRange-trough-border: 0;
    -GtkRange-stepper-size: 0;
    -GtkScrollbar-min-slider-length: $(3*subcell_size);
    -GtkScrollbar-has-forward-stepper: 0;
    -GtkScrollbar-has-backward-stepper: 0;
    -GtkScrollbar-has-secondary-forward-stepper: 0;
    -GtkScrollbar-has-secondary-backward-stepper: 0;
}

.scrollbar.trough {
    background-color: @button_grey;
    border-width: 0;
}

.scrollbar.slider {
    background-color: @white;
    border-radius: $(2*subcell_size);
    border-width: 0;
}

.scrollbar.slider:active {
    background-color: @text_field_grey;
}

/* Progress bars */

/* Scrollbar padding hack? What was that about? */

GtkProgressBar.progressbar {
    background-color: @white;
    border-color: @white;
    border-radius: 10;
    border-style: solid;
    border-width: 0;
}

GtkProgressBar.trough {
    background-color: alpha (@black, 0.0);
    border-style: solid;
    border-radius: 10;
    border-color: @button_grey;
    border-width: 2;
}

/* Separators */

GtkVSeparator, GtkHSeparator,
.toolbar GtkSeparatorToolItem {
    color: @button_grey;
}

/* Tool buttons */

.toolbar GtkToolButton .button,
SugarPaletteWindowWidget GtkToolButton .button {
    border-radius: $toolbutton_padding;
}

.toolbar GtkToolButton .button:prelight,
SugarPaletteWindowWidget GtkToolButton .button:prelight {
    background-color: @black;
}

.toolbar GtkToolButton .button:active:prelight,
SugarPaletteWindowWidget GtkToolButton .button:active:prelight {
    background-color: @button_grey;
}

/* Scales */

GtkScale {
    -GtkScale-slider-length: $scale_slider_width;
    -GtkRange-slider-width: $scale_slider_width;
}

GtkScale.trough {
    border-style: solid;
    border-radius: 30;
    border-color: @button_grey;
    border-width: 2;
}

GtkScale.trough:focused {
    border-color: @white;
}

GtkScale.trough.top, GtkScale.trough.left {
    background-color: @white;
}

GtkScale.slider {
    color: alpha(@theme_base_color, 0.0);
    background-color: alpha(@theme_base_color, 0.0);
    background-image: url("assets/scale-slider.svg");
}

GtkScale.slider:active {
    color: alpha(@theme_base_color, 0.0);
    background-image: url("assets/scale-slider-active.svg");
}

/* Radio and check buttons */

GtkCheckButton:prelight {
    background-color: alpha(@theme_base_color, 0.0);
}

.radio,
.radio row:selected,
.radio row:selected:focused {
    background-image: url("assets/radio.svg");
    background-color: alpha(@theme_base_color, 0.0);
}

.radio:selected,
.radio:selected row:selected,
.radio:selected row:selected:focused {
    background-image: url("assets/radio-selected.svg");
}

.radio:active,
.radio row:selected:active,
.radio row:selected:focused:active {
    background-image: url("assets/radio-active.svg");
}

.radio:active:selected,
.radio:selected row:selected:active,
.radio:selected row:selected:focused:active {
    background-image: url("assets/radio-active-selected.svg");
}

.check,
.check row:selected,
.check row:selected:focused {
    background-image: url("assets/checkbox-unchecked.svg");
    background-color: alpha(@theme_base_color, 0.0);
}

.check:selected,
.check:selected row:selected,
.check:selected row:selected:focused {
    background-image: url("assets/checkbox-unchecked-selected.svg");
}

.check:active,
.check row:selected:active,
.check row:selected:focused:active {
    background-image: url("assets/checkbox-checked.svg");
}

.check:active:selected,
.check:selected row:selected:active,
.check:selected row:selected:focused:active {
    background-image: url("assets/checkbox-checked-selected.svg");
}

/* Tool items */

.toolitem {
    /* arrow in the toolbarbox */
    color: @white;
    /* outline of the toolbarbutton when palette is expanded */
    border-style: solid;
    border-width: 2;
    border-color: @button_grey;
}
