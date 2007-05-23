AC_INIT(icons)

AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE(olpc-artwork, 0.33)

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

AM_MAINTAINER_MODE

AM_DISABLE_STATIC
	
AC_PROG_CC
AC_HEADER_STDC
AC_PROG_LIBTOOL
AC_PROG_INTLTOOL

PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= 2.0.0,,
	          AC_MSG_ERROR([GTK+-2.0 is required to compile redhat-artwork]))

GDK_PIXBUF2_CFLAGS=`$PKG_CONFIG --cflags gdk-pixbuf-2.0`
GDK_PIXBUF2_LIBS=`$PKG_CONFIG --libs gdk-pixbuf-2.0`
AC_SUBST(GDK_PIXBUF2_CFLAGS)
AC_SUBST(GDK_PIXBUF2_LIBS)

PKG_CHECK_MODULES(ENGINE, gtk+-2.0 >= 2.0 gobject-2.0 >= 2.0 cairo >= 0.1.1)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

dnl ===========================================================================
dnl Use lots of warning flags with GCC

WARN_CFLAGS=""
if test "x$GCC" = "xyes"; then
	WARN_CFLAGS="-Wall -Wpointer-arith -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -fno-strict-aliasing"
fi
AC_SUBST(WARN_CFLAGS)
dnl ===========================================================================

AM_GLIB_GNU_GETTEXT
	
AC_OUTPUT([
Makefile
cursor/Makefile
cursor/cursorthemegen/Makefile
cursor/sugar/Makefile
icons/Makefile
icons/16x16/Makefile
icons/16x16/stock/Makefile
icons/16x16/stock/emoticons/Makefile
icons/scalable/Makefile
icons/scalable/actions/Makefile
icons/scalable/control/Makefile
icons/scalable/device/Makefile
icons/scalable/stock/Makefile
icons/scalable/object/Makefile
gtk/Makefile
gtk/engine/Makefile
gtk/theme/Makefile
matchbox/Makefile
test/Makefile
po/Makefile.in
])
