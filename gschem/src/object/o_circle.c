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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */
#include <config.h>
#include <stdio.h>
#include <math.h>

#include "gschem.h"

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_circle_invalidate_rubber (GschemToplevel *w_current)
{
  int cx, cy, radius;

  WORLDtoSCREEN (w_current, w_current->first_wx, w_current->first_wy, &cx, &cy);
  radius = SCREENabs (w_current, w_current->distance);

  o_invalidate_rect (w_current, cx - radius, cy - radius,
                                cx + radius, cy + radius);
}

/*! \brief Start process to input a new circle.
 *  \par Function Description
 *  This function starts the process to input a new circle. Parameters for
 *  this circle are pu into/extracted from the <B>w_current</B> toplevel
 *  structure.
 *  <B>w_x</B> and <B>w_y</B> are current coordinates of the mouse pointer in
 *  world units.
 *
 *  The first step of the circle input is to set the center of the arc.
 *  This center is kept in (<B>w_current->first_wx</B>,<B>w_current->first_wy</B>).
 *
 *  \param [in] w_current  The GschemToplevel object.
 *  \param [in] w_x        Current x coordinate of pointer in world units.
 *  \param [in] w_y        Current y coordinate of pointer in world units.
 */
void o_circle_start(GschemToplevel *w_current, int w_x, int w_y)
{
  /* center of circle */
  w_current->first_wx = w_x;
  w_current->first_wy = w_y;

  /* radius */
  w_current->distance = 0;

  /* first temporary circle */
  o_circle_invalidate_rubber (w_current);
  w_current->rubber_visible = 1;
}

/*! \brief End the input of a circle.
 *  \par Function Description
 *  This function ends the input of the radius of the circle.
 *  The (<B>w_x</B>,<B>w_y</B>) point is taken as the other end of the radius
 *  segment, i.e. on the circle. The distance between this point and the
 *  center is the radius of the circle.
 *  <B>w_x</B> and <B>w_y</B> are in world coords.
 *
 *  The center has previously been input and saved as
 *  (<B>w_current->first_wx</B>,<B>w_current->first_wy</B>).
 *
 *  The temporary circle drawn during the input of the radius is erased.
 *  A new object is allocated, initialized and linked in the object list.
 *  This new object is finally drawn.
 *
 *  \param [in] w_current  The GschemToplevel object.
 *  \param [in] w_x        (unused)
 *  \param [in] w_y        (unused)
 */
void o_circle_end(GschemToplevel *w_current, int w_x, int w_y)
{
  GedaToplevel *toplevel = w_current->toplevel;
  Object *new_obj;

  if (w_current->inside_action == 0) {
    u_log_message("Internal Error Detected: <o_circle_end> Not inside action\n");
    return;
  }
  /* erase the temporary circle */
  /* o_circle_invalidate_rubber (w_current); */
  w_current->rubber_visible = 0;

  /* circle with null radius are not allowed */
  if (w_current->distance == 0) {
    /* cancel the object creation */
    return;
  }

  /* create the object */
  new_obj = o_circle_new (GRAPHIC_COLOR,
                          w_current->first_wx, w_current->first_wy,
                          w_current->distance);
  new_obj->line_options->line_width =  o_style_get_line_width(toplevel);
  s_page_append_object (toplevel->page_current, new_obj);

  /* Call add-objects-hook */
  g_run_hook_object (w_current, "%add-objects-hook", new_obj);

  toplevel->page_current->CHANGED = 1;
  o_undo_savestate(w_current, UNDO_ALL);
}

/*! \brief Draw temporary circle while dragging edge.
 *  \par Function Description
 *  This function draws a circle according to its internal representation and
 *  allows the modification of its radius. The radius is updated according to
 *  the current mouse position in <B>w_x</B> and <B>w_y</B>.
 *  It draws a full circle and the horizontal segment of the radius in the
 *  right half of the circle.
 *
 *  The previous temporary circle is erased, the radius is then computed and
 *  updated and finally a new temporary circle is drawn.
 *
 *  The arc is internally described by :
 *  <DL>
 *    <DT>*</DT><DD>(<B>w_current->first_wx</B>,<B>w_current->first_wy</B>) as its
 *                   center ;
 *    <DT>*</DT><DD><B>w_current->distance</B> as its radius.
 *  </DL>
 *
 *  \param [in] w_current  The GschemToplevel object.
 *  \param [in] w_x        Current x coordinate of pointer in world units.
 *  \param [in] w_y        Current y coordinate of pointer in world units.
 */
void o_circle_motion (GschemToplevel *w_current, int w_x, int w_y)
{
  int diff_x, diff_y;

  if (w_current->inside_action == 0) {
    u_log_message("Internal Error Detected: <o_circle_motion> Not inside action\n");
    return;
  }

  /* erase the previous temporary circle if it is visible */
  if (w_current->rubber_visible)
    o_circle_invalidate_rubber (w_current);

  /*
   * The radius is taken as the biggest distance on the x and y axis between
   * the center of the circle and the mouse position.
   */
  diff_x = abs(w_current->first_wx - w_x);
  diff_y = abs(w_current->first_wy - w_y);
  w_current->distance = max(diff_x, diff_y);

  /* draw the new temporary circle */
  o_circle_invalidate_rubber (w_current);
  w_current->rubber_visible =1;
}

/*! \brief Draw circle from GschemToplevel object.
 *  \par Function Description
 *  This function draws the circle from the variables in the GschemToplevel
 *  structure <B>*w_current</B>.
 *  The center of the circle is at (<B>w_current->first_wx</B>,
 *  <B>w_current->first_wy</B>) and its radius is in <B>w_current->distance</B>.
 *
 *  It draws a horizontal radius segment on the right half of the circle and
 *  the circle with the selection color.
 *
 *  \param [in] w_current  The GschemToplevel object.
 */
void o_circle_draw_rubber (GschemToplevel *w_current)
{
  double wwidth = 0;
  cairo_t *cr = eda_renderer_get_cairo_context (w_current->renderer);
  GArray *color_map = eda_renderer_get_color_map (w_current->renderer);
  int flags = eda_renderer_get_cairo_flags (w_current->renderer);

  eda_cairo_center_arc (cr, flags, wwidth, wwidth,
                        w_current->first_wx, w_current->first_wy,
                        w_current->distance,
                        0, 360);

  eda_cairo_line (cr, flags, END_NONE, wwidth,
                  w_current->first_wx,
                  w_current->first_wy,
                  w_current->first_wx + w_current->distance,
                  w_current->first_wy);

  eda_cairo_set_source_color (cr, SELECT_COLOR, color_map);
  eda_cairo_stroke (cr, flags, TYPE_SOLID, END_NONE, wwidth, -1, -1);
}