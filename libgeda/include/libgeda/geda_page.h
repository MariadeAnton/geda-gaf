/* C header                                           -*- geda_page.h -*-
 * file: geda_page.h
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
 *  Date Contributed: November, 4, 2013
 */
#ifndef __GEDA_PAGE_H__
#define __GEDA_PAGE_H__

#define IS_ACTIVE_PAGE(page) page == page->toplevel->page_current

#define GEDA_TYPE_PAGE            (geda_page_get_type())
#define GEDA_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GEDA_TYPE_PAGE, Page))
#define GEDA_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GEDA_TYPE_PAGE, PageClass))
#define GEDA_IS_PAGE(obj)         (is_a_geda_page(obj))
#define GEDA_IS_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GEDA_TYPE_PAGE))
#define GEDA_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GEDA_TYPE_PAGE, PageClass))

G_BEGIN_DECLS

typedef struct _GedaPageClass PageClass;

struct _GedaPageClass {
  GObjectClass parent;
};

struct _GedaPage {

  GObject parent;

  unsigned int head_marker;            /* structure type signature */

  GedaToplevel *toplevel;

  int pid;

  GList      *_object_list;            /* Glist of all Object on this page*/
  SELECTION  *selection_list;          /* new selection mechanism */
  GList      *place_list;
  Object     *object_lastplace;        /* the last found item */

  GList* major_changed_refdes;         /* A list of all refdes's that have */
                                       /* major symbol version changes */
  char       *filename;
  int         CHANGED;                 /* changed flag */

  int left, right, top, bottom;        /* World coord limits */
  int width, height;                   /* Global height, width of page */

  double coord_aspectratio;            /* Real worldcoords ratio (?) */

  float to_screen_x_constant;
  float to_screen_y_constant;

  float to_world_x_constant;
  float to_world_y_constant;

  TILE world_tiles[MAX_TILES_X][MAX_TILES_Y];

  /* Undo/Redo Stacks and pointers */
  /* needs to go into page mechanism actually */
  UNDO *undo_bottom;
  UNDO *undo_current;
  UNDO *undo_tos;                      /* Top Of Stack */

  /* For hierarchy */
  int up;                    /* pid of the parent page */

  /* used to control which pages are viewable when moving around */
  int page_control; /* WEH sounds hokey */

  /* Flag to indicate if hidden text should be displayed */
  int show_hidden_text;

  /* backup variables */
  GTimeVal last_load_or_save_time;
  char saved_since_first_loaded;
  int  ops_since_last_backup;
  char do_autosave_backup;

  /* Callback function for calculating text bounds */
  RenderedBoundsFunc rendered_text_bounds_func;
  void *rendered_text_bounds_data;

  /* Callback functions for object change notification */
  GList *change_notify_funcs;

  /* Callback functions for object attribute change notification */
  GList *attribs_changed_hooks;

  /* Callback functions for object connections change notification */
  GList *conns_changed_hooks;

  GList   *weak_refs;             /* Weak references */

  unsigned int tail_marker;       /* structure type signature */
};

unsigned int geda_page_get_type             (void);
bool      is_a_geda_page                 (Page *page);

Page     *geda_page_new                  (void);
void      geda_page_weakref_notify       (Page *page);
void      geda_page_weak_ref             (Page *page, WeakNotifyFunc notify_func, void *user_data);
void      geda_page_weak_unref           (Page *page, WeakNotifyFunc notify_func, void *user_data);
void      geda_page_add_weak_ptr         (Page *page, void *weak_pointer_loc);
void      geda_page_remove_weak_ptr      (Page *page, void *weak_pointer_loc);

Page     *geda_page_open                 (const char *filename);
int       geda_page_rename               (Page *page);
int       geda_page_save                 (Page *page);
void      geda_page_close                (Page *page);
int       geda_page_copy                 (Page *page);

void      geda_page_add_object           (Page *page, Object *object);
Object   *geda_page_get_object           (Page *page, int sid);
void      geda_page_remove_object        (Page *page, Object *object);

G_END_DECLS
#endif /* __GEDA_PAGE_H__ */