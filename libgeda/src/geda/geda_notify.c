/* -*- geda_notify_list.c -*-
 *
 * gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 *
 * Copyright (C) 2014 gEDA Contributors (see ChangeLog for details)
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
 * The GedaNotifyList object was inspired by Peter Clifton's GList
 * wrapper, GedaList, and his code was used as the basis for the
 * GedaNotifyList module.
 *
 * Contributing Author: Wiley Edward Hill
 * Date Contributed: November, 18, 2013
 */

/*! \file geda_notify.c
 *  \brief list derived from GList with GObject properties
 *
 *  GedaNotifyList provides a GOBJECT wrapper for GLIST simular to
 *  a GedaList but without the change signal. The list is intented
 *  hold change_notify_entry structures that are free when the
 *  the GedaNotifyList is destroyed, more importantly, it is not
 *  destroyed until all referances are removed, this property is
 *  allows the list to survive an Undo operation of the Page to
 *  which the GedaNotifyList is assigned.
 */
/** \defgroup geda-notify-list-object Geda Notify List
 *  @{
 */

#include <config.h>

#include <glib-object.h>
#include "libgeda_priv.h"

static GObjectClass *geda_notify_list_parent_class = NULL;


/*! \brief GedaType instance initialiser for GedaNotifyList
 *
 *  GedaType Function Description
 *  GedaType instance initialiser for GedaNotifyList.
 *
 *  \param [in]  instance  The GedaNotifyList we are initialising.
 *  \param [in]  class     The class of the type the instance is created for.
 */
static void geda_notify_list_instance_init (GTypeInstance *instance, void *class)
{
  GedaNotifyList *list = (GedaNotifyList *)instance;

  /* Strictly un-necessary, as the memory is zero'd after allocation */
  list->glist = NULL;
}


/*! \brief GObject finalise handler
 *
 *  \par Function Description
 *  Called before the GedaNotifyList GObject is finalized, free our
 *  allocated data, and then chain up to the parent's finalize handler.
 *
 *  \param [in] object  The GObject being finalized.
 */
static void geda_notify_list_finalize (GObject *object)
{
  GedaNotifyList *list = GEDA_NOTIFY_LIST( object );
  g_list_free_full (list->glist, g_free);
  list->glist = NULL;
  G_OBJECT_CLASS( geda_notify_list_parent_class )->finalize (object);
}


/*! \brief GedaType class initialiser for GedaNotifyList
 *
 *  \par Function Description
 *  GedaType class initialiser for GedaNotifyList. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  class       The GedaNotifyList we are initialising
 *  \param [in]  class_data  (unused)
 */
static void geda_notify_list_class_init(void *class, void *class_data)
{
  GedaNotifyListClass *klass    = GEDA_NOTIFY_LIST_CLASS (class);
  GObjectClass *gobject_class   = G_OBJECT_CLASS (klass);

  geda_notify_list_parent_class = g_type_class_peek_parent (klass);

  gobject_class->finalize       = geda_notify_list_finalize;

}


/*! \brief Function to retrieve GedaNotifyList's Type identifier.
 *
 *  \par Function Description
 *  Function to retrieve GedaNotifyList's Type identifier. On the first call,
 *  this registers the pagesel in the GedaType system. Subsequently
 *  the functions returns the saved value from its first execution..
 *
 *  \return the Type identifier associated with GedaNotifyList.
 */
GedaType geda_notify_list_get_type (void)
{
  static GedaType type = 0;
  if (type == 0) {
    static const GTypeInfo info = {
      sizeof (GedaNotifyListClass),
      NULL,                            /* base_init */
      NULL,                            /* base_finalize */
      geda_notify_list_class_init,     /* class_init */
      NULL,                            /* class_finalize */
      NULL,                            /* class_data */
      sizeof (GedaNotifyList),
      0,                               /* n_preallocs */
      geda_notify_list_instance_init   /* instance_init */
    };
    type = g_type_register_static (G_TYPE_OBJECT, "GedaNotifyList", &info, 0);
  }
  return type;
}


/*! \brief Returns a pointer to a new GedaNotifyList object.
 *
 *  \par Function Description
 *  Returns a pointer to a new GedaNotifyList object.
 *
 *  \return pointer to the new GedaNotifyList object.
 */
GedaNotifyList *geda_notify_list_new (void) {
  return g_object_new( GEDA_TYPE_LIST, NULL );
}


/*! \brief Adds the given item to the GedaNotifyList
 *
 *  \par Function Description
 *  Adds the given item to the GedaNotifyList
 *
 *  \param [in] list Pointer to the GedaNotifyList
 *  \param [in] item item to add to the GedaNotifyList.
 */
void geda_notify_list_add (GedaNotifyList *list, void *item)
{
  list->glist = g_list_append (list->glist, item);
}


/*! \brief Adds the given glist of items to the GedaNotifyList
 *
 *  \par Function Description
 *  Adds the given glist of items to the GedaNotifyList
 *  A copy is made, so the original GList is not modified.
 *
 *  \param [in] list Pointer to the GedaNotifyList
 *  \param [in] items GList of items to add to the GedaNotifyList.
 */
void geda_notify_list_add_glist (GedaNotifyList *list, GList *items)
{
  GList *glist_copy = g_list_copy (items);
  list->glist = g_list_concat (list->glist, glist_copy);
}

/*! \brief Returns a copy of the glist associated with GedaNotifyList
 *
 *  \par Function Description
 *  A copy is made of the glist and returned
 *
 *  \param [in] list Pointer to the GedaNotifyList
 *
 *  \retval pointer to copy of a Glist  the GedaNotifyList.
 */
GList *geda_notify_list_copy_glist (GedaNotifyList *list)
{
  return g_list_copy (list->glist);
}

int geda_notify_list_in_list (GedaNotifyList *list, void *func)
{
  change_notify *entry;
  int     count;
  int     index;
  bool    answer;

  answer = FALSE;

  if (GEDA_IS_NOTIFY_LIST(list)) {

    count = g_list_length(list->glist);

    for ( index = 0; index < count; index++) {

      entry = (change_notify *)g_list_nth_data(list->glist, index);

      if (entry->pre_change_func == func) {
        answer = TRUE;
        break;
      }
    }
  }
  return answer;
}
/*! \brief Removes the given item from the GedaNotifyList
 *
 *  \par Function Description
 *  Removes the given item from the GedaNotifyList.
 *  It's ok to call this function with an item which
 *  is not necessarily in the list.
 *
 *  \param [in] list Pointer to the GedaNotifyList
 *  \param [in] item to remove from the GedaNotifyList.
 */
void geda_notify_list_remove (GedaNotifyList *list, void *item)
{
  if (g_list_find (list->glist, item) == NULL)
    return;

  list->glist = g_list_remove (list->glist, item);
}


/*! \brief Removes all the items in the given GedaNotifyList.
 *
 *  \par Function Description
 *  Removes all items in the given GedaNotifyList.
 *
 *  \param [in] list Pointer to the GedaNotifyList
 */
void geda_notify_list_remove_all (GedaNotifyList *list)
{
  g_list_free_full (list->glist, g_free);

  list->glist = NULL;
}

/** @} endgroup geda-notify-list-object */