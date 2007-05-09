INCLUDES = \
	-DGTK_DISABLE_DEPRECATED \
	-DGDK_DISABLE_DEPRECATED \
	-DG_DISABLE_DEPRECATED \
	$(ENGINE_CFLAGS) $(WARN_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libsugar.la

libsugar_la_SOURCES =		\
	sugar.c			\
	sugar-rc-style.c	\
	sugar-rc-style.h	\
	sugar-style.h		\
	sugar-style.c		\
	sugar-info.h		\
	sugar-info.c		\
	sugar-drawing.h		\
	sugar-drawing.c

libsugar_la_LDFLAGS =				\
	-avoid-version -no-undefined -module	\
	$(ENGINE_LIBS)
