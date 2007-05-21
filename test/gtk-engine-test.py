#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

class Base:
	def __init__(self):
		window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		window.set_default_size(300, 200)

		vbox = gtk.VBox(False, 12)
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


		sw = gtk.ScrolledWindow()
		sw.set_policy(gtk.POLICY_NEVER, gtk.POLICY_ALWAYS)
		sw.set_size_request(150, 120)

		text_view = gtk.TextView()
		sw.add(text_view)
		text_view.show()

		vbox.pack_start(sw, False)
		sw.show()

		window.add(vbox)
		vbox.show()

		window.show()

	def main(self):
		gtk.main()

settings = gtk.settings_get_default()
	
if settings.get_property('gtk-theme-name') != 'sugar':
	settings.set_string_property('gtk-theme-name', 'sugar', '')

base = Base()
base.main()
