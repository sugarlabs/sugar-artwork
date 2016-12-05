SUBDIRS = assets 3.20

gtk-widgets-72.css: gtk-widgets.css.em
	$(srcdir)/em.py -p $$ -D scaling=\'72\' -D gtk=\'$(GTK3_VERSION)\' \
		$(srcdir)/gtk-widgets.css.em > \
		$(top_builddir)/gtk3/theme/gtk-widgets-72.css

gtk-widgets-100.css: gtk-widgets.css.em
	$(srcdir)/em.py -p $$ -D scaling=\'100\' -D gtk=\'$(GTK3_VERSION)\' \
		$(srcdir)/gtk-widgets.css.em > \
		$(top_builddir)/gtk3/theme/gtk-widgets-100.css

settings-72.ini: settings.ini.em
	$(srcdir)/em.py -p $$ -D scaling=\'72\' $(srcdir)/settings.ini.em > \
		$(top_builddir)/gtk3/theme/settings-72.ini

settings-100.ini: settings.ini.em
	$(srcdir)/em.py -p $$ -D scaling=\'100\' $(srcdir)/settings.ini.em > \
		$(top_builddir)/gtk3/theme/settings-100.ini

clean:
	$(RM) gtk-widgets-100.css
	$(RM) gtk-widgets-72.css
	$(RM) settings-100.ini
	$(RM) settings-72.ini

GEN_FILES = \
       gtk-widgets-72.css \
       gtk-widgets-100.css \
       settings-72.ini \
       settings-100.ini

install-data-local: $(GEN_FILES)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.0
	$(INSTALL_DATA) $(top_builddir)/gtk3/theme/gtk-widgets-72.css \
		$(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.0/gtk-widgets.css
	$(INSTALL_DATA) $(top_builddir)/gtk3/theme/settings-72.ini \
		$(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.0/settings.ini
	$(INSTALL_DATA) $(srcdir)/gtk.css \
		$(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.0/gtk.css
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.0
	$(INSTALL_DATA) $(top_builddir)/gtk3/theme/gtk-widgets-100.css \
		$(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.0/gtk-widgets.css
	$(INSTALL_DATA) $(top_builddir)/gtk3/theme/settings-100.ini \
		$(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.0/settings.ini
	$(INSTALL_DATA) $(srcdir)/gtk.css \
		$(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.0/gtk.css
	$(INSTALL_DATA) $(srcdir)/gtk.css \
		$(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.20/gtk.css
	$(INSTALL_DATA) $(srcdir)/gtk.css \
		$(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.20/gtk.css
	cp -r $(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.0/assets \
	    $(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.20/assets
	cp -r $(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.0/assets \
	    $(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.20/assets


uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.0
	rm -rf $(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.0

EXTRA_DIST = gtk-widgets.css.em settings.ini.em em.py gtk.css
CLEANFILES = $(GEN_FILES)
