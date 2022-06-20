#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([Hello], [1.0], [contact@rafaelrc.com])
AC_CONFIG_SRCDIR([src/provolone.c])
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_DEFINE([GNULIB_NO_VLA], [1], [Define to 1 to disable use of VLAs])
AX_CHECK_COMPILE_FLAG([-std=gnu11], [], [
  echo "gnu11 standard is unsupported by compiler"
  exit -1
])

AC_PROG_LEX([noyywrap])
AC_PROG_YACC

AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

