SUBDIRS = 16x16 scalable

THEME_IN_FILES = index.theme.in

%.theme:   %.theme.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

themedir = ${datadir}/icons/sugar
theme_DATA = index.theme

EXTRA_DIST=				\
	index.theme.in

CLEANFILES = 				\
	index.theme
