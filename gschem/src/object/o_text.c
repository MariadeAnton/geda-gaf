/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 * Copyright (C) 1998-2014 Ales Hvezda
 * Copyright (C) 1998-2014 gEDA Contributors (see ChangeLog for details)
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

#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "gschem.h"

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
int o_text_get_rendered_bounds (void *user_data, Object *o_current,
                                int *min_x, int *min_y,
                                int *max_x, int *max_y)
{
  GschemToplevel *w_current = (GschemToplevel *) user_data;
  EdaRenderer    *renderer;
  cairo_t *cr;
  cairo_matrix_t render_mtx;
  int result, render_flags = 0;
  double t= 0;
  double l = 0;
  double r = 0;
  double b = 0;

  g_return_val_if_fail ((w_current != NULL), FALSE);

  cr = gdk_cairo_create (w_current->drawable);

  /* Set up renderer based on configuration in w_current. Note that we
   * *don't* enable hinting, because if its enabled the calculated
   * bounds are zoom-level-dependent. */
  //if (Current_Page->show_hidden_text)
  //  render_flags |= EDA_RENDERER_FLAG_TEXT_HIDDEN;

  renderer = g_object_ref (w_current->renderer);
  g_object_set (G_OBJECT (renderer),
                "cairo-context", cr,
                "render-flags", render_flags,
                NULL);

  /* Transform the cairo context to approximate world coordinates. */
  cairo_matrix_init (&render_mtx, 1, 0, 0, -1, -1, 1);
  cairo_set_matrix (cr, &render_mtx);

  /* Use the renderer to calculate text bounds */
  result = eda_renderer_get_text_user_bounds (renderer, o_current, &l, &t, &r, &b);

  /* Clean up */
  GEDA_UNREF (renderer);
  cairo_destroy (cr);

  /* Round bounds to nearest integer */
  *min_x = lrint (fmin (l, r));
  *min_y = lrint (fmin (t, b));
  *max_x = lrint (fmax (l, r));
  *max_y = lrint (fmax (t, b));

  return result;
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Description
 *
 */
void o_text_prepare_place(GschemToplevel *w_current, char *text)
{
  GedaToplevel *toplevel = w_current->toplevel;
  Object *object;

  /* Insert the new object into the buffer at world coordinates (0,0).
   * It will be translated to the mouse coordinates during placement. */

  w_current->first_wx = 0;
  w_current->first_wy = 0;

  w_current->last_drawb_mode = LAST_DRAWB_MODE_NONE;
  object = o_text_new (TEXT_COLOR,
                       0, 0, LOWER_LEFT, 0, /* zero is angle */
                       text,
                       w_current->text_size,
                       /* has to be visible so you can place it */
                       /* visibility is set when you create the object */
                       VISIBLE, SHOW_NAME_VALUE);

  o_text_set_rendered_bounds_func (object,
                                   o_text_get_rendered_bounds,
                                   w_current);

  /* remove the old place list if it exists */
  s_object_release_objects(toplevel->page_current->place_list);
  toplevel->page_current->place_list = NULL;

  /* here you need to add OBJ_TEXT when it's done */
  toplevel->page_current->place_list =
    g_list_append(toplevel->page_current->place_list,object);

  w_current->inside_action = 1;
  i_set_state (w_current, ENDTEXT);
}

/*! \brief Launch the Edit Text Dialog
 *
 *  \par Function Description
 *
 *  This function calls x_dialog_edit_text() which could have been done
 *  in i_command do_edit_text or o_edit directly so maybe this function
 *  is just adding an unnecessary stack push and pops.
 */
void
o_text_edit(GschemToplevel *w_current, Object *o_current)
{
  if(o_current->type == OBJ_TEXT) {
    x_dialog_edit_text(w_current, o_current);
  }
}

/*! \brief Complete Text Editing
 *
 *  \par Function Description
 *
 *    This function is called by x_dialog_edit_text_ok if the OKAY button was
 *  pressed. There may or by not have been any changes so we will loop thru
 *  all of the selected objects and make changes when there is a difference.
 *  If we changed something we recreate that object and set the document
 *  CHANGED flag and undate UNDO.
 *
 *  \param [in] w_current  Ptr to Window specific data structure
 *  \param [in] string     Ptr to new char string  or NULL if multible selection
 *  \param [in] text_align integer, The new text alignment
 *  \param [in] text_color integer, The new text color
 *  \param [in] text_size  integer, The new text size integer
 *  \param [in] rotate     integer, The rotation angle
 */
/*
 * 02/27/13 WEH Added text_color, changed_something conditionals and documentation
 * 07/20/13 WEH Added rotatation, eliminated numselect and len & add conditonal
 *          skip for values less then 0.
 */
void
o_text_edit_end(GschemToplevel *w_current, char *string, int text_align,
                int text_color, int text_size, int rotate)
{
  GedaToplevel *toplevel = w_current->toplevel;
  Object *object;
  GList  *s_current;
  bool    changed_something;

  /* skip over head */
  s_current = geda_list_get_glist( toplevel->page_current->selection_list );

  changed_something = FALSE;
  while(s_current != NULL) {
    object = (Object *) s_current->data;

    if (object) {
      if (object->type == OBJ_TEXT) {

        /* Text string is only applicable if string has length */
        if ( string && strlen (string) != 0 ) {
          if (strcmp(object->text->string, string) != 0) {
            changed_something = TRUE;
            o_text_set_string (object, string);
            /* handle slot= attribute, it's a special case */
            if (object->attached_to != NULL &&
              g_ascii_strncasecmp (string, "slot=", 5) == 0) {
              o_slot_end (w_current, object->attached_to, string);
            }
          }
        }

        /* Change Size */
        if( text_size >= 0 && object->text->size != text_size) {
          object->text->size = text_size;
          changed_something = TRUE;
        }

        /* Change Alignment */
        if(text_align >= 0 && object->text->alignment != text_align) {
          object->text->alignment = text_align;
          changed_something = TRUE;
        }

        /* Change Color */
        if( text_color >= 0 && object->color != text_color) {
          object->color = text_color;
          changed_something = TRUE;
        }
        /* Change Rotation */
        if (rotate >= 0 && object->text->angle != rotate) {
          object->text->angle = rotate;
          changed_something = TRUE;
        }

        if (changed_something) {
          o_text_recreate(object);
        }
      }
    }

    s_current = g_list_next(s_current);
  }

  if (changed_something) {
    toplevel->page_current->CHANGED = 1;
    o_undo_savestate(w_current, UNDO_ALL);
  }

}

/*! \brief Change Text String
 *  \par Function Description
 *  This function is call by
 *
 *  \note
 *  The object passed in should be the REAL object, NOT any copy in any
 *  selection list
 */
void
o_text_change(GschemToplevel *w_current, Object *object,
              char *string, int visibility, int show)
{
  GedaToplevel *toplevel = w_current->toplevel;

  if (object == NULL) {
    BUG_MSG("object == NULL");
  }
  else {
    if (object->type != OBJ_TEXT) {
      BUG_MSG("object != OBJ_TEXT");
    }
    else {
      o_text_set_string (object, string);

      o_set_visibility (object, visibility);
      object->show_name_value = show;
      o_text_recreate(object);

      /* handle slot= attribute, it's a special case */
      if (object->attached_to != NULL &&
        g_ascii_strncasecmp (string, "slot=", 5) == 0)
      {
        o_slot_end (w_current, object->attached_to, string);
      }
      toplevel->page_current->CHANGED = 1;
    }
  }
}