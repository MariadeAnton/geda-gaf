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
 * Foundation, Inc., 51 Franklin Street, Boston, MA 02110-1301 USA
 */
#include <config.h>
#include <stdio.h>

#include "gschem.h"

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

/*! \brief Copy Current Selection to Buffer
 *  \par Function Description
 *   The function is used both the "clipboard" copy and the
 *   "clipboard" cut routines. This function first deleted
 *   the contents of the assigned buffer before copying the
 *   current selection to the buffer.
 *
 *   \param w_current A pointer to a GSCHEM top level object
 *   \param buf_num   integer value of the buffer to use.
 */
static void
selection_to_buffer(GschemToplevel *w_current, int buf_num)
{
  TOPLEVEL *toplevel = w_current->toplevel;
  GList *s_current = NULL;

  s_current = geda_list_get_glist( toplevel->page_current->selection_list );

  if (object_buffer[buf_num] != NULL) {
    s_delete_object_glist(toplevel, object_buffer[buf_num]);
    object_buffer[buf_num] = NULL;
  }

  object_buffer[buf_num] = o_glist_copy_all (toplevel, s_current,
                                             object_buffer[buf_num]);
}

/*! \brief Copy Selection to Buffer
 *  \par Function Description
 *  This function calls selection_to_buffer to copy the current
 *  selection data to a buffer and then iterator over the objects
 *  in the buffer, calling o_complex_reset_refdes for each complex
 *  in the copied buffer.
 *
 *  \param w_current A pointer to a GSCHEM top level object
 *  \param buf_num   integer value of the buffer to use.
 */
void o_buffer_copy(GschemToplevel *w_current, int buf_num)
{
  GList *iter;

  if (buf_num < 0 || buf_num >= MAX_BUFFERS) {
    g_warning (_("o_buffer_copy: Invalid buffer %i\n"), buf_num);
  }
  else
    selection_to_buffer (w_current, buf_num);

  /* Get a pointer to the glist in the buffer */
  iter = object_buffer[buf_num];

  /* reset all the refdes in the copied buffer */
  while (iter != NULL) {

    OBJECT *object = (OBJECT*) iter->data;

    if (object->type == OBJ_COMPLEX || object->type == OBJ_PLACEHOLDER) {
      o_complex_reset_refdes(w_current->toplevel ,object);
    }

    iter = g_list_next(iter);
  }
}

/*! \brief Cut Selection to Buffer
 *  \par Function Description
 *  This function calls selection_to_buffer to copy the current
 *  selection data to a buffer and then deleted the selected
 *  data.
 *
 *  \param w_current A pointer to a GSCHEM top level object
 *  \param buf_num   integer value of the buffer to use.
 */
void o_buffer_cut(GschemToplevel *w_current, int buf_num)
{
  if (buf_num < 0 || buf_num >= MAX_BUFFERS) {
    g_warning (_("o_buffer_cut: Invalid buffer %i\n"), buf_num);
  }
  else {
    selection_to_buffer (w_current, buf_num);
    o_delete_selected(w_current);
    w_current->toplevel->page_current->CHANGED = TRUE;
  }
}

/*! \brief Paste Buffer Contents to drawing
 *  \par Function Description
 *  This function initiates the pasting of data by copying the buffer
 *  contents and updates the global state variable so the user can
 *  position/place the any objects that were in the buffer.
 */
bool o_buffer_paste_start(GschemToplevel *w_current, int w_x, int w_y,
                          int buf_num)
{
  TOPLEVEL *toplevel = w_current->toplevel;
  int rleft, rtop, rbottom, rright;
  int x, y;

  if (w_current == NULL) {
    fprintf (stderr, "Internal Error: <%s><o_buffer_paste_start>"
                     "w_current is NULL, line %d.\n", __FILE__, __LINE__);
    return FALSE;
  }

  if (buf_num < 0 || buf_num >= MAX_BUFFERS) {
    fprintf(stderr, _("Got an invalid buffer_number [o_buffer_paste_start]\n"));
    return FALSE;
  }

  w_current->last_drawb_mode = LAST_DRAWB_MODE_NONE;

  /* remove the old place list if it exists */
  s_delete_object_glist(toplevel, toplevel->page_current->place_list);
  toplevel->page_current->place_list = NULL;

  toplevel->page_current->place_list =
    o_glist_copy_all (toplevel, object_buffer[buf_num],
                      toplevel->page_current->place_list);


#if DEBUG || DEBUG_DND_EVENTS || DEBUG_PASTE
    int dint;
    dint = g_list_length(toplevel->page_current->place_list);
    printf("o_buffer_paste_start: buffers has %d objects\n", dint);
#endif

  if (!world_get_object_glist_bounds (toplevel,
                                      toplevel->page_current->place_list,
                                     &rleft, &rtop,
                                     &rright, &rbottom)) {
    /* If the place buffer doesn't have any objects to define its any
     * bounds, we drop out here */
    return FALSE;
  }

  /* Place the objects into the buffer at the mouse origin, (w_x, w_y). */

  w_current->first_wx = w_x;
  w_current->first_wy = w_y;

  /* snap x and y to the grid, pointed out by Martin Benes */
  x = snap_grid (w_current, rleft);
  y = snap_grid (w_current, rtop);

  o_glist_translate_world (toplevel, w_x - x, w_y - y,
                           toplevel->page_current->place_list);

  w_current->inside_action = 1;
  i_set_state(w_current, ENDPASTE);

#if DEBUG || DEBUG_DND_EVENTS || DEBUG_PASTE
printf("o_buffer_paste_start: calling o_place_start with %d objects\n", dint);
#endif

  o_place_start (w_current, w_x, w_y);

  return TRUE;
}


/*! \brief Initialize the Buffers
 *  \par Function Description
 *  This function just set each member of our global array
 *  of buffer pointers to NULL.
 * 
 */
void o_buffer_init(void)
{
  int i;

  for (i = 0 ; i < MAX_BUFFERS; i++) {
    object_buffer[i] = NULL;
  }
}

/*! \brief Free All buffers
 *  \par Function Description
 *  This function iterates over each buffer and deletes any
 *  found by calling s_delete_object_glist.
 */
void o_buffer_free(GschemToplevel *w_current)
{
  TOPLEVEL *toplevel = w_current->toplevel;
  int i;

  for (i = 0 ; i < MAX_BUFFERS; i++) {
    if (object_buffer[i]) {
      s_delete_object_glist(toplevel, object_buffer[i]);
      object_buffer[i] = NULL;
    }
  }
}
