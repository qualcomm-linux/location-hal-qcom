# configure.ac -- Autoconf script for gps location-hal
#
# Process this file with autoconf to produce a configure script

# Requires autoconf tool later than 2.61
AC_PREREQ(2.61)
# Initialize the gps location-hal package version 1.0.0
AC_INIT([location-hal],1.0.0)
# Does not strictly follow GNU Coding standards
AM_INIT_AUTOMAKE([foreign subdir-objects])
# Disables auto rebuilding of configure, Makefile.ins
AM_MAINTAINER_MODE
# Verifies the --srcdir is correct by checking for the path
AC_CONFIG_SRCDIR([Makefile.am])
# defines some macros variable to be included by source
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
PKG_PROG_PKG_CONFIG

m4_define([LT_MAJOR], [1])
m4_define([LT_MINOR], [0])
m4_define([LT_PATCH], [0])

AC_SUBST([LT_VERSION], LT_MAJOR.LT_MINOR.LT_PATCH)
AC_SUBST([LT_VERSION_NUMBER], LT_MAJOR:LT_MINOR:LT_PATCH)

# Checks for libraries.
PKG_CHECK_MODULES([LIBXML], [libxml-2.0])
AC_SUBST([LIBXML_CFLAGS])
AC_SUBST([LIBXML_LIBS])

AC_ARG_WITH([auto_feature],
    AC_HELP_STRING([--with-auto_feature=@<:@dir@:>@],
        [Using Automotive feature]),
    [],
    with_auto_feature=no)

if test "x$with_auto_feature" != "xno"; then
    CPPFLAGS="${CPPFLAGS} -DFEATURE_AUTOMOTIVE"
fi

AM_CONDITIONAL(USE_FEATURE_AUTOMOTIVE, test "x${with_auto_feature}" = "xyes")

AC_ARG_WITH([external_ap],
    AC_HELP_STRING([--with-external_ap=@<:@dir@:>@],
        [Using External Application Processor]),
    [],
    with_external_ap=no)

if test "x$with_external_ap" != "xno"; then
    CPPFLAGS="${CPPFLAGS} -DFEATURE_EXTERNAL_AP"
fi

AM_CONDITIONAL(USE_EXTERNAL_AP, test "x${with_external_ap}" = "xyes")

AC_ARG_WITH([propertyvault],
      AC_HELP_STRING([--with-propertyvault=@<:@dir@:>@],
         [specify whether the target has support for property vault]),
      [propertyvault=$withval],
      propertyvault=no)

AM_CONDITIONAL(HAS_PROPERTY_VAULT, test "x$propertyvault" == "xyes")

AC_CHECK_FUNCS([strlcpy strlcat])

if test "x$ac_cv_func_strlcpy" = "xno"; then
    AC_DEFINE([USE_G_STRLCPY], [1], [use G LIB Strcpy])
    AM_CONDITIONAL([USE_G_STRLCPY], [true])
else
    AC_DEFINE([USE_G_STRLCPY], [0], [Do not use G LIB Strcpy])
    AM_CONDITIONAL([USE_G_STRLCPY], [false])
fi

if test "x$ac_cv_func_strlcat" = "xno"; then
    AC_DEFINE([USE_G_STRLCAT], [1], [use G LIB Strcat])
else
    AC_DEFINE([USE_G_STRLCAT], [0], [Do not use G LIB Strcat])
fi

AC_ARG_ENABLE([ptp],
        [  --enable-ptp    enable ptp],
        [case "${enableval}" in
            yes) ptp=true ;;
            no)  ptp=false ;;
            *) AC_MSG_ERROR([bad value ${enableval} for --enable-ptp]) ;;
        esac],[ptp=false])

AM_CONDITIONAL(PTP_SUPPORTED, [test x$ptp = xtrue])

AC_ARG_WITH([locpla_includes],
      AC_HELP_STRING([--with-locpla-includes=@<:@dir@:>@],
         [specify the path to locpla-includes in loc-pla_git.bb]),
      [locpla_incdir=$withval],
      with_locpla_includes=no)

if test "x$with_locpla_includes" != "xno"; then
   AC_SUBST(LOCPLA_CFLAGS, "-I${locpla_incdir}")
fi

AC_SUBST([CPPFLAGS])

AC_ARG_WITH([telsdk],
      AC_HELP_STRING([--with-telsdk],
         [enable telsdk, check prescence of telsdk]))

AM_CONDITIONAL(USE_TELSDK, test "x${with_telsdk}" = "xyes")

AC_ARG_WITH([glib],
      AC_HELP_STRING([--with-glib],
         [enable glib, building HLOS systems which use glib]))

if (test "x${with_glib}" = "xyes"); then
        AC_DEFINE(ENABLE_USEGLIB, 1, [Define if HLOS systems uses glib])
        PKG_CHECK_MODULES(GTHREAD, gthread-2.0 >= 2.16, dummy=yes,
                                AC_MSG_ERROR(GThread >= 2.16 is required))
        PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.16, dummy=yes,
                                AC_MSG_ERROR(GLib >= 2.16 is required))
        GLIB_CFLAGS="$GLIB_CFLAGS $GTHREAD_CFLAGS"
        GLIB_LIBS="$GLIB_LIBS $GTHREAD_LIBS"

        AC_SUBST(GLIB_CFLAGS)
        AC_SUBST(GLIB_LIBS)
fi

AM_CONDITIONAL(USE_GLIB, test "x${with_glib}" = "xyes")

AC_CONFIG_FILES([ \
        Makefile \
        pla/oe/Makefile \
        utils/Makefile \
        utils/base_util/Makefile \
        utils/base_util/src/Makefile \
        core/Makefile \
        batching/Makefile \
        geofence/Makefile \
        location/Makefile \
        gnss/Makefile \
        location-hal.pc \
        ])

AC_OUTPUT
