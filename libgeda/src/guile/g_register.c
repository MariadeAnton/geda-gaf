/* gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 * Copyright (C) 1998-2013 Ales Hvezda
 * Copyright (C) 1998-2013 gEDA Contributors (see ChangeLog for details)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <config.h>
#include <missing.h>

#include <stdio.h>
#include <sys/stat.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "libgeda_priv.h"

/*! \brief */
struct gsubr_t {
  char* name;
  int req;
  int opt;
  int rst;
  SCM (*fnc)();
};

/*! \brief */
static struct gsubr_t libgeda_funcs[] = {
  { "eval-protected",             1, 1, 0, g_scm_eval_protected },
  { "eval-string-protected",      1, 0, 0, g_scm_eval_string_protected },

  { "component-groups",           1, 0, 0, g_rc_component_groups },
  { "component-library",          1, 1, 0, g_rc_component_library },
  { "component-library-command",  3, 0, 0, g_rc_component_library_command },
  { "component-library-funcs",    3, 0, 0, g_rc_component_library_funcs },
  { "source-library",             1, 0, 0,  g_rc_source_library },
  { "source-library-search",      1, 0, 0, g_rc_source_library_search },

  { "reset-component-library",    0, 0, 0, g_rc_reset_component_library },
  { "reset-source-library",       0, 0, 0, g_rc_reset_source_library },

 // { "enable-style-zero",        1, 0, 0, g_rc_enable_style_zero },
 // { "component-style",          1, 0, 0, g_rc_component_style },

  { "net-style",                  1, 0, 0, g_rc_net_style },
  { "bus-style",                  1, 0, 0, g_rc_bus_style },
  { "pin-style",                  1, 0, 0, g_rc_pin_style },
  { "line-style",                 1, 0, 0, g_rc_line_style },

  { "thick-bus-width",            1, 0, 0, g_rc_thick_bus_width },
  { "thick-line-width",           1, 0, 0, g_rc_thick_line_width },
  { "thick-net-width",            1, 0, 0, g_rc_thick_net_width },
  { "thick-pin-width",            1, 0, 0, g_rc_thick_pin_width },
  { "thin-bus-width",             1, 0, 0, g_rc_thin_bus_width },
  { "thin-line-width",            1, 0, 0, g_rc_thin_line_width },
  { "thin-net-width",             1, 0, 0, g_rc_thin_net_width },
  { "thin-pin-width",             1, 0, 0, g_rc_thin_pin_width },

  { "untitled-name",              1, 0, 0, g_rc_untitled_name },
  { "scheme-directory",           1, 0, 0, g_rc_scheme_directory },
  { "bitmap-directory",           1, 0, 0, g_rc_bitmap_directory },
  { "postscript-prolog",          1, 0, 0, g_rc_postscript_prolog },
  { "attribute-promotion",        1, 0, 0, g_rc_attribute_promotion },
  { "promote-invisible",          1, 0, 0, g_rc_promote_invisible },
  { "keep-invisible",             1, 0, 0, g_rc_keep_invisible },
  { "always-promote-attributes",  1, 0, 0, g_rc_always_promote_attributes },
  { "make-backup-files",          1, 0, 0, g_rc_make_backup_files },
  { "print-color-map",            0, 1, 0, g_rc_print_color_map },
  { "rc-filename",                0, 0, 0, g_rc_rc_filename },
  { "rc-config",                  0, 0, 0, g_rc_rc_config },
  { NULL,                         0, 0, 0, NULL } };

/*! \brief Register all libgeda functions with scheme.
 *  \par Function Description
 *  Creates g_subr_t objects to make g_rc_* functions that are defined
 *  in g_rc.c visible to Scheme.
 */
void g_register_libgeda_funcs (void)
{
  struct gsubr_t *tmp = libgeda_funcs;
  while (tmp->name != NULL) {
    scm_c_define_gsubr (tmp->name, tmp->req, tmp->opt, tmp->rst, tmp->fnc);
    tmp++;
  }
}

/*! \brief Register some libgeda directories with Scheme.
 * \par Function Description
 * Ensures that the default gEDA Scheme directory is added to the
 * Guile load path.
 */
void
g_register_libgeda_dirs (void)
{
  char *scheme_dir;

  scheme_dir = g_build_filename (f_path_sys_data (), "scheme", NULL);
  g_rc_scheme_directory (scm_from_utf8_string (scheme_dir));
  GEDA_FREE (scheme_dir);
}