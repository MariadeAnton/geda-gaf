/* gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

/*! \file o_set.c
 *  \brief functions for the basic object type
 *
 *  This file contains routines used to set the properties of
 *  <b>Objects</b>.
 *
 */

#include <config.h>
#include <stdio.h>

#include <libgeda_priv.h>

/*! \brief Mark an Object's cached bounds as invalid
 *
 *  \par Function Description
 *  Recursively marks the cached bounds of the given Object and its
 *  parents as having been invalidated and in need of an update. They
 *  will be recalculated next time the Object's bounds are requested
 *  (e.g. via o_get_bounds() ).
 *
 *  \param [in] obj
 *
 *  \todo Turn this into a macro?
 */
void o_set_bounds_invalid(Object *obj)
{
  do {
    obj->w_bounds_valid_for = NULL;
  } while ((obj = obj->parent_object) != NULL);
}

/*! \brief Change the color of an object
 *
 *  \par Function Description
 *  This function changes the object's color property to \a color if
 *  \a color is different from the current object color. If modified,
 *  the page change flag is set if the object is on a page.
 *
 *  \note For complex objects, this function is called recursively,
 *        since o_list_set_color calls this function for each child
 *        of the complex.
 *
 *  \param [in] object    The Object to change color.
 *  \param [in] color     The new color.
 */
void o_set_color (Object *object, int color)
{
  if (GEDA_IS_OBJECT(object)) {

    if (object->color != color) {

      object->color = color;

      if (object->page) {
        object->page->CHANGED = TRUE;
      }
    }

    if (object->type == OBJ_COMPLEX || object->type == OBJ_PLACEHOLDER) {
      o_list_set_color (object->complex->prim_objs, color);
    }

  }
  else {
    BUG_MSG ("object is not a GedaObject");
  }
}

/*! \brief Set #Object's fill options.
 *  \par Function Description
 *  This function allows an #Object's fill options to be configured.
 *  See OBJECT_FILLING for information on valid fill types.
 *
 *  \param [in,out]  object         Object to be updated.
 *  \param [in]      fill_options   OBJECT_FILLING type.
 *
 */
void
o_set_fill_options(Object *object, FILL_OPTIONS *fill_options)
{
  g_return_if_fail(GEDA_IS_BOX    (object) ||
                   GEDA_IS_CIRCLE (object) ||
                   GEDA_IS_ARC    (object) ||
                   GEDA_IS_PATH   (object) );

  bool modified = FALSE;
  FILL_OPTIONS *o_property = object->fill_options;

  if (object->page) {

    if (o_property->fill_type != fill_options->fill_type)
      modified = TRUE;
    else if (o_property->fill_width != fill_options->fill_width)
      modified = TRUE;
    else if (o_property->fill_pitch1 != fill_options->fill_pitch1)
      modified = TRUE;
    else if (o_property->fill_angle1 != fill_options->fill_angle1)
      modified = TRUE;
    else if (o_property->fill_pitch2 != fill_options->fill_pitch2)
      modified = TRUE;
    else if (o_property->fill_angle2 != fill_options->fill_angle2)
      modified = TRUE;
  }

  o_property->fill_type   = fill_options->fill_type;
  o_property->fill_width  = fill_options->fill_width;
  o_property->fill_pitch1 = fill_options->fill_pitch1;
  o_property->fill_angle1 = fill_options->fill_angle1;
  o_property->fill_pitch2 = fill_options->fill_pitch2;
  o_property->fill_angle2 = fill_options->fill_angle2;

  if (modified) {
    object->page->CHANGED = 1;
  }
}

/*! \brief Set an #Object's line options.
 *  \par Function Description
 *  This function allows a line's end, type, width, length and space to be set.
 *  See LINE_END and LINE_TYPE for information on valid
 *  object end and type values.
 *
 *  \param [in,out] object       Object to set line options on.
 *  \param [in]     line_options A option data structure.
 *
 *  \todo Make space an unsigned int and check for a max value instead.
 *        If a max value is not required, then it would simplify the code.
 */
void o_set_line_options(Object *object, LINE_OPTIONS *line_options)
{
  g_return_if_fail( GEDA_IS_LINE(object)   ||
                    GEDA_IS_CIRCLE(object) ||
                    GEDA_IS_ARC(object)    ||
                    GEDA_IS_BOX(object)    ||
                    GEDA_IS_PATH(object));

  LINE_OPTIONS *o_property = object->line_options;

  int  line_length = line_options->line_length;
  int  line_space  = line_options->line_space;
  bool modified    = FALSE;

  /* do some error checking / correcting */
  switch(line_options->line_type) {

    case(TYPE_DASHED):
    case(TYPE_CENTER):
    case(TYPE_PHANTOM):

      if (line_length < 1) {
        if (o_property->line_length < 1) {
          line_length = default_line_length;
          u_log_message(_("Setting line length to default=%d\n"), line_length);
        }
        else { /* Use current value */
          line_length = o_property->line_length;
        }
      }

    case(TYPE_DOTTED):

      if (line_space < 1) {

        if (o_property->line_space < 1) {
          line_space = default_line_space;
          u_log_message(_("Setting line space to default=%d\n"), line_space);
        }
        else { /* Use current value */
          line_space = o_property->line_space;
        }

      }

      break;

    default:
      break;
  }

  if (object->page) {

    if (o_property->line_width != line_options->line_width)
      modified = TRUE;
    else if (o_property->line_end != line_options->line_end)
      modified = TRUE;
    else if (o_property->line_type != line_options->line_type)
      modified = TRUE;
    else if (o_property->line_length != line_options->line_length)
      modified = TRUE;
    else if (o_property->line_space != line_options->line_space)
      modified = TRUE;
  }

  o_property->line_width  = line_options->line_width;
  o_property->line_end    = line_options->line_end;
  o_property->line_type   = line_options->line_type;
  o_property->line_length = line_length;
  o_property->line_space  = line_space;

  if (modified) {
    object->page->CHANGED = 1;
  }
}

/*! \brief Set visibility of the object.
 *  \par Function Description
 *  Set value of visibility field within the object.
 *  If resulting visibility value is changed,
 *  invalidate the bounds of the object and parent objects.
 *
 *  \param object     The #Object structure to be modified
 *  \param visibility Boolean desired state
 */
void
o_set_visibility (Object *object, int visibility)
{
  if(GEDA_IS_OBJECT(object)) {
    if (object->visibility != visibility) {
      object->visibility = visibility;
      o_set_bounds_invalid (object);
      s_object_set_page_changed (object);
    }
  }
  else {
    BUG_MSG ("object is not a GedaObject");
  }
}
