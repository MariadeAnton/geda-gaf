/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2 tab-width: 4 -*- */
/*
 * File: geda_picture.h
 *
 * gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 *
 * Copyright (C) 2013-2015 Wiley Edward Hill
 * Copyright (C) 2013-2015 gEDA Contributors (see ChangeLog for details)
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
 * 02110-1301 USA, <http://www.gnu.org/licenses/>.
 *
 *  Contributing Author: Wiley Edward Hill
 *  Date Contributed: November, 18, 2013
 */
/*! \class Picture geda_picture.h "libgeda/geda_picture.h"
 *  \brief GedaType for GedaPicture Objects.
 *
 *  GedaPicture is a derivative of the GedaObject class specialized
 *  for representation and manipulation of image data.
 */
#ifndef __GEDA_PICTURE_H__
#define __GEDA_PICTURE_H__

#define GEDA_TYPE_PICTURE            (geda_picture_get_type())
#define GEDA_PICTURE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GEDA_TYPE_PICTURE, Picture))
#define GEDA_PICTURE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GEDA_TYPE_PICTURE, PictureClass))
#define GEDA_IS_PICTURE(obj)         (is_a_geda_picture_object((Picture*)obj))
#define GEDA_IS_PICTURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GEDA_TYPE_PICTURE))
#define GEDA_PICTURE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GEDA_TYPE_PICTURE, PictureClass))

#ifndef GDK_PIXBUF_H
#define GdkPixbuf void
#endif

BEGIN_DECLS

typedef struct _GedaPictureClass PictureClass;

struct _GedaPictureClass {
  ObjectClass parent_class;
};

struct _GedaPicture {

  Object parent_instance;

  unsigned int head_marker;       /* structure type signature */

  GdkPixbuf     *pixbuf;
  char          *file_content;
  unsigned int   file_length;

  double ratio;
  char  *filename;
  int    angle;
  bool   mirrored;
  bool   is_embedded;             /* is picture component? */;
  bool   missing;                 /* flag to indicate missing file */

  /* upper is considered the origin, world units */
  int    upper_x;
  int    upper_y;
  int    lower_x;
  int    lower_y;

  unsigned int tail_marker;       /* structure type signature */
};

GedaType     geda_picture_get_type     (void) GEDA_CONST;
bool         is_a_geda_picture_object  (Picture *object);

Object      *geda_picture_new          (void);

END_DECLS

#ifndef GDK_PIXBUF_H
#undef GdkPixbuf
#endif
#endif /* __GEDA_PICTURE_H__ */
