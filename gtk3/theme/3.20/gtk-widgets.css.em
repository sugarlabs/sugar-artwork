${
import os
import math

gtk_major, gtk_minor, gtk_patch = map(int, gtk.split('.'))
treeview_pseudo_element = gtk_major >= 3 and gtk_minor >= 16

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

disabled_opacity = 0.5

}

* {
    -sugar-focus-line: @white;

    /* 0.05 works good for both the sugar and sugar-xo themes */
    -GtkWidget-cursor-aspect-ratio: 0.05;

    -GtkWidget-wide-separators: true;
    -GtkWidget-separator-height: $thickness;
    -GtkWidget-separator-width: $thickness;

    -GtkWidget-scroll-arrow-hlength: $subcell_size;
    -GtkWidget-scroll-arrow-vlength: $subcell_size;

    -GtkRange-activate-slider: 1;

    -GtkScrolledWindow-scrollbar-spacing: 0;

    -GtkExpander-expander-size: $font_height;
    -GtkExpander-expander-spacing: 2;

    -GtkTreeView-expander-size: $font_height;

    -GtkArrow-arrow-size: 1.0;

    -GtkToolbar-space-size: $(2*subcell_size);

    -GtkProgressBar-min-horizontal-bar-height: $subcell_size;
    -GtkProgressBar-min-vertical-bar-width: $subcell_size;

    -GtkCheckButton-indicator-size: $radio_size;
    -GtkCheckButton-indicator-spacing: 3;

    -GtkWidget-text-handle-width: 110px;
    -GtkWidget-text-handle-height: 110px;
}

*:disabled {
    color: @panel_grey;
}

/* Backgrounds and windows */

window {
    background: @panel_grey;
    color: @black;
}

dialog {
    background: @black;
    color: @white;
}

introwindow {
    background: @white;
    color: @black;
}

label, label:disabled {
    background: transparent;
}

/* Buttons */

button {
    ${ border = max(0, my_ceil((3*subcell_size/2.0 - icon_small / 2.0))) }
    padding: $(border)px $(border)px $(border)px $(border)px;
    border-width: $(thickness)px;
    border-color: @button_grey;
    border-style: solid;
    border-radius: $(2*subcell_size)px;
    background: @button_grey;
    color: @white;
}

button:focus {
    border-color: @white;
    color: @white;
}

button:active {
    background: @white;
    color: @black;
}

button:disabled {
    background: transparent;
}

button:active:focus {
    color: @black;
    border-color: @button_grey;
}

/* Linked boxes (eg. combobox entries) */

box.linked *:not(:first-child):not(:last-child) {
    border-radius: 0;
}

box.linked *:first-child:not(:last-child) {
    border-top-right-radius: 0;
    border-bottom-right-radius: 0;
}

box.linked *:not(:first-child):last-child {
    border-top-left-radius: 0;
    border-bottom-left-radius: 0;
}

/* Spin buttons */

spinbutton button {
    background: @button_grey;
    border-style: solid;
    border-radius: 0;
}

spinbutton.horizontal button:last-child {
    border-radius: 0 $(2*subcell_size)px $(2*subcell_size)px 0;
}

spinbutton.vertical button:first-child {
    border-radius: $(2*subcell_size)px $(2*subcell_size)px 0 0;
    border-width: $(thickness)px $(thickness)px 0 $(thickness)px;
    border-style: solid;
}

spinbutton.vertical button:last-child {
    border-radius: 0 0 $(2*subcell_size)px $(2*subcell_size)px;
    border-width: 0 $(thickness)px $(thickness)px $(thickness)px;
    border-style: solid;
}

spinbutton button:active {
    background: @black;
}

spinbutton button:disabled {
    background: @selection_grey;
}

/* Toggle buttons */

button:checked {
    background: @white;
    color: @black;
}

/* Entries and views (textview, treeview) */

.view,
textview text {  /* Setting a textview to background white makes it black,
                    unless you only style the text css node */
    border-width: 0px;
    border-style: none;
    border-radius: 0px;
    padding: 0px;
    background: @white;
    color: @black;
}

treeview header button,
treeview header button:hover:active {
    border-radius: 0px;
    background: @button_grey;
    border-width: 0px;
    color: @white;
}

/* FIXME: treeview row css nodes aren't a thing

    $[if treeview_pseudo_element] GtkTreeView row:even
    $[else] GtkTreeView row:nth-child(even)
    $[end if] {
        background: @row_even;
    }

    $[if treeview_pseudo_element] GtkTreeView row:odd
    $[else] GtkTreeView row:nth-child(odd)
    $[end if] {
        background: @row_odd;
    }
*/

entry {
    border-radius: $(2 * subcell_size)px;
    border-width: $(thickness)px;
    border-color: @text_field_grey;
    border-style: solid;
    background: @text_field_grey;
    color: @black;
    ${ entry_ythickness = my_ceil(0.2 * (subcell_size*3.0/2.0 - thickness) + thickness) }
    ${ entry_xthickness = my_ceil(0.32 * (subcell_size*3.0/2.0 - thickness) + thickness) }
    padding: $(2 * max(my_ceil((3*subcell_size - font_height - entry_ythickness*2)/2.0),0))px $(2 * max(subcell_size - entry_xthickness, 0))px $(2 * max(my_floor((3*subcell_size - font_height - entry_ythickness*2)/2.0), 0))px $(2 * max(subcell_size - entry_xthickness, 0))px;
}

entry image {
    /* Default padding is too large and causes toolbar height expansion */
    margin: $(default_padding-1)px;
}

toolitem entry {
    margin: $(subcell_size)px;
}

entry progress {
    border-radius: $(2 * subcell_size)px;
    border-width: $(thickness)px;
    background: @selection_grey;
}

entry:focus {
    background: @white;
}

toolbar entry:focus {
    border-color: @white;
}

entry:disabled {
    background: @button_grey;
    border-color: @button_grey;
}

treeview:selected {
    background: @panel_grey;
    color: @black;
}

entry:selected,
entry:selected:focus {
    background: @selection_grey;
    color: @black;
}

entry:selected,
entry:selected:focus {
    border-color: @selection_grey;
}

selection {
    background: @selected_bg_color;
    color: @selected_fg_color;
}

/* Frames */

frame {
    border-style: solid;
    border-color: @selection_grey;
    border-width: $(thickness)px;
    border-radius: 0;
    padding: $(default_padding)px;
}

frame.journal-preview-box {
    border-color: @button_grey;
}

/* Combo boxes */

combobox {
    color: @white;
}

combobox button.combo arrow {
    -gtk-icon-source: -gtk-icontheme('control-popup-arrow');
    -gtk-icon-transform: scale(0.3);
    min-width: $(subcell_size)px;
    min-height: $(subcell_size)px;
}

/* What does this do?
    combobox .menu.button {
        border-color: transparent;
        border-radius: 0px;
        border-style: none;
        background: transparent;
        color: @white;
    }
*/

combobox menu {
    -GtkWidget-scroll-arrow-hlength: $(my_floor(2.5 * subcell_size));
    -GtkWidget-scroll-arrow-vlength: $(my_floor(2.5 * subcell_size));
}

/* Notebooks */

notebook {
    background: @selection_grey;
    color: @black;
    padding: 0px;
    -GtkNotebook-tab-overlap: -2;
    -GtkNotebook-tab-curvature: $default_padding;
    -GtkWidget-scroll-arrow-hlength: $(my_floor(2.5 * subcell_size));
    -GtkWidget-scroll-arrow-vlength: $(my_floor(2.5 * subcell_size));
}

notebook arrow {
    color: @white;
}

notebook tab {
    background: @selection_grey;
}

/* Setting white color to the tab labels using only the previous rule */
/* doesn't work, so we have to set white color to the GtkLabel, and make */
/* sure the buttons get black color in the next two rules: */
notebook tab label {
    color: @white;
    padding: $(subcell_size)px 0;
}

notebook tab button label {
    color: @black;
}

notebook tab:checked {
    background: @toolbar_grey;
}

notebook tab button {
    border-radius: $(toolbutton_padding)px;
}

/* Browse Widgets */

BrowseSearchWindow treeview {
    background: @black;
    color: @white;
    border-color: @button_grey;
    border-width: 0 $(thickness)px $(thickness)px $(thickness)px;
    border-style: solid;
}

/* Control panel */

cpanelsectionview {
    background: @white;
    color: @black;
}

/* TODO:  this should only effect the control panel */
controlpanel image:disabled {
    opacity: $(disabled_opacity);
}

/* Alert */

alert {
    background: @black;
    color: @white;
}

alert *:disabled {
    background: @black;
}

button timeouticon label,
button timeouticon label:hover {
    background: @white;
    color: @button_grey;
    border-radius: $(2 * subcell_size)px;
}

button:active timeouticon label {
    background: @toolbar_grey;
    color: @white;
}

/* Tray */

htray, vtray {
    background: @toolbar_grey;
}

htray * , vtray * { background: @toolbar_grey;}

/* Menus and palettes */

palette,
popover {
    border: $(thickness)px solid @button_grey;
    background: @black;
    color: @white;
}

palette .view {
    color: @black;
}

SugarPaletteMenuWidget {
    background: @black;
}

SugarPaletteMenuWidget * {
    color: @white;
}

SugarPaletteWindow SugarGroupBox {
    background: @toolbar_grey;
    color: @white;
}

SugarPaletteWindow SugarGroupBox *:disabled {
    background: @toolbar_grey;
}

menu {
    background: @black;
    color: @white;

    -GtkMenu-scroll-arrow-vlength: $(my_floor(subcell_size/0.7 + 2*thickness));
    /* This means the outline of the submenu overlaps with a palette.
     * However in the case of two normal menus, they are next to each other.
     * It is not possible to be smarter about this, because the style comes from
     * the submenu. */
    -GtkMenu-horizontal-offset :  -$(line_width)px;
    -GtkMenu-vertical-offset   : 0;

    padding: 0px;

    border: $(thickness)px solid @button_grey;
}

menuitem {
    padding: $(subcell_size)px $((subcell_size * 3 - font_height) / 2)px;
}

menuitem:hover,
menuitem:hover {
    background: @button_grey;
}

menuitem separator {
    padding: 0;
}

tooltip {
    background: @black;
    border-style: solid;
    border-width: $(thickness)px;
    border-color: @button_grey;
}

tooltip * {
    color: @white;
}

/* Scrollbars */

scrollbar {
    -GtkRange-slider-width: $(subcell_size + 2*thickness);
    -GtkRange-trough-border: 0;
    -GtkRange-stepper-size: 0;
    -GtkScrollbar-min-slider-length: $(3*subcell_size);
    -GtkScrollbar-has-forward-stepper: 0;
    -GtkScrollbar-has-backward-stepper: 0;
    -GtkScrollbar-has-secondary-forward-stepper: 0;
    -GtkScrollbar-has-secondary-backward-stepper: 0;
}

scrollbar trough {
    background: @button_grey;
    border-width: 0px;
}

scrollbar slider {
    background: @white;
    border-radius: $(2*subcell_size)px;
    border-width: $(thickness)px;
    border-color: @button_grey;
    border-style: solid;
}

scrollbar slider:active {
    background: @text_field_grey;
}

/* Progress bars */

progressbar progress {
    background: @white;
    border-color: @button_grey;
    border-radius: $(subcell_size)px;
    border-style: solid;
    border-width: $(thickness)px;
}

progressbar trough {
    background: @selection_grey;
    border-style: solid;
    border-radius: $(subcell_size)px;
    border-color: @button_grey;
    border-width: $(thickness)px;
}

toolbar progressbar trough,
palette progressbar trough {
    background: @black;
}

/* Separators */

separator {
    /*border-top: $(thickness)px solid @button_grey;*/
    background-color: @button_grey;
}

/* Tool buttons */

toolbar,
toolbarbox {
    padding: 0;
    background: @toolbar_grey;
    color: @white;
}

/* FIXME: I needed to add the margin in the gtk3.20 port, but that wasn't
          there in the code before.  I ended up having to reduce the border
          to 2px */
toolbutton button,
filtertoolbutton button {
    border-style: none;
    background: transparent;
    border-radius: $(toolbutton_padding)px;
    padding: $(toolbutton_padding - default_padding)px;
    margin: $(default_padding)px;
}

toolbutton button #gtk-toolbar-arrow {
    padding: 0 $(subcell_size + default_padding)px;
}

toolbutton button:disabled {
    opacity: $(disabled_opacity);
}

toolbutton button:hover {
    padding: $(toolbutton_padding - 2 - default_padding)px;
    border-width: 2px;
    border-style: solid;
    background-clip: padding-box;
}

toolbutton button:hover:not(:checked),
toolbutton button:hover:not(:checked) * {
    background: @black;
}

toolbutton button.radio:checked,
toolbutton button.radio:checked * {
    background: @button_grey;
    border-radius: $(toolbutton_padding)px;
}

toolbutton button.radio:checked:hover,
toolbutton button.radio:checked:hover * {
    background: @button_grey;
    border-color: @button_grey;
}

toolbutton button:active,
toolbutton button:active * {
    background: @button_grey;
    border-radius: $(toolbutton_padding)px;
}

palette scrolledwindow {
    background: @black;
}

toolbar combobox button {
    border-radius: $(2*subcell_size)px;
}

/* Scales */

scale {
    -GtkScale-slider-length: $scale_slider_width;
    -GtkRange-slider-width: $scale_slider_width;
}

scale highlight {
    background: @white;
    border-style: solid;
    border-color: @button_grey;
    border-width: $(thickness)px;
}

scale trough {
    background: @button_grey;
    border-style: solid;
    border-color: @button_grey;
    border-width: $(thickness)px;
    margin: $(scale_trough_margin)px 0;
}

scale.vertical trough {
    margin: 0 $(scale_trough_margin)px;
}

scale trough:focus {
    border-color: @white;
}

scale trough,
scale highlight {
    border-radius: $(2*subcell_size)px $(2*subcell_size)px;
}

scale slider,
scale slider:active {
    background: transparent;
}

scale slider {
    -gtk-icon-source: url("assets/scale-slider.svg");
    margin: -$(my_floor(subcell_size/2))px;
    min-width: $(2*subcell_size)px;
    min-height: $(2*subcell_size)px;
}

scale slider:active {
    -gtk-icon-source: url("assets/scale-slider-active.svg");
    margin: -$(my_floor(subcell_size/2))px;
    min-width: $(2*subcell_size)px;
    min-height: $(2*subcell_size)px;
}

/* Radio and check buttons */

check:hover {
    background: alpha(@theme_base_color, 0.0);
}

toolbar check,
palette check {
    color: @theme_base_color;
    border: 1px solid @theme_base_color;
}

radio,
radio row:selected,
radio row:selected:focus {
    -gtk-icon-source: url("assets/radio.svg");
    background: alpha(@theme_base_color, 0.0);
}

radio:selected,
radio:selected row:selected,
radio:selected row:selected:focus {
    -gtk-icon-source: url("assets/radio-selected.svg");
}

radio:active,
radio row:selected:active,
radio row:selected:focus:active {
    -gtk-icon-source: url("assets/radio-active.svg");
}

radio:checked,
radio row:selected:checked,
radio row:selected:focus:checked {
   -gtk-icon-source: url("assets/radio-active.svg");
}

radio:active:selected,
radio:selected row:selected:active,
radio:selected row:selected:focus:active {
    -gtk-icon-source: url("assets/radio-active-selected.svg");
}

radio:checked:selected,
radio:selected row:selected:checked,
radio:selected row:selected:focus:checked {
    -gtk-icon-source: url("assets/radio-active-selected.svg");
}

check,
check row:selected,
check row:selected:focus {
    -gtk-icon-source: url("assets/checkbox-unchecked.svg");
    background: alpha(@theme_base_color, 0.0);
}

check:selected,
check:selected row:selected,
check:selected row:selected:focus {
    -gtk-icon-source: url("assets/checkbox-unchecked-selected.svg");
}

check:active,
check row:selected:active,
check row:selected:focus:active {
    -gtk-icon-source: url("assets/checkbox-checked.svg");
}

check:checked,
check row:selected:checked,
check row:selected:focus:checked {
    -gtk-icon-source: url("assets/checkbox-checked.svg");
}

check:active:selected,
check:selected row:selected:active,
check:selected row:selected:focus:active {
    -gtk-icon-source: url("assets/checkbox-checked-selected.svg");
}

check:checked:selected,
check:selected row:selected:checked,
check:selected row:selected:focus:checked {
    background-image: url("assets/checkbox-checked-selected.svg");
    -gtk-icon-source: url("assets/checkbox-checked-selected.svg");
}

/* Tool items */

toolitem {
    /* arrow in the toolbarbox */
    color: @white;
}

.palette-down,
.toolbar-down {
    /* outline of the toolbarbutton when palette is expanded */
    border-style: solid;
    border-width: 2px;
    border-color: @button_grey;
}


/* GtkImage */

image {
    background: transparent;
}

/* Sugar Frame Window */

framewindow {
    background: @toolbar_grey;
}

/* Sugar Canvas icon */

canvasicon {
    border-color: transparent;
    border-radius: $(4 * thickness)px;
    border-width: 2px;
    border-style: solid;
    background: transparent;
}

canvasicon:hover {
    border-color: @zoom_views_prelight;
    background: @zoom_views_prelight;
}

canvasicon.toggle-press {
    border-color: @zoom_views_active;
    background: @zoom_views_active;
}

/* Sugar CellRenderer Icons */

.cell.sugar-icon-cell {
    background: transparent;
    border-radius: $(4 * thickness)px;
    border-color: @white;
    border-width: $(thickness)px;
    border-style: solid;
}

.cell.sugar-icon-cell:hover {
    background: @zoom_views_prelight;
}

.cell.sugar-icon-cell:active {
    background: @zoom_views_active;
}

/* CellRendererProgress */

.cell.progressbar, .cell.trough {
    border-style: solid;
    border-width: $(thickness)px;
    border-color: @selection_grey;
    border-radius: $(2*subcell_size)px;
}

.cell.progressbar {
    background: @white;
}

.cell.trough {
    background: @text_field_grey;
}

/* Text cursor handles */

cursor-handle.top,
cursor-handle.bottom {
    background: transparent;
    border-radius: 10px;
    border-width: $(thickness)px;
    border-color: @panel_grey;
    border-style: solid;
}

cursor-handle.bottom {
    -gtk-icon-source: url("assets/cursor-handle-bottom.svg");
}

cursor-handle.top {
    -gtk-icon-source: url("assets/cursor-handle-top.svg");
}

cursor-handle.insertion-cursor {
    -gtk-icon-source: url("assets/cursor-handle-insertion.svg");
}

/* Application specific properties */

EvView {
    background: @panel_grey;
}

EvView:selected,
ApDocView:selected {
    background: @selection_grey;
}
