/* -*- C++ o_draw.cpp indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-
 *
 * File: o_draw.cpp
 *
 * gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 *
 * Copyright (C) 2014 Wiley Edward Hill <wileyhill@gmail.com>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Date: Nov, 27, 2014
 * Contributing Author: Wiley Edward Hill
 *
*/
/*!
 * \file o_draw.cpp
 * \brief Interface for Drawing routines in Libgedadraw.
 *
 * \remarks This module is under development
 *
 */

#include <string>

#include <gschem.h>
#include <geda_draw.h>

#include <geda_debug.h>

static EdaX11Render *RenderAdaptor;

static void o_draw_set_color(GschemToplevel *w_current)
{
  GdkColor *color;
  Object   *o_current = RenderAdaptor->object;

  if (!o_current->selected) {
    color = x_color_get_color_from_index(o_current->color);
  }
  else {
    color = x_color_get_color_from_index(SELECT_COLOR);
  }

  RenderAdaptor->geda_draw_set_color(color->red, color->green, color->blue);
}

static void
o_draw_arc (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_ARC(o_current)) {

    Arc *o_arc;
    int scx, scy;
    int radius;
    int sradi;
    int angle1;
    int angle2;

    o_draw_set_color (w_current);

    o_arc   = o_current->arc;
    radius  = o_arc->width / 2;
    sradi   = SCREENabs(w_current, radius);
    angle1  = o_arc->start_angle;
    angle2  = o_arc->arc_sweep;

    WORLDtoSCREEN (w_current, o_arc->x,  o_arc->y, &scx,  &scy);

    RenderAdaptor->geda_draw_arc (scx, scy, sradi, angle1, angle2);
  }
}

static void
o_draw_circle (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_CIRCLE(o_current)) {

    Circle *o_circle;
    int scx, scy;
    int radius;

    o_draw_set_color (w_current);

    o_circle = o_current->circle;
    radius   = pix_x (w_current, o_current->circle->radius);

    WORLDtoSCREEN (w_current, o_circle->center_x, o_circle->center_y, &scx,  &scy);

    RenderAdaptor->geda_draw_circle (scx, scy, radius);
  }
}

static void
o_draw_box (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_BOX(o_current)) {

    Box *o_box;
    int sx1, sy1, sx2, sy2;
    int width, height;

    o_draw_set_color (w_current);

    o_box = o_current->box;

    WORLDtoSCREEN (w_current, o_box->upper_x,  o_box->upper_y, &sx1,  &sy1);
    WORLDtoSCREEN (w_current, o_box->lower_x,  o_box->lower_y, &sx2,  &sy2);

    width  = sx2 - sx1;
    height = sy2 - sy1;

    RenderAdaptor->geda_draw_box (sx1, sy1, width, height);
  }
}

static void
o_draw_line (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_LINE(o_current)) {

    Line *o_line;
    int sx1, sy1, sx2, sy2;

    o_draw_set_color (w_current);

    o_line = o_current->line;

    WORLDtoSCREEN (w_current, o_line->x[0],  o_line->y[0], &sx1,  &sy1);
    WORLDtoSCREEN (w_current, o_line->x[1],  o_line->y[1], &sx2,  &sy2);

    RenderAdaptor->geda_draw_line (sx1, sy1, sx2, sy2);
  }
}

static void
o_draw_bus (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_LINE(o_current)) {

    Line *o_line;
    int sx1, sy1, sx2, sy2;

    o_draw_set_color (w_current);

    o_line = o_current->line;

    WORLDtoSCREEN (w_current, o_line->x[0],  o_line->y[0], &sx1,  &sy1);
    WORLDtoSCREEN (w_current, o_line->x[1],  o_line->y[1], &sx2,  &sy2);

    RenderAdaptor->geda_draw_net (sx1, sy1, sx2, sy2);
  }
}

static void
o_draw_net (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_LINE(o_current)) {

    Line *o_line;
    int sx1, sy1, sx2, sy2;

    o_draw_set_color (w_current);

    o_line = o_current->line;

    WORLDtoSCREEN (w_current, o_line->x[0],  o_line->y[0], &sx1,  &sy1);
    WORLDtoSCREEN (w_current, o_line->x[1],  o_line->y[1], &sx2,  &sy2);

    RenderAdaptor->geda_draw_net (sx1, sy1, sx2, sy2);
  }
}

static void
o_draw_path (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  if (GEDA_IS_PATH(o_current)) {

    Object *tmp_obj;
    Path   *s_path;
    int     nsections;
    int     i;

    o_draw_set_color (w_current);

    tmp_obj   = o_path_copy (o_current);
    s_path    = tmp_obj->path;
    nsections = s_path->num_sections;

    for (i = 0; i < nsections; i++) {

      PATH_SECTION *section = &s_path->sections[i];
      int tmp_x, tmp_y;

      tmp_x = section->x1;
      tmp_y = section->y1;
      WORLDtoSCREEN (w_current, tmp_x, tmp_y, &section->x1,  &section->y1);

      tmp_x = section->x2;
      tmp_y = section->y2;
      WORLDtoSCREEN (w_current, tmp_x, tmp_y, &section->x2,  &section->y2);

      tmp_x = section->x3;
      tmp_y = section->y3;
      WORLDtoSCREEN (w_current, tmp_x, tmp_y, &section->x3,  &section->y3);
    }

    RenderAdaptor->geda_draw_path (nsections, s_path->sections);
    g_object_unref(tmp_obj);
  }
}

static void
o_draw_pin (GschemToplevel *w_current)
{
  o_draw_line (w_current);
}
static void
o_draw_picture (GschemToplevel *w_current)
{

}

static void
o_draw_text (GschemToplevel *w_current)
{
  Object *o_current = RenderAdaptor->object;

  /* First check if this is visible */
  if ( o_get_is_visible(o_current)) {

    if (GEDA_IS_TEXT(o_current)) {

      Text *o_text;
      int   sx, sy;

      o_draw_set_color (w_current);

      o_text = o_current->text;

      WORLDtoSCREEN (w_current, o_text->x,  o_text->y, &sx,  &sy);

      RenderAdaptor->geda_draw_text (sx, sy);
    }
  }
}

static void
o_draw_complex (GschemToplevel *w_current)
{
  GList    *iter;

  iter = RenderAdaptor->object->complex->prim_objs;

  while (iter) {
    Object *o_child = (Object*)iter->data;
    o_draw_object(w_current, o_child);
    iter = iter->next;
  }
}

extern "C" void
o_draw_object (GschemToplevel *w_current, Object *o_current)
{
  void (*draw_func)(GschemToplevel *w_current);

  g_return_if_fail (o_current != NULL);

  switch (o_current->type) {
    case OBJ_LINE:        draw_func = o_draw_line;    break;
    case OBJ_NET:         draw_func = o_draw_net;     break;
    case OBJ_BUS:         draw_func = o_draw_bus;     break;
    case OBJ_PIN:         draw_func = o_draw_pin;     break;
    case OBJ_BOX:         draw_func = o_draw_box;     break;
    case OBJ_ARC:         draw_func = o_draw_arc;     break;
    case OBJ_CIRCLE:      draw_func = o_draw_circle;  break;
    case OBJ_PATH:        draw_func = o_draw_path;    break;
    case OBJ_TEXT:        draw_func = o_draw_text;    break;
    case OBJ_PICTURE:     draw_func = o_draw_picture; break;
    case OBJ_COMPLEX:
    case OBJ_PLACEHOLDER: draw_func = o_draw_complex;
      break;

    default:
      g_return_if_reached ();
  }

  RenderAdaptor->object = o_current;

  draw_func (w_current);
}

extern "C" void o_draw_set_surface(GschemToplevel *w_current)
{
  RenderAdaptor->geda_draw_set_surface(w_current->cr, Current_Page->to_world_x_constant);
}

extern "C" void o_draw_initialize(void)
{
  EdaConfig  *cfg      = eda_config_get_user_context();
  const char *group    = IVAR_CONFIG_GROUP;
  const char *font_name;

  v_log_message(_("Initializing: Graphics Renderer Adaptor...."));

  font_name = eda_config_get_string (cfg, group, "default-font-name", NULL);

  RenderAdaptor = new EdaX11Render(font_name);

  geda_atexit(o_draw_shutdown, NULL);

  v_log_message(_("done\n"));
}

extern "C" void o_draw_shutdown(void *user_data)
{
  v_log_message(_("Shutting down: Graphics RenderAdaptorer..."));
  delete RenderAdaptor;
  v_log_message(_("done\n"));
}