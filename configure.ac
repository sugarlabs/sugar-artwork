AC_PREREQ(2.53)

AC_INIT([sugar-artwork],[0.105.2],[],[sugar-artwork])

AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([1.9 foreign dist-xz no-dist-gzip])

AM_MAINTAINER_MODE
AM_DISABLE_STATIC

PKG_PROG_PKG_CONFIG([0.19])

AC_PROG_CC
AC_HEADER_STDC
AC_PROG_LIBTOOL

AC_PATH_PROG([ICON_SLICER], [icon-slicer])
if test -z "$ICON_SLICER"; then
    AC_MSG_ERROR([icon-slicer is required])
fi

PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= 2.16.0,,
	          AC_MSG_ERROR([GTK+-2.0 is required to compile sugar-artwork]))

PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= 3.0.0,,
	          AC_MSG_ERROR([GTK+-3.0 is required to compile sugar-artwork]))

PKG_CHECK_MODULES(ENGINE, gtk+-2.0 >= 2.16 gobject-2.0 >= 2.0 cairo >= 0.1.1)
PKG_CHECK_MODULES(ENGINE3, gtk+-3.0 >= 3.0 gobject-2.0 >= 2.0 cairo >= 0.1.1)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

ICON_NAMING_UTILS_REQUIRED=0.8.2

AC_MSG_CHECKING([icon-naming-utils >= $ICON_NAMING_UTILS_REQUIRED])
PKG_CHECK_EXISTS(icon-naming-utils >= $ICON_NAMING_UTILS_REQUIRED,
                 have_utils=yes, have_utils=no)
if test "x$have_utils" = "xyes"; then
   UTILS_PATH="`$PKG_CONFIG --variable=program_path icon-naming-utils`"
   ICONMAP="$UTILS_PATH/icon-name-mapping"
   AC_SUBST(ICONMAP)
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
   AC_MSG_ERROR([icon-naming-utils >= $ICON_NAMING_UTILS_REQUIRED is required to build
                 and install the icon theme])
fi

AC_OUTPUT([
Makefile
cursor/Makefile
cursor/sugar/Makefile
icons/Makefile
icons/scalable/Makefile
icons/scalable/actions/Makefile
icons/scalable/apps/Makefile
icons/scalable/categories/Makefile
icons/scalable/control/Makefile
icons/scalable/device/Makefile
icons/scalable/emblems/Makefile
icons/scalable/mimetypes/Makefile
icons/scalable/status/Makefile
gtk/Makefile
gtk/engine/Makefile
gtk/theme/Makefile
gtk3/Makefile
gtk3/theme/Makefile
gtk3/theme/assets/Makefile
gtk3/theme/assets/72/Makefile
gtk3/theme/assets/100/Makefile
])
