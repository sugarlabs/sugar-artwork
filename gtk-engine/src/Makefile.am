INCLUDES = \
	-DGTK_DISABLE_DEPRECATED \
	-DGDK_DISABLE_DEPRECATED \
	-DG_DISABLE_DEPRECATED \
	$(ENGINE_CFLAGS) $(WARN_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libolpc.la

libolpc_la_SOURCES =		\
	olpc.h			\
	olpc.c			\
	olpc-rc-style.c		\
	olpc-rc-style.h		\
	olpc-style.c		\
	olpc-style.h

libolpc_la_LDFLAGS =				\
	-avoid-version -no-undefined -module	\
	$(ENGINE_LIBS)
