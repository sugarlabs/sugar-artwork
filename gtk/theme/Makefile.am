sugar.gtkrc: gtkrc.em
	empy -p $$ -D theme=\'sugar\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar.gtkrc

sugar-xo.gtkrc: gtkrc.em
	empy -p $$ -D theme=\'sugar-xo\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar-xo.gtkrc

clean:
	$(RM) sugar.gtkrc
	$(RM) sugar-xo.gtkrc

GTKRC_FILES =		\
	sugar.gtkrc	\
	sugar-xo.gtkrc

install-data-local: $(GTKRC_FILES)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar/gtk-2.0
	$(INSTALL_DATA) $(top_builddir)/gtk/theme/sugar.gtkrc \
		$(DESTDIR)$(datadir)/themes/sugar/gtk-2.0/gtkrc
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/sugar-xo/gtk-2.0
	$(INSTALL_DATA) $(top_builddir)/gtk/theme/sugar-xo.gtkrc \
		$(DESTDIR)$(datadir)/themes/sugar-xo/gtk-2.0/gtkrc

uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/themes/sugar/gtk-2.0
	rm -rf $(DESTDIR)$(datadir)/themes/sugar-xo/gtk-2.0

EXTRA_DIST = em.py gtkrc.em
CLEANFILES = $(GTKRC_FILES)
