/* -*- C o_attrib.c indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-
 *
 * gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
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
 * MA 02110-1301 USA
 */
/*!
 * file o_attrib.c
 * \brief Low-level module for manipulating Attribute objects
 * \todo o_attrib.c conflicts with o_attrib.c in libgeda
 */

#include <gschem.h>
#include <geda_debug.h>

/* No special type for attributes */
/* You can only edit text attributes */

/* be sure in o_copy o_move o_delete you maintain the attributes delete
 * delete is a bare, because you will have to unattach the other end and
 * and in o_save o_read as well and in o_select when selecting objects,
 * select the attributes.
 *
 * There needs to be a modifier (in struct.h, such as a flag) which
 * signifies that this is an attribute
 */

/*! \brief Add attributes of object to selection
 *  \par Function Description
 *  Adds non-selected attributes of \a selected to \a selection list.
 *
 */
void o_attrib_attached_2_selection(GschemToplevel *w_current,
                                   SELECTION      *selection,
                                   Object         *object)
{
  Object *a_current;
  GList  *a_iter;
  GList  *objects_added;

  if (selection) {

    objects_added = NULL;

    for (a_iter = object->attribs; a_iter; NEXT(a_iter)) {

      a_current = a_iter->data;

      /* make sure object isn't selected already */
      if (!a_current->selected) {
        o_selection_add (selection, a_current);
        objects_added = g_list_prepend (objects_added, a_current);
      }
    }

    if (objects_added) {

      /* Update Multi-Attribute Dialog */
      x_multiattrib_update (w_current);

      /* Run select-objects-hook */
      g_hook_run_object_list(w_current, SELECT_OBJECTS_HOOK, objects_added);
      g_list_free (objects_added);
    }
  }
  else {
    BUG_MSG("selection == NULL\n");
  }
}

/*! \brief Attach attributes in list to preset object
 *  \par Function Description
 *  Add every text object in list that is an attribute (=) and is not object,
 *  to object.
 */
void o_attrib_attach_list_2_object(GschemToplevel *w_current, GList *list)
{
  GList  *attached_objects = NULL;
  GList  *iter             = list;
  Object *target           = w_current->which_object;

  while (iter != NULL) {

    Object *object = iter->data;

    if (object != NULL && object->attached_to == NULL && object != target) {

      if (o_get_is_valid_attribute(object)) {
        o_attrib_attach (target, object, TRUE);
        attached_objects = g_list_prepend (attached_objects, object);
      }
    }

    iter = iter->next;
  }

  if (attached_objects != NULL) {

    /* Update Multi-Attribute Dialog */
    x_multiattrib_update (w_current);

    g_hook_run_object_list (w_current, ATTACH_ATTRIBS_HOOK, attached_objects);
    g_list_free (attached_objects);
    o_undo_savestate(w_current, UNDO_ALL);
  }
}

/*! \brief Remove invisible attributes of an object from the selection list.
 *  \par Function Description
 *
 *  Remove all invisible attributes attached to the given object
 *  from the selection list. If hidden text is being shown, this
 *  function returns immediately.
 *
 *  \param [in]     w_current  The GschemToplevel object.
 *  \param [in,out] selection  The SELECTION list to remove from.
 *  \param [in]     object     Selected Object whose invisible attributes to remove.
 */
void o_attrib_deselect_invisible (GschemToplevel *w_current,
                                  SELECTION      *selection,
                                  Object         *object)
{
  GList *a_iter;

  if (selection == NULL) {
    BUG_MSG("selection == NULL");
    return;
  }

  if (Current_Page->show_hidden_text) {
    return;
  }

  for (a_iter = object->attribs; a_iter != NULL; NEXT(a_iter)) {

    Object *a_current = a_iter->data;

    if (a_current->selected && !o_get_is_visible(a_current)) {
      o_selection_remove (selection, a_current);
    }
  }
}

/*! \brief Add invisible attributes of an object to the selection list.
 *  \par Function Description
 *
 *  Add all invisible attributes attached to the given object
 *  to the selection list. If hidden text is being shown, this
 *  function returns immediately.
 *
 *  \param [in]     w_current  The GschemToplevel object.
 *  \param [in,out] selection  The SELECTION list to add to.
 *  \param [in]     object     Selected Object whose invisible attributes to add.
 */
void o_attrib_select_invisible (GschemToplevel *w_current,
                                SELECTION      *selection,
                                Object         *object)
{
  GList  *a_iter;

  if (selection == NULL) {
    BUG_MSG("selection == NULL");
    return;
  }

  if (Current_Page->show_hidden_text) {
    return;
  }

  for (a_iter = object->attribs; a_iter != NULL; NEXT(a_iter)) {

    Object *a_current = a_iter->data;

    if (!a_current->selected && !o_get_is_visible(a_current)) {
      o_selection_add (selection, a_current);
    }
  }
}

/*! \brief Change visibility status of attribute object.
 *  \par Function Description
 *  This function toggles the visibility status of the attribute \a
 *  object and updates it. The object is erased or redrawn if
 *  necessary.
 *
 *  \param [in] w_current  The GschemToplevel object.
 *  \param [in] object     The attribute object.
 */
void o_attrib_toggle_visibility(GschemToplevel *w_current, Object *object)
{
  g_return_if_fail (object != NULL && object->type == OBJ_TEXT);

  if (object->visibility == VISIBLE) {

    /* Must hide before changing or libgedacairo will not redraw */
    o_invalidate_object (w_current, object);

    o_set_visibility (object, INVISIBLE);

    if (Current_Page->show_hidden_text) {
      o_invalidate_object (w_current, object);
    }

  }
  else {

    o_set_visibility (object, VISIBLE);

    o_text_recreate(object);
  }
}

/*! \brief Set what part of an attribute is shown.
 *  \par Function Description
 *  This function changes what part (name, value or both) of an
 *  attribute is shown by its attribute object. The attribute object
 *  is erased, updated and finally redrawn.
 *
 *  \param [in] w_current  The GschemToplevel object.
 *  \param [in] object     The attribute object.
 *  \param [in] show_name_value  The new display flag for attribute.
 */
void o_attrib_toggle_show_name_value(GschemToplevel *w_current,
                                     Object *object, int show_name_value)
{
  g_return_if_fail (object != NULL && object->type == OBJ_TEXT);

  o_invalidate_object (w_current, object);
  object->show_name_value = show_name_value;
  o_text_recreate(object);
}

/*! \brief Create and Add and Attribute Text Object
 *  \par Function Description
 *   Creates a new text attribute using the supplied values and properties.
 *   If \a parent is not NULL, the new attribute will be added the parent
 *   and to the page associated with the parent. The position of the new
 *   attribute is adjusted based on the parent object type. If parent is
 *   NULL the new object is added as a floating attribute to the current
 *   page.
 *
 *  \note This function no longer returns NULL, instead the new object,
 *        aka text item, is always return */
Object *o_attrib_add_attrib(GschemToplevel *w_current,
                            const char     *text_string,
                            int             visibility,
                            int             show_name_value,
                            Object         *parent)
{
  Page *page;
  int world_x, world_y;
  int align;
  int angle;
  int color;
  int left, right, top, bottom;

  Object *new_obj;

  world_x = -1;
  world_y = -1;

  /* change later of needed */
  align   = LOWER_LEFT;
  angle   = 0;

  page = gschem_toplevel_get_current_page (w_current);

  /* creating a toplevel or unattached attribute */
  if (parent) {

    int offset;

    color = ATTRIBUTE_COLOR;

    /* get coordinates of where to place the text object */
    switch(parent->type) {
      case(OBJ_COMPLEX):
      case(OBJ_PLACEHOLDER):
        world_x = parent->complex->x;
        world_y = parent->complex->y;
        break;

      case(OBJ_ARC):
        world_x = parent->arc->x;
        world_y = parent->arc->y;
        break;

      case(OBJ_CIRCLE):
        world_x = parent->circle->center_x;
        world_y = parent->circle->center_y;
        break;

      case(OBJ_BOX):
        world_x = parent->box->upper_x;
        world_y = parent->box->upper_y;
        break;

      case(OBJ_LINE):
      case(OBJ_NET):
      case(OBJ_PIN):
      case(OBJ_BUS):
      {
        int dx = parent->line->x[1] - parent->line->x[0];
        int dy = parent->line->y[1] - parent->line->y[0];

        if (dy == 0) {
          if (dx > 0) {
            world_x = parent->line->x[0] + ( 2 * DEFAULT_ATTRIBUTE_OFFSET);
            world_y = parent->line->y[0] + DEFAULT_ATTRIBUTE_OFFSET;
          }
          else {
            world_x = parent->line->x[0] - ( 2 * DEFAULT_ATTRIBUTE_OFFSET);
            world_y = parent->line->y[0] + DEFAULT_ATTRIBUTE_OFFSET;

            align = LOWER_RIGHT;
          }
        }
        else if (dx == 0) {
          if (dy > 0) {
            world_x = parent->line->x[0] - DEFAULT_ATTRIBUTE_OFFSET;
            world_y = parent->line->y[0] + ( 2 * DEFAULT_ATTRIBUTE_OFFSET);

            angle = 90;
          }
          else {
            world_x = parent->line->x[0] - DEFAULT_ATTRIBUTE_OFFSET;
            world_y = parent->line->y[0] - ( 2 * DEFAULT_ATTRIBUTE_OFFSET);

            align = LOWER_RIGHT;
            angle = 90;
          }
        }
        else {
          world_x = parent->line->x[0];
          world_y = parent->line->y[0];
        }
      }
      break;

      case(OBJ_TEXT):
        offset = 1.3 * w_current->text_size * page->to_world_x_constant;
        world_x = parent->text->x;
        world_y = parent->text->y - offset - 2 * DEFAULT_ATTRIBUTE_OFFSET;
        color = DETACHED_ATTRIBUTE_COLOR;
        break;
    }
  }
  else {

    color = DETACHED_ATTRIBUTE_COLOR;

    o_get_bounds_list (s_page_get_objects (page),
                             &left, &top, &right, &bottom);

    /* this really is the lower left hand corner */
    world_x = left;
    world_y = top;

  }

  /* printf("%d %d\n", world_x, world_y); */

  /* first create text item */
  new_obj = o_text_new(color, world_x, world_y,
                       align, angle,            /* zero is angle */
                       w_current->text_size,    /* current text size */
                       visibility,              /* we did not check */
                       show_name_value, text_string);

  /* Attach the new attribute to the object if parent is not NULL */
  /* remember that parent contains the object to get the attribute */
  if (parent) {
    s_object_add_child (parent, new_obj);
  }
  else {
    s_page_append_object (page, new_obj);
  }

  /* handle slot= attribute, it's a special case */
  if (parent != NULL &&
    g_ascii_strncasecmp (text_string, "slot=", 5) == 0) {
    o_slot_end (w_current, parent, text_string);
  }
  else if (parent && parent->selected) {
    o_selection_add (Current_Selection, new_obj);
  }

  /* Call add-objects-hook. */
  g_hook_run_object (w_current, ADD_OBJECT_HOOK, new_obj);
  g_hook_run_object (w_current, SELECT_OBJECTS_HOOK, new_obj);

  return new_obj;
}

/*! \brief Reset Attributes to original positions
 *  \par Function Description
 *  The functions searches for a floating version of the given attribute
 *  and if found, compares the positional data and sets the orientation
 *  of \a attrib to match the floating version if the the positions do
 *  do not match. Essentially, this resets the position to the position
 *  defined in the symbol file if the attribute was inherited.
 *
 *  \returns TRUE if the attribute was modified, otherwise FALSE.
 *
 * TODO: This function assumes there is only attribute with the given
 *       name.
 */
bool o_attrib_reset_position (GschemToplevel *w_current, Object *parent, Object *attrib)
{
  char   *name;
  bool    modified;
  Text   *floater;

  if (o_attrib_get_name_value (attrib, &name,  NULL)) {

    GList *attributes = o_attrib_return_attribs (parent);
    GList *floating   = o_attrib_find_floating_attribs(attributes);
    floater           = (Text*)o_attrib_find_attrib_by_name(floating, name, 0);
    g_list_free (attributes);
    g_list_free (floating);

    if (floater != NULL) {
      Text *attribute = (Text*)attrib;
      if ((attribute->x - floater->x) ||
          (attribute->y - floater->y) ||
          (attribute->angle - floater->angle) ||
          (attribute->alignment - floater->alignment)
         )
      {
        int save_visible = attrib->visibility;
        attrib->visibility = INVISIBLE;
        o_invalidate_force (w_current, attrib); /* Erase from screen at old positon */
        attribute->x = floater->x;
        attribute->y = floater->y;
        attribute->angle = floater->angle;
        attribute->alignment = floater->alignment;
        attrib->visibility = save_visible;
        o_invalidate_object (w_current, attrib);
        modified = TRUE;
      }
      else {
        modified = FALSE;
      }
    }
    else {
      modified = FALSE;
    }
    GEDA_FREE(name);
  }
  else {
    modified = FALSE;
  }
  return modified;
}
