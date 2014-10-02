/* -*- C o_place.c indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-
 *
 * gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 *
 * Copyright (C) 1998-2014 Ales Hvezda
 * Copyright (C) 1998-2014 gEDA Contributors (see ChangeLog for details)
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
/*!
 * \file o_place.c
 * \brief Low-level module for placing objects
 */
#include <gschem.h>
#include <geda_debug.h>

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_place_start (GschemToplevel *w_current, int w_x, int w_y)
{
  w_current->second_wx = w_x;
  w_current->second_wy = w_y;

  o_place_invalidate_rubber (w_current, TRUE);
  w_current->rubber_visible = 1;
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
o_place_end (GschemToplevel *w_current, int w_x, int w_y,
             int continue_placing, GList **ret_new_objects, const char* hook_name)
{
  GedaToplevel *toplevel = w_current->toplevel;

  Object *o_current;
  Page   *p_current;
  GList  *temp_dest_list = NULL;
  GList  *connected_objects = NULL;
  GList  *iter;

  int w_diff_x, w_diff_y;

  /* erase old image */
  w_current->rubber_visible = 0;

  /* Calc final object positions */
  w_current->second_wx = w_x;
  w_current->second_wy = w_y;

  w_diff_x = w_current->second_wx - w_current->first_wx;
  w_diff_y = w_current->second_wy - w_current->first_wy;

  if (continue_placing) {
    /* Make a copy of the place list if we want to keep it afterwards */
    temp_dest_list = o_glist_copy_all (toplevel->page_current->place_list,
                                       temp_dest_list);
  }
  else {
    /* Otherwise just take it */
    temp_dest_list = toplevel->page_current->place_list;
    toplevel->page_current->place_list = NULL;
  }

  if (ret_new_objects != NULL) {
    *ret_new_objects = g_list_copy (temp_dest_list);
  }

  o_glist_translate_world(w_diff_x, w_diff_y, temp_dest_list);

  /* Attach each item back onto the page's object list. Update object
   * connectivity and add the new objects to the selection list.*/
  p_current = toplevel->page_current;

  for (iter = temp_dest_list; iter != NULL; NEXT(iter)) {

    o_current = iter->data;

    s_page_append_object (p_current, o_current);

    /* Update object connectivity */
    s_conn_update_object (o_current);
    connected_objects = s_conn_return_others (connected_objects, o_current);
  }

  if (hook_name != NULL) {
    g_run_hook_object_list (w_current, hook_name, temp_dest_list);
  }

  o_invalidate_glist (w_current, connected_objects);
  g_list_free (connected_objects);
  connected_objects = NULL;

  toplevel->page_current->CHANGED = 1;
  o_invalidate_glist (w_current, temp_dest_list); /* only redraw new objects */
  g_list_free (temp_dest_list);

  o_undo_savestate (w_current, UNDO_ALL);
  i_status_update_sensitivities (w_current);
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_place_motion (GschemToplevel *w_current, int w_x, int w_y)
{
  if (w_current->rubber_visible) {
    o_place_invalidate_rubber (w_current, FALSE);
  }
  w_current->second_wx = w_x;
  w_current->second_wy = w_y;
  o_place_invalidate_rubber (w_current, TRUE);
  w_current->rubber_visible = 1;
}

/*! \brief Invalidate bounding box or outline for Object placement
 *
 *  \par Function Description
 *  This function invalidates the bounding box where objects would be
 *  drawn by o_place_draw_rubber()
 *
 * The function applies manhatten mode constraints to the coordinates
 * before drawing if the CONTROL key is recording as being pressed in
 * the w_current structure.
 *
 * The "drawing" parameter is used to indicate if this drawing should
 * immediately use the selected feedback mode and positioning constraints.
 *
 * With drawing=TRUE, the selected conditions are used immediately,
 * otherwise the conditions from the last drawing operation are used,
 * saving the new state for next time.
 *
 * This function should be called with drawing=TRUE when starting a
 * rubberbanding operation and when otherwise refreshing the rubberbanded
 * outline (e.g. after a screen redraw). For any undraw operation, should
 * be called with drawing=FALSE, ensuring that the undraw XOR matches the
 * mode and constraints of the corresponding "draw" operation.
 *
 * If any mode / constraint changes are made between a undraw, redraw XOR
 * pair, the latter (draw) operation must be called with drawing=TRUE. If
 * no mode / constraint changes were made between the pair, it is not
 * harmful to call the draw operation with "drawing=FALSE".
 *
 *  \param [in] w_current   GschemToplevel which we're drawing for.
 *  \param [in] drawing     Set to FALSE for undraw operations to ensure
 *                            matching conditions to a previous draw operation.
 */
void o_place_invalidate_rubber (GschemToplevel *w_current, int drawing)
{
  GedaToplevel *toplevel = w_current->toplevel;
  int diff_x, diff_y;
  int left, top, bottom, right;
  int s_left, s_top, s_bottom, s_right;

  if (toplevel->page_current->place_list != NULL) {

    /* If drawing is true, then don't worry about the previous drawing
     * method and movement constraints, use with the current settings */
    if (drawing) {
      /* Ensure we set this to flag there is "something" supposed to be
       * drawn when the invaliate call below causes an expose event. */
      w_current->last_drawb_mode = w_current->action_feedback_mode;
      w_current->drawbounding_action_mode = (w_current->CONTROLKEY)
      ? CONSTRAINED : FREE;
    }

    /* Calculate delta of X-Y positions from buffer's origin */
    diff_x = w_current->second_wx - w_current->first_wx;
    diff_y = w_current->second_wy - w_current->first_wy;

    /* Adjust the coordinates according to the movement constraints */

    /* Need to update the w_current->{first,second}_w{x,y} coords even
     * though we're only invalidating because the move rubberband code
     * (which may execute right after this function) expects these
     * coordinates to be correct.
     */
    if (w_current->drawbounding_action_mode == CONSTRAINED) {
      if (abs (diff_x) >= abs (diff_y)) {
        w_current->second_wy = w_current->first_wy;
        diff_y = 0;
      }
      else {
        w_current->second_wx = w_current->first_wx;
        diff_x = 0;
      }
    }

    /* Find the bounds of the drawing to be done */
    if (world_get_object_glist_bounds (toplevel->page_current->place_list,
      &left, &top, &right, &bottom))
    {

      WORLDtoSCREEN (w_current, left + diff_x, top + diff_y, &s_left, &s_top);
      WORLDtoSCREEN (w_current, right + diff_x, bottom + diff_y, &s_right, &s_bottom);

      o_invalidate_rectangle (w_current, s_left, s_top, s_right, s_bottom);
    }
    else {
      BUG_TRACE("Error No bounds");
    }
  }
  else {
    BUG_TRACE("page_current->place_list is NULL");
  }
}

/*! \brief Draw a bounding box or outline for Object placement
 *
 *  \par Function Description
 *  This function draws either the Object in the place list
 *  or a rectangle around their bounding box, depending upon the
 *  currently selected w_current->action_feedback_mode. This takes the
 *  value BOUNDINGBOX or OUTLINE.
 *
 * The function applies manhatten mode constraints to the coordinates
 * before drawing if the CONTROL key is recording as being pressed in
 * the w_current structure.
 *
 * The "drawing" parameter is used to indicate if this drawing should
 * immediately use the selected feedback mode and positioning constraints.
 *
 * With drawing=TRUE, the selected conditions are used immediately,
 * otherwise the conditions from the last drawing operation are used,
 * saving the new state for next time.
 *
 * This function should be called with drawing=TRUE when starting a
 * rubberbanding operation and when otherwise refreshing the rubberbanded
 * outline (e.g. after a screen redraw). For any undraw operation, should
 * be called with drawing=FALSE, ensuring that the undraw XOR matches the
 * mode and constraints of the corresponding "draw" operation.
 *
 * If any mode / constraint changes are made between a undraw, redraw XOR
 * pair, the latter (draw) operation must be called with drawing=TRUE. If
 * no mode / constraint changes were made between the pair, it is not
 * harmful to call the draw operation with "drawing=FALSE".
 *
 *  \param [in] w_current   GschemToplevel which we're drawing for.
 *  \param [in] drawing     Set to FALSE for undraw operations to ensure
 *                            matching conditions to a previous draw operation.
 */
void o_place_draw_rubber (GschemToplevel *w_current, int drawing)
{
  GedaToplevel *toplevel = w_current->toplevel;
  cairo_t *cr = eda_renderer_get_cairo_context (w_current->renderer);
  int diff_x, diff_y;

  g_return_if_fail (toplevel->page_current->place_list != NULL);

  /* If drawing is true, then don't worry about the previous drawing
   * method and movement constraints, use with the current settings */
  if (drawing) {
    w_current->last_drawb_mode = w_current->action_feedback_mode;
    w_current->drawbounding_action_mode = (w_current->CONTROLKEY)
                                            ? CONSTRAINED : FREE;
  }

  /* Calculate delta of X-Y positions from buffer's origin */
  diff_x = w_current->second_wx - w_current->first_wx;
  diff_y = w_current->second_wy - w_current->first_wy;

  /* Adjust the coordinates according to the movement constraints */
  if (w_current->drawbounding_action_mode == CONSTRAINED ) {
    if (abs(diff_x) >= abs(diff_y)) {
      w_current->second_wy = w_current->first_wy;
      diff_y = 0;
    } else {
      w_current->second_wx = w_current->first_wx;
      diff_x = 0;
    }
  }

  /* Translate the cairo context to the required offset before drawing. */
  cairo_save (cr);
  cairo_translate (cr, diff_x, diff_y);

  /* Draw with the appropriate mode */
  if (w_current->last_drawb_mode == BOUNDINGBOX) {
    GArray *map = eda_renderer_get_color_map (w_current->renderer);
    int flags = eda_renderer_get_cairo_flags (w_current->renderer);
    int left, top, bottom, right;

    /* Find the bounds of the drawing to be done */
    world_get_object_glist_bounds (toplevel->page_current->place_list,
                                   &left, &top, &right, &bottom);

    /* Draw box outline */
    eda_cairo_box (cr, flags, 0, left, top, right, bottom);

    eda_cairo_set_source_color (cr, BOUNDINGBOX_COLOR, map);

    eda_cairo_stroke (cr, flags, TYPE_SOLID, END_NONE, 0, -1, -1);
  }
  else {
    GList *iter;
    for (iter = Place_List; iter != NULL; NEXT(iter)) {
      eda_renderer_draw (w_current->renderer, (Object *) iter->data);
    }
  }
  cairo_restore (cr);
}


/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_place_rotate (GschemToplevel *w_current)
{
  GedaToplevel *toplevel = w_current->toplevel;

  o_glist_rotate_world (w_current->first_wx, w_current->first_wy, 90,
                        toplevel->page_current->place_list);


  /* Run rotate-objects-hook */
  g_run_hook_object_list (w_current, "%rotate-objects-hook",
                          toplevel->page_current->place_list);
}
