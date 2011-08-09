enginedir = $(libdir)/gtk-3.0/$(GTK_VERSION)/theming-engines
engine_LTLIBRARIES = libsugar.la

INCLUDES = \
	$(ENGINE3_CFLAGS)

libsugar_la_SOURCES =			\
	sugar_utils.h			\
	sugar_utils.c			\
	sugar_engine.c

libsugar_la_LDFLAGS = -module -avoid-version -no-undefined -export-symbols $(top_srcdir)/gtk3/engine/engine.symbols
libsugar_la_LIBADD =  $(ENGINE3_LIBS)

EXTRA_DIST = engine.symbols

-include $(top_srcdir)/git.mk
