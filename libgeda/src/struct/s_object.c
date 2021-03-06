/* gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 *
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

#include <config.h>

#include <libgeda_priv.h>

/*!
 *  \brief Create an Empty Object
 *  \par Function Description
 *  Create and return an empty <b>Object</b> object with sensible defaults
 *  for its properties.
 *
 *  \returns the newly created Object.
 */
Object *s_object_new (int type, char const *name)
{
  return geda_object_new(type, name);
}

/*! \brief Attach attribute to an Object and append Parent's Page
 *  \par Function Description
 *  This function is similar to o_attrib_add, which is called by
 *  the function, the difference being that this function also
 *  adds the attribute being attached to the page if the parent
 *  is already on a page.
 *
 *  \param [in]  parent The Object that child is being added to.
 *  \param [in]  child  The item you want to add as an attribute.
 *
 *  \return nothing.
 */
void s_object_add_child(Object *parent, Object *child) {

  /* if the object is on a page then add the child */
  Page *page = geda_object_get_page(parent);

  if (page && (GEDA_IS_PAGE(page))) {
    s_page_append_object(page, child);
  }

  o_attrib_add(parent, child);
}

/*! \brief Remove an Object
 *  \par Function Description
 *  This function unreferences an Object after first removing the Object
 *  from a page if the object is on a page, and disconnecting any "elec-
 *  trical" connections.
 */
/*
 * Note: WEH (11/04/13): Modified to add conditional for s_conn_remove_object
 * to else clause of if page member, because the connections would be removed
 * by pre_object_remove if the object was on a page, Also added check for NULL
 * conn_list since there is no point in making the call if no connections exist.
 *
 */
void
s_object_release(Object *o_current)
{
  if (GEDA_IS_OBJECT(o_current)) {

    /* If currently attached to a page, remove it from the page */
    if (GEDA_IS_PAGE(o_current->page)) {
      s_page_remove_object (o_current->page, o_current);
    }
    else if ( o_current->conn_list != NULL ) {
      s_conn_remove_object (o_current);
    }

    if (o_current->attached_to != NULL) {
      /* do the actual remove */
      o_attrib_remove(&o_current->attached_to->attribs, o_current);
    }
    o_attrib_detach_all (o_current);

    if (o_current->complex) {

      if (o_current->complex->prim_objs) {
        s_object_release_objects (o_current->complex->prim_objs);
        o_current->complex->prim_objs = NULL;
      }
    }

    geda_object_weakref_notify(o_current);
    geda_object_unref(o_current);

    o_current = NULL; /* misc clean up */
  }
}

/*! \brief Deletes a list of GedaObjects
 *  \par Function Description
 *   Deletes everything including the GList
 */
void
s_object_release_objects(GList *list)
{
  Object *o_current = NULL;
  GList  *ptr;

  ptr = g_list_last(list);

  /* do the delete backwards */
  while(ptr != NULL) {
    o_current = GEDA_OBJECT(ptr->data);
    s_object_release(o_current);
    ptr = g_list_previous (ptr);
  }

  g_list_free(list);

}

/*! \brief Mark Object's Page as modified
 *  \par Function Description
 *   Updates the CHANGED flag of the page associated with
 *  \a object if the page is attached to a page.
 */
void s_object_set_page_changed (Object *object)
{
  Page *page = geda_object_get_page (object);
  if (page != NULL){
    page->CHANGED = TRUE;
  }
}
