/* -*- geda_path.c -*-
 *
 * gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
 *
 * Copyright (C) 2013-2014 Ales Hvezda
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
 * 02110-1301 USA, <http://www.gnu.org/licenses/>.
 *
 *  Contributing Author: Wiley Edward Hill
 *  Date Contributed: November, 18, 2013
 */
/*! \file geda_path.c
 *  \brief Geda Path Object Class derived from the GedaObject Class
 */
/** \defgroup geda-path-object Geda Path Object
 *  @{
 */
/*! \class Path geda_path.h "include/libgeda/geda_path.h"
 *  \implements geda-object
 *  \brief This is an implementaion class for GEDA Path Objects.
 *  A Geda Path Object is a graphical object that does not involve electrical
 *  interconnections. A Path object has line-type and fill-type properties.
 */

#include <config.h>

#include <libgeda_priv.h>

static GObjectClass *geda_path_parent_class = NULL;

/*! \brief Get path bounding rectangle in WORLD coordinates.
 *  \par Function Description
 *  This function sets the <B>left</B>, <B>top</B>, <B>right</B> and
 *  <B>bottom</B> parameters to the boundings of the path object described
 *  in <B>*path</B> in world units.
 *
 *  \note Bounding box for bezier curves is loose because we just consider
 *        the convex hull of the curve control and end-points.
 *
 *  \param [in]  object     Path Object to read coordinates from.
 */
int
geda_path_bounds (Object *object)
{
  PATH_SECTION *section;
  int halfwidth;
  int i;
  int found_bound = FALSE;
  int left   = 0;
  int top    = 0;
  int right  = 0;
  int bottom = 0;

  g_return_val_if_fail (GEDA_IS_PATH (object), FALSE);

  /* Find the bounds of the path region */
  for (i = 0; i < object->path->num_sections; i++) {
    section = &object->path->sections[i];
    switch (section->code) {
      case PATH_CURVETO:
        /* Bezier curves with this construction of control points will lie
         * within the convex hull of the control and curve end points */
        left   = (found_bound) ? MIN (left,   section->x1) : section->x1;
        top    = (found_bound) ? MIN (top,    section->y1) : section->y1;
        right  = (found_bound) ? MAX (right,  section->x1) : section->x1;
        bottom = (found_bound) ? MAX (bottom, section->y1) : section->y1;
        found_bound = TRUE;
        left   = MIN (left,   section->x2);
        top    = MIN (top,    section->y2);
        right  = MAX (right,  section->x2);
        bottom = MAX (bottom, section->y2);
        /* Fall through */
      case PATH_MOVETO:
      case PATH_MOVETO_OPEN:
      case PATH_LINETO:
        left   = (found_bound) ? MIN (left,   section->x3) : section->x3;
        top    = (found_bound) ? MIN (top,    section->y3) : section->y3;
        right  = (found_bound) ? MAX (right,  section->x3) : section->x3;
        bottom = (found_bound) ? MAX (bottom, section->y3) : section->y3;
        found_bound = TRUE;
        break;
      case PATH_END:
        break;
    }
  }

  if (found_bound) {
    /* This isn't strictly correct, but a 1st order approximation */
    halfwidth = object->line_options->line_width / 2;
    object->left   = left   - halfwidth;
    object->top    = top    - halfwidth;
    object->right  = right  + halfwidth;
    object->bottom = bottom + halfwidth;
  }
  return found_bound;
}

/*! \brief GedaType instance initializer for Path
 *
 *  \par Function Description
 *  GedaType instance initializer for Path, initializes a new empty
 *  Path object by setting pointers to NULL and numbers to zero,
 *  the path PID variable is set to the next path index.
 *
 *  \param [in] instance The Path structure being initialized,
 *  \param [in] g_class  The Path class we are initializing.
 */
static void geda_path_instance_init(GTypeInstance *instance, void *g_class)
{
  Path    *path                   = (Path*)instance;
  Object *object                  = &path->parent_instance;

  path->sections                  = NULL;
  path->num_sections              = 0;
  path->num_sections_max          = 0;

  path->fill_options.fill_type    = default_fill_type;
  path->fill_options.fill_width   = default_fill_width;
  path->fill_options.fill_angle1  = default_fill_angle1;
  path->fill_options.fill_angle2  = default_fill_angle2;
  path->fill_options.fill_pitch1  = default_fill_pitch1;
  path->fill_options.fill_pitch2  = default_fill_pitch2;

  path->line_options.line_end     = default_line_end;
  path->line_options.line_type    = default_line_type;
  path->line_options.line_width   = default_line_width;
  path->line_options.line_space   = default_line_space;
  path->line_options.line_length  = default_line_length;

  object->path                    = path;
  object->fill_options            = &path->fill_options;
  object->line_options            = &path->line_options;

  path->head_marker               = GEDA_TYPE_PATH;
  path->tail_marker               = path->head_marker;
}

static void
geda_path_dispose(GObject *object)
{
  G_OBJECT_CLASS(geda_path_parent_class)->dispose(object);
}

/*! \brief Geda Path Object Finalization Function
 *  \par Function Description
 *   This function invalidates the Path's markers and then chains up to
 *   the parent's finalize handler. Once invalidated, GEDA_IS_PATH will
 *   fail.
 */
static void geda_path_finalize(GObject *object)
{
  Path *path = GEDA_PATH(object);

  if (path->sections)
    GEDA_FREE(path->sections);

  /* The object is no longer a GedaPath */
  path->head_marker = 1;
  path->tail_marker = 0;

  /* Finialize the parent GedaObject Class */
  GEDA_OBJECT_CLASS( geda_path_parent_class )->finalize(object);
}

/*! \brief GedaType class initializer for Path
 *
 *  \par Function Description
 *  GedaType class initializer for Path. We override our parents
 *  virtual class methods as needed and register our GObject signals.
 *
 *  \param [in]  g_class      The Path class we are initialising
 *  \param [in]  class_data   The Path structure associated with the class
 */
static void geda_path_class_init(void *g_class, void *class_data)
{
  PathClass    *class          = (PathClass*)g_class;
  GObjectClass *gobject_class  = G_OBJECT_CLASS( class );
  ObjectClass  *object_class   = GEDA_OBJECT_CLASS( class );

  geda_path_parent_class       = g_type_class_peek_parent( class );

  gobject_class->dispose       = geda_path_dispose;
  gobject_class->finalize      = geda_path_finalize;

  object_class->bounds         = geda_path_bounds;
}

/*! \brief Function to retrieve Path's Type identifier.
 *
 *  \par Function Description
 *  Function to retrieve a #Path Type identifier. When first called,
 *  the function registers a #Path in the GedaType system to obtain
 *  an identifier that uniquely itentifies a Path and returns the
 *  unsigned integer value. The retained value is returned on all
 *  Subsequent calls.
 *
 *  \return GedaType identifier associated with Path.
 */
GedaType geda_path_get_type (void)
{
  static GedaType geda_path_type = 0;

  if (g_once_init_enter (&geda_path_type)) {

    static const GTypeInfo info = {
      sizeof(PathClass),
      NULL,                   /* base_init           */
      NULL,                   /* base_finalize       */
      geda_path_class_init,   /* (GClassInitFunc)    */
      NULL,                   /* class_finalize      */
      NULL,                   /* class_data          */
      sizeof(Path),
      0,                      /* n_preallocs         */
      geda_path_instance_init /* (GInstanceInitFunc) */
    };

    const char *string;
    GedaType    type;

    string = g_intern_static_string ("Path");
    type   = g_type_register_static (GEDA_TYPE_OBJECT, string, &info, 0);

    g_once_init_leave (&geda_path_type, type);
  }

  return geda_path_type;
}

/*! \brief Returns a pointer to a new Path object.
 *
 *  \par Function Description
 *  Returns a pointer to a new Path object.
 *
 *  \return pointer to the new Path object.
 */
Object *geda_path_new (void)
{
  Object *path = g_object_new( GEDA_TYPE_PATH,
                              "type", OBJ_PATH,
                              "name", "path",
                               NULL );
  return GEDA_OBJECT(path);
}

/*! \brief Determine if object is a Geda Path Object.
 *
 *  \par Function Description
 *  Returns true if the argument is a Geda Path object.
 *
 *  \return boolean.
 */
bool is_a_geda_path_object (Path *path)
{
  return GEDA_IS_OBJECT(path) &&
        (GEDA_TYPE_PATH == (path->head_marker & path->tail_marker));
}
/** @} endgroup geda-path-object */