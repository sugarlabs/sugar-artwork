INCLUDES = \
	-DGTK_DISABLE_DEPRECATED \
	-DGDK_DISABLE_DEPRECATED \
	-DG_DISABLE_DEPRECATED \
	$(ENGINE_CFLAGS) $(WARN_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libolpc.la

libolpc_la_SOURCES =		\
	cairo-support.c		\
	cairo-support.h		\
	general-support.h	\
	ge-support.h		\
	olpc_main.c		\
	olpc_misc.c		\
	olpc_misc.h		\
	olpc_rc_style.c		\
	olpc_rc_style.h		\
	olpc_style.h		\
	olpc_theme_draw.c	\
	widget-information.c	\
	widget-information.h

libolpc_la_LDFLAGS =				\
	-avoid-version -no-undefined -module	\
	$(ENGINE_LIBS)
