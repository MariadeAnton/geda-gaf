
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

/*!
 * \file x_edit_color.c
 *
 * \brief A dialog box for adding editing object color.
 */

#include <config.h>

#include <stdio.h>

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include "gschem.h"
#include <geda_widgets.h>

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

#include <geda_dialog_controls.h>
#include "x_dialog.h"

/*! \defgroup Edit-Color-Dialog Edit-Color Dialogs Functions
 *  @{
 */

/*! \brief Handle selection change event for x_dialog_edit_color
 *  \par Function Description
 *  Updates the color combobox when the selection changes.
 *
 *  \param w_current pointer to GschemToplevel context
 *  \param object    pointer to a selected Object.
 */
static void
x_dialog_color_update_selection (GschemToplevel *w_current, Object *object)
{
  GtkComboBox *ColorCombo;
  int index;

  if (object != NULL) {
    index = object->color;

    ColorCombo = g_object_get_data (G_OBJECT (w_current->clwindow), IDS_COLOR_EDIT);
    gtk_combo_box_set_active((GtkComboBox *)ColorCombo, index);
  }
}

/*! \brief Apply a color change to selected objects
 *  \par Function Description
 *  This function applies a color change to the currently selected objects.
 */
static void
x_dialog_edit_color_apply(GtkWidget *dialog, GschemToplevel *w_current)
{
  GList  *s_current = NULL;
  Object *object = NULL;
  int     color_index;

  GtkComboBox *ColorCombo;
  GtkTreeIter  iter;
  GValue       value = {0, };

  s_current = geda_list_get_glist( Current_Selection );

  if(s_current != NULL) {

    /* Get ptr to the Combo widget */
    ColorCombo = g_object_get_data (G_OBJECT (dialog), IDS_COLOR_EDIT);

    /* Retrieve the current index from the tree model in the widget */
    if( gtk_combo_box_get_active_iter(GTK_COMBO_BOX(ColorCombo), &iter)) {
      gtk_tree_model_get_value ( gtk_combo_box_get_model (ColorCombo), &iter, 1, &value);
      color_index = g_value_get_int (&value);

      while(s_current != NULL) {

        object = (Object *) s_current->data;
        if (object == NULL) {
          fprintf(stderr, _("ERROR: NULL object in x_dialog_edit_color_apply!\n"));
        }
        else {
          if(object->color != color_index) {
            o_set_color (object, color_index);
            w_current->toplevel->page_current->CHANGED = 1;
          }
        }
        s_current = g_list_next(s_current);
      }
    }
    o_undo_savestate(w_current, UNDO_ALL);
  }
}

/*! \brief response function for the color edit dialog
 *  \par Function Description
 *  This function takes the user response from the color edit dialog
 */
static void
x_dialog_edit_color_response(GtkWidget *Dialog, int response,
                           GschemToplevel *w_current)
{
  switch (response) {
  case GTK_RESPONSE_REJECT:
  case GTK_RESPONSE_DELETE_EVENT:
    /* cut link from dialog to selection */
    gtk_widget_destroy(w_current->clwindow);
    w_current->clwindow = NULL;
    break;
  case GTK_RESPONSE_ACCEPT:
    x_dialog_edit_color_apply(Dialog, w_current);
    break;
  default:
    printf(_("ERROR: <x_dialog_edit_color_response>: strange signal %d\n"),
           response);
  }
}

/*! \brief Create the color edit dialog
 *  \par Function Description
 *  This function creates the color edit dialog
 */
void x_dialog_edit_color (GschemToplevel *w_current)
{
  GtkWidget *Dialog;
  GtkWidget *optionmenu;
  GtkWidget *label;
  GtkWidget *vbox;

  Object *object;
  int color_index;

  object = o_select_return_first_object (w_current);

  if (object != NULL)
    color_index = object->color;
  else
    color_index = ATTRIBUTE_COLOR;

  Dialog = w_current->clwindow;
  if (!Dialog) {
    Dialog = gschem_dialog_new_with_buttons(_("Color Edit"),
                                            GTK_WINDOW(w_current->main_window),
           /* nonmodal Editing Dialog */    GSCHEM_MODELESS_DIALOG,
                                            IDS_COLOR_EDIT, w_current,
                                            GTK_STOCK_CLOSE, GTK_RESPONSE_REJECT,
                                            GTK_STOCK_APPLY, GTK_RESPONSE_ACCEPT,
                                            NULL);

  /* Set the alternative button order (ok, cancel, help) for other systems */
    gtk_dialog_set_alternative_button_order(GTK_DIALOG(Dialog),
                                            GTK_RESPONSE_ACCEPT,
                                            GTK_RESPONSE_REJECT,
                                            -1);

    gtk_window_position (GTK_WINDOW (Dialog),
                         GTK_WIN_POS_MOUSE);

    gtk_dialog_set_default_response (GTK_DIALOG (Dialog),
                                     GTK_RESPONSE_ACCEPT);

    gtk_container_border_width(GTK_CONTAINER(Dialog),
                               DIALOG_BORDER_SPACING);
    vbox = GTK_DIALOG(Dialog)->vbox;
    gtk_box_set_spacing(GTK_BOX(vbox), DIALOG_V_SPACING);

    label = geda_aligned_label_new(_("Object color:"), 0, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    optionmenu = create_color_menu ( w_current, color_index );

    gtk_box_pack_start(GTK_BOX(vbox), optionmenu, FALSE, FALSE, 0);
    gtk_widget_set_tooltip_text(optionmenu, _("Select the color for the select objects."));

    GSCHEM_HOOKUP_OBJECT(Dialog, optionmenu, IDS_COLOR_EDIT);

    gtk_window_set_transient_for (GTK_WINDOW(Dialog),
                                  GTK_WINDOW(w_current->main_window));

    g_signal_connect (G_OBJECT (Dialog), "response",
                      G_CALLBACK (x_dialog_edit_color_response),
                      w_current);

    g_object_set (G_OBJECT (Dialog),
                  DIALOG_DATA_SELECTION,
                  x_dialog_color_update_selection, NULL);

    gtk_widget_show_all(Dialog);
    w_current->clwindow = Dialog;
  }

  else { /* dialog already created */
    x_dialog_color_update_selection (w_current, object);
    gtk_window_present(GTK_WINDOW(Dialog));
  }
}

/******************** End of color edit dialog box **********************/
/*! @} end group Edit-Color-Dialog */