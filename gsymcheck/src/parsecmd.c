/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4 tab-width: 4 -*- */
/*
 * File: parsecmd.c
 *
 * gEDA - GPL Electronic Design Automation
 * gsymcheck - gEDA Symbol Check
 *
 * Copyright (C) 1998-2015 Ales Hvezda
 * Copyright (C) 1998-2015 gEDA Contributors (see ChangeLog for details)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301 USA
 */

#include <config.h>

#include <libgeda/libgeda.h>

#include "../include/struct.h"
#include "../include/globals.h"
#include "../include/prototype.h"
#include "../include/gettext.h"

#define OPTIONS "hquv"

#ifndef OPTARG_IN_UNISTD
  extern char *optarg;
  extern int   optind;
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifdef HAVE_GETOPT_LONG
struct option long_options[] =
  {
    {"help",     0, 0, 'h'},
    {"quiet",    0, 0, 'q'},
    {"suppress", 0, 0, 'u'},
    {"verbose",  0, 0, 'v'}
  };
#endif

/*! \brief Print brief usage information and exit.
 * \par Function Description
 * Print brief help message describing gsymcheck usage & command-line
 * options, then exit with exit_status 0.
 *
 * \param cmd First element of argv (name of program as run).
 */
void
usage(char *cmd)
{
  printf(_(
    "\n"
    "Usage: %s [OPTIONS] filename1 ... filenameN\n"
    "  -h, --help        Print usage\n"
    "  -q, --quiet       Quiet mode\n"
    "  -u, --suppress    Suppress \"No errors found\" when no errors\n"
    "  -v, --verbose     Verbose mode (cumulative: errors, warnings, info)\n"
    "                    Use this to get the actual symbol error messages\n"
    "\nfilename1 ... filenameN are the symbols to check\n"
    "\n"),
    cmd);
    exit(0);
}

/*! \brief Parse gsymcheck command-line options.
 * \par Function Description
 * Parse command line options, displaying usage message or version
 * information as required.
 *
 * \param argc Number of command-line arguments.
 * \param argv Array of command-line arguments.
 *
 * \return index into \a argv of first non-option argument.
 *
 * \todo Version option?
 * \todo strict option?
 * \todo Consider adding option for attributes to selectively check,
 *       example: gsymcheck --documentation *.sch
 */
int parse_commandline(int argc, char *argv[])
{
  int ch;

#ifdef HAVE_GETOPT_LONG
  while ((ch = getopt_long (argc, argv, OPTIONS, long_options, NULL)) != -1) {
#else
  while ((ch = getopt (argc, argv, OPTIONS)) != -1) {
#endif
    switch (ch) {

      case 'h':
        usage(argv[0]);
        break;

      case 'q':
        quiet_mode=TRUE;
        break;

      case 'u':
        suppress_mode=TRUE;
        break;

      case 'v':
        verbose_mode++;
        break;

#if 0
      case 'f':
        printf("f arg: %s\n", optarg);
        break;
#endif

      case '?':
      default:
        usage(argv[0]);
        break;
    }
  }

  return(optind);
}
