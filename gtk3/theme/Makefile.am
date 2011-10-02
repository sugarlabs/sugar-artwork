gtk-widgets.css: gtk-widgets.css.em
	$(srcdir)/em.py -p $$ -D scaling=\'72\' $(srcdir)/gtk-widgets.css.em > \
		$(top_builddir)/gtk3/theme/gtk-widgets.css


themedir = $(datadir)/themes/sugar/gtk-3.0
theme_DATA = 		\
	gtk.css 	\
	gtk-dark.css	\
	gtk-widgets.css \
	settings.ini

clean:
	$(RM) gtk-widgets.css


EXTRA_DIST = $(theme_DATA)

-include $(top_srcdir)/git.mk
