/* -*- geda_pin.c -*-
 *
 * gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 *
 * Copyright (C) 2013-2014 Wiley Edward Hill
 *
 * Copyright (C) 2013-2014 gEDA Contributors (see ChangeLog for details)
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
 *  Contributing Author: Wiley Edward Hill
 *  Date Contributed: November, 18, 2013
 */
#include <config.h>
#include <ctype.h>

#include "libgeda_priv.h"

G_DEFINE_TYPE (Pin, geda_pin, GEDA_TYPE_LINE);

enum {
  PROP_0,
  PROP_ELECTRICAL,
  PROP_LABEL,
  PROP_MECHANICAL,
  PROP_NUMBER,
  PROP_SEQUENCE,
  PROP_TYPE,         /* Obsolete, used to sort to new */
  PROP_WHICHEND
};

static char *e_strings[] = { "in",  "out", "io",  "oc", "oe", "pas", "tp",
                             "tri", "clk", "pwr", NULL };

static char *m_strings[] = { "lead", "body",  "pad", "bump",
                             "ball", "wedge", "ribbon", NULL };

PIN_ELECT geda_pin_lookup_etype(const char *e_str) {

  PIN_ELECT index;
  for (index = PIN_ELECT_IN; e_strings[index] != NULL; index++) {
    if (strcmp(e_str, e_strings[index]) == 0)
      break;
  }
  if(e_strings[index] == NULL ) index = PIN_ELECT_VOID;
  return index;
}

const char *geda_pin_lookup_estring(PIN_ELECT e_type) {

  char *str   = NULL;
  int   index = (int)e_type;

  if (index > -1 && index <= PIN_ELECT_VOID) {
    str = e_strings[index];
  }

  return str;
}
PIN_MECH geda_pin_lookup_mtype(const char *m_str) {

  PIN_MECH index;
  for (index = PIN_MECH_LEAD; m_strings[index] != NULL; index++) {
    if (strcmp(m_str, m_strings[index]) == 0)
      break;
  }
  if(m_strings[index] == NULL ) index = PIN_MECH_VOID;
  return index;
}

const char *geda_pin_lookup_mstring(PIN_MECH m_type) {

  char *str   = NULL;
  int   index = (int)m_type;

  if (index > -1 && index < PIN_MECH_VOID + 1) {
    str = m_strings[index];
  }

  return str;
}

/*! \brief GType instance initialiser for Pin
 *
 *  \par Function Description
 *  GType instance initialiser for Pin, initializes a new empty
 *  Pin object by setting pointers to NULL and numbers to zero,
 *  the pin PID variable is set to the next pin index.
 *
 *  \param [in]  pin  The Pin instance being initialising.
 */
static void geda_pin_init(Pin *pin)
{
  Line   *line      = &pin->parent_instance;
  Object *object    = &line->parent_instance;

  pin->number       = -1;
  pin->whichend     = 0;    /* either 0 or 1 */

  pin->elect_type   = PIN_ELECT_VOID; /* electrical type code */
  pin->mech_type    = PIN_MECH_VOID;  /* mechanical type code */
  pin->node_type    = PIN_NET_NODE;   /* either NET or BUS */

  pin->label        = NULL; /* Pointer to label string */
  pin->electrical   = NULL; /* Pointer to electrical string */
  pin->mechanical   = NULL; /* Pointer to mechanical string */

  object->pin       = pin;

  pin->line_width   = &line->line_options.line_width;

  line->line_options.line_width   = default_thick_pin_width;

  pin->head_marker                = GEDA_TYPE_PIN;
  pin->tail_marker                = pin->head_marker;
}

static void
geda_pin_set_property (GObject *object,     unsigned int  prop_id,
                       const GValue *value, GParamSpec   *pspec)
{
  Pin *pin = GEDA_PIN (object);

  switch (prop_id)
  {
    case PROP_ELECTRICAL:
      geda_pin_set_electrical (pin, g_value_get_string (value));
      break;
    case PROP_LABEL:
      geda_pin_set_label (pin, g_value_get_string (value));
      break;
    case PROP_MECHANICAL:
      geda_pin_set_mechanical (pin, g_value_get_string (value));
      break;
    case PROP_NUMBER:
      geda_pin_set_number (pin, g_value_get_string (value));
      break;
    case PROP_SEQUENCE:
      geda_pin_set_sequence (pin, g_value_get_string (value));
      break;
    case PROP_TYPE:
      geda_pin_set_electrical (pin, g_value_get_string (value));
      break;
    case PROP_WHICHEND:
      pin->whichend = g_value_get_int (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
geda_pin_get_property (GObject *object, unsigned int  prop_id,
                       GValue  *value,  GParamSpec   *pspec)
{
  Pin *pin = GEDA_PIN (object);
  char s_val[4];

  switch (prop_id)
    {
    case PROP_ELECTRICAL:
      g_value_set_string (value, pin->electrical);
      break;

    case PROP_LABEL:
      g_value_set_string (value, pin->label);
      break;

    case PROP_MECHANICAL:
      g_value_set_string (value, pin->mechanical);
      break;

    case PROP_NUMBER:
      g_value_set_string (value, int2str( pin->number, s_val, 10 ));
      break;

    case PROP_SEQUENCE:
      g_value_set_string (value, int2str( pin->sequence, s_val, 10 ));
      break;

    case PROP_TYPE:
      g_value_set_string (value, pin->electrical);
      break;

    case PROP_WHICHEND:
      g_value_set_int (value, pin->whichend);
    default:

      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
geda_pin_dispose(GObject *object)
{
  G_OBJECT_CLASS(geda_pin_parent_class)->dispose(object);
}

/*! \brief Geda Pin Object Finalization Function
 *  \par Function Description
 *   This function removes or releases all internal references
 *   and releases the memory allocated to the given Pin
 *   data structure and then chain up to the parent's finalize
 *   handler.
 */
static void geda_pin_finalize(GObject *object)
{
  Pin *pin = GEDA_PIN(object);
  if (pin->electrical) {
    GEDA_FREE(pin->electrical);
    pin->electrical = NULL;
  }
  if (pin->mechanical) {
    GEDA_FREE(pin->mechanical);
    pin->mechanical = NULL;
  }
  GEDA_FREE(pin->label);

  GEDA_LINE_CLASS( geda_pin_parent_class )->finalize(object);
}

/*! \brief GType class initialiser for Pin
 *
 *  \par Function Description
 *  GType class initialiser for Pin. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  class       The Pin we are initialising
 */
static void geda_pin_class_init(PinClass *class)
{
  GParamSpec *params;

  GObjectClass *gobject_class  = G_OBJECT_CLASS( class );

  geda_pin_parent_class        = g_type_class_peek_parent( class );

  gobject_class->set_property  = geda_pin_set_property;
  gobject_class->get_property  = geda_pin_get_property;

  gobject_class->dispose       = geda_pin_dispose;
  gobject_class->finalize      = geda_pin_finalize;

  params = g_param_spec_string ("electrical", _("Electrical"),
                              _("Electrical Properties of the pin"),
                                "",
                                G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_ELECTRICAL, params);

  params = g_param_spec_string ("label", _("Label"),
                              _("A Label for the pin"),
                                "",
                                G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_LABEL, params);

  params = g_param_spec_string ("number", _("Number"),
                              _("The pin number"),
                                "",
                                G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_NUMBER, params);

  params = g_param_spec_string ("mechanical", _("Mechanical"),
                              _("Mechanical Properties of the pin"),
                                "",
                                G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_MECHANICAL, params);

  params = g_param_spec_string ("sequence", _("Sequence"),
                              _("The pin sequence"),
                              "",
                              G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_SEQUENCE, params);

  params = g_param_spec_string ("pin-type", _("Type"),
                              _("Either Mechanical or Electrical Properties of the pin"),
                                "",
                                G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_TYPE, params);

  params = g_param_spec_int ("whichend", _("Whichend"),
                              _("Which end of the pin gets connected"),
                              -1,
                               1,
                              -1,
                              G_PARAM_READWRITE);

  g_object_class_install_property (gobject_class, PROP_WHICHEND, params);

}

/*! \brief Returns a pointer to a new Pin object.
 *
 *  \par Function Description
 *  Returns a pointer to a new Pin object.
 *
 *  \return pointer to the new Pin object.
 */
Object *geda_pin_new (void)
{
  Object *pin = g_object_new( GEDA_TYPE_PIN,
                             "type", OBJ_PIN,
                             "name", "pin",
                              NULL );
  return GEDA_OBJECT(pin);
}

/*! \brief Determine if object is a Geda Pin Object.
 *
 *  \par Function Description
 *  Returns true if the argument is a Geda Pin object.
 *
 *  \return boolean.
 */
bool is_a_geda_pin_object (Pin *pin)
{
  return pin && (GEDA_TYPE_PIN == (pin->head_marker & pin->tail_marker));
}

const char*
geda_pin_get_electrical(Pin *pin)
{
  g_return_val_if_fail(GEDA_IS_PIN(pin), NULL);
  return pin->electrical;
}
void geda_pin_set_electrical(Pin *pin, const char *electrical)
{
  g_return_if_fail(GEDA_IS_PIN(pin));
  GEDA_FREE(pin->electrical);
  pin->electrical = g_strdup(electrical);

  /* Check if there is a code associated with the description */
  PIN_ELECT current_type = geda_pin_lookup_etype(electrical);

  /* Update the code if discrepant */
  if (current_type != pin->elect_type)
    current_type = pin->elect_type;
}
const char*
geda_pin_get_label(Pin *pin)
{
  g_return_val_if_fail(GEDA_IS_PIN(pin), NULL);
  return pin->label;
}
void geda_pin_set_label(Pin *pin, const char *label)
{
  g_return_if_fail(GEDA_IS_PIN(pin));
  GEDA_FREE(pin->label);
  pin->label = g_strdup(label);
}
const char*
geda_pin_get_mechanical(Pin *pin)
{
  g_return_val_if_fail(GEDA_IS_PIN(pin), NULL);
  return pin->mechanical;
}
void geda_pin_set_mechanical(Pin *pin, const char *mechanical)
{
  g_return_if_fail(GEDA_IS_PIN(pin));
  GEDA_FREE(pin->mechanical);
  pin->mechanical = g_strdup(mechanical);

  /* Check if there is a code associated with the description */
  PIN_MECH current_type = geda_pin_lookup_mtype(mechanical);

  /* Update the code if discrepant */
  if (current_type != pin->mech_type)
    current_type = pin->mech_type;
}

/* TODO: This does not allow BGA or LGA letter numbers, going to
 * have to fix someday and handle pin number as strings */
void geda_pin_set_number(Pin *pin, const char *number)
{
  const char *ptr = number;

  g_return_if_fail(GEDA_IS_PIN(pin));

  while (*ptr) {
    if(!isdigit(*ptr)) {
      fprintf(stderr, "<geda_pin_set_number>invalid number character[%s]\n", number);
      return;
    }
    ptr++;
  }
  pin->number = atoi(number);
}
void geda_pin_set_sequence(Pin *pin, const char *sequence)
{
  const char *ptr = sequence;

  g_return_if_fail(GEDA_IS_PIN(pin));

  while (*ptr) {
    if(!isdigit(*ptr)) {
      fprintf(stderr, "<geda_pin_set_sequence>invalid sequence character[%s]\n", sequence);
      return;
    }
    ptr++;
  }
  pin->sequence = atoi(sequence);
}
void geda_pin_set_whichend(Pin *pin, int whichend)
{
  g_return_if_fail(GEDA_IS_PIN(pin));
  pin->whichend = whichend;
}