#!/bin/bash

# Author: Wiley Edward Hill
# Date: 10/20/12
#
# gEDA - GPL Electronic Design Automation
# gcheck-library - gEDA Symbol Library Batch Processor
# Copyright (C) 2012-2014 Wiley Edward Hill <wileyhill@gmail.com>
#
#------------------------------------------------------------------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#------------------------------------------------------------------

VER=1.0.0

KEEP=false
VERBOSE=false
QUIET=false

FILTER="*.sym"
LIBRARY="${@: -1}"
PROGRAM="gsymfix"
OPT=""
ERR_FILE_NOT_FOUND=2
ERR_BAD_ARGS=65

do_show_help()
{
   case $1 in
         "1") # Quick help
              echo Usage:   `basename $0` '[-h] | [-u] | [--version] | [-v] [-k] [-f <pattern>]' ;;
         "2") # Basic help
              echo
              echo "A script to assist in maintaining symbol libraries"
              echo Usage:   `basename $0` '[-options] [-d] [-f <pattern>] [library]'
              echo
              echo "Use the follow options to control behavior:"
              echo
              echo   '  -f | --filter <pattern>   filter pattern is changed to pattern'
              echo   ''
              echo   '  -h | --help      show this information'
              echo   '  -k | --keep      keep orginal file, do not delete'
              echo   '  -u | --usage     show basic usage format'
              echo   '  -v | --verbose   display extra information'
              echo   '       --version   Display version information'
              echo   ''
              echo ' Note: arguments are case sensitive. See documentation for more'
              echo ' detailed information.'
              echo ;;
          *)  echo "Help not avaliable for:$1" ;;
   esac
   exit 0
}

vecho()
{
   if [[ ! $1 = "" ]] ; then
     if $VERBOSE ; then
      echo $1
     fi
   fi
}

do_Assimilate_Arguments(){
   for Arg in $*; do
      case $Arg in
         --keep | -k) KEEP=true                                       ;;
         --help | -h) do_show_help "2"                                ;;
        --usage | -u) do_show_help "1"                                ;;
      --verbose | -v) VERBOSE=true  ; OPT="-vv"                       ;;
           --version) echo `basename $0` "Version $VER" ; exit        ;;
        --quiet | -q) QUIET=true                                      ;;
    	           *) case $LAST_ARG in
       --filter | -f) FILTER="$Arg"                                   ;;
                   *)                                                 ;;
                      esac
      esac
      LAST_ARG=$Arg
   done
}

do_fix_symbols (){
  exist=$(which $PROGRAM)
  if [[ ! $exist = "" ]] ; then
    for eachfile in $LIBRARY/$FILTER; do
      vecho "Checking $eachfile"
      $PROGRAM $OPT $eachfile
      if [ -f $eachfile.fix ] && [ !$KEEP ]; then
         rm -f $eachfile
         mv $eachfile.fix $eachfile
      fi
    done
  else
    echo "Error, recheck installation. Could not locate [$PROGRAM]"
  fi
}
# ----------------- Check for Empty Command Line ------------------
if [ "$#" -eq 0 ] ; then do_show_help "1"; exit $ERR_BAD_ARGS ; fi

# ----------------------- Parse Command Line ----------------------
do_Assimilate_Arguments $*

if [ "$LIBRARY" != "" ] && [ -d $LIBRARY ] ; then
    do_fix_symbols
else
    echo "Error, recheck argument: Could not access [$LIBRARY]"
fi
exit 0

