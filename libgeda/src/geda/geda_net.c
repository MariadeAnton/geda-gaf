/* -*- geda_net.c -*-
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

G_DEFINE_TYPE (Net, geda_net, GEDA_TYPE_LINE);

/*! \brief GType instance initialiser for Net
 *
 *  \par Function Description
 *  GType instance initialiser for Net, initializes a new empty
 *  Net object by setting pointers to NULL and numbers to zero,
 *  the net PID variable is set to the next net index.
 *
 *  \param [in]  net The Net instance being initialising.
 */
static void geda_net_init(Net *net)
{
  Line   *line      = &net->parent_instance;
  Object *object    = &line->parent_instance;

  net->nid                   = -1;
  net->net_name_has_priority = FALSE;

  net->net_name              = NULL;
  net->pin_label             = NULL;

  net->connected_to          = NULL;

  /* Tracking total number of entities connected by this net */
  net->net_num_connected     = 0;
  net->valid_num_connected   = FALSE;

  net->prev                  = NULL;
  net->next                  = NULL;

  object->net                = net;

  net->line_width            = &line->line_options.line_width;

  net->head_marker           = GEDA_TYPE_NET;
  net->tail_marker           = net->head_marker;
}

static void
geda_net_dispose(GObject *object)
{
  G_OBJECT_CLASS(geda_net_parent_class)->dispose(object);

}

/*! \brief Geda Net Object Finalization Function
 *  \par Function Description
 *   This function removes or releases all internal references
 *   and releases the memory allocated to the given Net
 *   data structure and then chain up to the parent's finalize
 *   handler.
 */
static void geda_net_finalize(GObject *object)
{
  Net *net = GEDA_NET(object);

  if(net->net_name)
    g_free(net->net_name);

  if(net->pin_label)
    g_free(net->pin_label);

  if(net->connected_to)
    g_free(net->connected_to);

  GEDA_LINE_CLASS( geda_net_parent_class )->finalize(object);
}

/*! \brief GType class initialiser for Net
 *
 *  \par Function Description
 *  GType class initialiser for Net. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  class       The Net we are initialising
 */
static void geda_net_class_init(NetClass *class)
{
  GObjectClass *gobject_class  = G_OBJECT_CLASS( class );

  geda_net_parent_class        = g_type_class_peek_parent( class );

  gobject_class->dispose       = geda_net_dispose;
  gobject_class->finalize      = geda_net_finalize;

}

/*! \brief Returns a pointer to a new Net object.
 *
 *  \par Function Description
 *  Returns a pointer to a new Net object.
 *
 *  \return pointer to the new Net object.
 */
Object *geda_net_new (void)
{
  Object *net = g_object_new( GEDA_TYPE_NET,
                             "type", OBJ_NET,
                             "name", "net",
                              NULL );
  return GEDA_OBJECT(net);
}

/*! \brief Determine if object is a Geda Net Object.
 *
 *  \par Function Description
 *  Returns true if the argument is a Geda Net object.
 *
 *  \return boolean.
 */
bool is_a_geda_net_object (Net *net)
{
  return net && (GEDA_TYPE_NET == (net->head_marker & net->tail_marker));
}