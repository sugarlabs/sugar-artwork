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
    toolbutton_padding = 10
    radio_size = 26
    scale_trough_margin = 8
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
    toolbutton_padding = 7      # 7.68
    radio_size = 18             # 18.72
    scale_trough_margin = 6     # 5.76


scale_slider_width = my_floor(2 * subcell_size + line_width)
thickness = my_ceil(line_width)

icon_small = icon_base * 3
icon_large = icon_base * 5

}

* {
    -sugar-focus-line: @white;

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
    -GtkWidget-focus-padding: 0;
    /* 0.05 works good for both the sugar and sugar-xo themes */
    -GtkWidget-cursor-aspect-ratio: 0.05;

    -GtkWidget-wide-separators: true;
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
    -GtkToolbar-shadow-type: none;

    -GtkProgressBar-min-horizontal-bar-height: $subcell_size;
    -GtkProgressBar-min-vertical-bar-width: $subcell_size;

    -GtkButtonBox-child-min-height: $(3*subcell_size);
    -GtkButtonBox-child-min-width: $(3*subcell_size);
    -GtkButtonBox-child-internal-pad-x: 0;
    -GtkButtonBox-child-internal-pad-y: 0;

    -GtkCheckButton-indicator-size: $radio_size;
    -GtkCheckButton-indicator-spacing: 3;

    -GtkWidget-text-handle-width: 110px;
    -GtkWidget-text-handle-height: 110px;
}

*:insensitive {
    color: @button_grey;
}

/* Backgrounds and windows */

GtkWindow {
    background-color: @panel_grey;
    color: @black;
}

GtkDialog {
    background-color: @black;
    color: @white;
}

SugarIntroWindow {
    background-color: @white;
}

SugarIntroWindow {
    color: @black;
}

GtkLabel, GtkLabel:insensitive {
    background-color: transparent;
}

/* Buttons */

.button {
    ${ border = max(0, my_ceil((3*subcell_size/2.0 - icon_small / 2.0))) }
    padding: $(border)px $(border)px $(border)px $(border)px;
    border-width: $(thickness)px;
    border-color: @button_grey;
    border-style: solid;
    border-radius: $(2*subcell_size)px;
    background-color: @button_grey;
    color: @white;
}

.button:focused {
    border-color: @white;
}

.button:active {
    background-color: @white;
}

.button:insensitive {
    background-color: transparent;
}

.button:active:focused {
    color: @black;
    border-color: @button_grey;
}

/* Spin buttons */

.spinbutton.button {
    border-radius: 0px;
    border-width: 0px;
    border-style: solid;
    background-color: @button_grey;
}

.spinbutton.button:last-child {
    border-radius: 0px $(2*subcell_size)px $(2*subcell_size)px 0px;
    border-width: 0px 0px 0px $(thickness)px;
    border-style: solid;
    border-color: @selection_grey;
}


.spinbutton.button:active {
    background-color: @black;
}

.spinbutton.button:insensitive {
    background-color: @selection_grey;
}

/* Toggle buttons */

GtkToggleButton.button:active {
    background-color: @white;
    color: @black;
}

/* Entries and views */

.view {
    border-width: 0px;
    border-style: none;
    border-radius: 0px;
    padding: 0px;
    background-color: @white;
}

GtkTreeView {
    background-color: @button_grey;
}

column-header .button,
column-header .button:hover:active {
    border-radius: 0px;
    background-color: @button_grey;
    border-width: 0px;
}

GtkTreeView row:even {
    background-color: @row_even;
}
GtkTreeView row:odd {
    background-color: @row_odd;
}

GtkTreeView row:nth-child(even) {
    background-color: @row_even;
}
GtkTreeView row:nth-child(odd) {
    background-color: @row_odd;
}

.entry {
    border-radius: $(2 * subcell_size)px;
    border-width: $(thickness)px;
    border-color: @text_field_grey;
    border-style: solid;
    background-color: @text_field_grey;
    color: @black;
    ${ entry_ythickness = my_ceil(0.2 * (subcell_size*3.0/2.0 - thickness) + thickness) }
    ${ entry_xthickness = my_ceil(0.32 * (subcell_size*3.0/2.0 - thickness) + thickness) }
    padding: $(2 * max(my_ceil((3*subcell_size - font_height - entry_ythickness*2)/2.0),0))px $(2 * max(subcell_size - entry_xthickness, 0))px $(2 * max(my_floor((3*subcell_size - font_height - entry_ythickness*2)/2.0), 0))px $(2 * max(subcell_size - entry_xthickness, 0))px;
}

.entry.progressbar {
    border-radius: $(2 * subcell_size)px;
    border-width: $(thickness)px;
    background-color: @selection_grey;
}

.entry:focused {
    background-color: @white;
}

.toolbar .entry:focused {
    border-color: @white;
}

.entry:insensitive {
    background-color: @button_grey;
}

.entry:insensitive {
    border-color: @button_grey;
}

.view:selected {
    background-color: @panel_grey;
    color: @black;
}

.entry:selected,
.entry:selected:focused,
.view:selected:focused {
    background-color: @selection_grey;
    color: @black;
}

.entry:selected,
.entry:selected:focused {
    border-color: @selection_grey;
}

/* Frames */

GtkScrolledWindow.frame {
    border-style: solid;
    border-color: @selection_grey;
    border-width: $(thickness)px;
    border-radius: 0;
    padding: $(default_padding)px;
}

GtkFrame.frame.journal-preview-box {
    border-style: solid;
    border-color: @button_grey;
    border-width: $(thickness)px;
}

/* Combo boxes */

GtkComboBox {
    color: @white;
}

GtkComboBox .menu.button {
    border-color: transparent;
    border-radius: 0px;
    border-style: none;
    background-color: transparent;
    color: @white;
}

GtkComboBox .menu {
    -GtkWidget-scroll-arrow-hlength: $(my_floor(2.5 * subcell_size));
    -GtkWidget-scroll-arrow-vlength: $(my_floor(2.5 * subcell_size));
}

/* Notebooks */

.notebook {
    background-color: @selection_grey;
    color: @black;
    padding: 0px;
    -GtkNotebook-tab-overlap: -2;
    -GtkNotebook-tab-curvature: $default_padding;
    -GtkWidget-scroll-arrow-hlength: $(my_floor(2.5 * subcell_size));
    -GtkWidget-scroll-arrow-vlength: $(my_floor(2.5 * subcell_size));
}

.notebook.arrow {
    color: @white;
}

.notebook tab {
    background-color: @selection_grey;
}

/* Setting white color to the tab labels using only the previous rule */
/* doesn't work, so we have to set white color to the GtkLabel, and make */
/* sure the buttons get black color in the next two rules: */
.notebook tab GtkLabel {
    color: @white;
    padding: $(subcell_size)px 0;
}

.notebook tab .button GtkLabel {
    color: @black;
}

.notebook tab:active {
    background-color: @toolbar_grey;
}

.notebook tab .button {
    border-radius: $(toolbutton_padding)px;
}

/* Browse Widgets */

BrowseSearchWindow .view {
    background-color: @black;
    color: @white;
    border-color: @button_grey;
    border-width: 0 $(thickness)px $(thickness)px $(thickness)px;
    border-style: solid;
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

.button SugarTimeoutIcon GtkLabel,
.button SugarTimeoutIcon GtkLabel:prelight {
    background-color: @white;
    color: @button_grey;
    border-radius: $(2 * subcell_size)px;
}

.button SugarTimeoutIcon GtkLabel:active {
    background-color: @toolbar_grey;
    color: @white;
}

/* Tray */

SugarHTray, SugarVTray {
    background-color: @toolbar_grey;
}

SugarHTray * , SugarVTray * { background-color: @toolbar_grey;}

/* Menus and palettes */

SugarPaletteWindowWidget,
GtkPopover {
    border-width: $(thickness)px;
    border-color: @button_grey;
    border-style: solid;
    background-color: @black;
    color: @white;
}

SugarPaletteWindowWidget .view {
    color: @black;
}

SugarPaletteMenuWidget {
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

.menu {
    background-color: @black;
    color: @white;

    -GtkMenu-scroll-arrow-vlength: $(my_floor(subcell_size/0.7 + 2*thickness));
    -GtkMenu-horizontal-padding: $thickness;
    -GtkMenu-vertical-padding  : $thickness;
    /* This means the outline of the submenu overlaps with a palette.
     * However in the case of two normal menus, they are next to each other.
     * It is not possible to be smarter about this, because the style comes from
     * the submenu. */
    -GtkMenu-horizontal-offset :  -$(line_width)px;
    -GtkMenu-vertical-offset   : 0;

    padding: 0px;

    border-width: 2px;
    border-color: @button_grey;
    border-style: solid;
}

.menuitem {
    padding: $(subcell_size)px $((subcell_size * 3 - font_height) / 2)px;
}

.menuitem:prelight, .menuitem:hover {
    background-color: @button_grey;
}

.menuitem.separator {
    padding: 0;
}

.tooltip {
    background-color: @black;
    border-style: solid;
    border-width: $(thickness)px;
    border-color: @button_grey;
}

.tooltip * {
    color: @white;
}

/* Scrollbars */

.scrollbar {
    -GtkRange-slider-width: $(subcell_size + 2*thickness);
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
    border-width: 0px;
}

.scrollbar.slider {
    background-color: @white;
    border-radius: $(2*subcell_size)px;
    border-width: $(thickness)px;
    border-color: @button_grey;
    border-style: solid;
}

.scrollbar.slider:active {
    background-color: @text_field_grey;
}

/* Progress bars */

/* Scrollbar padding hack? What was that about? */

GtkProgressBar.progressbar {
    background-color: @white;
    border-color: @button_grey;
    border-radius: $(subcell_size)px;
    border-style: solid;
    border-width: $(thickness)px;
}

GtkProgressBar.trough {
    background-color: @selection_grey;
    border-style: solid;
    border-radius: $(subcell_size)px;
    border-color: @button_grey;
    border-width: $(thickness)px;
}

.toolbar GtkProgressBar.trough,
SugarPaletteWindowWidget GtkProgressBar.trough {
    background-color: @black;
}

/* Separators */

.separator {
    border-style: solid;
    border-color: @button_grey;
    border-width: $(thickness)px;
}

/* Tool buttons */

.toolbar {
padding: 0px;
background-color: @toolbar_grey;
color: @white;
}

.toolbar .button,
SugarPaletteWindowWidget SugarRadioToolButton .button {
    border-color: transparent;
    border-radius: 0px;
    border-style: none;
}

.toolbar .button #gtk-toolbar-arrow {
    padding: 0 $(subcell_size + default_padding)px;
}

.toolbar GtkToolButton .button,
.toolbar SugarRadioToolButton .button,
SugarPaletteWindowWidget SugarRadioToolButton .button,
SugarPaletteWindowWidget GtkToolButton .button {
    background-color: transparent;
    border-radius: $(toolbutton_padding)px;
    padding: $(toolbutton_padding)px;
}

.toolbar GtkToolButton .button,
SugarPaletteWindowWidget GtkToolButton .button:prelight {
    padding: $(toolbutton_padding - default_padding)px;
    border-width: $(default_padding)px;
    border-style: solid;
    background-clip: padding-box;
}

.toolbar GtkToolButton .button:prelight,
.toolbar GtkToolButton .button:prelight GtkBox,
SugarPaletteWindowWidget GtkToolButton .button:prelight {
    background-color: @black;
}

.toolbar SugarRadioToolButton *:active,
SugarPaletteWindowWidget SugarRadioToolButton *:active,
.toolbar SugarRadioToolButton *:active GtkBox,
.toolbar SugarRadioToolButton *:checked,
.toolbar SugarRadioToolButton *:checked GtkBox,
SugarPaletteWindowWidget SugarRadioToolButton *:checked,
.toolbar SugarToggleToolButton *:checked,
.toolbar SugarToggleToolButton *:checked GtkBox,
SugarPaletteWindowWidget SugarToggleToolButton *:checked {
    background-color: @button_grey;
    border-radius: $(toolbutton_padding)px;
}

.toolbar SugarRadioToolButton *:checked:prelight,
.toolbar SugarRadioToolButton *:checked:prelight GtkBox,
SugarPaletteWindowWidget SugarRadioToolButton *:checked:prelight,
.toolbar SugarToggleToolButton *:checked:prelight,
.toolbar SugarToggleToolButton *:checked:prelight GtkBox,
SugarPaletteWindowWidget SugarToggleToolButton *:checked:prelight {
    background-color: @button_grey;
}

SugarPaletteWindowWidget SugarRadioToolButton *:checked:prelight,
SugarPaletteWindowWidget SugarToggleToolButton *:checked:prelight {
    border-color: @button_grey;
}

SugarPaletteWindowWidget GtkToolButton .button:active {
    background-color: @button_grey;
}

.toolbar GtkToolButton .button:active {
    background-color: @button_grey;
    border-radius: $(toolbutton_padding)px;
}

SugarPaletteWindowWidget GtkScrolledWindow {
    background-color: @black;
}

.toolbar GtkComboBox .button {
    border-radius: $(2*subcell_size)px;
}

/* Scales */

.scale {
    -GtkScale-slider-length: $scale_slider_width;
    -GtkRange-slider-width: $scale_slider_width;
}

/* We have to override the color of the scale, otherwise the slider
   background image is invisible or not set for this palettes.
   Upstream bug: https://bugzilla.gnome.org/show_bug.cgi?id=686703 */
SugarPaletteWindowWidget .scale {
    color: transparent;
}

.scale.trough {
    background-color: @button_grey;
    border-style: solid;
    border-color: @button_grey;
    border-width: $(thickness)px;
    margin: $(scale_trough_margin)px 0;
}

.scale.trough.vertical {
    margin: 0 $(scale_trough_margin)px;
}

.scale.trough:focused {
    border-color: @white;
}

.scale.trough.top,
.scale.trough.left {
    background-color: @white;
}

.scale.trough.top:focused,
.scale.trough.left:focused {
    border-color: @selection_grey;
}

.scale.trough {
    border-radius: 0px $(2*subcell_size)px $(2*subcell_size)px 0px;
}

.scale.trough.vertical {
    border-radius: 0px 0px $(2*subcell_size)px $(2*subcell_size)px;
}

.scale.trough.top {
    border-radius: $(2*subcell_size)px $(2*subcell_size)px 0px 0px;
}

.scale.trough.left {
    border-radius: $(2*subcell_size)px 0px 0px $(2*subcell_size)px;
}

.scale.slider,
.scale.slider:active {
    background-color: transparent;
}

.scale.slider {
    background-image: url("assets/scale-slider.svg");
}

.scale.slider:active {
    background-image: url("assets/scale-slider-active.svg");
}

/* Radio and check buttons */

GtkCheckButton {
    border: 1px solid @black;
}

GtkCheckButton:prelight {
    background-color: alpha(@theme_base_color, 0.0);
}

.toolbar GtkCheckButton,.toolbar .check,
SugarPaletteWindowWidget .check {
    color: @theme_base_color;
    border: 1px solid @theme_base_color;
}

.radio,
.radio row:selected,
.radio row:selected:focused {
    background-image: url("assets/radio.svg");
    /* Gtk 3.14+ expect these to be included in icon theme however we want to
       override that */
    -gtk-icon-source: url("assets/radio.svg");
    background-color: alpha(@theme_base_color, 0.0);
}

.radio:selected,
.radio:selected row:selected,
.radio:selected row:selected:focused {
    background-image: url("assets/radio-selected.svg");
    -gtk-icon-source: url("assets/radio-selected.svg");
}

.radio:active,
.radio row:selected:active,
.radio row:selected:focused:active {
    background-image: url("assets/radio-active.svg");
    -gtk-icon-source: url("assets/radio-active.svg");
}

.radio:checked,
.radio row:selected:checked,
.radio row:selected:focused:checked {
    background-image: url("assets/radio-active.svg");
    -gtk-icon-source: url("assets/radio-active.svg");
}

.radio:active:selected,
.radio:selected row:selected:active,
.radio:selected row:selected:focused:active {
    background-image: url("assets/radio-active-selected.svg");
    -gtk-icon-source: url("assets/radio-active-selected.svg");
}

.radio:checked:selected,
.radio:selected row:selected:checked,
.radio:selected row:selected:focused:checked {
    background-image: url("assets/radio-active-selected.svg");
    -gtk-icon-source: url("assets/radio-active-selected.svg");
}

.check,
.check row:selected,
.check row:selected:focused {
    background-image: url("assets/checkbox-unchecked.svg");
    -gtk-icon-source: url("assets/checkbox-unchecked.svg");
    background-color: alpha(@theme_base_color, 0.0);
}

.check:selected,
.check:selected row:selected,
.check:selected row:selected:focused {
    background-image: url("assets/checkbox-unchecked-selected.svg");
    -gtk-icon-source: url("assets/checkbox-unchecked-selected.svg");
}

.check:active,
.check row:selected:active,
.check row:selected:focused:active {
    background-image: url("assets/checkbox-checked.svg");
    -gtk-icon-source: url("assets/checkbox-checked.svg");
}

.check:checked,
.check row:selected:checked,
.check row:selected:focused:checked {
    background-image: url("assets/checkbox-checked.svg");
    -gtk-icon-source: url("assets/checkbox-checked.svg");
}

.check:active:selected,
.check:selected row:selected:active,
.check:selected row:selected:focused:active {
    background-image: url("assets/checkbox-checked-selected.svg");
    -gtk-icon-source: url("assets/checkbox-checked-selected.svg");
}

.check:checked:selected,
.check:selected row:selected:checked,
.check:selected row:selected:focused:checked {
    background-image: url("assets/checkbox-checked-selected.svg");
    -gtk-icon-source: url("assets/checkbox-checked-selected.svg");
}

/* Tool items */

.toolitem {
    /* arrow in the toolbarbox */
    color: @white;
    /* outline of the toolbarbutton when palette is expanded */
    border-style: solid;
    border-width: 2px;
    border-color: @button_grey;
}

/* GtkImage */

GtkImage {
    background-color: transparent;
}

/* Sugar Frame Window */

SugarFrameWindow {
    background-color: @toolbar_grey;
}

/* Sugar Canvas icon */

SugarCanvasIcon,
SugarKeepIcon.button {
    border-color: transparent;
    border-radius: $(4 * thickness)px;
    border-width: 2px;
    border-style: solid;
    background-color: transparent;
}

SugarCanvasIcon:prelight,
SugarKeepIcon.button:prelight {
    border-color: @zoom_views_prelight;
    background-color: @zoom_views_prelight;
}

SugarCanvasIcon:active,
SugarKeepIcon.button.toggle-press {
    border-color: @zoom_views_active;
    background-color: @zoom_views_active;
}

/* Sugar CellRenderer Icons */

.cell.sugar-icon-cell {
    background-color: transparent;
    border-radius: $(4 * thickness)px;
    border-color: @white;
    border-width: $(thickness)px;
    border-style: solid;
}

.cell.sugar-icon-cell:prelight {
    background-color: @zoom_views_prelight;
}

.cell.sugar-icon-cell:active {
    background-color: @zoom_views_active;
}

/* CellRendererProgress */

.cell.progressbar, .cell.trough {
    border-style: solid;
    border-width: $(thickness)px;
    border-color: @selection_grey;
    border-radius: $(2*subcell_size)px;
}

.cell.progressbar {
    background-color: @white;
}

.cell.trough {
    background-color: @text_field_grey;
}

/* Text cursor handles */

.cursor-handle.top,
.cursor-handle.bottom {
    background-color: transparent;
    box-shadow: none;
    border-style: none;
    border-image: none;
    border-radius: 0px;
    border-width: 0px;
}

.cursor-handle.bottom {
    background-image: url("assets/cursor-handle-bottom.svg");
}

.cursor-handle.top {
    background-image: url("assets/cursor-handle-top.svg");
}

.cursor-handle.insertion-cursor {
    background-image: url("assets/cursor-handle-insertion.svg");
}

/* Application specific properties */

EvView {
    background-color: @panel_grey;
}

EvView:selected,
ApDocView:selected {
    background-color: @selection_grey;
}
