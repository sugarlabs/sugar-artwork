sugar.gtkrc: gtkrc.em
	$(srcdir)/em.py -p $$ -D theme=\'sugar\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar.gtkrc

sugar-xo.gtkrc: gtkrc.em
	$(srcdir)/em.py -p $$ -D theme=\'sugar-xo\' $(srcdir)/gtkrc.em > \
		$(top_builddir)/gtk/theme/sugar-xo.gtkrc

GTKRC_FILES =		\
	sugar.gtkrc	\
	sugar-xo.gtkrc

install-data-local: $(GTKRC_FILES)
	$(INSTALL) $(top_builddir)/gtk/theme/sugar.gtkrc $(datadir)/themes/sugar/gtk-2.0

EXTRA_DIST = $(sugar_DATA)
CLEANFILES = $(GTKRC_FILES)
