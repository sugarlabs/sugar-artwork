AC_INIT([sugar-artwork],[0.79.1],[],[sugar-artwork])

AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([1.9 foreign dist-bzip2 no-dist-gzip])

AM_DISABLE_STATIC
	
AC_PROG_CC
AC_HEADER_STDC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= 2.0.0,,
	          AC_MSG_ERROR([GTK+-2.0 is required to compile redhat-artwork]))

GDK_PIXBUF2_CFLAGS=`$PKG_CONFIG --cflags gdk-pixbuf-2.0`
GDK_PIXBUF2_LIBS=`$PKG_CONFIG --libs gdk-pixbuf-2.0`
AC_SUBST(GDK_PIXBUF2_CFLAGS)
AC_SUBST(GDK_PIXBUF2_LIBS)

PKG_CHECK_MODULES(ENGINE, gtk+-2.0 >= 2.0 gobject-2.0 >= 2.0 cairo >= 0.1.1)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

AC_OUTPUT([
Makefile
cursor/Makefile
cursor/cursorthemegen/Makefile
cursor/sugar/Makefile
icons/Makefile
icons/scalable/Makefile
icons/scalable/actions/Makefile
icons/scalable/control/Makefile
icons/scalable/device/Makefile
icons/scalable/emblems/Makefile
icons/scalable/mimetypes/Makefile
icons/scalable/status/Makefile
gtk/Makefile
gtk/engine/Makefile
gtk/theme/Makefile
matchbox/Makefile
])
