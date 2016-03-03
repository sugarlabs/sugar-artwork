gtk-widgets-72.css: gtk-widgets.css.em
	$(srcdir)/../em.py -p $$ -D scaling=\'72\' -D gtk=\'$(GTK3_VERSION)\' \
		$(srcdir)/gtk-widgets.css.em > \
		$(top_builddir)/gtk3/theme/3.20/gtk-widgets-72.css

gtk-widgets-100.css: gtk-widgets.css.em
	$(srcdir)/../em.py -p $$ -D scaling=\'100\' -D gtk=\'$(GTK3_VERSION)\' \
		$(srcdir)/gtk-widgets.css.em > \
		$(top_builddir)/gtk3/theme/3.20/gtk-widgets-100.css

clean:
	$(RM) gtk-widgets-100.css
	$(RM) gtk-widgets-72.css

GEN_FILES = \
       gtk-widgets-72.css \
       gtk-widgets-100.css

install-data-local: $(GEN_FILES)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.20
	$(INSTALL_DATA) $(top_builddir)/gtk3/theme/3.20/gtk-widgets-72.css \
		$(DESTDIR)$(datadir)/themes/sugar-72/gtk-3.20/gtk-widgets.css
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.20
	$(INSTALL_DATA) $(top_builddir)/gtk3/theme/3.20/gtk-widgets-100.css \
		$(DESTDIR)$(datadir)/themes/sugar-100/gtk-3.20/gtk-widgets.css

EXTRA_DIST = gtk-widgets.css.em
CLEANFILES = $(GEN_FILES)
