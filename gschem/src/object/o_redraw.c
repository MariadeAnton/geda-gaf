/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <gschem.h>
#include <geda_debug.h>

#define INVALIDATE_MARGIN 1

/*! \todo Lots of Gross code... needs lots of cleanup - mainly
 *        readability issues
 */


/*! \todo Finish function documentation!!!
 *  \brief Function Clean State - if drawing action in-progress
 *  \par Function Description
 *  This function is neccesary to make jumps between event_states.
 *  If we are inside an drawing action that created something on the dc,
 *  e.g. if we are drawing a box and then jump to line drawing without
 *  leaving the box drawing mode, there will remain some rubberbands on the
 *  screen.
 *  Usually a intermediate select state would clean (redraw) the screen.
 */
int o_redraw_cleanstates(GschemToplevel *w_current)
{
  GedaToplevel *toplevel = w_current->toplevel;

  /* returns FALSE if the function was'nt nessecary */
  if (w_current->inside_action == 0) {
    return FALSE;
  }

  switch (w_current->event_state) {
    /* all states with something on the dc */
    case ( ENDCOMP ):

      /* De-select the lists in the component selector */
      x_compselect_deselect (w_current);

      /* Fall through */
    case ( COPY ):
    case ( MCOPY ):
    case ( DRAWBUS ):
    case ( DRAWNET ):
    case ( ENDARC ):
    case ( ENDBOX ):
    case ( ENDCIRCLE ):
    case ( ENDCOPY ):
    case ( ENDMCOPY ):
    case ( ENDLINE ):
    case ( PATHCONT ):
    case ( ENDPATH ):
    case ( ENDMOVE ):
    case ( ENDPASTE ):
    case ( ENDPIN ):
    case ( ENDTEXT ):
    case ( GRIPS ):
    case ( MOVE ):
    case ( NETCONT ):
    case ( ZOOMBOXEND ):
      /* it is possible to cancel in the middle of a place,
       * so lets be sure to clean up the place_list structure */

      /* If we're cancelling from a move action, re-wind the
       * page contents back to their state before we started. */
      if ((w_current->event_state == MOVE) ||
          (w_current->event_state == ENDMOVE)) {
        o_move_cancel (w_current);
      }

      /* If we're cancelling from a grip action, call the specific cancel
       * routine to reset the visibility of the object being modified */
      if (w_current->event_state == GRIPS)
        o_grips_cancel (w_current);

      /* Free the place list and its contents. If we were in a move
       * action, the list (refering to objects on the page) would
       * already have been cleared in o_move_cancel(), so this is OK. */
      s_object_release_objects(toplevel->page_current->place_list);
      toplevel->page_current->place_list = NULL;

      w_current->inside_action = 0;

      /* touch the select state */
      i_status_set_state(w_current, SELECT);

      /* from i_callback_cancel() */
      o_invalidate_all (w_current);
      return TRUE;

    /* all remaining states without dc changes */
    case ( NONE ):
    case ( SELECT ):
    case ( DRAWLINE ):
    case ( DRAWBOX ):
    case ( DRAWCIRCLE ):
    case ( ZOOM ):
    case ( PAN ):
    case ( BUSCONT ):
    case ( DRAWARC ):
    case ( DRAWPICTURE ):
    case ( DRAWPIN ):
    case ( ENDMIRROR ):
    case ( ENDPICTURE ):
    case ( ENDROTATEP ):
    case ( ENDROUTENET ):
    case ( MOUSEPAN ):
    case ( SBOX ):
    case ( STARTCOPY ):
    case ( STARTMCOPY ):
    case ( STARTDRAWBUS ):
    case ( STARTDRAWNET ):
    case ( STARTMOVE ):
    case ( STARTPAN ):
    case ( STARTPASTE ):
    case ( STARTROUTENET ):
    case ( STARTDESELECT ):
    case ( STARTSELECT ):
    case ( ZOOMBOXSTART ):
      return FALSE;
  }
  return FALSE;
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void
o_redraw_rectangles (GschemToplevel *w_current, GdkRectangle *rectangles, int n_rectangles)
{
  GedaToplevel *toplevel = w_current->toplevel;

  bool draw_selected;
  bool is_only_text;

  int bloat;
  int cue_half_size;
  int grip_half_size;
  int i;
  int render_flags;
  int zoom;

  GList *obj_list;
  GList *iter;
  RECTANGLE   *world_rects;
  EdaRenderer *renderer;

  GArray *render_color_map         = NULL;
  GArray *render_outline_color_map = NULL;
  cairo_matrix_t render_mtx;

  for (i = 0; i < n_rectangles; i++) {
    x_repaint_background_region (w_current, rectangles[i].x, rectangles[i].y,
                                 rectangles[i].width, rectangles[i].height);
  }

  g_return_if_fail (toplevel != NULL);
  g_return_if_fail (toplevel->page_current != NULL);

  grip_half_size = w_current->grip_pixel_size / 2;

  /*grip_half_size = o_grips_half_size (w_current, NULL); */

  cue_half_size = SCREENabs (w_current, CUE_BOX_SIZE);
  bloat = MAX (grip_half_size, cue_half_size);

  world_rects = g_new (RECTANGLE, n_rectangles);

  for (i = 0; i < n_rectangles; i++) {
    int x, y, width, height;

    x = rectangles[i].x;
    y = rectangles[i].y;
    width = rectangles[i].width;
    height = rectangles[i].height;

    SCREENtoWORLD (w_current, x - bloat, y + height + bloat,
                   &world_rects[i].lower_x, &world_rects[i].lower_y);
    SCREENtoWORLD (w_current, x + width + bloat, y - bloat,
                   &world_rects[i].upper_x, &world_rects[i].upper_y);
  }

  obj_list = s_page_objects_in_regions (toplevel->page_current,
                                        world_rects, n_rectangles);

  GEDA_FREE (world_rects);

  /* Set up renderer based on configuration in w_current and list */
  render_flags = EDA_RENDERER_FLAG_HINTING;
 /* if (toplevel->page_current->show_hidden_text) {
    render_flags |= EDA_RENDERER_FLAG_TEXT_HIDDEN;
  }*/

  is_only_text = TRUE;
  iter = g_list_first(obj_list);
  while(iter != NULL) {
    Object *object = iter->data;
    if (object->type != OBJ_TEXT) {
       is_only_text = FALSE;
       break;
    }
    NEXT(iter);
  }

  if (w_current->fast_mousepan && w_current->doing_pan) {
    render_flags |= (EDA_RENDERER_FLAG_TEXT_OUTLINE
                 |   EDA_RENDERER_FLAG_PICTURE_OUTLINE);
  }
  else {
    zoom = toplevel->page_current->to_world_x_constant;
    if ((zoom > w_current->text_display_zoomfactor) &&
        (w_current->text_feedback != ALWAYS_FEEDBACK))
       render_flags |= (EDA_RENDERER_FLAG_TEXT_OUTLINE);
  }

  if ((is_only_text) &&
      ( w_current->text_feedback != ALWAYS_FEEDBACK) &&
      ( w_current->inside_action))
     render_flags |= (EDA_RENDERER_FLAG_TEXT_OUTLINE);

  /* The display color map is used for "normal" rendering. */
  render_color_map = x_color_get_display_color_map();

  /* Retrive a copy pf the outine color map used for rendering rubber
   * banding nets and buses, and objects which are in the process of
   * being placed. */
  render_outline_color_map = x_color_get_outline_color_map();

  /* Set up renderer */
  renderer = g_object_ref (w_current->renderer);

  g_object_set (G_OBJECT (renderer),
                "cairo-context", w_current->cr,
                "grip-size", ((double) grip_half_size * toplevel->page_current->to_world_x_constant),
                "render-flags", render_flags,
                "color-map", render_color_map,
                NULL);

  /* We need to transform the cairo context to world coordinates while
   * we're drawing using the renderer. */
  cairo_matrix_init (&render_mtx,
                     (double) toplevel->page_current->to_screen_x_constant,
                     0,
                     0,
                     - (double) toplevel->page_current->to_screen_y_constant,
                     (- (double) toplevel->page_current->left * toplevel->page_current->to_screen_x_constant),
                     ((double) toplevel->page_current->to_screen_y_constant * toplevel->page_current->top + w_current->screen_height)
                    );

  cairo_save (w_current->cr);
  cairo_set_matrix (w_current->cr, &render_mtx);

  /* Determine whether we should draw the selection at all */
  draw_selected = !(w_current->inside_action &&
                  ((w_current->event_state == MOVE) ||
                   (w_current->event_state == ENDMOVE)));

  /* First pass -- render non-selected objects */
  for (iter = obj_list; iter != NULL; NEXT(iter)) {
    Object *o_current = iter->data;
    if (!(o_current->dont_redraw || o_current->selected)) {
      o_style_set_object(toplevel, o_current);
      eda_renderer_draw (renderer, o_current);
    }
  }

  if (!is_only_text) {
    /* Second pass -- render cues */
    for (iter = obj_list; iter != NULL; NEXT(iter)) {
      Object *o_current = iter->data;
      if (!(o_current->dont_redraw || o_current->selected)) {
        /* o_style_set_object(toplevel, o_current); cues don't have style yet*/
        eda_renderer_draw_cues (renderer, o_current);
      }
    }
  }

  /* Third pass -- render selected objects, cues & grips. This is
   * done in a separate pass to non-selected items to make sure that
   * the selection and grips are never obscured by other objects. */
  if (draw_selected) {

    g_object_set (G_OBJECT (renderer), "override-color", SELECT_COLOR, NULL);
    for (iter = geda_list_get_glist (toplevel->page_current->selection_list);
         iter != NULL; NEXT(iter)) {
      Object *o_current = iter->data;
      if (!o_current->dont_redraw) {
        o_style_set_object(toplevel, o_current);
        eda_renderer_draw (renderer, o_current);
        eda_renderer_draw_cues (renderer, o_current);
        if (w_current->renderer->draw_grips ) {
          /* get the dynamic size of the grip */
          //grip_half_size = o_grips_half_size (w_current, o_current);
          //g_object_set (G_OBJECT (renderer), "grip-size",((double) grip_half_size * toplevel->page_current->to_world_x_constant), NULL);
          eda_renderer_draw_grips (renderer, o_current);
        }
      }
    }
    g_object_set (G_OBJECT (renderer), "override-color", -1, NULL);
  }

  if (w_current->inside_action) {
    /* Redraw the rubberband objects (if they were previously visible) */

    switch (w_current->event_state) {
      case MOVE:
      case ENDMOVE:
        if (w_current->last_drawb_mode != -1) {
          /* FIXME shouldn't need to save/restore matrix/colormap here */
          cairo_save (w_current->cr);
          cairo_set_matrix (w_current->cr, &render_mtx);
          eda_renderer_set_color_map (renderer, render_outline_color_map);

          o_move_draw_rubber (w_current, draw_selected);

          eda_renderer_set_color_map (renderer, render_color_map);
          cairo_restore (w_current->cr);
        }
        break;

      case ENDCOPY:
      case ENDMCOPY:
      case ENDCOMP:
      case ENDTEXT:
      case ENDPASTE:
        if (w_current->rubber_visible) {
          /* FIXME shouldn't need to save/restore matrix/colormap here */
          cairo_save (w_current->cr);
          cairo_set_matrix (w_current->cr, &render_mtx);
          eda_renderer_set_color_map (renderer, render_outline_color_map);

          o_place_draw_rubber (w_current, draw_selected);

          eda_renderer_set_color_map (renderer, render_color_map);
          cairo_restore (w_current->cr);
        }
        break;

      case STARTDRAWNET:
      case DRAWNET:
      case NETCONT:
        if (w_current->rubber_visible) {
          /* FIXME shouldn't need to save/restore matrix/colormap here */
          cairo_save (w_current->cr);
          cairo_set_matrix (w_current->cr, &render_mtx);
          eda_renderer_set_color_map (renderer, render_outline_color_map);

          o_net_draw_rubber (w_current);

          eda_renderer_set_color_map (renderer, render_color_map);
          cairo_restore (w_current->cr);
        }
        break;

      case STARTDRAWBUS:
      case DRAWBUS:
      case BUSCONT:
        if (w_current->rubber_visible) {
          /* FIXME shouldn't need to save/restore matrix/colormap here */
          cairo_save (w_current->cr);
          cairo_set_matrix (w_current->cr, &render_mtx);
          eda_renderer_set_color_map (renderer, render_outline_color_map);

          o_bus_draw_rubber(w_current);

          eda_renderer_set_color_map (renderer, render_color_map);
          cairo_restore (w_current->cr);
        }
        break;

      case GRIPS:
        if (w_current->rubber_visible)
          o_grips_draw_rubber (w_current);
        break;

      case SBOX:
        if (w_current->rubber_visible)
          o_select_box_draw_rubber (w_current);
        break;

      case ZOOMBOXEND:
        if (w_current->rubber_visible)
          i_zoom_world_box_draw_rubber (w_current);
        break;

      case ENDLINE:
        if (w_current->rubber_visible)
          o_line_draw_rubber (w_current);
        break;
      case PATHCONT:
      case ENDPATH:
        if (w_current->rubber_visible)
          o_path_draw_rubber (w_current);
        break;

      case ENDBOX:
        if (w_current->rubber_visible)
          o_box_draw_rubber (w_current);
        break;

      case ENDPICTURE:
        if (w_current->rubber_visible)
          o_picture_draw_rubber (w_current);
        break;

      case ENDCIRCLE:
        if (w_current->rubber_visible)
          o_circle_draw_rubber (w_current);
        break;

      case ENDARC:
        if (w_current->rubber_visible)
          o_arc_draw_rubber (w_current);
        break;

      case ENDPIN:
        if (w_current->rubber_visible)
          o_pin_draw_rubber (w_current);
        break;
    }
  }

  g_list_free (obj_list);
  GEDA_UNREF (renderer);
  g_array_free (render_color_map, TRUE);
  g_array_free (render_outline_color_map, TRUE);

}