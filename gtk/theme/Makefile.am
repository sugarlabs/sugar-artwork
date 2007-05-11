sugar.gtkrc: gtkrc.em
	$(srcdir)/em.py -p $$ -D theme=\'sugar\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar.gtkrc

sugar-xo.gtkrc: gtkrc.em
	$(srcdir)/em.py -p $$ -D theme=\'sugar-xo\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar-xo.gtkrc

clean:
	$(RM) sugar.gtkrc
	$(RM) sugar-xo.gtkrc

GTKRC_FILES =		\
	sugar.gtkrc	\
	sugar-xo.gtkrc

install-data-local: $(GTKRC_FILES)
	$(INSTALL_DATA) $(top_builddir)/gtk/theme/sugar.gtkrc $(datadir)/themes/sugar/gtk-2.0/gtkrc
	$(INSTALL_DATA) $(top_builddir)/gtk/theme/sugar-xo.gtkrc $(datadir)/themes/sugar-xo/gtk-2.0/gtkrc

EXTRA_DIST = $(sugar_DATA)
CLEANFILES = $(GTKRC_FILES)
