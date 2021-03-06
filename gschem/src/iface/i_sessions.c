/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4 tab-width: 4 -*- */
/*
 * File: i_sessions.c
 *
 * gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 *
 * Copyright (C) 2014-2015 Wiley Edward Hill <wileyhill@gmail.com>
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
 * Date: July, 13, 2014
 * Contributing Author: Wiley Edward Hill
 *
 */
/*! \file i_sessions.c
 *  \remarks
 *     1.) w_current->session_name should not be modified outside of this
 *         module! w_current->session_name is type char* that is either NULL
 *         to indicate a "session" is not active or w_current->session_name
 *         points to a string in the session structure, and therefore
 *         should only be manipulated by routines within this module.
 *         Do not free w_current->session_name!
 *
 *     2.) i_sessions_init must be called first to initialize the array
 *         data from files found in the session configuration directory.
 *         i_sessions_init calls i_sessions_update_menus so i_sessions_init
 *         should only be called after the main menu is contructed.
 *
 *     4.) i_session_is_enabled, which return TRUE is at least one session
 *         is defined, should be used in conjuction with the configuration
 *         variable sessions-at-startup, along with other factors, to
 *         determine is the Sessions Open dialog should be presented
 *         after program startup, see x_sessions_open_dialog for more
 *         details. The function i_sessions_show_at_startup facilitates
 *         usage of this operation.
 */

#include <gschem.h>
#include <geda_stat.h>
#include <geda_debug.h>

static char *advance2char(const char *string)
{
  const char *ptr = string;
  while ((*ptr != ASCII_NUL) && ((*ptr == SPACE) || (*ptr == ASCII_HT)))++ptr;
  return (char*)ptr;
}

/** \defgroup Gschem-Session-System Sessions System
 *  @{ \par This Group contains core Routines for Sessions.
*/

static GArray *sessions;

/** \defgroup sessions-internal Sessions Internal Functions
 *  @{ \par This Group contains core Routine for Sessions.
*/

/*! \brief Get Session Record
 *  \par Function Documentation
 *   Returns a pointer to Session record entry whose name matches
 *   the specified name or NULL if the record was not found.
 *
 *  \param name is the name of the session whose record is to be retieved
 */
static Session *i_session_get_record(const char *name)
{
  Session *record = NULL;
  int      index;

  for (index=0; index<sessions->len; index++) {

    record = &g_array_index(sessions, Session, index);

    if (strcmp(record->session_name, name) == 0) {
      break;
    }
    record = NULL;
  }

  return record;
}

/*! \brief Remove Session Record
 *  \par Function Documentation
 *  The function removes the specified session from the GArray and
 *  returns TRUE  or FASLE if the record was not found.
 *
 *  \sa i_sessions_remove, i_sessions_delete
 *
 *  \param record Pointer to the session record to be removed
 */
static int i_sessions_remove_session (Session *record) {

  Session *session = NULL;
  int      found   = FALSE;
  int      index;

  for(index = 0; index < sessions->len; index++) {

    session = &g_array_index(sessions, Session, index);

    if (strcmp(record->session_name, session->session_name) == 0) {

      GEDA_FREE(session->session_file);
      GEDA_FREE(session->session_name);
      g_array_remove_index (sessions, index);
      found  = TRUE;
      break;
    }
  }

  return found;
}

/*! \brief Close All Open Documents
 *  \par Function Description
 *  Attempts to cause all documents to be closed.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *
 *  \retval TRUE unless user cancels during request to save
 *
 *  \note x_window_close_page will cause a new blank page to be
 *        created which must be deleted after the session is
 *        restored.
 */
static bool i_session_close_all (GschemToplevel *w_current)
{
  GList *iter;
  GList *pages;
  Page  *p_current;
  bool   can_close  = TRUE;
  bool   close_all;

  if (w_current->inside_action &&
     (w_current->event_state == MOVEMODE ||
      w_current->event_state == DRAGMOVE))
  {
    o_move_cancel (w_current);
  }

  x_window_close_edit_dialogs(w_current);

  pages = g_list_copy(geda_list_get_glist(w_current->toplevel->pages));

  /* Loop through all the pages looking for unsaved pages */
  for ( iter = pages; iter != NULL; NEXT(iter)) {

    /* if page has been modified */
    if (geda_page_get_changed(iter->data) > 0) {
      can_close = FALSE;
      break;                 /* if at least one page */
    }
  }

  if (!can_close) {         /* Ask to save unsaved pages */

    close_all = x_confirm_close_window (w_current);
    if (!close_all) {       /* user cancelled the close */
      v_log_message(_("Canceled Close all\n"));
    }
  }
  else {
    close_all = TRUE;       /* There were no unsaved pages */
  }

  if (close_all) {          /* Still want to close all? */

    v_log_message(_("Closing all documents\n"));

    /* Loop through all the pages */
    for ( iter = pages; iter != NULL; NEXT(iter))
    {
      /* get ptr to a page */
      p_current = (Page*)iter->data;
      if (p_current->filename) {
        x_window_close_page (w_current, p_current);
      }
    }
    w_current->session_name = NULL;
  }

  g_list_free (pages);

  return close_all;
}

/*! \brief Get List of Files from Session File.
 *  \par Function Description
 *  This functions reads the session file and collects lists of
 *  accessible files and non-accessible files. The non-accessible
 *  file names are reported, the list of accessible file is returned.
 *
 *  \param record of session whose file list is to be returned
 *
 *  \retval List of file names in the session file.
*/
static GSList *i_sessions_get_file_list(Session *record)
{
  FILE   *fp;
  GSList *files  = NULL;
  GSList *bad    = NULL;
  char   *fname  = NULL;
  char   *sfile  = record->session_file;
  size_t  len    = 0;
  ssize_t read;
  int     result;

  /* Check if the file exists */
  fp = fopen((char *) sfile, "r");

  if (fp) {
    while ((read = getline(&fname, &len, fp)) != -1) {
      char *ptr = advance2char(&fname[0]);
      if (read > 3 && *ptr != ASCII_NUMBER_SIGN) {
        u_string_remove_last_nl(fname);
        /* Could check file extension here */
        result = access(fname, R_OK );
        if (result == 0) {
          files = g_slist_append(files, u_string_strdup(fname));
        }
        else {
          bad = g_slist_append(bad, u_string_strdup(fname));
        }
      }
    }
    free(fname);
    fclose(fp);
  }
  else {
    /* Remove this entry from all menus */
    u_log_message(_("Could not open session file \"%s\"\n"), sfile);
  }

  /* Check for any bad files names and resolve */
  if (bad) {

    while (bad) {
      fname = bad->data;
      u_log_message(_("file: <%s> is not accessible\n"), fname);
      bad = g_slist_remove(bad, fname);
      free(fname);
    }
    g_slist_free (bad);
  }
  record->page_count = g_slist_length (files);

  return files;
}

/*! \brief Load a Session
 *  \par Function Description
 *   Calls i_session_close_all to close all open documents and if
 *   successfull, calls x_window_open_page to open each document
 *   listed in the session file.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param record    Pointer to the session record to be opened
 *
 *  \return count of the number of pages opened
 *
 * TODO: See cmd_do_open in i_command.c
 */
static int i_session_load_session(GschemToplevel *w_current, Session *record)
{
  int     load_count;

  load_count  = 0;

  if (i_session_close_all(w_current)) {

    GSList *iter;
    Page   *blank;
    char   *filename;
    int     exist_count;
    int     missing_path;

    blank        = gschem_toplevel_get_current_page(w_current);
    iter         = i_sessions_get_file_list(record);
    exist_count  = 0;
    missing_path = 0;

    while (iter) {
      exist_count++;
      filename = iter->data;
      if (!f_get_is_path_absolute(filename)) {
        missing_path++;
      }
      if (x_window_open_page(w_current, filename)) {
        load_count++;
      }
      iter = g_slist_next(iter);
    }

    if (missing_path) {
      q_log_message(_("Warning relative file names detected in session \"%s\"\n"),
                       record->session_name);
    }

    /* Note: blank could be NULL if x_window_set_current_page
     * was not called after loading a blank "dummy" page */
    if (load_count) {
      q_log_message(_("Session %s, opening %d of %d documents\n"),
                       record->session_name, load_count, exist_count);
      if (blank != NULL) {
        x_window_close_page (w_current, blank);
      }
      /* Only set session_name if a file was loaded */
      w_current->session_name = record->session_name;
    }
    else { /* do error recovery */
      if (!exist_count) {
        v_log_message(_("Session \"%s\" did not contain any accessible documents\n"),
                         record->session_name);
      }
      if (blank == NULL) { /* Do Error recovery */
        i_command_process(w_current, "file-new", 0, NULL, ID_ORIGIN_CCODE);
        /* Give the medicine a chance to take effect */
        sleep(1);
      }
    }

    /* Update the window for the current page */
    i_status_update_sensitivities(w_current);
    i_status_update_title (w_current);
    x_pagesel_update (w_current); /* If dialog open, update tree */

    g_slist_free (iter);
  }

  return load_count;
}

/*! \brief Create a New Session
 *  \par Function Description
 *  Creates a new session file using the specified name and
 *  saves all of the file names of open document in the current
 *  context to the session file. A new record is added to the
 *  Session Garray.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param name      Pointer to a name for the session record
 *  \param err        Pointer to GError structure
 *
 *  \retval int count of document files saved for the session
 */
static int
i_sessions_create(GschemToplevel *w_current, const char *name, GError **err)
{
  FILE    *fp;
  GList   *iter;
  Page    *page;

  char    *filename;
  char    *session_file;
  int      count = 0;

  Session  record;

  /* It's called "pre-reusing" a variable */
  filename = u_string_concat(name, SESSIONS_FILE_DOT_SUFFIX, NULL);
  session_file = g_build_filename(f_path_user_config (),
                                  SESSIONS_DIRECTORY,
                                  filename,
                                  NULL);

  GEDA_FREE(filename);

  fp = fopen (session_file, "w");

  if (fp) {

    GedaToplevel *toplevel = w_current->toplevel;

    count = 0;
    iter = geda_toplevel_get_pages(toplevel);

    /* Loop through all the pages looking for unsaved pages */
    while (iter) {

      /* get ptr to a page */
      page     = (Page*)iter->data;
      filename = page->filename;
      fprintf(fp, "%s\n", filename);
      ++count;
      NEXT(iter);
    }

    fclose(fp);

    g_list_free (iter);

    record.page_count   = count;
    record.session_file = session_file;
    record.session_name = u_string_strdup(name);

    if (sessions == NULL) {
      sessions = g_array_new (FALSE, FALSE, sizeof(Session));
    }

    g_array_append_val(sessions, record);

    w_current->session_name = record.session_name;
  }
  else {

    g_set_error (err, G_FILE_ERROR, errno, "\"%s\": %s",
                 session_file, strerror(errno));

    GEDA_FREE(session_file);
  }
  return count;
}

/*! \brief Save the Current Session
 *  \par Function Description
 *  Saves the file names of open document in the current context
 *  to the session file assocated with the current session. The
 *  Session Garray is updated with the file count.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param err        Pointer to GError structure
 *
 *  \return count of document files saved for the session
 */
static int i_sessions_save(GschemToplevel *w_current, GError *err)
{
  FILE    *fp;
  GList   *iter;
  Page    *page;
  Session *record;
  char    *filename;
  char    *session_file;
  int      count;

  count = 0;

  if (w_current->session_name != NULL) {

    record = i_session_get_record(w_current->session_name);

    session_file = record->session_file; /* do not free, belongs to the record */

    fp = fopen (session_file, "w");

    if (fp) {

      GedaToplevel *toplevel = w_current->toplevel;

      iter = geda_toplevel_get_pages(toplevel);

      /* Loop through all the pages looking for unsaved pages */
      while (iter) {

        /* get ptr to a page */
        page     = (Page*)iter->data;
        filename = page->filename;
        fprintf(fp, "%s\n", filename);
        ++count;
        NEXT(iter);
      }

      fclose(fp);

      g_list_free (iter);

      record->page_count = count;

      w_current->session_name = record->session_name;
    }
    else {
      g_set_error (&err, G_FILE_ERROR, errno, "%s: %s",
      session_file, strerror(errno));
    }
  }
  else {
    BUG_MSG("w_current->session_name == NULL");
  }
  return count;
}

/** \defgroup sessions-menu Sessions Menu Support Functions
 *  @{ \remark
 */

/*! \brief GNoitfier Menu Item Callback
 *  \par Function Description
 *  This function is called with a menu item is destroyed in order
 *  to free the data record attached to the menu items that was
 *  allocated in i_sessions_attach_submenu().
 */
static void
session_free_menu_data (void *data, GClosure *closure) {
  GEDA_FREE (data);
}

/*! \brief Session Menu Item Callback
 *  \par Function Description
 *   This function is called when a menu item under the Sessions/
 *   Restore submenu is activated.
 *
 *  \param menuitem  Pointer to Session menu item (with embed data)
 *  \param user_data Pointer to embed SessionMenuData structure
 */
static void session_menu_item_clicked(GtkMenuItem *menuitem, void *user_data)
{
  SessionMenuData *data      = (SessionMenuData*) user_data;
  GschemToplevel  *w_current = data->w_current;
  Session         *session   = data->session;

  i_session_load_session(w_current, session);

}

/*! \brief Attach Submenu with Session names to Sessions/Restore Menu
 *  \par Function Description
 *   This function add a menu item foreach session under the Sessions/
 *   Restore menu. Each menu item is allocated a data record containing
 *   a pointer to the toplevel object, and the session recorder associated
 *   with the item.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 */
static void i_sessions_attach_submenu(GschemToplevel *w_current)
{
  GtkWidget *tmp;
  GtkWidget *sessions_menu_item;
  GtkWidget *sessions_submenu;
  GtkWidget *menubar;
  int        index;
  unsigned long id;

  menubar = x_menu_get_main_menu(w_current);

  if (GTK_IS_MENU_BAR(menubar)) {

    sessions_menu_item = GEDA_OBJECT_GET_DATA(menubar, SESSIONS_RESTORE_SUBMENU);

    if(sessions_menu_item != NULL) {

      /* disconnect all unblocked signals */
      while(1) {
        id = g_signal_handler_find(sessions_menu_item, G_SIGNAL_MATCH_UNBLOCKED,
                                   0, 0, NULL, NULL, NULL);
        if(id == 0)
          break;
        g_signal_handler_disconnect(sessions_menu_item, id);
      }

      sessions_submenu = gtk_menu_new();

      for(index=0; index < sessions->len; index++) {

        Session *record = &g_array_index(sessions, Session, index);

        SessionMenuData *menu_data = GEDA_MEM_ALLOC (sizeof(SessionMenuData));

        menu_data->session   = record;
        menu_data->w_current = w_current;

        tmp = gtk_menu_item_new_with_label((char *)record->session_name);
        g_object_set (tmp, "visible", TRUE, NULL);

        g_signal_connect_data (GTK_OBJECT(tmp), "activate",
                              (GCallback) session_menu_item_clicked,
                               menu_data,
                              (GClosureNotify) session_free_menu_data,
                               0);
        gtk_menu_append(GTK_MENU(sessions_submenu), tmp);
      }

      gtk_widget_show_all(sessions_submenu);

      gtk_menu_item_set_submenu(GTK_MENU_ITEM(sessions_menu_item),
                                sessions_submenu);
    }
  }
}

/*! \brief Update Session Restore Submenu
 *  \par Function Description
 *   This function attempts to destory the current submenu items under
 *   Menu/Session/Restore and then calls i_sessions_attach_submenu
 *   to rebuilt new menu items for each session. If the Restore
 *   submenu is not found the operation is aborted.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 */
static void update_sessions_menus(GschemToplevel *w_current)
{
   GtkWidget        *menubar;
   GtkWidget        *menu_item;
   GtkWidget        *submenu;
   GList            *iter;

   for (iter = global_window_list; iter != NULL; iter = g_list_next (iter)) {

      w_current = (GschemToplevel *)iter->data;
      menubar = x_menu_get_main_menu(w_current);
      if (menubar == NULL)
        continue;

      menu_item = GEDA_OBJECT_GET_DATA (menubar,SESSIONS_RESTORE_SUBMENU);

      if (menu_item == NULL) {
         /* The main Session menu did not get defined so do nothing */
         break;
      }

      submenu = gtk_menu_item_get_submenu (GTK_MENU_ITEM(menu_item));

      if (submenu != NULL) {
        gtk_widget_destroy(submenu);
      }

      i_sessions_attach_submenu(w_current);
   }
}

/** @} endgroup sessions-menu */

/*! \brief Get Count of Documents for Session
 *  \par Function Description
 *  This function attemps to open a session file and read each line,
 *  and each line not beginning with "#" longer and than 3 char is
 *  The resulting count of the lines is returned. The function does
 *  not check whether the files are accessible.
 *
 *  \param session_file Pointer to the name of the session file name
 *
 *  \note This function does load a session
 */
static int i_sessions_get_count(const char *session_file)
{
  char   *line = NULL;
  int     lc   = 0;
  size_t  len  = 0;
  ssize_t read;
  FILE   *fp;

  fp = fopen (session_file, "r");

  if (fp) {
    while ((read = getline(&line, &len, fp)) != -1) {
      char *ptr = advance2char(&line[0]);
      if (read > 3 && *ptr != ASCII_NUMBER_SIGN) {
        //errno = 0;
        //access(ptr, F_OK | R_OK);
        //if (!errno) {
          ++lc;
        //}
      }
    }
    //errno = 0;
    free(line);
    fclose(fp);
  }

  return lc;
}

/*! \brief Load Session Data
 *  \par Function Description
 *  This function collects the names of files in the sesssion
 *  directory and create a new record for each session definition.
 *  Each record is stored in the Sessions Garray. If the sessions
 *  directory does not exist then a new empty directory is created.
 *
 *  \note This function does not load a session
 */
static void i_sessions_load_data(void)
{
  GSList  *session_files;
  GSList  *iter;
  char     buffer[MAX_FILE];
  char    *path;
  char    *file;
  char    *tmpname;
  int      num_sessions;

  Session  record;

  sessions = NULL;   /* The main array, global to this module */

  path = g_build_filename(f_path_user_config (), SESSIONS_DIRECTORY, NULL);

  if(!g_file_test(path, G_FILE_TEST_EXISTS)) {

    /* There was no session directory so create an empty one */
    f_path_create(path, S_IRWXU | S_IRWXG);

  }
  else {

    session_files = f_get_dir_list_files(path, SESSIONS_FILE_SUFFIX);

    if (session_files) {

      num_sessions = g_slist_length(session_files);

      sessions = g_array_sized_new (FALSE, TRUE, sizeof(Session), num_sessions);

      for (iter = session_files; iter; iter = g_slist_next(iter)) {

        if (iter->data) {

          /* Copy filename from list to the buffer AND set file pointer */
          file = strcpy (&buffer[0], iter->data);

          record.session_file = g_build_filename(path, file, NULL);
          record.page_count   = i_sessions_get_count(record.session_file);

          tmpname = f_get_basename(file);
          f_file_remove_extension(tmpname);

          record.session_name = u_string_strdup(tmpname);

          sessions = g_array_append_val(sessions, record);

          GEDA_FREE(iter->data);
        }
      } /* next session file */

      g_slist_free(session_files);

    }
  }

  GEDA_FREE(path);
}

/*! \brief Release All Memory Associated with Session Data
 *  \par Function Description
 *   This function frees the strings in the Sessions GArray
 *   and the array itself.
 *
 *  \note Does not free memory assocated with the menu items
 */
static void i_sessions_destroy_sessions(void)
{
  Session *record;
  int      index;

  if (sessions != NULL) {

  for(index=0; index < sessions->len; index++) {

    record = &g_array_index(sessions, Session, index);

    if (record != NULL) {
      GEDA_FREE(record->session_file);
      GEDA_FREE(record->session_name);
    }
  }

  g_array_free (sessions, TRUE);
  }
}

/** @} endgroup sessions-internal */

/** \defgroup sessions-global-functions Sessions Global Functions
 *  @{ \par This Group contains global routines for Sessions System
 */

/*! \brief Delete an Existing Session
 *  \par Function Description
 *  The remove the session file, calls i_sessions_remove_session to
 *  remove the session record and then i_sessions_update_menus to
 *  regenerate the restore submenu.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param name      Pointer to the name of session to be deleted
 *
 *  \retval TRUE on success, otherwise FALSE
 */
int i_sessions_delete_session(GschemToplevel *w_current, const char *name)
{
  Session *record;
  int      result;

  record = i_session_get_record(name);

  u_log_message( _("Removing session: %s"), name);

  if (!f_file_remove(record->session_file)) {

    /* Do this BEFORE we delete name! */
    if (w_current->session_name != NULL) {
      if (strcmp(w_current->session_name, name) == 0 ); {
        /* If the current session was deleted, update the toplevel */
        w_current->session_name = NULL;
      }
    }

    i_sessions_remove_session(record);

    i_sessions_update_menus(w_current);

    result = TRUE;
  }
  else {

    char    *msg;

    msg = u_string_sprintf ("%s, %s", record->session_file, strerror(errno));

    /* Log the error */
    u_log_message( _("%s: Failed to remove session: %s"), __func__, msg);

    /* inform the user */
    pango_error_dialog ( _("<b>Could not remove session</b>"), msg);
    GEDA_FREE(msg);
    result = FALSE;
  }

  return result;
}

/*! \brief Export an Existing Session
 *  \par Function Description
 *  The Session system stores files in the configuration subdirectory.
 *  This function copies a stored file to a specified location. This
 *  is useful for creating project files.
 *
 *  \param name      Pointer to the name of the session to be export
 *  \param filename  Pointer to file name session is to be export to.
 *
 *  \retval result of fcopy function
 */
int i_sessions_export_session(const char *name, const char *filename)
{
  Session *record = i_session_get_record(name);
  return f_file_copy (record->session_file, filename);
}

/*! \brief Create a New Session
 *  \par Function Description
 *  Creates a new session file using the specified name and saves all of
 *  the filename of open document in the current context to the session
 *  file. A new record is added to the Session Garray and the menus are
 *  updated.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param name      Pointer to the name for the new session
 *
 *  \retval count of the files associated with the new session
 */
int i_sessions_new_session(GschemToplevel *w_current, const char *name)
{
  GError *err   = NULL;
  int     count = -1;
  char   *msg;

  count = i_sessions_create(w_current, name, &err);

  if (!err) {

    u_log_message( _("session %s created with %d documents.\n"), name, count);

    update_sessions_menus(w_current);

  }
  else {

    msg = u_string_sprintf ( _("An error occurred attemting to create session %s: %s."),
                         name, err->message);
    /* Log the error */
    u_log_message( "%s %s", __func__, msg);

    /* inform the user */
    titled_pango_error_dialog ( _("<b>Session Error</b>"), msg, _("Creation failed") );

    GEDA_FREE(msg);
    g_error_free(err);
  }

  return count;
}

/*! \brief Open a Session
 *  \par Function Description
 *   Calls i_session_load_session to open all document files
 *   associated with the given session.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param name      Pointer to session name to be saved
 *
 *  \return TRUE on success otherwise FALSE
 */
bool i_sessions_open_session(GschemToplevel *w_current, const char *name)
{
  Session *record;
  bool     result;
  char    *old_session_name;

  record = i_session_get_record(name);

  if (record) {

    /* temporarily preserve existing session name  */
    old_session_name = w_current->session_name;

    w_current->session_name = NULL;

    i_session_load_session(w_current, record);

    if (w_current->session_name == NULL) {

      /* failed so restore preserved session name  */
      w_current->session_name = old_session_name;
      result = FALSE;
    }
    else {
      v_log_message(_("Opened session %s\n"), name);
      result = TRUE;
    }

  }
  else {
    result = FALSE;
  }

  return result;
}

/*! \brief Rename an Existing Session
 *  \par Function Description
 *   This function changes the name of an existing session. The session name
 *   is the basename of the session file. After renaming the file the Garray
 *   record is update and i_sessions_update_menus() is called to update the
 *   Session Restore submenu.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param old_name  Pointer to session name
 *  \param new_name  Pointer to the new session name
 *
 *  \return TRUE on success otherwise FALSE
 */
int i_sessions_rename_session(GschemToplevel *w_current, const char *old_name,
                                                         const char *new_name)
{
  Session *record;
  char    *filename;
  char    *new_filename;
  char    *str;
  int      result = FALSE;

  record = i_session_get_record(old_name);

  filename = u_string_concat(new_name, SESSIONS_FILE_DOT_SUFFIX, NULL);

  new_filename = g_build_filename(f_path_user_config (),
                                  SESSIONS_DIRECTORY,
                                  filename,
                                  NULL);
  GEDA_FREE(filename);

  if (rename(record->session_file, new_filename) == 0) {

    str = u_string_strdup(new_name);

    q_log_message(_("Renaming session %s to renamed %s\n"),
                  record->session_name, str);

    /* If we are renaming the active session update toplevel */
    if ((w_current->session_name != NULL) &&
      (strcmp(w_current->session_name, old_name) == 0))
    {
      w_current->session_name = str;
    }

    GEDA_FREE(record->session_name);
    GEDA_FREE(record->session_file);

    record->session_file = new_filename;
    record->session_name = str;

    i_sessions_update_menus(w_current);

    result = TRUE;
  }
  else {

    str = u_string_sprintf ("%s, %s", record->session_file, strerror(errno));

    /* Log the error */
    u_log_message( _("%s: Failed to rename session: %s"), __func__, str);

    /* inform the user */
    pango_error_dialog ( _("<b>Could not rename session</b>"), str);
    GEDA_FREE(str);
    GEDA_FREE(new_filename);
  }
  return result;
}

/*! \brief Save Session in the Current Context
 *  \par Function Description
 *   This function either calls i_sessions_create() to create a new
 *   session or i_sessions_save() to update an existing session depending
 *   on whether name is NULL. When name is non-Null a new session record
 *   is created, in essence, a save-as operation.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *  \param name      Optional pointer to string name for the session
 *
 *  \retval count of the files associated with the saved session
 */
int i_sessions_save_session(GschemToplevel *w_current, const char *name)
{
  GError *err = NULL;
  char   *msg;
  int     count;

  if (name) {

    count = i_sessions_create(w_current, name, &err);

    if (!err) {
      update_sessions_menus(w_current);
      msg = u_string_sprintf(_("Created session %s with %d documents.\n"), name, count);
    }
  }
  else {

    count = i_sessions_save(w_current, err);

    if (!err) {
      msg = u_string_sprintf(_("Saved %d documents to session %s\n"), count, w_current->session_name);
    }
  }

  if (err) {

    msg = u_string_sprintf(_("An error occurred attempting to save session %s: %s."), name, err->message);

    u_log_message ("%s", msg); /* Log the error */

    /* inform the user */
    titled_pango_error_dialog( _("<b>Session Error</b>"), msg, _("Save session failed") );

    g_error_free(err);
    count = -1;
  }
  else {
    q_log_message(msg);
  }

  GEDA_FREE(msg);

  return count;
}

/** \defgroup sessions-global-utilities Sessions Global Utilities
 *  @{ \par Contains system-level utilities for Sessions.
*/

/*! \brief Get Sessions Data
 *  \par Function Documentation
 *   Returns a pointer to Session Garray. This is used by dialog routines
 *   to manage or access the session data.
 *
 *  \retval pointer to sessions GArray
 */
GArray *i_sessions_get_sessions(void)
{
  return sessions;
}

/*! \brief Get is Sessions Data Available
 *  \par Function Documentation
 *   Returns TRUE if the Session Garray references data.
 *
 *  \retval TRUE if the Session Garray references data, otherwise FALSE
 */
bool i_sessions_is_enabled(void)
{
  return (sessions != NULL && sessions->len > 0) ? 1 : 0;
}

/*! \brief Get Show Open Session at Starup Setting
 *  \par Function Documentation
 *   Returns the value of the "sessions-at-startup" configuration variable.
 *
 *  \sa i_sessions_show_at_startup
 *
 *  \retval value of configuration setting
 */
bool i_sessions_get_show_at_startup(void)
{
        bool  answer;
        bool  tmp_bool;
  const char *group     = SESSIONS_CONFIG_GROUP;
  const char *key       = "sessions-at-startup";
  GError     *err       = NULL;
  EdaConfig  *cfg       = eda_config_get_user_context();

  tmp_bool = eda_config_get_boolean (cfg, group, key, &err);

  if (err != NULL) {
    g_clear_error (&err);
    answer= FALSE;
  }
  else {
    answer = tmp_bool;
  }
  return answer;
}

/*! \brief Set Show Open Session at Starup Setting
 *  \par Function Documentation
 *   Sets the value of the "sessions-at-startup" configuration variable.
 *
 *  \sa i_sessions_show_at_startup
 *
 *  \param show new boolean value of the configuration setting
 */
void i_sessions_set_show_at_startup(bool show)
{
  const char *group = SESSIONS_CONFIG_GROUP;
  const char *key   = "sessions-at-startup";
  EdaConfig  *cfg   = eda_config_get_user_context();

  eda_config_set_boolean (cfg, group, key, show);

  if (show) {
    v_log_message(_("Show Sessions at startup is now ENABLED.\n"));
  }
  else {
    v_log_message(_("Show Sessions at startup is now DISABLED\n"));
  }
}

/*! \brief Get Should Show Open Session at Starup
 *  \par Function Documentation
 *   Returns TRUE if the Open Session should be displayed based
 *   on the availability or existence of session data and the
 *   "sessions-at-startup" configuration setting.
 *
 *  \retval value TRUE is the Open Session Dialog should be presented
 *
 *  \note Other factors such as arguments on the command-line should
 *        also be considered before lauching the sessions dialog.
 */
bool i_sessions_present_at_startup(void)
{
  return i_sessions_is_enabled() && i_sessions_get_show_at_startup();
}

/*! \brief Update the Session Submenu
 *  \par Function Description
 *   This function is calls i_sessions_attach_submenu if any sessions
 *   records exist, indicated by a non-null array. The i_sessions_init()
 *   should be call before calling this function.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 */
void i_sessions_update_menus(GschemToplevel *w_current)
{
  if (sessions) {
    update_sessions_menus(w_current);
  }
}

/*! \brief Initialize Session system
 *  \par Function Description
 *   This function is called to initialized the Session system. The
 *   function to calls i_sessions_load_data() to load the Session
 *   array. This function also restores the auto_sessions variable
 *   preserved by x_sessions_save_settings.
 *
 *  \param w_current Pointer to #GschemToplevel Object
 *
 *  \sa x_sessions_save_settings
 */
void i_sessions_init(GschemToplevel *w_current)
{
  bool  tmp_bool;
  const char *group     = SESSIONS_CONFIG_GROUP;
  const char *key       = "auto-update-sessions";
  GError     *err       = NULL;
  EdaConfig  *cfg       = eda_config_get_user_context();

  i_sessions_load_data();
  geda_atexit((geda_atexit_func)i_sessions_destroy_sessions, NULL);
  w_current->session_name = NULL;     /* means no session */
  i_sessions_update_menus(w_current);

  tmp_bool = eda_config_get_boolean (cfg, group, key, &err);

  if (err != NULL) {
    g_clear_error (&err);
  }
  else {
    w_current->auto_sessions = tmp_bool;
  }
  v_log_message(_("Session system initialized!\n"));
}
/** @} endgroup sessions-global-utilities */
/** @} endgroup sessions-global-functions */
/** @} endgroup Gschem-Session-System */
