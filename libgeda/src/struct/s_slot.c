/* gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*! \file s_slot.c
 *  \brief utility functions for slotted components
 */

#include <config.h>

#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <math.h>

#include <libgeda_priv.h>

#include <geda_debug.h>

/*! Basic string splitting delimiters */
#define DELIMITERS ",; "


/*! \brief Search for slot attribute.
 *  \par Function Description
 *  Search for slot attribute.
 *
 *  The returned value will only come from an attached attribute.
 *
 *  \param [in] object        Object list to search.
 *  \param [in] return_found  attached slot attribute if found, NULL otherwise.
 *
 *  \returns Character string with attribute value, NULL otherwise.
 *
 *  \warning
 *  Caller must GEDA_FREE returned character string
 */
char *s_slot_search_slot (Object *object, Object **return_found)
{
  GList *attributes;
  Object *attrib;
  char *value = NULL;

  attributes = o_attrib_return_attribs (object);
  attrib = o_attrib_find_attrib_by_name (attributes, "slot", 0);
  g_list_free (attributes);

  if (attrib != NULL)
    o_attrib_get_name_value (attrib, NULL, &value);

  if (return_found)
    *return_found = attrib;

  return value;
}


/*! \brief Search for slotdef attribute.
 *  \par Function Description
 *  Search for slotdef attribute.
 *
 *  \param [in] object      The Object list to search.
 *  \param [in] slotnumber  The slot number to search for.
 *
 *  \returns Character string with attribute value, NULL otherwise.
 *
 *  \warning
 *  Caller must GEDA_FREE returned character string.
 */
static char *s_slot_search_slotdef (Object *object, int slotnumber)
{
  int counter = 0;
  char *slotdef;
  char *search_for;

  search_for = u_string_sprintf ("%d:", slotnumber);

  while (1) {
    slotdef = o_attrib_search_object_attribs_by_name (object, "slotdef",
                                                      counter++);
    if (slotdef == NULL ||
        strncmp (slotdef, search_for, strlen (search_for)) == 0)
      break;

    GEDA_FREE (slotdef);
  }

  GEDA_FREE (search_for);
  return slotdef;
}

/*! \brief Update all slot attributes in an object.
 *
 *  \par Function Description
 *  Update pinnumber attributes in a graphic object. The interesting
 *  case is where the object is an instantiation of a slotted part.
 *  This means that s_slot_update_object iterates through all pins
 *  found on object and sets the pinnumber= attrib on each. This
 *  doesn't matter for non-slotted parts, but on slotted parts,
 *  this is what sets the pinnumber= attribute on slots 2, 3, 4....
 *
 *  \param [in,out] object    The Object to update.
 *
 *  TODO: If old symbol has slot def and the slot has been set, and
 *  replacement does not then "Did not find slotdef" error message
 *  below shows up in the log. if this occurs, then should check if
 *  "numslot=0", in which case the call should remove "slotdef"
 *   attribute.
 */
void s_slot_update_object (Object *object)
{
  Object *o_pin_object;
  Object *o_pinnum_attrib;
  GList  *attributes;

  char *string;
  char *slotdef;
  char *pinseq;
  char *current_pin;  /* text from slotdef= to be made into pinnumber= */
  char *cptr;         /* char pointer pointing to pinnumbers in slotdef=#:#,#,# string */

  int slot;
  int slot_string;
  int pin_counter;    /* Internal pin counter private to this fcn. */


  /* For this particular graphic object (component instantiation) */
  /* get the slot number as a string */
  string = o_attrib_search_object_attribs_by_name (object, "slot", 0);

  if (string == NULL) {
    /* Did not find slot= attribute.
     * This happens if there is no attached slot attribute, and
     * there is no default slot= attribute inside the symbol.
     * Assume slot=1.
     */
    slot = 1;
    slot_string = 0;
  }
  else {
    slot_string = 1;
    slot = atoi (string);
    GEDA_FREE (string);
  }

  /* OK, now that we have the slot number, use it to get the */
  /* corresponding slotdef=#:#,#,# string.  */
  slotdef = s_slot_search_slotdef (object, slot);

  if (slotdef == NULL) {

    if (slot_string) /* only an error if there's a slot string */

      /* Note: slot_string is base Zero, tell user about missing slotdef */
      u_log_message (_("Did not find slotdef=#:#,#,# attribute for slot <%d>\n"),
                        slot_string + 1);
    return;
  }

  if (!strstr (slotdef, ":")) {
    /* Didn't find proper slotdef=#:... put warning into log */
    u_log_message (_("Improper slotdef syntax: missing \":\".\n"));
    GEDA_FREE (slotdef);
    return;
  }

  /* skip over slotdef number */
  /* slotdef is in the form #:#,#,# */
  /* this code skips first #: */
  cptr = slotdef;
  while (*cptr != '\0' && *cptr != ':') {
    cptr++;
  }
  cptr++; /* skip colon */

  if (*cptr == '\0') {
    u_log_message (_("Did not find proper slotdef=#:#,#,#... attribute\n"));
    GEDA_FREE (slotdef);
    return;
  }

  /* loop on all pins found in slotdef= attribute */
  pin_counter = 1;  /* internal pin_counter */

  /* get current pinnumber= from slotdef= attrib */
  current_pin = strtok (cptr, DELIMITERS);

  while (current_pin != NULL) {

    /* get pin on this component with pinseq == pin_counter */
    pinseq = u_string_sprintf ("%d", pin_counter);
    o_pin_object = o_complex_find_pin_by_attribute (object, "pinseq", pinseq);
    GEDA_FREE (pinseq);

    if (o_pin_object != NULL) {

      /* Now rename pinnumber= attrib on this part with value found */
      /* in slotdef attribute  */
      attributes = o_attrib_return_attribs (o_pin_object);
      o_pinnum_attrib = o_attrib_find_attrib_by_name (attributes, "pinnumber", 0);
      g_list_free (attributes);

      if (o_pinnum_attrib != NULL) {
        char *tmp_str = u_string_sprintf ("pinnumber=%s", current_pin);
        o_text_set_string (o_pinnum_attrib, tmp_str);
        GEDA_FREE(tmp_str);
      }

      pin_counter++;
    }
    else {
      u_log_message (_("component missing pinseq= attribute\n"));
    }

    current_pin = strtok (NULL, DELIMITERS);
  }

  GEDA_FREE (slotdef);
}
