sugar-72.gtkrc: gtkrc.em
	$(PYTHON) - -p $$ -D scaling=\'72\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar-72.gtkrc < $(EMPY)

sugar-100.gtkrc: gtkrc.em
	$(PYTHON) - -p $$ -D scaling=\'100\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar-100.gtkrc < $(EMPY)

clean:
	$(RM) sugar-72.gtkrc
	$(RM) sugar-100.gtkrc

GTKRC_FILES =		\
	sugar-72.gtkrc	\
	sugar-100.gtkrc

install-data-local: $(GTKRC_FILES)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-72/gtk-2.0
	$(INSTALL_DATA) $(top_builddir)/gtk/theme/sugar-72.gtkrc \
		$(DESTDIR)$(datadir)/themes/sugar-72/gtk-2.0/gtkrc
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-100/gtk-2.0
	$(INSTALL_DATA) $(top_builddir)/gtk/theme/sugar-100.gtkrc \
		$(DESTDIR)$(datadir)/themes/sugar-100/gtk-2.0/gtkrc

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/themes/sugar-72/gtk-2.0
	rm -rf $(DESTDIR)$(datadir)/themes/sugar-100/gtk-2.0

EXTRA_DIST = gtkrc.em
CLEANFILES = $(GTKRC_FILES)
