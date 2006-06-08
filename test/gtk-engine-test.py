#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

class Base:
	def __init__(self):
		window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		window.set_default_size(400, 400)

		vbox = gtk.VBox(False, 6)
		vbox.set_border_width(12)

		hbox = gtk.HBox(False, 12)

		button = gtk.Button("Button")
		hbox.pack_start(button, False)
		button.show()

		entry = gtk.Entry()
		hbox.pack_start(entry, False)
		entry.show()

		vbox.pack_start(hbox, False)
		hbox.show()

		window.add(vbox)
		vbox.show()

		window.show()

	def main(self):
		gtk.main()

settings = gtk.settings_get_default()
	
if settings.get_property('gtk-theme-name') != 'olpc':
	settings.set_string_property('gtk-theme-name', 'olpc', '')

base = Base()
base.main()
