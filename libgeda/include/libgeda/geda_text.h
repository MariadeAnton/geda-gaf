/* C header                                           -*- geda_text.h -*-
 * file: geda_text.h
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
#ifndef __GEDA_TEXT_H__
#define __GEDA_TEXT_H__

#define GEDA_TYPE_TEXT            (geda_text_get_type())
#define GEDA_TEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GEDA_TYPE_TEXT, Text))
#define GEDA_TEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GEDA_TYPE_TEXT, TextClass))
#define GEDA_IS_TEXT(obj)         (is_a_geda_text_object((Text*)obj))
#define GEDA_IS_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GEDA_TYPE_TEXT))
#define GEDA_TEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GEDA_TYPE_TEXT, TextClass))

G_BEGIN_DECLS

typedef struct _GedaTextClass TextClass;

struct _GedaTextClass {
  ObjectClass parent_class;
};

struct _GedaText {

  Object parent_instance;

  unsigned int head_marker;  /* Marker to identify this structure type */

  int x;                     /* world origin */
  int y;

  char *string;              /* text stuff */
  char *disp_string;
  int   length;
  int   size;
  int   alignment;
  int   angle;

  int    font_text_size;     /* used only with fonts defs */
  GList *font_prim_objs;     /* used only with fonts defs */

  /* Callback function for calculating text bounds */
  RenderedBoundsFunc rendered_text_bounds_func;
  void *rendered_text_bounds_data;

  unsigned int tail_marker;  /* structure type signature */
};

unsigned int geda_text_get_type        (void);
bool    is_a_geda_text_object     (Text *object);

Object *geda_text_new             (void);

G_END_DECLS
#endif /* __GEDA_TEXT_H__ */