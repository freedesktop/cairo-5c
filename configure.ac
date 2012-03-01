dnl Process this file with autoconf to produce a configure script.

dnl Copyright © 2007-2007 Keith Packard
dnl All Rights Reserved.  See the file COPYING in this directory
dnl for licensing information.

AC_PREREQ(2.64)

AC_INIT([cairo-5c],
	[1.5],
	[http://cairographics.org],
	[cairo-5c])

AC_CONFIG_SRCDIR([cairo.5c])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

dnl libtool versioning

LT_CURRENT=2
LT_REVISION=0
LT_AGE=2
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

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

NICKLE_PATH=`nickle -e 'printf ("%s\n", Command::nickle_path);'`
NICKLE_SYS_LIBDIR=`nickle -e 'printf ("%s\n", Command::nickle_libdir);'`


AC_ARG_WITH(nickle-libdir, AS_HELP_STRING([--with-nickle-libdir=LIBDIR],
				[Directory where nickle source files should be installed (default $NICKLE_SYS_LIBDIR)]),
			   [ NICKLE_LIBDIR="$withval" ],
			   [ NICKLE_LIBDIR="$NICKLE_SYS_LIBDIR" ])

AC_SUBST(NICKLE_LIBDIR)
AC_SUBST(NICKLE_PATH)

AC_SEARCH_LIBS(pthread_create, pthread)

AC_SEARCH_LIBS(clock_gettime, rt)

AC_CHECK_FUNCS(clock_gettime)

WARN_CFLAGS=""

if test "x$GCC" = "xyes"; then
	WARN_CFLAGS="-Wall -Wpointer-arith -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -fno-strict-aliasing"
fi
AC_SUBST(WARN_CFLAGS)

CAIRO_5C_PC_MODULES="librsvg-2.0 cairo cairo-ft cairo-xlib x11 fontconfig"

PKG_CHECK_MODULES(CAIRO_5C, $CAIRO_5C_PC_MODULES)

save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="${CAIRO_5C_CFLAGS} ${CPPFLAGS}"
AC_CHECK_HEADERS([cairo-xlib.h cairo-pdf.h cairo-svg.h cairo-ps.h])
CPPFLAGS="${save_CPPFLAGS}"

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 nichrome/Makefile
		 test/Makefile
		 examples/Makefile
		])
		
AC_OUTPUT
