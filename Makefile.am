SUBDIRS = art gtk-engine po

intltool_extra = intltool-extract.in intltool-merge.in intltool-update.in

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(sysconfdir)/gtk-2.0
	$(INSTALL_DATA) $(srcdir)/gtkrc2-default $(DESTDIR)$(sysconfdir)/gtk-2.0/gtkrc

EXTRA_DIST = $(intltool_extra) gtkrc2-default
