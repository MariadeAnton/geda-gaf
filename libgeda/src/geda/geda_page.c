/* -*- geda_page.c -*-
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
 *  Date Contributed: November, 4, 2013
 */
#include <config.h>

#include "libgeda_priv.h"

static GObjectClass *geda_page_parent_class = NULL;

static int global_pid = 0; /* Global integer for Page Indentification */

static GList *new_page_hooks = NULL;

typedef struct {
  NewPageFunc func;
  void *data;
} NewPageHook;

typedef struct {
  ConnsChangedFunc func;
  void *data;
} ConnsChangedHook;

/*! \brief Internal Function to Call Register Page Hooks
 *  \par Function Description
 *
 */
static void call_new_page_hook (void *hook, void *page)
{
  NewPageHook *h = (NewPageHook*) hook;
  Page *p = (Page*) page;

  h->func (p, h->data);
}

/*! \brief Append New Page Hook List to this Page.
 * \par Function Description
 * Adds a callback hook \a notify_func to \a page. After a new
 * \a page is created, \a notify_func will be called with two
 * arguments: the \a page, and the \a user_data.
 *
 * \sa page_weak_unref
 *
 * \param [in] func      Page to add the Hook.
 * \param [in] data      Data to be passed to \a notify_func
 *
 */
void geda_page_append_new_hook (NewPageFunc func, void *data)
{
  NewPageHook *new_hook;

  new_hook = g_new0 (NewPageHook, 1);
  new_hook->func = func;
  new_hook->data = data;

  new_page_hooks = g_list_append (new_page_hooks, new_hook);
}

/*! \brief Search for an Object given the sid.
 *  \par Function Description
 *  This functions returns the <b>Object</b> that has the \a sid
 *  from the page object list or NULL if there is no such <b>Object</b>.
 *
 *  \param [in] page    Pointer to a Page.
 *  \param [in] sid     The ID of the Object to find.
 *
 *  \returns A pointer on the object found or NULL if not found.
 */
Object *geda_page_get_object(Page *page, int sid)
{
  const GList *iter;

  g_return_val_if_fail(GEDA_IS_PAGE(page), NULL);

  iter = g_list_first(page->_object_list);
  while (iter != NULL)
  {
    Object *object = (Object *)iter->data;
    if (object->sid == sid) {
      return object;
    }
    NEXT(iter);
  }

  return NULL;
}

/*! \brief Remove an object from a Page
 *
 *  \par Function Description
 *  The function decreases the reference count of object. The object's page
 * must be @page. Increase its reference count prior to calling this function
 *
 * Emits the remove-object signal.
 *
 *  \sa eda_page_remove_object
 *
 *  \param [in] page    The Page from which all objects are to be removed.
 *  \param [in] object  The object to be removed.
 */
void
geda_page_remove_object(Page *page, Object *object)
{
    g_return_if_fail(GEDA_IS_PAGE(page));
    g_return_if_fail(GEDA_IS_OBJECT(object));
    g_return_if_fail(g_list_find(page->_object_list, object) != NULL);

    //g_signal_emit(page, signals[REMOVE_OBJECT], 0, object);
}

/*! \brief Remove all objects from a Page object
 *
 *  \par Function Description
 *  The function derecments the reference for all objects on a Page and then
 *  sets the Page object list to NULL.
 *
 *  \sa eda_page_remove_object
 *
 *  \param [in]  page  The Page from which all objects are to be removed.
 */
void
geda_page_remove_all_objects(Page *page)
{
    GList *iter;
    Object *object;

    g_return_if_fail(GEDA_IS_PAGE(page));

    while ((iter = g_list_last(page->_object_list))) {
      object = GEDA_OBJECT(iter->data);
      if (object) {
        GEDA_UNREF (object);
      }
    }

    page->_object_list = NULL;
}

/*! \brief GType instance initialiser for Page
 *
 *  \par Function Description
 *  GType instance initialiser for Page, initializes a new empty
 *  Page object by setting pointers to NULL and numbers to zero,
 *  the page PID variable is set to the next page index.
 *
 *  \param [in]  instance  The Page being initialising.
 *  \param [in]  g_class   The class of the type the instance is created for.
 */
static void
geda_page_instance_init( GTypeInstance *instance, void *g_class )
{
  Page *page                      = (Page *)instance;
  page->pid                       = global_pid++;

  page->_object_list              = NULL;
  page->selection_list            = NULL;
  page->place_list                = NULL;
  page->object_lastplace          = NULL;

  page->major_changed_refdes      = NULL;

  page->filename                  = NULL;
  page->CHANGED                   = 0;

 /* World coord limits */
  page->left                      = 0;
  page->right                     = DEFAULT_PAGE_WIDTH;
  page->top                       = 0;
  page->bottom                    = DEFAULT_PAGE_HEIGHT;

  page->width                     = page->left - page->right;
  page->height                    = page->top - page->bottom;

  page->page_control              = 0;
  page->show_hidden_text          = 0;

  page->rendered_text_bounds_func = NULL;
  page->rendered_text_bounds_data = NULL;

  page->change_notify_funcs       = NULL;

  page->attribs_changed_hooks     = NULL;
  page->conns_changed_hooks       = NULL;

  page->weak_refs                 = NULL;

  page->head_marker               = GEDA_TYPE_PAGE;
  page->tail_marker               = page->head_marker;

  /* Call hooks */
  g_list_foreach (new_page_hooks, call_new_page_hook, page);
}

static void
geda_page_dispose(GObject *object)
{
  GList *iter;
  Page *page = GEDA_PAGE(object);

  geda_page_remove_all_objects(page);

  if (page->major_changed_refdes) {
    iter = page->major_changed_refdes;
    while (iter) {
      GEDA_FREE(iter->data);
      iter = g_list_next(iter);
    }
    g_list_free(page->major_changed_refdes);
  }

  /* Remove all change notification handlers */
  for (iter = page->change_notify_funcs; iter != NULL; NEXT(iter)) {
    GEDA_FREE (iter->data);
  }
  g_list_free (page->change_notify_funcs);
  page->change_notify_funcs = NULL;

  for (iter = page->attribs_changed_hooks; iter != NULL; NEXT(iter)) {
    GEDA_FREE (iter->data);
  }
  g_list_free (page->attribs_changed_hooks);
  page->attribs_changed_hooks = NULL;

  for (iter = page->conns_changed_hooks; iter != NULL; NEXT(iter)) {
    GEDA_FREE (iter->data);
  }
  g_list_free (page->conns_changed_hooks);
  page->conns_changed_hooks = NULL;
/*
  if (page->weak_refs) {
    g_list_free (page->weak_refs);
  }
*/
  if (page->weak_refs) {

    for (iter = page->weak_refs; iter != NULL; iter = g_list_next (iter)) {
      g_free (iter->data);
    }
    g_list_free (page->weak_refs);
  }
  page->weak_refs = NULL;

  G_OBJECT_CLASS(geda_page_parent_class)->dispose(object);
}

/*! \brief Geda Page Object Finalization Function
 *  \par Function Description
 *   This function removes or releases all internal references
 *   and releases the memory allocated to the given Page
 *   data structure and then chain up to the parent's finalize
 *   handler.
 */
static void geda_page_finalize(GObject *object)
{
  Page *page = GEDA_PAGE(object);

  if (page->filename)
    GEDA_FREE(page->filename);

  G_OBJECT_CLASS( geda_page_parent_class )->finalize(object);
}

/*! \brief GType class initialiser for Page
 *
 *  \par Function Description
 *  GType class initialiser for Page. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  g_class       The Page we are initialising
 *  \param [in]  g_class_data  (unused)
 */
static void geda_page_class_init( void *g_class, void *g_class_data )
{
  PageClass *klass            = GEDA_PAGE_CLASS( g_class );
  GObjectClass *gobject_class = G_OBJECT_CLASS( klass );
  geda_page_parent_class      = g_type_class_peek_parent( klass );

  gobject_class->dispose      = geda_page_dispose;
  gobject_class->finalize     = geda_page_finalize;

}

/*! \brief Function to retrieve Page's GType identifier.
 *
 *  \par Function Description
 *  Function to retrieve Page's GType identifier.
 *  Upon first call, this registers the Page in the GType system.
 *  Subsequently it returns the saved value from its first execution.
 *
 *  \return the GType identifier associated with Page.
 */
unsigned int geda_page_get_type(void)
{
  static unsigned int type = 0;
  if (type == 0) {
    static const GTypeInfo info = {
      sizeof (PageClass),
      NULL,                            /* base_init */
      NULL,                            /* base_finalize */
      geda_page_class_init,            /* class_init */
      NULL,                            /* class_finalize */
      NULL,                            /* class_data */
      sizeof (Page),
      0,                               /* n_preallocs */
      geda_page_instance_init          /* instance_init */
    };
    type = g_type_register_static (G_TYPE_OBJECT, "Page", &info, 0);
  }
  return type;
}

/*! \brief Returns a pointer to a new Page object.
 *
 *  \par Function Description
 *  Returns a pointer to a new Page object.
 *
 *  \return pointer to the new Page object.
 */
Page *geda_page_new (void)
{
  Page *page;
  page = g_object_new( geda_page_get_type(), NULL );
  return page;
}

/*! \brief Return True if object is Geda PageObject.
 *
 *  \par Function Description
 *  Returns true if the argument is a Geda Page object.
 *
 *  \return boolean.
 */
bool is_a_geda_page (Page *page)
{
  return page && (GEDA_TYPE_PAGE == (page->head_marker & page->tail_marker));
}

/*! \todo Finish function documentation!!!
 *  \brief Decrement the GObject Reference Count
 *  \par Function Description
 *
 */
void geda_page_unref(Page *page)
{
  g_return_if_fail (GEDA_IS_PAGE(page));
  g_object_unref ((GObject*)page);
}

/*! \brief Notify weak reference watchers that a structure is dead.
 * \par Function Description
 * For each entry in \a weak_refs, call notify function with the dead
 * pointer \a dead_ptr and the entry's specified user data, and free
 * \a weak_refs. Should be called during destruction of an structure
 * that allows weak references.
 *
 * \param [in] page  Pointer to Page Object being destroyed.
 *
 */
void
geda_page_weakref_notify (Page *page)
{
  unsigned int type  = G_TYPE_FROM_INSTANCE (page);
  if  ( type == geda_page_get_type()) {
    s_weakref_notify(page, page->weak_refs);
    page->weak_refs = NULL;
  }
}

/*! \brief Add a weak reference watcher to a Page Object
 *
 *  \par Function Description
 *
 *   Adds the weak reference callback \a notify_func to \a Page.
 * When \a Page is destroyed, the \a notify_func will be called
 * with two arguments: the \a Page, and the \a user_data.
 *
 * \note This function is for legacy purposes; since Page is
 *       now a GObject, just use g_object_weak_ref instead!
 *
 * \sa page_weak_unref
 *
 * \param [in,out] page       Page  to weak-reference.
 * \param [in] notify_func    Weak reference notify function.
 * \param [in] user_data      Data to be passed to \a notify_func.
 */
void geda_page_weak_ref (Page *page, WeakNotifyFunc notify_func, void *user_data)
{
  unsigned int type  = G_TYPE_FROM_INSTANCE (page);
  if  ( type == geda_page_get_type()) {
    page->weak_refs = s_weakref_add (page->weak_refs, notify_func, user_data);
  }
}

/*! \brief Remove a weak reference watcher from a Page.
 * \par Function Description
 * Removes the weak reference callback \a notify_func from \a Page.
 *
 * \sa page_weak_ref()
 *
 * \warning Do not use g_object_weak_unref instead!
 *
 * \param [in,out] page        Page to remove weak-reference function.
 * \param [in]     notify_func Notify function to search for.
 * \param [in]     user_data   Data to to search for.
 */
void geda_page_weak_unref (Page *page, WeakNotifyFunc notify_func, void *user_data)
{
  unsigned int type  = G_TYPE_FROM_INSTANCE (page);
  if  ( type == geda_page_get_type()) {
    page->weak_refs = s_weakref_remove (page->weak_refs, notify_func, user_data);
  }
}

/*! \brief Add a weak pointer to a Page.
 * \par Function Description
 * Adds the weak pointer at \a weak_pointer_loc to \a page. The
 * value of \a weak_pointer_loc will be set to NULL when \a page is
 * destroyed.
 *
 * \sa page_remove_weak_ptr
 *
 * \param [in,out] page          Page to weak-reference.
 * \param [in] weak_pointer_loc  Memory address of a pointer.
 */
void geda_page_add_weak_ptr (Page *page, void *weak_pointer_loc)
{
  g_return_if_fail (GEDA_IS_PAGE(page));
  g_object_add_weak_pointer ((GObject*)page, weak_pointer_loc);
}

/*! \brief Remove a weak pointer from an Page.
 * \par Function Description
 * Removes the weak pointer at \a weak_pointer_loc from \a page.
 *
 * \sa page_add_weak_ptr()
 *
 * \param [in,out] page          Page to weak-reference.
 * \param [in] weak_pointer_loc  Memory address of a pointer.
 */
void geda_page_remove_weak_ptr (Page *page, void *weak_pointer_loc)
{
  g_return_if_fail (GEDA_IS_PAGE(page));
  g_object_remove_weak_pointer ((GObject*)page, weak_pointer_loc);
}

/* For now, the toplevel should only be set once, we don't have a
 * low-level clone or copy method and the hooks are holding pointer
 * to the toplevel */
void geda_page_set_toplevel (Page *page, GedaToplevel *toplevel)
{
  g_return_if_fail (GEDA_IS_PAGE(page));
  g_return_if_fail (GEDA_IS_TOPLEVEL(toplevel));

  page->toplevel = toplevel;
}

GedaToplevel *geda_page_get_toplevel (Page *page)
{
  g_return_val_if_fail (GEDA_IS_PAGE(page), NULL);
  return page->toplevel;
}