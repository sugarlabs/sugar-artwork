${

# These sizes are copied from gtk2 rc files ...
if scaling == "100":
    icon_base = 11
else: # About 72% of the XO size, adjusted so that eg. toolbuttons work
    icon_base = 8               # 7.92

icon_small = icon_base * 3
icon_large = icon_base * 5

}

[Settings]
gtk-auto-mnemonics = 1
gtk-menu-images = 1
gtk-button-images = 1
gtk-toolbar-style = GTK_TOOLBAR_ICONS
gtk-cursor-theme-name = sugar
gtk-primary-button-warps-slider = 0

${
icon_small = icon_base * 3
icon_large = icon_base * 5

small_icons = [ "gtk-menu", "gtk-dnd", "gtk-small-toolbar", "gtk-button" ]
large_icons = [ "gtk-large-toolbar" ]

icon_sizes = []
for icon in small_icons:
    icon_sizes += [icon + "=" + str(icon_small) + ',' + str(icon_small)]
for icon in large_icons:
    icon_sizes += [icon + "=" + str(icon_large) + ',' + str(icon_large)]

icon_sizes = ":".join(icon_sizes)

}

gtk-icon-sizes = $icon_sizes
