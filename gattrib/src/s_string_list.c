/* gEDA - GPL Electronic Design Automation
 * gattrib -- gEDA component and net attribute manipulation using spreadsheet.
 * Copyright (C) 2003-2015 Stuart D. Brorson.
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
/*! \file s_string_list.c
 *  \brief Functions involved in manipulating the STRING_LIST
 *         structure.
 *
 * This file holds functions involved in manipulating the STRING_LIST
 * structure.  STRING_LIST is basically a linked list of strings
 * (text).
 *
 * \todo This could be implemented using an underlying GList
 *       structure.  The count parameter could also be eliminated -
 *       either store it in the struct or preferably, calculate it
 *       when needed - I don't think the speed penalty of traversing
 *       the list is significant at all. GDE
 */

#include <gattrib.h>
#include <geda_debug.h>

/*------------------------------------------------------------------*/
/*! \brief Return a pointer to a new STRING_LIST
 *
 * Returns a pointer to a new STRING_LIST struct. This list is empty.
 * \returns pointer to the new STRING_LIST struct.
 */
STRING_LIST *s_string_list_new() {
  STRING_LIST *local_string_list;

  local_string_list = GEDA_MEM_ALLOC(sizeof(STRING_LIST));
  local_string_list->data = NULL;
  local_string_list->next = NULL;
  local_string_list->prev = NULL;
  local_string_list->pos = -1;   /* can look for this later . . .  */

  return local_string_list;
}

void s_string_list_free(STRING_LIST *strlist) {

  char *data;

  if (strlist != NULL) {

    while (strlist != NULL) {

      data = strlist->data;

      if (data != NULL) {
        GEDA_FREE(data);
      }

      strlist = strlist->next;
    }
  }

  return;
}

/*------------------------------------------------------------------*/
/*! \brief Duplicate a STRING_LIST
 *
 * Given a STRING_LIST, duplicate it and returns a pointer
 * to the new, duplicate list.
 * \param old_string_list pointer to the STRING_LIST to be duplicated
 * \returns a pointer to the duplicate STRING_LIST
 */
STRING_LIST *s_string_list_duplicate_string_list(STRING_LIST *old_string_list) {

  STRING_LIST *new_string_list;
  STRING_LIST *local_string_list;
  char *data;
  int count;

  new_string_list = s_string_list_new();

  if (old_string_list->data == NULL) {
    return new_string_list;  /* This is an empty string list */
  }

  local_string_list = old_string_list;

  while (local_string_list != NULL) {
    data = u_string_strdup(local_string_list->data);
    s_string_list_add_item(new_string_list, &count, data);
    GEDA_FREE(data);
    local_string_list = local_string_list->next;
  }

  return new_string_list;
}

/*------------------------------------------------------------------*/
/*! \brief Add an item to a STRING_LIST
 *
 * This function adds \a item into a STRING_LIST, after iterating
 * through the list to make sure that there are no duplications.
 *
 * \param [in,out] list  pointer to STRING_LIST to be added to.
 * \param [in,out] count total count on input, is updated if \a item added
 * \param [in]     item  pointer to string to be added
 */
void s_string_list_add_item(STRING_LIST *list, int *count, char *item) {

  STRING_LIST *prev;
  STRING_LIST *local_list;
  char        *trial_item = NULL;

  if (list == NULL) {
    fprintf(stderr, _("In s_string_list_add_item, tried to add to a NULL list.\n"));
    return;
  }

  /* First check to see if list is empty.  Handle insertion of first item
     into empty list separately.  (Is this necessary?) */
  if (list->data == NULL) {
    list->data = u_string_strdup(item);
    list->next = NULL;
    list->prev = NULL;   /* this may have already been initialized. . . . */
    list->pos  = *count; /* This enumerates the pos on the list.  Value is reset later by sorting. */
    (*count)++;          /* increment count to 1 */
    return;
  }

  /* Otherwise, loop through list looking for duplicates */
  prev = list;
  while (list != NULL) {

    trial_item = u_string_strdup(list->data);

    if (strcmp(trial_item, item) == 0) {
      /* Found item already in list.  Just return. */
      GEDA_FREE(trial_item);
      return;
    }

    GEDA_FREE(trial_item);

    prev = list;
    list = list->next;
  }

  /* If we are here, it's 'cause we didn't find the item pre-existing in the list. */
  /* In this case, we insert it. */

  local_list = (STRING_LIST *) GEDA_MEM_ALLOC(sizeof(STRING_LIST));  /* allocate space for this list entry */
  local_list->data = u_string_strdup(item);   /* copy data into list */
  local_list->next = NULL;
  local_list->prev = prev;  /* point this item to last entry in old list */
  prev->next = local_list;  /* make last item in old list point to this one. */
  local_list->pos = *count; /* This enumerates the pos on the list.  Value is reset later by sorting. */
  (*count)++;               /* increment count */
  /*   list = local_list;  */
  return;

}

/*------------------------------------------------------------------*/
/*! \brief Delete an item from a STRING_LIST
 *
 * Deletes an item in a STRING_LIST.
 * \param list pointer to STRING_LIST
 * \param count pointer to count of items in list
 * \param item item to remove from list
 */
void s_string_list_delete_item(STRING_LIST **list, int *count, char *item) {

  STRING_LIST *list_item;
  STRING_LIST *next_item = NULL;
  STRING_LIST *prev_item = NULL;
  char        *trial_item = NULL;

  /* First check to see if list is empty.  If empty, spew error and return */
  if ( (*list)->data == NULL) {
    fprintf(stderr, _("In s_string_list_delete_item, tried to remove item from empty list\n"));
    return;
  }

#ifdef DEBUG
    printf("In s_string_list_delete_item, about to delete item %s from list.\n", item);
#endif

  /* Now loop through list looking for item */
  list_item = (*list);
  while (list_item != NULL) {
    trial_item = u_string_strdup(list_item->data);

#ifdef DEBUG
    printf("In s_string_list_delete_item, matching item against trial item = %s from list.\n", trial_item);
#endif

    if (strcmp(trial_item, item) == 0) { /* found item, now delete it. */

#ifdef DEBUG
    printf("In s_string_list_delete_item, found match . . . . . \n");
#endif

    prev_item = list_item->prev;
    next_item = list_item->next;

    /* Check position in list */
    if (next_item == NULL && prev_item == NULL) {
      /* pathological case of one item list. */
      (*list) = NULL;
    }
    else if (next_item == NULL && prev_item != NULL) {
      /* at list's end */
      prev_item->next = NULL;
    }
    else if (next_item != NULL && prev_item == NULL) {
      /* at list's beginning */
      next_item->prev = NULL;
      (*list) = next_item;         /* also need to fix pointer to list head */
      /*  GEDA_FREE(list);  */
    }
    else {
      /* normal case of element in middle of list */
      prev_item->next = next_item;
      next_item->prev = prev_item;
    }

#ifdef DEBUG
    printf("In s_string_list_delete_item, now free list_item\n");
#endif

      GEDA_FREE(list_item);  /* free current list item */
      (*count)--;       /* decrement count */
      /* Do we need to re-number the list? */

#ifdef DEBUG
    printf("In s_string_list_delete_item, now free trial_item\n");
#endif
      GEDA_FREE(trial_item); /* free trial item before returning */
#ifdef DEBUG
    printf("In s_string_list_delete_item, returning . . . .\n");
#endif

      return;
    }
    GEDA_FREE(trial_item);
    list_item = list_item->next;
  }

  /* If we are here, it's 'cause we didn't find the item.
   * Spew error and return.
   */
  fprintf(stderr, _("In s_string_list_delete_item, couldn't delete item %s\n"), item);
  return;

}

/*------------------------------------------------------------------*/
/*! \brief Insert item into STRING_LIST
 *
 * Inserts a new string into a STRING_LIST. The string is not check
 * for dupilcation, caller should use s_string_list_in_list first
 *
 * \param list      pointer to STRING_LIST to be added to.
 * \param old_count pointer to integer with total count to be updated
 * \param pos       integer index where string is to be inserted into the list
 * \param item      pointer to string to be added
 */
void s_string_list_insert (STRING_LIST *list, int *old_count, int pos, char *item) {
    STRING_LIST *new_list;
    int index;
    char *str;
    int count = 0;

    if (pos == *old_count) /* if just appending */
      s_string_list_add_item(list, old_count, item);
    else {

      new_list = s_string_list_new();

      for ( index = 0; index < pos; index++) {
        str = s_string_list_get_data_at_index(list, index);
        s_string_list_add_item(new_list, &count, u_string_strdup(str));
      }

      s_string_list_add_item(new_list, &count, u_string_strdup(item));

      for ( index = pos; index < *old_count; index++) {
        str = s_string_list_get_data_at_index(list, index);
        s_string_list_add_item(new_list, &count, u_string_strdup(str));
      }

      s_string_list_free(list);
      *list = *new_list;
      *old_count = count;
    }
}

/*------------------------------------------------------------------*/
/*! \brief Detect item in list
 *
 * Look for item in the list.
 *
 * \param list pointer to the STRING_LIST struct
 * \param item string to search for
 * \returns 0 if absent, 1 if present
 */
int s_string_list_in_list(STRING_LIST *list, char *item) {

  /* First check to see if list is empty.  If empty, return
   * 0 automatically.  (I probably don't need to handle this
   * separately.)  */
  if (list->data == NULL) {
    return 0;
  }

  /* Otherwise, loop through list looking for duplicates */
  while (list != NULL) {

    if (strcmp(list->data, item) == 0) {
      /* Found item already in list.  return 1. */
      return 1;
    }

    list = list->next;
  }

  /* item was not in the list, so return 0 */
  return 0;

}


/*------------------------------------------------------------------*/
/*! \brief Get an item from a STRING_LIST by index
 *
 * Returns the index'th item in the string list.
 * \param list pointer to STRING_LIST to get from
 * \param index index of item to return
 * \returns NULL if there is a problem otherwise a pointer to
 *          the string.
 */
char *s_string_list_get_data_at_index(STRING_LIST *list, int index)
{
  int i;
  STRING_LIST *iter;

  iter = list;

  for (i = 0 ; i < index ; i++) {
    if (iter == NULL) {
      return NULL;
    }
    else {
      iter = iter->next;
    }
  }

  return iter->data;
}

/*------------------------------------------------------------------*/

/*! \brief Sort the master component list
 *
 *  \par Function Description
 * Takes the master comp list sheet_head->master_comp_list_head
 * and sorts the list by putting the refrence desiginators (refdes)
 * in alphabetical order.
 *
 * Right now it does nothing other than fill in the "position".
 *
 */
void s_string_list_sort_master_comp_list() {

  STRING_LIST *local_list, *iter;
  int i;

  /* Sort the list */
  local_list = sheet_head->master_comp_list_head;

  for (iter = local_list; iter; iter = iter->next) {
    iter->pos = 0;
  }

  local_list = listsort(local_list, 0, 1);

  i = 0;

  /* Reset the order of the individual items in the list. */
  while (local_list != NULL) {  /* make sure item is not null */

    local_list->pos = i;

    if (local_list->next != NULL) {
      i++;
      local_list = local_list->next;
    }
    else {
      break; /* leave loop *before* iterating to NULL EOL marker */
    }
  }

  /* Now go to first item in local list and reassign list head to
   * the new first element */
  while (local_list->prev) {
    local_list = local_list->prev;
  }

  sheet_head->master_comp_list_head = local_list;

  return;
}


/* This list overrides the alphanumeric sort.  Attribs not found in
   this list are sorted as if they had a value of DEFAULT_ATTRIB_POS
   within this list, but alphanumerically relative to each other.  */
static struct {
  const char *attrib;
  int pos;
} certain_attribs[] = {
  {"device", 1},
  {"footprint", 2},
  {"value", 3},
  {"symversion", 200}
};

#define NUM_CERTAINS (sizeof(certain_attribs)/sizeof(certain_attribs[0]))
#define DEFAULT_ATTRIB_POS 100

/*------------------------------------------------------------------*/
/*! \brief Sort the master component attribute list
 *
 * Take the master comp attrib list
 * sheet_head->master_comp_attrib_list_head
 * and sort it in this order:
 *
 * all refdeses in alphabetical order
 *
 * Right now it does nothing other than fill in the "position".
 */
void s_string_list_sort_master_comp_attrib_list() {

  int i;
  STRING_LIST *local_list, *iter;

  /* Here's where we do the sort */
  local_list = sheet_head->master_comp_attrib_list_head;

  /* Note that this sort is TBD -- it is more than just an alphabetic
   * sort because we want certain attribs to go first.
   */
  for (iter = local_list; iter; iter = iter->next) {

    int i;

    iter->pos = DEFAULT_ATTRIB_POS;

    for (i = 0; i < NUM_CERTAINS; i++) {

      if (iter->data != NULL) {

        if (strcmp (certain_attribs[i].attrib, iter->data) == 0) {

          iter->pos = certain_attribs[i].pos;
          break;
        }
      }
    }
  }

  local_list = listsort(local_list, 0, 1);
  sheet_head->master_comp_attrib_list_head = local_list;

  /* After sorting, reset the order of the individual items in list. */
  i = 0;
  while (local_list) {

    local_list->pos = i;
    i++;
    local_list = local_list->next;
  }

  return;
}

/*------------------------------------------------------------------*/
/*! \brief Sort the master netlist
 *
 * This fcn takes the master net list sheet_head->master_net_list_head
 * and sorts the list by putting all nets in alphabetical order
 */
void s_string_list_sort_master_net_list() {
  int i = 0;
  STRING_LIST *local_list;

  /* Do this after sorting is done.  This resets the order of the
   * individual items in the list. */
  local_list = sheet_head->master_net_list_head;

  while (local_list != NULL) {

    local_list->pos = i;
    i++;
    local_list = local_list->next;
  }

  return;
}

/*------------------------------------------------------------------*/
/*! \brief Sort the master net attribute list
 *
 * Take the master net attribute list
 * sheet_head->master_net_attrib_list_head
 * and sort it in this order:
 * value, footprint, model-name, file,
 * all other attributes in alphabetical order
 */
void s_string_list_sort_master_net_attrib_list() {
  int i = 0;
  STRING_LIST *local_list;

  /* Do this after sorting is done.  This resets the order of the
   * individual items in the list. */
  local_list = sheet_head->master_net_attrib_list_head;

  while (local_list != NULL) {
    local_list->pos = i;
    i++;
    local_list = local_list->next;
  }

  return;
}

/*------------------------------------------------------------------*/
/*! \brief Sort the master pin list
 *
 * Take the master pin list
 * sheet_head->master_pin_list_head
 * and sorts it in this order:
 *
 * all refdeses in alphabetical order
 *
 * Right now it does nothing other than fill in the "position".
 */
void s_string_list_sort_master_pin_list() {
  int i = 0;
  STRING_LIST *local_list, *iter;

  /* Here's where we do the sort.  The sort is done using a fcn found on the web. */
  local_list = sheet_head->master_pin_list_head;
  for (iter = local_list; iter; iter=iter->next)
    iter->pos = 0;
  local_list = listsort(local_list, 0, 1);

  /* Do this after sorting is done.  This resets the order of the individual items
   * in the list.  */
  while (local_list != NULL) {  /* make sure item is not null */

    local_list->pos = i;

    if (local_list->next != NULL) {
      i++;
      local_list = local_list->next;
    }
    else {
      break;                    /* leave loop *before* iterating to NULL EOL marker */
    }
  }

  /* After assign list head to new first element */
  while (local_list->prev) {
    local_list = local_list->prev;
  }

  sheet_head->master_pin_list_head = local_list;

  return;
}

/*------------------------------------------------------------------*/
/*! \brief Sort the master pin attribute list
 *
 * Takes the master pin attrib list
 * sheet_head->master_pin_attrib_list_head
 * and sorts it in this order:
 *
 * all pin attribs in alphabetical order
 *
 * Right now it does nothing other than fill in the "position".
 */
void s_string_list_sort_master_pin_attrib_list() {
  int i = 0;
  STRING_LIST *local_list;

  /* Here's where we do the sort */

  /*
   * Note that this sort is TBD -- it is more than just an alphabetic sort 'cause we want
   * certain attribs to go first.
   */

  /* Do this after sorting is done.  This resets the order of the individual items
   * in the list.  */
  local_list = sheet_head->master_pin_attrib_list_head;
  while (local_list != NULL) {
    local_list->pos = i;
    i++;
    local_list = local_list->next;
  }

  return;
}

void s_string_list_sort_all_list() {
   /* ---------- Sort the master lists  ---------- */
  s_string_list_sort_master_comp_list();
  s_string_list_sort_master_comp_attrib_list();

#if 0
  /* Note that this must be changed.  We need to input the entire project
   * before doing anything with the nets because we need to first
   * determine where they are all connected!   */
  s_string_list_sort_master_net_list();
  s_string_list_sort_master_net_attrib_list();
#endif

  s_string_list_sort_master_pin_list();
  s_string_list_sort_master_pin_attrib_list();
}
