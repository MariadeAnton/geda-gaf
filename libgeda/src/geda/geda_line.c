/* -*- geda_line.c -*-
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

G_DEFINE_TYPE (Line, geda_line, GEDA_TYPE_OBJECT);

/*! \brief calculate and return the boundaries of a pin object
 *
 *  \par Function Description
 *  This function calculates the object boudaries of a pin \a object.
 *
 *  \param [in]  object    a pin object
 */
int
geda_line_bounds(Object *object)
{
  int halfwidth;

  g_return_val_if_fail(GEDA_IS_LINE(object), FALSE);

  halfwidth = object->line_options->line_width / 2;

  /* This isn't strictly correct, but a 1st order approximation */
  object->left   = min( object->line->x[0], object->line->x[1] ) - halfwidth;
  object->top    = min( object->line->y[0], object->line->y[1] ) - halfwidth;
  object->right  = max( object->line->x[0], object->line->x[1] ) + halfwidth;
  object->bottom = max( object->line->y[0], object->line->y[1] ) + halfwidth;

  return TRUE;
}

/*! \brief GType instance initialiser for Line
 *
 *  \par Function Description
 *  GType instance initialiser for Line, initializes a new empty
 *  Line object by setting pointers to NULL and numbers to zero,
 *  the line PID variable is set to the next line index.
 *
 *  \param [in] line  The Line instance being initialising.
 */
static void geda_line_init(Line *line)
{
  Object *object     = &line->parent_instance;

  line->x[0]         = 0;
  line->y[0]         = 0;
  line->x[1]         = 0;
  line->y[1]         = 0;

  line->line_options.line_end     = default_line_end;
  line->line_options.line_type    = default_line_type;
  line->line_options.line_width   = default_line_width;
  line->line_options.line_space   = default_line_space;
  line->line_options.line_length  = default_line_length;

  object->line                    = line;
  object->line_options            = &line->line_options;

  line->head_marker               = GEDA_TYPE_LINE;
  line->tail_marker               = line->head_marker;
}

static void
geda_line_dispose(GObject *object)
{
  G_OBJECT_CLASS(geda_line_parent_class)->dispose(object);
}

/*! \brief Geda Line Object Finalization Function
 *  \par Function Description
 *   This function removes or releases all internal references
 *   and releases the memory allocated to the given Line
 *   data structure and then chain up to the parent's finalize
 *   handler.
 */
static void geda_line_finalize(GObject *object)
{
  /*Object *obj = GEDA_OBJECT(object);*/
  GEDA_OBJECT_CLASS( geda_line_parent_class )->finalize(object);
}

/*! \brief GType class initialiser for Line
 *
 *  \par Function Description
 *  GType class initialiser for Line. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  class       The Line we are initialising
 */
static void geda_line_class_init(LineClass *class)
{
  GObjectClass *gobject_class  = G_OBJECT_CLASS( class );
  ObjectClass  *object_class   = GEDA_OBJECT_CLASS( class );

  geda_line_parent_class       = g_type_class_peek_parent( class );

  class->finalize              = geda_line_finalize;

  gobject_class->dispose       = geda_line_dispose;
  gobject_class->finalize      = class->finalize;

  object_class->bounds         = geda_line_bounds;
}

/*! \brief Returns a pointer to a new Line object.
 *
 *  \par Function Description
 *  Returns a pointer to a new Line object.
 *
 *  \return pointer to the new Line object.
 */
Object *geda_line_new (void)
{
  Object *line = g_object_new( GEDA_TYPE_LINE,
                              "type", OBJ_LINE,
                              "name", "line",
                               NULL );
  return GEDA_OBJECT(line);
}

/*! \brief Determine if object is a Geda Line Object.
 *
 *  \par Function Description
 *  Returns true if the argument is a Geda Line object.
 *
 *  \return boolean.
 */
bool is_a_geda_line_object (Line *lin)
{
  return lin && (GEDA_TYPE_LINE == (lin->head_marker & lin->tail_marker));
}