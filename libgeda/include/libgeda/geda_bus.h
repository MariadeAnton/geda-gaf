/* C header                                           -*- geda_bus.h -*-
 * file: geda_bus.h
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
#ifndef __GEDA_BUS_H__
#define __GEDA_BUS_H__

#define GEDA_TYPE_BUS            (geda_bus_get_type())
#define GEDA_BUS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GEDA_TYPE_BUS, Bus))
#define GEDA_BUS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GEDA_TYPE_BUS, BusClass))
#define GEDA_IS_BUS(obj)         (is_a_geda_bus_object((Bus*)obj))
#define GEDA_IS_BUS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GEDA_TYPE_BUS))
#define GEDA_BUS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GEDA_TYPE_BUS, BusClass))

G_BEGIN_DECLS

typedef struct _GedaBusClass BusClass;

struct _GedaBusClass {
  LineClass parent_class;
};

struct _GedaBus {
  Line parent_instance;

  unsigned int head_marker;            /* structure type signature */

  int  *line_width;

  /* controls which direction bus rippers goes, is either 0 for un-inited, */
  /* 1 for right, -1 for left (horizontal bus)  1 for up, -1 for down (vertial bus) */
  int   bus_ripper_direction;

  char *bus_name;

  unsigned int tail_marker;       /* structure type signature */
};

GType    geda_bus_get_type        (void);
bool     is_a_geda_bus_object     (Bus *object);

Object  *geda_bus_new             (void);


G_END_DECLS
#endif /* __GEDA_BUS_H__ */