/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */
#include <config.h>

#include <stdio.h>
#include <string.h>

#include "gschem.h"

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif


/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_copy_start(GschemToplevel *w_current, int w_x, int w_y)
{
  TOPLEVEL *toplevel = w_current->toplevel;
  GList *s_current;

  /* Copy the objects into the buffer at their current position,
   * with future motion relative to the mouse origin, (w_x, w_y). */

  w_current->first_wx = w_x;
  w_current->first_wy = w_y;

  if (o_select_is_selection (w_current)) {

    s_current = geda_list_get_glist( Top_Selection );

    s_place_set_place_list(toplevel, s_current );

    w_current->inside_action = 1;
    i_set_state(w_current, COPY);
    o_place_start (w_current, w_x, w_y);
  }
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_copy_cancel(GschemToplevel *w_current)
{
  s_place_free_place_list(w_current->toplevel);

  w_current->inside_action = 0;
  i_set_state (w_current, SELECT);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_copy_end(GschemToplevel *w_current)
{
  o_place_end (w_current, w_current->second_wx, w_current->second_wy, FALSE,
               NULL, "%paste-objects-hook");
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_copy_multiple_end(GschemToplevel *w_current)
{
  o_place_end (w_current, w_current->second_wx, w_current->second_wy, TRUE,
               NULL, "%paste-objects-hook");
}
