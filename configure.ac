dnl setup autotools
AC_INIT([dateshift], [1.2])
AC_CONFIG_SRCDIR([dateshift.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE

dnl Setup libtool
AC_DISABLE_STATIC([])
LT_INIT
dnl DOLT

# Build time sanity check...
AM_SANITY_CHECK

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

dnl Check for functons/declarations
AC_DEFINE(_GNU_SOURCE)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <time.h>]], [[
	int gettimeofday(struct timeval *tv,struct timezone *tz);
]])],[AC_DEFINE(GETTIMEOFDAY_HAS_TIMEZONE)],[])

dnl Check for functons/declarations
AC_SEARCH_LIBS([clock_gettime],[c rt posix4]) 
AC_CHECK_FUNCS(clock_gettime)

if test $CC = "gcc"; then
	CFLAGS="-Wall $CFLAGS"
fi

dnl Output files
AC_CONFIG_FILES([ Makefile config/Makefile ])
AC_OUTPUT
