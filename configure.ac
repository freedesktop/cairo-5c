dnl Process this file with autoconf to produce a configure script.

dnl Copyright © 2007-2007 Keith Packard
dnl All Rights Reserved.  See the file COPYING in this directory
dnl for licensing information.

AC_PREREQ(2.59)

AC_INIT([cairo-5c],
	1.0,
	[http://cairographics.org],
	cairo-5c)

AC_CONFIG_SRCDIR([cairo-5c.h])
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

AC_CONFIG_HEADERS(config.h)

dnl libtool versioning

LT_CURRENT=0
LT_REVISION=0
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
LT_AGE=0

LT_VERSION_INFO="$LT_CURRENT:$LT_REVISION:$LT_AGE"
AC_SUBST(LT_VERSION_INFO)

LT_CURRENT_MINUS_AGE=`expr $LT_CURRENT - $LT_AGE`
AC_SUBST(LT_CURRENT_MINUS_AGE)

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET

NICKLE_LIBDIR=`nickle -e 'printf ("%s\n", Command::nickle_libdir);'`
NICKLE_PATH=`nickle -e 'printf ("%s\n", Command::nickle_path);'`


AC_SUBST(NICKLE_LIBDIR)
AC_SUBST(NICKLE_PATH)

AC_CHECK_LIB(pthread, pthread_create)

WARN_CFLAGS=""

if test "x$GCC" = "xyes"; then
	WARN_CFLAGS="-Wall -Wpointer-arith -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -fno-strict-aliasing"
fi
AC_SUBST(WARN_CFLAGS)

CAIRO_5C_PC_MODULES="cairo fontconfig freetype2 gtk+-2.0 gthread-2.0"

PKG_CHECK_MODULES(CAIRO_5C, $CAIRO_5C_PC_MODULES)

save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="${CAIRO_5C_CFLAGS} ${CPPFLAGS}"
AC_CHECK_HEADERS([cairo-xlib.h cairo-pdf.h cairo-svg.h cairo-ps.h])
CPPFLAGS="${save_CPPFLAGS}"

AC_CONFIG_FILES([Makefile
		 examples/Makefile
		 test/Makefile
		])
		
AC_OUTPUT
