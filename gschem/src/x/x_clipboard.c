/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
#include <config.h>

#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "gschem.h"

#define CLIP_TYPE_SCHEMATIC 1

G_LOCK_DEFINE_STATIC(got_answer);
static bool got_answer;

struct query_usable {
  void (*callback) (int, void *);
  void *userdata;
};

/*! \brief Callback System Clipboard Change Ownership.
 *
 *  \par Function Description
 *
 */
static void
clip_handle_owner_change (GtkClipboard *cb, GdkEvent *event,
                          gpointer user_data)
{
  GschemToplevel *w_current = (GschemToplevel *) user_data;

  i_update_sensitivities (w_current);
}

static void
clip_get (GtkClipboard *cb,  GtkSelectionData *selection_data,
          unsigned int info, gpointer user_data_or_owner)
{
  GschemToplevel *w_current;
  GdkAtom         type;
  char           *buf;

  w_current = (GschemToplevel *) user_data_or_owner;

  type = gdk_atom_intern (MIME_TYPE_SCHEMATIC, FALSE);

  if (info != CLIP_TYPE_SCHEMATIC) return;
  /* Convert the objects in the clipboard buffer to gEDA schematic
   * format */
  buf = o_save_buffer (w_current->clipboard_buffer);
  /* Set the selection appropriately */
  gtk_selection_data_set (selection_data, type,
                          8, /* 8-bit data (UTF-8) */
                          (unsigned char *) buf,
                          (int) strlen(buf));
  GEDA_FREE (buf);
}

static void
clip_clear (GtkClipboard *cb, gpointer user_data_or_owner)
{
  GschemToplevel *w_current = user_data_or_owner;

  /* Free the objects in the clipboard buffer */
  s_object_release_objects (w_current->clipboard_buffer);
  w_current->clipboard_buffer = NULL;
}

/*! \brief Initialises system clipboard support
 *
 *  \par Function Description
 *
 * Registers a signal handler to detect if the clipboard has changed
 * and update the menu item sensitivity if necessary.
 */
void
x_clipboard_init (GschemToplevel *w_current)
{
  GtkClipboard *cb;

  cb = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
  g_signal_connect (G_OBJECT (cb),
                    "owner-change",
                    G_CALLBACK (clip_handle_owner_change),
                    w_current);

  got_answer = TRUE;
}

/*! \brief Initialises system clipboard support
 *
 *  \par Function Description
 *
 * Registers a signal handler to detect if the clipboard has changed
 * and update the menu item sensitivity if necessary.
 */
void
x_clipboard_finish (GschemToplevel *w_current)
{
  GtkClipboard *cb;

  cb = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);

  g_signal_handlers_disconnect_by_func (cb, clip_handle_owner_change, w_current);

  gtk_clipboard_store (cb);
}

/*! \brief Callback for determining if any clipboard targets are pastable
 *
 *  \par Function Description
 *
 * Checks if the clipboard targets match any format we recognise, then
 * calls back into a supplied callback function which is interested in
 * the TRUE / FALSE answer to whether we can paste from the clipboard.
 */
static void
query_usable_targets_cb (GtkClipboard *clip, GdkAtom *targets, int ntargets, gpointer data)
{
    struct query_usable *callback_info = data;

    char *name;
    int   i;
    int   is_usable = FALSE;

    for (i = 0; i < ntargets; i++) {

      name = gdk_atom_name (targets[i]);

      if (strcmp (name, MIME_TYPE_SCHEMATIC) == 0) {
        g_free(name);
        is_usable = TRUE;
        break;
      }

      g_free(name);
    }

    callback_info->callback (is_usable, callback_info->userdata);
    GEDA_FREE (callback_info);
    G_LOCK(got_answer);
    got_answer = TRUE; /* unblock submission of new request */
    G_UNLOCK(got_answer);
}


/*! \brief Checks if the system clipboard contains schematic data.
 *
 *  \par Function Description
 *
 * Checks whether the current owner of the system clipboard is
 * advertising gEDA schematic data.
 *
 * The check is performed asynchronously. When a response is
 * recieved, the provided callback is called with a TRUE / FALSE
 * result.
 *
 * \param [in] w_current   The current GschemToplevel.
 * \param [in] callback    The callback to recieve the response.
 * \param [in] userdata    Arbitrary data to pass the callback.
 */
/*
 * Note WEH (01/22/13): We do not want to reallocate another structure
 * and put in a second request if gtk has not responded to the previous
 * request.
 *
 */
void
x_clipboard_query_usable (GschemToplevel *w_current,
                          void (*callback) (int, void *), void *userdata)
{
  static int watch_dog;

  if (got_answer == TRUE) {
    GtkClipboard *clip = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);

    /* Create a new instance of our callback structure */
    struct query_usable *callback_info;
    callback_info = g_new (struct query_usable, 1);

    /* Save callers cbfunc ptr and data ptr to the new structure */
    callback_info->callback = callback;
    callback_info->userdata = userdata;

    got_answer = FALSE; /* Block ourself; the callback has to unblock */

    /* submit the request */
    gtk_clipboard_request_targets (clip, query_usable_targets_cb, callback_info);
  }
  else {
    if (watch_dog == 2) { /* Should never get here */
      G_LOCK(got_answer);
        got_answer = TRUE; /* Is hack, should somehow tell gtk to forget it */
      G_UNLOCK(got_answer);
      watch_dog = 0;
    }
    else {
      watch_dog++;
    }
  }
}

/*! \brief Set the contents of the system clipboard
 *
 *  \par Function Description
 *
 * Sets the system clipboard to contain the gschem objects listed in \a
 * object_list.
 *
 * \param [in,out] w_current   The current GschemToplevel.
 * \param [in]     object_list The objects to put in the clipboard.
 *
 * \return TRUE if the clipboard is successfully set.
 */
bool
x_clipboard_set (GschemToplevel *w_current, const GList *object_list)
{
  GtkClipboard  *cb;
  GtkTargetEntry target = { MIME_TYPE_SCHEMATIC, 0, CLIP_TYPE_SCHEMATIC };

  bool result;

  cb       = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);

  /* Clear the clipboard buffer */
  if (w_current->clipboard_buffer)
    gtk_clipboard_clear (cb);

  /* Copy the objects to the clipboard buffer */
  w_current->clipboard_buffer =
    o_glist_copy_all (object_list, w_current->clipboard_buffer);

  /* Advertise that the data is available */
  result = gtk_clipboard_set_with_data (cb, &target, 1,
                                        clip_get, clip_clear, w_current);

  /* Hint that the data can be stored to be accessed after the program
   * has quit. */
  gtk_clipboard_set_can_store (cb, NULL, 0);

  return result;
}

/*! \brief Get the contents of the system clipboard
 *
 *  \par Function Description
 *
 * If the system clipboard contains schematic data, retrieve it.
 *
 * \param [in,out] w_current   The current GschemToplevel.
 *
 * \returns Any Objects retrieved from the system clipboard, or NULL
 *          if none were available.
 */
GList *
x_clipboard_get (GschemToplevel *w_current)
{
  GedaToplevel            *toplevel;
  GtkClipboard        *cb;
  GdkAtom              type;
  GtkSelectionData    *selection_data;
  GList               *object_list;
  const unsigned char *buf;
  GError              *err;

  object_list = NULL;
  err = NULL;

  toplevel = w_current->toplevel;

  cb = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
  type = gdk_atom_intern (MIME_TYPE_SCHEMATIC, FALSE);

  /* Try to get the contents of the clipboard */
  selection_data = gtk_clipboard_wait_for_contents (cb, type);
  if (selection_data == NULL) return FALSE;

  /* Convert the data buffer to Objects */
  #if GTK_CHECK_VERSION(2,14,0)
  buf = gtk_selection_data_get_data (selection_data);
  #else
  buf = selection_data->data;
  #endif

  object_list = o_read_buffer (toplevel, object_list,
                               (char *) buf, -1, "Clipboard", &err);

  if (err) {
    u_log_message(_("x_clipboard_get: Invalid schematic on clipboard. %s\n"), err->message);
    char *errmsg = g_strdup_printf ( _("An error occurred while inserting clipboard data: %s."), err->message);
    titled_pango_error_dialog ( _("<b>Invalid schematic on clipboard.</b>"), errmsg, _("Clipboard Insertion Failed") );
    GEDA_FREE(errmsg);
    g_error_free(err);
  }

  gtk_selection_data_free (selection_data);
  return object_list;
}