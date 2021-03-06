# geda-windows.m4                                           -*-Autoconf-*-
# serial 1.1

dnl Check Windows-specific flags
dnl Copyright (C) 2009-2013  Cesar Strauss <cestrauss@gmail.com>
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
dnl Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

dnl The Windows platform has a native gcc port (MinGW) and a
dnl POSIX compliant one (Cygwin). Both need specific flags to
dnl build correctly.
dnl
dnlThe rules are:
dnl
dnlOn all Windows platforms, pass -no-undefined to libtool.
dnlThis allows shared libraries (DLLs) to be built.
dnl  
dnl On MinGW, use the -mms-bitfields compiler flag.
dnl This increases compatibility with the MSVC compiler.
dnl
dnl On MinGW, pass -mwindows when linking GUI-only applications.
dnl This avoids opening a text console when running from a shortcut.
dnl WEH:
dnl  m4_ifdef([AC_DEPLIBS_CHECK_METHOD], [m4_undefine([AC_DEPLIBS_CHECK_METHOD])]) 
dnl  m4_defun([AC_DEPLIBS_CHECK_METHOD],[])
dnl  lt_cv_deplibs_check_method=pass_all 
  
AC_DEFUN([AX_WINDOWS_FLAGS],
[
  AC_PREREQ([2.60])dnl
  AC_REQUIRE([AX_HOST])dnl

  if test "$PLATFORM_WIN32" = "yes"; then
    WINDOWS_LIBTOOL_FLAGS=-no-undefined
  fi

  if test "$PLATFORM_WIN32_NATIVE" = "yes"; then
    MINGW_GUI_LDFLAGS=-mwindows
    MINGW_CFLAGS="-mms-bitfields"
  fi

  AC_SUBST(WINDOWS_LIBTOOL_FLAGS)
  AC_SUBST(MINGW_GUI_LDFLAGS)
  AC_SUBST(MINGW_CFLAGS)
  []dnl
])dnl AX_WINDOWS_FLAGS
