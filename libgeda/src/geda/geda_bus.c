/* -*- geda_bus.c -*-
 *
 * gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 *
 * Copyright (C) 2013-2014 Ales Hvezda
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

#include "libgeda_priv.h"

G_DEFINE_TYPE (Bus, geda_bus, GEDA_TYPE_LINE);

/*! \brief Type instance initialiser for Bus
 *
 *  \par Function Description
 *  Type instance initialiser for Bus, initializes a new empty
 *  Bus object by setting pointers to NULL and numbers to zero,
 *  the bus PID variable is set to the next bus index.
 *
 *  \param [in]  bus The Bus instance being initialising.
 */
static void geda_bus_init(Bus *bus)
{
  Line   *line               = &bus->parent_instance;
  Object *object             = &line->parent_instance;

  bus->bus_ripper_direction  = 0;

  bus->bus_name              = NULL;

  object->bus                = bus;
  bus->line_width            = &line->line_options.line_width;

  bus->head_marker           = GEDA_TYPE_BUS;
  bus->tail_marker           = bus->head_marker;
}

static void
geda_bus_dispose(GObject *object)
{

  G_OBJECT_CLASS(geda_bus_parent_class)->dispose(object);

}

/*! \brief Geda Bus Object Finalization Function
 *  \par Function Description
 *   This function removes or releases all internal references
 *   and releases the memory allocated to the given Bus
 *   data structure and then chain up to the parent's finalize
 *   handler.
 */
static void geda_bus_finalize(GObject *object)
{
  Bus *bus = GEDA_BUS(object);

  if (bus->bus_name)
    GEDA_FREE(bus->bus_name);

  GEDA_LINE_CLASS( geda_bus_parent_class )->finalize(object);
}

/*! \brief Type class initialiser for Bus
 *
 *  \par Function Description
 *  Type class initialiser for Bus. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  class       The Bus we are initialising
 */
static void geda_bus_class_init(BusClass *class)
{
  GObjectClass *gobject_class  = G_OBJECT_CLASS( class );

  geda_bus_parent_class        = g_type_class_peek_parent( class );

  gobject_class->dispose       = geda_bus_dispose;
  gobject_class->finalize      = geda_bus_finalize;

}

/*! \brief Returns a pointer to a new Bus object.
 *
 *  \par Function Description
 *  Returns a pointer to a new Bus object.
 *
 *  \return pointer to the new Bus object.
 */
Object *geda_bus_new (void)
{
  Object *bus = g_object_new( GEDA_TYPE_BUS,
                             "type", OBJ_BUS,
                             "name", "bus",
                              NULL );
  return GEDA_OBJECT(bus);
}

/*! \brief Determine if object is a Geda Bus Object.
 *
 *  \par Function Description
 *  Returns true if the argument is a Geda Bus object.
 *
 *  \return boolean.
 */
bool is_a_geda_bus_object (Bus *bus)
{
  return bus && (GEDA_TYPE_BUS == (bus->head_marker & bus->tail_marker));
}