AC_PREREQ([2.71])

AC_INIT([sugar-artwork],[0.121],[],[sugar-artwork])

AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([1.9 foreign dist-xz no-dist-gzip])

AM_MAINTAINER_MODE
AC_DISABLE_STATIC([])

PKG_PROG_PKG_CONFIG([0.19])

AC_PROG_CC
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

LT_INIT

AC_PATH_PROG([XCURSORGEN], [xcursorgen])
if test -z "$XCURSORGEN"; then
    AC_MSG_ERROR([xcursorgen is required])
fi

PYTHON=python3
AM_PATH_PYTHON
AS_IF([$PYTHON -m em </dev/null >/dev/null 2>&1], AC_MSG_RESULT([empy found]), AC_MSG_ERROR([empy not found]))

PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= 3.0.0,,
	          AC_MSG_ERROR([GTK+-3.0 is required to compile sugar-artwork]))

PKG_CHECK_MODULES(ENGINE3, gtk+-3.0 >= 3.0 gobject-2.0 >= 2.0 cairo >= 0.1.1)

GTK3_VERSION=`$PKG_CONFIG --modversion gtk+-3.0`
AC_SUBST(GTK3_VERSION)

AC_ARG_WITH([gtk2],
  [AS_HELP_STRING([--without-gtk2], [Omit GTK2 dependency (default=no)])],
  [with_gtk2=$withval],
  [with_gtk2=yes])

if test "x${with_gtk2}" = "xyes"; then

    PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= 2.16.0,,
	              AC_MSG_ERROR([GTK+-2.0 is required to compile sugar-artwork]))

    PKG_CHECK_MODULES(ENGINE, gtk+-2.0 >= 2.16 gobject-2.0 >= 2.0 cairo >= 0.1.1)

    GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
    AC_SUBST(GTK_VERSION)

    AC_CONFIG_FILES([
        gtk/Makefile
        gtk/engine/Makefile
        gtk/theme/Makefile
    ])
fi

AM_CONDITIONAL([WITH_GTK2], [test "x${with_gtk2}" = "xyes"])

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

AC_CONFIG_FILES([
Makefile
cursor/Makefile
cursor/sugar/Makefile
cursor/sugar-lh/Makefile
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
gtk3/Makefile
gtk3/theme/Makefile
gtk3/theme/3.20/Makefile
gtk3/theme/assets/Makefile
gtk3/theme/assets/72/Makefile
gtk3/theme/assets/100/Makefile
])
AC_OUTPUT
