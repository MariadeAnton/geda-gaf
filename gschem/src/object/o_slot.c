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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */
#include <config.h>

#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "gschem.h"

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

#define MAX_SLOT_SIZE 10

/*! \brief Edit Complex Object's Slot Attribute
 *  \par Function Description
 *  This is really a pre-dialog launch for the Edit Slot Dialog.
 *  The function insures the selected object is a complex and
 *  checks for an existing slot attribute. Either the found slot
 *  attribute or a fictitious values is passed to the Edit-Slot
 *  Dialog.
 *
 *  \param w_current pointer to GschemToplevel context
 *  \param object    pointer to a "slotable" Object.
 */
void o_slot_start (GschemToplevel *w_current, Object *object)
{
  char *slot_value;

  /* single object for now */
  if (object->type != OBJ_COMPLEX)
    return;

  slot_value = o_attrib_search_object_attribs_by_name (object, "slot", 0);

  if (slot_value == NULL) {
    /* we didn't find a slot=? attribute, make something up */
    /* for now.. this is an error condition */
    slot_value = g_strdup ("1");
  }

  x_dialog_edit_slot (w_current, slot_value);
  GEDA_FREE (slot_value);
}

/*! \brief Add a Slot Attribute
 *  \par Function Description
 *  This is function could be called x_dialog_edit_slot_response_ok
 *  but it is also called from o_attrib, x_autonumber and o_text.
 *  The function creates a new type "slots" attribute to the drawing
 *  and attaches it to the object
 *
 *  \param w_current pointer to GschemToplevel context
 *  \param object    pointer to Object to receive slot attribute.
 *  \param string    pointer to attribute string.
 *
 */
void o_slot_end(GschemToplevel *w_current, Object *object, const char *string)
{
  GedaToplevel *toplevel = w_current->toplevel;
  Object *new_obj;
  char *slot_value;
  char *numslots_value;
  Object *o_slot;
  char *value = NULL;
  int numslots;
  int new_slot_number;
  int status;

  g_return_if_fail (object != NULL);

  status = o_attrib_string_get_name_value (string, NULL, &value);
  if (!status) {
    u_log_message (_("Slot attribute malformed\n"));
    return;
  }

  numslots_value =
    o_attrib_search_object_attribs_by_name (object, "numslots", 0);

  if (!numslots_value) {
    u_log_message (_("numslots attribute missing\n"));
    u_log_message (_("Slotting not allowed for this component\n"));
    GEDA_FREE (value);
    return;
  }

  numslots = atoi (numslots_value);
  GEDA_FREE (numslots_value);

  new_slot_number = atoi (value);

#if DEBUG
  printf ("numslots = %d\n", numslots);
#endif

  if (new_slot_number > numslots || new_slot_number <=0 ) {
    u_log_message (_("New slot number [%d], is out of range\n"), new_slot_number);
    GEDA_FREE (value);
    return;
  }

  /* first see if slot attribute already exists outside
   * complex */
  slot_value = s_slot_search_slot (object, &o_slot);
  GEDA_FREE (slot_value);

  if (o_slot != NULL && !o_attrib_is_inherited (o_slot)) {
    o_text_set_string (o_slot, string);
  } else {
    /* here you need to do the add the slot
       attribute since it doesn't exist */
    new_obj = o_text_new (ATTRIBUTE_COLOR,
                          object->complex->x, object->complex->y,
                          LOWER_LEFT, 0, /* zero is angle */
                          string, 10, INVISIBLE, SHOW_NAME_VALUE);
    s_page_append_object (toplevel->page_current, new_obj);

    /* manually attach attribute */
    o_attrib_attach (new_obj, object, FALSE);

    /* Call add-objects-hook */
    g_run_hook_object (w_current, "%add-objects-hook", new_obj);
  }

  s_slot_update_object (object);

  toplevel->page_current->CHANGED = 1;
  GEDA_FREE (value);
}