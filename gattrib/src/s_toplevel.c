/* gEDA - GPL Electronic Design Automation
 * gattrib -- gEDA component and net attribute manipulation using spreadsheet.
 * Copyright (C) 2003-2015 Stuart D. Brorson.
 * Copyright (C) 2012-2015 gEDA Contributors (see ChangeLog for details)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*------------------------------------------------------------------*/
/*! \file
 *  \brief Functions to manipulate the GedaToplevel struct.
 *
 * This file holds functions involved in manipulating the GedaToplevel data
 * structure.  GedaToplevel is the data structure inherited from gEDA's
 * other programs, and holds all info about a project in a form
 * native to gEDA.
 */

#include <gattrib.h>
#include <geda_debug.h>

/* ===================  Public Functions  ====================== */
/*------------------------------------------------------------------*/
/*! \brief Close the TopLevel
 *
 *  This function gets calls the Libgeda f_close functions
 *
 */
void s_toplevel_close(PageDataSet *PageData) {
  s_table_destroy(PageData->component_table, PageData->comp_count, PageData->comp_attrib_count);
  s_table_destroy(PageData->net_table, PageData->net_count, PageData->net_attrib_count);
  s_table_destroy(PageData->pin_table , PageData->pin_count, PageData->pin_attrib_count);
  s_sheet_data_reset(PageData);

  if (pr_current->page_current != NULL) {
    s_page_delete (pr_current, pr_current->page_current, FALSE);
  }

  f_close(pr_current);  /*  Does absolutely nothing */
}

/*! \brief Read a schematic page
 *
 * Reads in a schematic page & calls f_open, which fills out the
 * toplevel structure.
 *
 *  \param toplevel GedaToplevel structure
 *  \param filename file to be opened
 *  \returns 1 on success, 0 on failure
 */
int s_toplevel_read_page(GedaToplevel *toplevel, char *filename)
{

  GError *err = NULL;
  int result;

  /* Set the new filename */
  toplevel->page_current->filename = u_string_strdup(filename);

  /* Read in and fill out toplevel using f_open and its callees */
  if(!f_open (toplevel, toplevel->page_current, filename, &err)) {
    u_log_message ("%s\n", err->message);
    result = 0;
    g_error_free (err);
  }
  else {
    result = 1;
  }

  return result;
}

/*! \brief Verify the entire design
 *
 * This function loops through all components in the
 * design looking for components which are placeholders.
 *
 *  Placeholders are inserted into the object list when
 *  no symbol file is found.  If this function finds a
 *  placeholder, it warns the user.
 *
 *  \param toplevel pointer to the toplevel object to be verified
 */
void s_toplevel_verify_design (GedaToplevel *toplevel)
{
  GList *p_iter;
  const GList *o_iter;

  int missing_sym_flag = 0;

  for (p_iter  = geda_toplevel_get_pages (toplevel);
       p_iter != NULL;
       p_iter  = g_list_next (p_iter)) {

    Page *p_current = p_iter->data;

    for (o_iter = s_page_get_objects (p_current);
         o_iter != NULL;
         o_iter = g_list_next (o_iter)) {
      Object *o_current = o_iter->data;

      /* --- look for object, and verify that it has a symbol file attached. ---- */
      if (o_current->type == OBJ_PLACEHOLDER) {
        missing_sym_flag = 1;  /* flag to signal that problem exists.  */
      }
    }
  }

  if (missing_sym_flag) {
    x_dialog_missing_sym();  /* dialog gives user option to quit */
  }
}

/*------------------------------------------------------------------*/
/*! \brief Copy data from gtksheet into GedaToplevel struct
 *
 * Called when the user invokes "save".  It first
 * places all data from gtksheet into SHEET_DATA.  Then it
 * loops through all pages & calls s_toplevel_sheetdata_to_toplevel()
 * to place all
 * stuff in SHEET_DATA into the libgeda GedaToplevel structure.
 */
void s_toplevel_gtksheet_to_toplevel(GedaToplevel *toplevel)
{
  GList *iter;
  Page *p_current;
g_print("s_toplevel_gtksheet_to_toplevel, begin\n");
  /* read data from gtksheet into SHEET_DATA */
  s_table_gtksheet_to_all_tables();

  /* iterate over all pages in design */
  for ( iter  = geda_toplevel_get_pages(toplevel);
        iter != NULL;
        iter  = g_list_next( iter ) ) {

    p_current = (Page *)iter->data;

    /*toplevel->page_current = p_current;*/
    if(s_page_set_current (toplevel, p_current)) {
      /* only traverse pages which are toplevel */
      if (p_current->page_control == 0) {
        s_toplevel_sheetdata_to_toplevel (toplevel, p_current);    /* adds all objects from page */
      }
    }
    else {
       strcpy(msg_buffer, "Unknown error selecting page <");
       strcat(msg_buffer, p_current->filename);
       strcat(msg_buffer, ">, \n!");
       generic_msg_dialog( msg_buffer );
    }
  }
  return;
}

/*------------------------------------------------------------------*/
/*! \brief Add a new attribute to the top level
 *
 *  This function gets called when the user has entered a new attrib name,
 *  and clicked the OK button.  It does this:
 *  -# It figures out which attrib/sheet is being added to
 *  -# It destroys the old table in preparation for the new attrib.
 *  -# It adds the new attrib to the master lists.
 *  -# It creates a new table with the new attrib.
 *  -# It then adds the appropriate col to the gtksheet.
 *
 * \param column_location The column the attribute is to be added
 */
void s_toplevel_add_new_attrib(int column_location) {

  int cur_tab;                 /* current page in notebook  */

  char *new_attrib_name;
  GtkSheet *sheet;

  new_attrib_name = x_dialog_new_attrib();

  if (new_attrib_name) { /* user di NOT cancel or close window with no value in entry */

    cur_tab = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    sheet = sheets[cur_tab];

    if(column_location < 0) column_location = sheet->maxcol + 1;

    switch (cur_tab) {
      case Components:  /* component attribute sheet */

        if (s_string_list_in_list(sheet_head->master_comp_attrib_list_head, new_attrib_name)) {
          strcpy(msg_buffer, "Can not add \"");
          strcat(msg_buffer, new_attrib_name);
          strcat(msg_buffer, "\", \nAttribute name are ready exist!");
          generic_msg_dialog( msg_buffer );
          GEDA_FREE(new_attrib_name);
          return;
        }

        /* Fill out new sheet with new stuff from gtksheet */
        gtk_sheet_insert_columns(sheet, column_location, 1);

        s_string_list_add_item(sheet_head->attached_attrib,
                               &sheet_head->attached_attrib_count,
                               new_attrib_name);

        s_string_list_insert(sheet_head->master_comp_attrib_list_head,
                             &sheet_head->comp_attrib_count,
                             column_location, new_attrib_name);

        x_gtksheet_add_col_labels(sheet,
                                  sheet_head->comp_attrib_count,
                                  sheet_head->master_comp_attrib_list_head);

        s_string_list_sort_master_comp_attrib_list();

        /* resize table to accomodate new attrib col      */
        sheet_head->component_table = s_table_add_column(sheet_head->component_table,  /* Table */
                                                         sheet_head->comp_count,             /* number of rows */
                                                         column_location, sheet_head->comp_attrib_count-1);

        break;
      case 1:  /* net attribute  */
        /* insert into net attribute list  */
        break;

      case 2:  /* pin attribute  */
        /* insert into pin attribute list  */
        break;
    }  /* switch  */

#if DEBUG
    int i; char* str;
    for ( i = 0; i < sheet_head->comp_attrib_count; i++) {
      str = s_string_list_get_data_at_index(sheet_head->master_comp_attrib_list_head, i);
      fprintf(stderr, "s_string comp_attrib[%d] = [%s]\n",i , str);
    }
#endif

    GEDA_FREE(new_attrib_name);
  }
  return;
}

/*------------------------------------------------------------------*/
/*! \brief Delete an attribute column
 *
 *  This function gets called when the user has selected a single attrib
 *  column, selected the edit->delete attrib item from the pull-down
 *  menu, and then said "yes" to the confirm dialog.
 */
void s_toplevel_delete_attrib_col(GtkSheet *sheet) {

  char *attrib_name;
  int col;

  if (sheet == NULL) return;

  col = sheet->range.col0;

  /* Get name of the column to delete */
  attrib_name = u_string_strdup(sheet->column[col]->title);

  /* Ask user to confirm deletion */
  strcpy(msg_buffer, "Are you sure you want to\n delete the Attribute \"");
  strcat(msg_buffer, attrib_name);
  strcat(msg_buffer, "\"?");
  if (!x_dialog_generic_confirm_dialog (msg_buffer, GTK_MESSAGE_WARNING))
     return;

  /* Remove the attributes from the data table */
  s_table_remove_attribute(sheet_head->component_table, col);

  /* Remove the attribute from the column headings */
  s_string_list_delete_item(&(sheet_head->master_comp_attrib_list_head),
                            &(sheet_head->comp_attrib_count),
                              attrib_name);
  GEDA_FREE(attrib_name);

#ifdef DEBUG
  printf("In s_toplevel_delete_attrib_col, about to delete col in gtksheet.\n");
#endif
  /* Delete col on gtksheet  */

  gtk_sheet_delete_columns (sheet, sheet->range.col0, 1);

  sheet_head->CHANGED = TRUE;  /* Set changed flag so user is prompted when exiting */

  return;
}

/* =======================  Private functions  ====================== */

/*------------------------------------------------------------------*/
/*! \brief Copy SHEET_DATA content to TOP_LEVEL
 *
 * This function
 * loops through all objects on (Page page)->(Object *start_obj).
 * It takes the updated SHEET_DATA->TABLE data and then updates the
 * objects with the new attribs & attrib values.
 * For each component, it updates the attached
 * attrib values using the updated values held in the SHEET_DATA->TABLE
 * structure.  It does so in three steps:
 * -# First find and update component attribs.
 * -# Then find and update net attribs.
 * -# Finally find and update pin attribs.
 * \param toplevel GedaToplevel structure
 * \param page schematic page to copy
 */
void
s_toplevel_sheetdata_to_toplevel (GedaToplevel *toplevel, Page *page)
{
  GList *copy_list;
  GList *o_iter, *prim_iter;
  char *temp_uref;
  STRING_LIST *new_comp_attrib_pair_list;
  STRING_LIST *new_pin_attrib_list;

  /* -----  First deal with all components on the page.  ----- */
#ifdef DEBUG
  fprintf(stderr, "-----  In s_toplevel_sheetdata_to_toplevel, handling components\n");
#endif

  /* Work from a copy list, as objects can be deleted
   * from the list during iteration over the list.
   */
  /* NB: g_list_copy doesn't declare its input const, so we cast */
  copy_list = g_list_copy ((GList *)s_page_get_objects (page));

  /* Iterate backwards since attributes are attached after their
   * parent objects in the list. Attributes can get deleted during
   * the iteration.
   */
  for (o_iter = g_list_last (copy_list);
       o_iter != NULL;
       o_iter = g_list_previous (o_iter)) {

    Object *o_current = o_iter->data;

    /* ------- Object is a component.  Handle component attributes. ------- */
    if (o_current->type == OBJ_COMPLEX) {    /* Note that OBJ_COMPLEX = component + attribs */

#if 0
      if (o_attrib_search_object_attribs_by_name (o_current, "graphical", 0)) {
        break;  /* Ignore graphical components */
      }
#endif

      temp_uref = s_attrib_get_refdes(o_current);
      if ( (temp_uref) &&
	 (strcmp (temp_uref, "none")) &&
	 (strcmp (temp_uref, "pinlabel")) ){
	/* Must create a name=value pair list for each particular component
	 * which we can pass to function updating o_current.  This function
         * places all attribs
	 * found in the row into new_comp_attrib_pair_list.  */
	new_comp_attrib_pair_list = s_table_create_attrib_pair(temp_uref,
							       sheet_head->component_table,
							       sheet_head->master_comp_list_head,
							       sheet_head->comp_attrib_count);

	/* Now update attribs in toplevel using this list.  */
	s_toplevel_update_component_attribs_in_toplevel(toplevel,
							o_current,
							new_comp_attrib_pair_list);
	GEDA_FREE(temp_uref);
      }
      else {

#ifdef DEBUG
	fprintf(stderr, "In s_toplevel_sheetdata_to_toplevel, found complex with no refdes. name = %s\n",
	       o_current->name);
#endif
      }
    }  /* if (o_current->type == OBJ_COMPLEX) */

  }

  g_list_free (copy_list);

#if 0
  /* -----  Next deal with all nets on the page.  ----- */
  /* This is TBD */
#endif

  /* -----  Finally deal with all pins on the page.  ----- */
  /* -----  Next deal with all nets on the page.  ----- */
#ifdef DEBUG
	fprintf( stderr, "-----  In s_toplevel_sheetdata_to_toplevel, handling pins\n");
#endif

  /* Work from a copy list in case objects are
   * deleted from the list during its iteration.
   */
  /* NB: g_list_copy doesn't declare its input const, so we cast */
  copy_list = g_list_copy ((GList *)s_page_get_objects (page));

  for (o_iter = g_list_last (copy_list);
       o_iter != NULL;
       o_iter = g_list_previous (o_iter))
  {
    Object *o_current = o_iter->data;

    /* ------- Object is a complex.  Handle pins by looking ------ */
    /* ------- for all pins attached to a component.        ------ */
    if (o_current->type == OBJ_COMPLEX) {
      /*  Upon finding a component, here's what to do:
      *  0.  Get refdes of component.
      *  1.  Loop over prim_objects, looking for pins.
      *  2.  When a pin is found, create refdes:pinnumber pair
      *      used in searching TABLE.
      *  3.  Search TABLE using refdes:pinnumber as key, and get list of
      *      attribs corresponding to this refdes:pinnumber
      *  4.  Stick the attribs into the GedaToplevel data structure.
      */
      temp_uref =  s_attrib_get_refdes(o_current);

      /* make sure object complex has a refdes  */
      if ((temp_uref != NULL) &&
          (strcmp (temp_uref, "none")) &&
          (strcmp (temp_uref, "pinlabel")) &&
          (o_current->complex->prim_objs))
      {
        for (prim_iter = o_current->complex->prim_objs;
            prim_iter != NULL;
            prim_iter = g_list_next (prim_iter)) {
          Object *comp_prim_obj = prim_iter->data;

          if (comp_prim_obj->type == OBJ_PIN) {
            new_pin_attrib_list = s_toplevel_get_pin_attribs_in_sheet (temp_uref, comp_prim_obj);
            s_toplevel_update_pin_attribs_in_toplevel (toplevel,
                                                      temp_uref,
                                                      comp_prim_obj,
                                                      new_pin_attrib_list);
          }
        }
      }     /* if(temp_uref  */

      GEDA_FREE(temp_uref);
    }
  }
  g_list_free (copy_list);

  return;
}
#undef DEBUG

/*------------------------------------------------------------------*/
/*! \brief Get the component attributes from the top level
 *
 * This function returns a list of attributes attached to obj_name = comp
 * refdes or netlist.
 * \param refdes component refdes to return values from
 * \returns a STRING_LIST where the data field holds a name=value string.
 */
STRING_LIST *s_toplevel_get_component_attribs_in_sheet(char *refdes)
{
  STRING_LIST *new_attrib_list;
  STRING_LIST *local_attrib_list;
  int i;
  int row = -1;
  int count = 0;
  char *name_value_pair;
  char *new_attrib_value;
  char *new_attrib_name;

#if DEBUG
  printf("-----  Entering s_toplevel_get_component_attribs_in_sheet.\n");
#endif

  /* First find pos of this refdes in the master list */
  row = s_table_get_index(sheet_head->master_comp_list_head, refdes);

  /* Sanity check */
  if (row == -1) {

    /* we didn't find the item in the list */
    fprintf(stderr,
	    _("In s_toplevel_get_component_attribs_in_sheet, we didn't find the refdes in the master list!\n"));
    return NULL;
  }

  /* Now get all attribs associated with this refdes (in TABLE, indexed
   * by position), and insert them into new_attrib_list.  */
  new_attrib_list = s_string_list_new();  /* init new_attrib_list */

  i = 0;
  local_attrib_list = sheet_head->master_comp_attrib_list_head;
  while (local_attrib_list != NULL) {  /* iterate over all possible attribs */
    new_attrib_name = u_string_strdup(local_attrib_list->data);  /* take attrib name from column headings */

    if ( ((sheet_head->component_table)[i][row]).attrib_value ) {
      new_attrib_value = u_string_strdup( ((sheet_head->component_table)[i][row]).attrib_value );
      name_value_pair = u_string_concat(new_attrib_name, "=", new_attrib_value, NULL);
      GEDA_FREE(new_attrib_value);
    }
    else {
      name_value_pair = u_string_concat(new_attrib_name, "=", NULL);  /* empty attrib */
    }
    s_string_list_add_item(new_attrib_list, &count, name_value_pair);  /* add name=value to new list */
    GEDA_FREE(new_attrib_name);
    GEDA_FREE(name_value_pair);

    /* Sanity check */
    if (count != i+1) {
      /* for some reason, we have lost a name_value_pair somewhere . . .  */
      fprintf(stderr,
	      _("In s_toplevel_get_component_attribs_in_sheet, count != i!  Exiting . . . .\n"));
      return NULL;
    }

    /* iterate */
    i++;
    local_attrib_list = local_attrib_list->next;
  } /* while (local_attrib_list != NULL)  */

  return new_attrib_list;
}

/*------------------------------------------------------------------*/
/*! \brief Update component attributes in TOP_LEVEL
 *
 * For each attrib string attached to the component, update it using the value
 * held in new_comp_attrib_list.  Algorithm:
 * -# Form list of all component attribs held on both the component
 *    (o_current), as well as in the attrib list (SHEET_DATA).
 * -# Loop over name=value pairs held in complete_comp_attrib_list.
 * -# For each name=value pair, look for corresponding attrib on o_current.
 * -# For each name=value pair, look for the corresponding attrib in
 *    new_comp_attrib_list.
 * -# If the attrib exists on o_current and in new_comp_attrib_list, write the
 *    new value (from new_comp_attrib_list) into o_current.
 * -# If the attrib exists on o_current, but is null in name=value pair,
 *    delete the attrib from o_current.
 * -# If the attribs doesn't exist on o_current, but is non-null in
 *    the name=value pair, create an attrib object and add it to the part
 *    on o_current.
 * \param toplevel GedaToplevel structure
 * \param o_current Component (complex) to be updated.
 * \param new_comp_attrib_list list of name=value attribute pairs
 *                             from SHEET_DATA.
 */
void
s_toplevel_update_component_attribs_in_toplevel (
                                        GedaToplevel *toplevel,
                                        Object *o_current,
                                        STRING_LIST *new_comp_attrib_list)
{
  STRING_LIST *local_list;
  STRING_LIST *complete_comp_attrib_list;

  GList  *a_iter;
  Object *a_current;
  char   *old_name_value_pair;
  char   *new_attrib_name;
  char   *new_attrib_value;
  char   *old_attrib_name;
  char   *old_attrib_value;
  char   *refdes;

  int count = 0;  /* This is to fake out a function called later */
  int row, col;
  int visibility = 0;
  int show_name_value = 0;

#if DEBUG
  printf("-----  Entering s_toplevel_update_component_attribs_in_toplevel.\n");
#endif

  /*
   * To remove dead attribs from o_current, we need to form a complete list of unique
   * attribs by taking the union of the new attribs from the SHEET_DATA, and
   * the old attribs living on o_current.  That's what we're doing here.
   * Later, we can delete those attribs in o_current which don't apear in
   * new_comp_attrib_list.
   */
  /* First duplicate new_comp_attrib_list */
  complete_comp_attrib_list = s_string_list_duplicate_string_list(new_comp_attrib_list);

  /* Now create a complete list of unique attribute names.  This will be used in
  *  the loop below when updating attributes.  */
  a_iter = o_current->attribs;

  while (a_iter != NULL) {

    a_current = a_iter->data;

    if (a_current->type == OBJ_TEXT && a_current->text != NULL) {
      /* found a name=value attribute pair. */
      /* may need to check more thoroughly here. . . . */
      old_name_value_pair = u_string_strdup(a_current->text->string);

      /* Else clause is suggestion from Ales */
      old_attrib_name = u_string_split(old_name_value_pair, '=', 0);
      if ( (strcmp(old_attrib_name, "refdes") != 0) &&
        (strcmp(old_attrib_name, "net") != 0) &&
        (strcmp(old_attrib_name, "slot") != 0) &&
        (s_attrib_name_in_list(new_comp_attrib_list, old_attrib_name) == FALSE) ) {
        s_string_list_add_item(complete_comp_attrib_list, &count, old_name_value_pair);
        }
        else {

          int status;

          status = o_attrib_get_name_value (a_current, &old_attrib_name, &old_attrib_value);

          if (status == 0) {

            /* Don't put "refdes" or "slot" into list.  Don't put old name=value pair into list if a new
             * one is already in there. */
            if ( (strcmp(old_attrib_name, "refdes") != 0) &&
              (strcmp(old_attrib_name, "net") != 0) &&
              (strcmp(old_attrib_name, "slot") != 0) &&
              (s_attrib_name_in_list(new_comp_attrib_list, old_attrib_name) == FALSE) ) {
              s_string_list_add_item(complete_comp_attrib_list, &count, old_name_value_pair);
              }
              GEDA_FREE (old_attrib_name);
            GEDA_FREE (old_attrib_value);
          }
        }
        GEDA_FREE(old_name_value_pair);
        GEDA_FREE(old_attrib_name);
    }
    a_iter = g_list_next (a_iter);
  }  /* end while (a_current != NULL) */


  /*
   *Now the main business of this function:  updating the attribs attached to this o_current.
   * Loop on name=value pairs held in complete_comp_attrib_list , and then use this to get the
   * name=value pairs out of new_comp_attrib_list and from o_current.
   */

  /* First handle a special case: the component has no attribs (beside refdes). */
  if (complete_comp_attrib_list->data == NULL)
    return;

  /* Now the normal case. . . . */
  local_list = complete_comp_attrib_list;
  while (local_list != NULL) {

#if DEBUG
  printf("\n\n");
  printf("        In s_toplevel_update_component_attribs_in_toplevel, handling entry in complete list %s .\n",
	 local_list->data);
#endif

  /*  Now get the old attrib name & value from complete_comp_attrib_list
   *  and value from o_current  */
  old_attrib_name = u_string_split(local_list->data, '=', 0);
  old_attrib_value = o_attrib_search_attached_attribs_by_name (o_current, old_attrib_name, 0);

#if DEBUG
  printf("        In s_toplevel_update_component_attribs_in_toplevel, old name = \"%s\" .\n",
	 old_attrib_name);
  printf("        In s_toplevel_update_component_attribs_in_toplevel, old value = \"%s\" .\n",
	 old_attrib_value);
#endif

  /*  Next try to get this attrib from new_comp_attrib_list  */
  new_attrib_name = u_string_split(local_list->data, '=', 0);
  if (s_string_list_in_list(new_comp_attrib_list, local_list->data)) {
    new_attrib_value = s_misc_remaining_string(local_list->data, '=', 1);
  } else {
    new_attrib_value = NULL;
  }

#if DEBUG
  printf("        In s_toplevel_update_component_attribs_in_toplevel, new name = \"%s\" .\n",
	 new_attrib_name);
  printf("        In s_toplevel_update_component_attribs_in_toplevel, new value = \"%s\" .\n",
	 new_attrib_value);
#endif

  /* Now get row and col where this new attrib lives.  Then get
   * visibility of the new attrib stored in the component table */
  /* We'll need this later */
  refdes = u_string_strdup(s_attrib_get_refdes(o_current));
  row = s_table_get_index(sheet_head->master_comp_list_head, refdes);
  col = s_table_get_index(sheet_head->master_comp_attrib_list_head, new_attrib_name);
  /* if attribute has been deleted from the sheet, here is where we detect that */
  if ( (row == -1) || (col == -1) ) {
    new_attrib_value = NULL;  /* attrib will be deleted below */
  } else { /* we need a better place to get this info since the TABLE can be out of date */
    visibility = sheet_head->component_table[col][row].visibility;
    show_name_value = sheet_head->component_table[col][row].show_name_value;
  }

  GEDA_FREE(refdes);


    /* -------  Four cases to consider: Case 1 ----- */
  if ((old_attrib_value != NULL) && (new_attrib_value != NULL) &&
      (strlen(new_attrib_value) != 0) )
  {
    /* simply write new attrib into place of old one. */

#if DEBUG
    printf("     -- In s_toplevel_update_component_attribs_in_toplevel,\n");
    printf("               about to replace old attrib with name= %s, value= %s\n",
           new_attrib_name, new_attrib_value);
    printf("               visibility = %d, show_name_value = %d.\n",
           visibility, show_name_value);
#endif

    s_object_replace_attrib_in_object(toplevel,
                                      o_current,
                                      new_attrib_name,
                                      new_attrib_value,
                                      visibility,
                                      show_name_value);
  }

  /* -------  Four cases to consider: Case 2 ----- */
  else if ( (old_attrib_value != NULL) && (new_attrib_value == NULL) ) {
      /* remove attrib from component*/

#if DEBUG
    printf("     -- In s_toplevel_update_component_attribs_in_toplevel, about to remove old attrib with name= %s, value= %s\n",
	       old_attrib_name, old_attrib_value);
#endif

    s_object_release_attrib_in_object (toplevel, o_current, old_attrib_name);
  }

    /* -------  Four cases to consider: Case 3 ----- */
    else if ( (old_attrib_value == NULL) && (new_attrib_value != NULL) ) {
      /* add new attrib to component. */

#if DEBUG
      printf("     -- In s_toplevel_update_component_attribs_in_toplevel, about to add new attrib with name= %s, value= %s\n",
	     new_attrib_name, new_attrib_value);
#endif

      s_object_add_comp_attrib_to_object (toplevel,
                                          o_current,
                                          new_attrib_name,
                                          new_attrib_value,
                                          visibility,
                                          show_name_value);

      /* -------  Four cases to consider: Case 4 ----- */
    }
    else {

      /* Do nothing. */

#if DEBUG
      printf("     -- In s_toplevel_update_component_attribs_in_toplevel, nothing needs to be done.\n");
#endif
    }

    /* Toggle attribute visibility and name/value setting */


    /* free everything and iterate */
    GEDA_FREE(new_attrib_name);
    GEDA_FREE(new_attrib_value);
    GEDA_FREE(old_attrib_name);
    GEDA_FREE(old_attrib_value);
    local_list = local_list->next;
  }   /*   while (local_list != NULL)  */
  return;
}


/*------------------------------------------------------------------*/
/*!
 * \todo Function doesn't do anything - candidate for removal?
 */
STRING_LIST *s_toplevel_get_net_attribs_in_sheet(char *netname)
{
  /* must be filled in */
  return NULL;
}


/*------------------------------------------------------------------*/
/*!
 * \todo Function doesn't do anything - candidate for removal?
 */
void s_toplevel_update_net_attribs_in_toplevel(Object *o_current,
				   STRING_LIST *new_net_attrib_list)
{
  /* must be filled in */
  return;
}


/*------------------------------------------------------------------*/
/*! \brief Get pin attributes
 *
 * This function takes a pointer to the Object pin, and returns a list
 * of attribs found attached to the pin.  The returned list is a
 * STRING_LIST where the ->data holds a name=value string.
 * The algorithm is as follows:
 * -# Form refdes:pinnumber label for this pin.
 * -# Get row number of this refdes:pinnumber
 * -# Create a list of name=value pairs from entries in the pin_table
 *    on this row.
 * -# Return list of name=value pairs found.
 *
 * \param refdes Ref des string
 * \param pin Pin object
 * \returns name=value pair as a STRING_LIST
 */
STRING_LIST *s_toplevel_get_pin_attribs_in_sheet(char *refdes, Object *pin)
{
  STRING_LIST *new_attrib_list;
  STRING_LIST *local_attrib_list;
  int i;
  int row = -1;
  int count = 0;
  char *pinnumber;
  char *row_label;
  char *name_value_pair;
  char *new_attrib_value;
  char *new_attrib_name;

#if DEBUG
  printf("-----  Entering s_toplevel_get_pin_attribs_in_sheet.\n");
#endif

  /* First find pos of this pin in the master pin list */
  /* first convert refdes, pin to refdes:pinno text string. Then call table_get_index.  */

  pinnumber = o_attrib_search_object_attribs_by_name (pin, "pinnumber", 0);

  if ( (refdes != NULL) && (pinnumber != NULL) ) {
    row_label = u_string_concat(refdes, ":", pinnumber, NULL);
  }
  else {
    fprintf(stderr,
	    _("In s_toplevel_get_pin_attribs_in_sheet, either refdes or pinnumber of object missing!\n"));
    return NULL;
  }
  row = s_table_get_index(sheet_head->master_pin_list_head, row_label);

  /* Sanity check */
  if (row == -1) {
    /* we didn't find the item in the list */
    fprintf(stderr, "In s_toplevel_get_pin_attribs_in_sheet, didn't find the refdes:pin [%s]in the master list!\n",row_label );
    return NULL;
  }

  /* Now get all attribs associated with this refdes (in TABLE, indexed
   * by position), and insert them into new_attrib_list.  */
  new_attrib_list = s_string_list_new();  /* init new_attrib_list */

  i = 0;
  local_attrib_list = sheet_head->master_pin_attrib_list_head;
  while (local_attrib_list != NULL) {  /* iterate over all possible attribs */
    new_attrib_name = u_string_strdup(local_attrib_list->data);  /* take attrib name from column headings */

    if ( ((sheet_head->pin_table)[i][row]).attrib_value ) {
      new_attrib_value = u_string_strdup( ((sheet_head->pin_table)[i][row]).attrib_value );
      name_value_pair = u_string_concat(new_attrib_name, "=", new_attrib_value, NULL);
      GEDA_FREE(new_attrib_value);
    }
    else {
      name_value_pair = u_string_concat(new_attrib_name, "=", NULL);  /* empty attrib */
    }

    s_string_list_add_item(new_attrib_list, &count, name_value_pair);  /* add name=value to new list */
    GEDA_FREE(new_attrib_name);
    GEDA_FREE(name_value_pair);

    /* Sanity check */
    if (count != i+1) {
      /* for some reason, we have lost a name_value_pair somewhere . . .  */
      fprintf(stderr,
	      _("In s_toplevel_get_pin_attribs_in_sheet, count != i!  Exiting . . . .\n"));
      return NULL;
    }

    /* iterate */
    i++;
    local_attrib_list = local_attrib_list->next;
  } /* while (local_attrib_list != NULL)  */

  return new_attrib_list;
}

/*------------------------------------------------------------------*/
/*! \brief Update pin attributes in toplevel
 *
 * For each attrib string attached to the pin, update it using the value
 * held in new_pin_attrib_list.  Algorithm:
 * -# Loop over name=value pairs held in new_pin_attrib_list.
 * -# For each name=value pair, look for corresponding attrib on pin.
 * -# If the attrib exists on pin and in name=value pair, write the
 *    new value in.
 * -# If the attrib exists on pin, but is null in name=value pair,
 *    delete the attrib.
 * -# If the attribs doesn't exist on pin, but is non-null in
 *    the name=value pair, create an attrib object and add it to the pin.
 * \param toplevel GedaToplevel structure
 * \param refdes Unused - needs refactored out
 * \param [in,out] o_pin pin to update
 * \param [in] new_pin_attrib_list New pin attribute list to apply
 */
void
s_toplevel_update_pin_attribs_in_toplevel (GedaToplevel *toplevel,
                                           char *refdes,
                                           Object *o_pin,
                                           STRING_LIST *new_pin_attrib_list)
{
  STRING_LIST *local_list;
  char *new_name_value_pair;
  char *new_attrib_name;
  char *new_attrib_value;
  char *old_attrib_value;

#if DEBUG
  printf("-----  Entering s_toplevel_update_pin_attribs_in_toplevel.\n");
#endif

  /* loop on name=value pairs held in new_pin_attrib_list */
  local_list = new_pin_attrib_list;
  while (local_list != NULL) {
    new_name_value_pair = u_string_strdup(local_list->data);
#if DEBUG
  printf("        In s_toplevel_update_pin_attribs_in_toplevel, handling entry in master list %s .\n", new_name_value_pair);
#endif

  new_attrib_name = u_string_split(new_name_value_pair, '=', 0);
  new_attrib_value = u_string_split(new_name_value_pair, '=', 1);

  if (strlen(new_attrib_value) == 0) {
    GEDA_FREE(new_attrib_value);
    new_attrib_value = NULL;  /* s_misc_remaining_string doesn't return NULL for empty substring. */
  }
  old_attrib_value = o_attrib_search_attached_attribs_by_name (o_pin, new_attrib_name, 0);

  /* -------  Four cases to consider: Case 1: old and new attribs exist ----- */
  if ((old_attrib_value != NULL) && (new_attrib_value != NULL) &&
      (strlen(new_attrib_value) != 0) )
  {
      /* simply write new attrib into place of old one. */
#if DEBUG
      printf("In s_toplevel_update_pin_attribs_in_toplevel, about to replace old attrib with new one: name= %s, value= %s\n",
             new_attrib_name, new_attrib_value);
#endif
      s_object_replace_attrib_in_object(toplevel, o_pin, new_attrib_name,
                                                         new_attrib_value,
                                        LEAVE_VISIBILITY_ALONE,
                                        LEAVE_NAME_VALUE_ALONE);
  }

    /* -------  Four cases to consider: Case 2: old attrib exists, new one doesn't ----- */
    else if ( (old_attrib_value != NULL) && (new_attrib_value == NULL) ) {
      /* remove attrib from pin */
#if DEBUG
      printf("In s_toplevel_update_pin_attribs_in_toplevel, about to remove old attrib with name= %s, value= %s\n",
             new_attrib_name, old_attrib_value);
#endif
      s_object_release_attrib_in_object (toplevel, o_pin, new_attrib_name);
    }

    /* -------  Four cases to consider: Case 3: No old attrib, new one exists. ----- */
    else if ( (old_attrib_value == NULL) && (new_attrib_value != NULL) ) {
      /* add new attrib to pin. */

#if DEBUG
      printf("In s_toplevel_update_pin_attribs_in_toplevel, about to add new attrib with name= %s, value= %s\n",
             new_attrib_name, new_attrib_value);
#endif

      s_object_add_pin_attrib_to_object (toplevel,
                                         o_pin,
                                         new_attrib_name,
                                         new_attrib_value);

      /* -------  Four cases to consider: Case 4 ----- */
    }
    else {
      /* Do nothing. */

#if DEBUG
      printf("In s_toplevel_update_pin_attribs_in_toplevel, nothing needs to be done.\n");
#endif

    }

    /* free everything and iterate */
    GEDA_FREE(new_name_value_pair);
    GEDA_FREE(new_attrib_name);
    GEDA_FREE(new_attrib_value);
    GEDA_FREE(old_attrib_value);
    local_list = local_list->next;
  }   /*   while (local_list != NULL)  */

  return;
}

void s_toplevel_init_data_set(GedaToplevel *toplevel, PageDataSet *PageData) {

  /* ---------- Create and load the tables  ---------- */
  s_table_load_new_page(PageData);

  if (sort_components) {
    s_string_list_sort_all_list();
  }

  /* ---------- Now verify correctness of entire design.  ---------- */
  s_toplevel_verify_design(toplevel);  /* pr_current is a global */

}
