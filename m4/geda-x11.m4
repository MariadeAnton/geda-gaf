# geda-x11.m4              -*-Autoconf-*-
# serial 1.0

dnl gEDA Prebuild checks for GTK Library Headers and Functions
dnl
dnl Copyright (C) 2014  Wiley Edward Hill <wileyhill@gmail.com>
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Boston, MA 02110-1301 USA

  
AC_DEFUN([AX_CHECK_X11],
[
  AC_PREREQ([2.52])dnl
  AC_REQUIRE([AX_HOST])dnl

  X11_LIBS=

  if test "$OS_LINUX" = "yes"; then
    AC_CHECK_LIB([X11], [XOpenDisplay], [X11=yes], [X11=no])
  fi
  AC_MSG_CHECKING([for X11 server])
  AC_MSG_RESULT($X11)

  if test "$X11" = "yes" ; then
    X11_LIBS="-lX11"
    AC_DEFINE([HAVE_X11], [1], [Define to 1 if on X11])
    AC_PATH_X             # used for inclusion of X11.h, XInitThreads();
  fi

  if test "$OS_LINUX" = "yes"; then
    AC_CHECK_LIB([Xft], [XftDrawCreate], [XFT=yes], [XFT=no])
  fi
  AC_MSG_CHECKING([for Xft])
  AC_MSG_RESULT($XFT)

  if test "$XFT"  = "yes" ; then
    X11_LIBS="$X11_LIBS -lXft"
    AC_DEFINE([HAVE_XFT], [1], [Define to 1 if have Xft])
  fi

  AC_SUBST(X11_LIBS)
  []dnl
])dnl AX_CHECK_X11