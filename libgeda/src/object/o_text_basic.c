/* gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's library
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

/*! \file o_text_basic.c
 *  \brief functions for the text and fonts
 *
 *  \par The font definitions
 *
 *  Each letter of the font is defined in a single font symbol file. In
 *  the font symbol file, the character width is defined in the second
 *  line. The first line contains the file format version.
 *
 *  All remaining lines are basic graphical lines. They build the
 *  appearance of the character.
 *
 *  \image html o_text_font_overview.png
 *  \image latex o_text_font_overview.pdf "font overview" width=14cm
 *
 *  The height of capital characters in the font files is 26. The size
 *  of small letters is 16. The space below the zero line is used by
 *  characters like <b>g</b>, <b>p</b> or <b>q</b>. The space above 26
 *  is used by diacritic marks like accents, breve, circumflex mostly in
 *  european characters.
 *
 *  \par The text definitions
 *
 *  The text is stored and printed in several different representations.
 *
 *  In the gEDA files the text is just a string. It is stored unmodified
 *  in <b>Object->text->string</b>.
 *
 *  If the string is an attribute with an equal sign as delimiter between
 *  an attribute name and an attribute value, then it is possible to
 *  hide some parts of the text. The still visible part of an attribute
 *  is stored in <b>Object->text->disp_string</b>.
 *
 *  \image html o_text_text_overview.png
 *  \image latex o_text_text_overview.pdf "text overview" width=14cm
 *
 *  To draw the text in gschem, the string is interpreted and converted
 *  to a list of basic graphical objects. The basic line objects are
 *  collected from the font character objects.
 */

#include <config.h>
#include <missing.h>

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "libgeda_priv.h"

/*! \brief Scale factor between legacy gschem font units and postscript points.
 *
 *  \par Description
 *  gschem fonts are nominally specified in points, however there is a
 *  difference in how the specified font size corresponds to the metrics of
 *  the font when compared to typical typographic usage.
 *
 *  The following factor was empirically determined to approximately match the
 *  cap-height between the legacy gschem font, and fonts rendered using pango.
 */
#define GEDA_FONT_FACTOR 1.3

/*! \brief Scale factor font height and line-spacing (for print only)
 *
 *  \par Description
 *  Specifies the scale factor between the nominal font size and the inter-
 *  line spacing used to render it when printing.
 */
#define PRINT_LINE_SPACING 1.12

/*! Size of a tab in characters */
int tab_in_chars = 8;

/*! \brief update the visible part of a string
 *
 *  \par Function Description
 *  If a string is an attribute, then it is possible to hide the name or
 *  the value part of the attribute string. This functions updates the
 *  text->disp_string according to the object->show_name_value settings
 *
 *  \param [in] object  The Object to update
 */
static void update_disp_string (Object *object)
{
  char *name  = NULL;
  char *value = NULL;
  Text *text  = object->text;

  GEDA_FREE (text->disp_string);

  if (o_attrib_get_name_value (object, &name, &value)) {
    switch (object->show_name_value) {
      case (SHOW_NAME_VALUE):
        text->disp_string = g_strdup (text->string);
        break;

      case (SHOW_NAME):
        if (name[0] != '\0') {
          text->disp_string = g_strdup (name);
        } else {
          g_critical ("Got an improper attribute: %s\n",
                      text->string);
          text->disp_string = g_strdup ("invalid");
        }
        break;

      case (SHOW_VALUE):
        if (value[0] != '\0') {
          text->disp_string = g_strdup(value);
        } else {
          g_critical ("Got an improper attribute: %s\n",
                      text->string);
          text->disp_string = g_strdup ("invalid");
        }
        break;
    }
    /* free the strings allocated by o_attrib_get_name_value */
    GEDA_FREE(name);
    GEDA_FREE(value);
  }
  else {
    text->disp_string = g_strdup (text->string);
  }
}

/*! \brief get the position of a text object
 *
 *  \par Function Description
 *  This function gets the position of the base point of a text object.
 *
 *  \param [out] x       pointer to the x-position
 *  \param [out] y       pointer to the y-position
 *  \param [in] object   The object to get the position.
 *
 *  \return TRUE if successfully determined the position, FALSE otherwise
 */
bool o_text_get_position (int *x, int *y, Object *object)
{
  *x = object->text->x;
  *y = object->text->y;
  return TRUE;
}

/*! \brief count the lines of a text string
 *
 *  \par Function Description
 *  This function just counts the number of lines that are
 *  in the \a string.

 *  \param [in] string  text string to count the lines
 *
 *  \return the number of lines
 */
int o_text_num_lines(const char *string)
{
  int line_count = 0;
  const char *aux;
  gunichar current_char;

  if (string == NULL) {
    return 0;
  }

  /* if it's not null, then we have at least one line */
  line_count++;
  /* Count how many \n are in the string */
  aux = string;
  while (aux && ((gunichar) (*aux) != 0) ) {
    current_char = g_utf8_get_char_validated(aux, -1);
    if (current_char == '\n')
      line_count++;
    aux = g_utf8_find_next_char(aux, NULL);
  }

  return (line_count);
}


/*! \brief Creates a text Object and the graphical objects representing it
 *  \par Function Description
 *  Create an Object of type OBJ_TEXT.
 *
 *  \param [in]  color                  The color of the text.
 *  \param [in]  x                      World x coord of text.
 *  \param [in]  y                      World y coord of text.
 *  \param [in]  alignment              How text bounding box aligns on (x, y).
 *  \param [in]  angle                  Angle at which text will appear.
 *  \param [in]  string                 The text (TODO: can be char const *)!
 *  \param [in]  size                   Text size.
 *  \param [in]  visibility             VISIBLE or INVISIBLE.
 *  \param [in]  show_name_value        SHOW_NAME_VALUE or friends.
 *  \return Pointer to text Object.
 *
 *  \note
 *  Caller is responsible for string; this function allocates its own copy.
 */
Object*
o_text_new(int color, int x, int y, int alignment, int angle, const char *string,
           int size, int visibility, int show_name_value)
{
  Object *new_obj=NULL;
  Text   *text;

  if (string == NULL) {
    return(NULL);
  }

  new_obj = geda_text_new();

  text = GEDA_TEXT(new_obj);

  text->string      = g_strdup (string);
  text->disp_string = NULL; /* We'll fix this up later */
  text->length      = strlen(string);
  text->size        = size;
  text->alignment   = alignment;
  text->x           = x;
  text->y           = y;
  text->angle       = angle;

  new_obj->color    = color;

  o_set_visibility (new_obj, visibility);
  new_obj->show_name_value = show_name_value;

  /* Call directly so no emmision */
  update_disp_string (new_obj);

  return new_obj;
}

/*! \brief read a text object from a char buffer
 *
 *  \par Function Description
 *  This function reads a text object from the textbuffer \a tb and
 *  the text starting with the line \a firstline.
 *  If the line object was read successfully, a new object is
 *  create and appended to the \a object_list.
 *
 *  \param [in] first_line     the first line of the text
 *  \param [in] tb             Text buffer (usually a line of a schematic file)
 *  \param [in] release_ver    The release number gEDA
 *  \param [in] fileformat_ver A integer value of the file format
 *
 *  \param [out] err           A GError obejct
 *
 *  \return The object list, or NULL on error.
 */
Object *
o_text_read (const char *first_line, TextBuffer *tb, unsigned int release_ver,
             unsigned int fileformat_ver, GError **err)
{
  Object *new_obj;
  char type;
  int x, y;
  int color;
  int size;
  int visibility;
  int show_name_value;
  int angle;
  int alignment;
  int num_lines = 0;
  int i;
  char* string = NULL;
  GString *textstr;

  if (fileformat_ver >= 1) {
    if (sscanf(first_line, "%c %d %d %d %d %d %d %d %d %d\n", &type, &x, &y,
      &color, &size, &visibility, &show_name_value,&angle, &alignment, &num_lines) != 10)
    {
      g_set_error(err, EDA_ERROR, EDA_ERROR_PARSE, _("Failed to parse text object"));
      return NULL;
    }
  }
  else if (release_ver < VERSION_20000220) {
    /* yes, above less than (not less than and equal) is correct. The format */
    /* change occurred in 20000220 */
    if (sscanf(first_line, "%c %d %d %d %d %d %d %d\n", &type, &x, &y,
      &color, &size,
      &visibility, &show_name_value,
      &angle) != 8) {
      g_set_error(err, EDA_ERROR, EDA_ERROR_PARSE, _("Failed to parse text object"));
    return NULL;
      }
      alignment = LOWER_LEFT; /* older versions didn't have this */
      num_lines = 1; /* only support a single line */
  } else {
    if (sscanf(first_line, "%c %d %d %d %d %d %d %d %d\n", &type, &x, &y,
      &color, &size,
      &visibility, &show_name_value,
      &angle, &alignment) != 9) {
      g_set_error (err, EDA_ERROR, EDA_ERROR_PARSE, _("Failed to parse text object"));
    return NULL;
      }
      num_lines = 1; /* only support a single line */
  }

  if (size == 0) {
    u_log_message(_("Found a zero size text string [ %c %d %d %d %d %d %d %d %d ]\n"),
                  type, x, y, color, size, visibility, show_name_value, angle, alignment);
  }

  switch(alignment) {
    case(LOWER_LEFT):
    case(MIDDLE_LEFT):
    case(UPPER_LEFT):
    case(LOWER_MIDDLE):
    case(MIDDLE_MIDDLE):
    case(UPPER_MIDDLE):
    case(LOWER_RIGHT):
    case(MIDDLE_RIGHT):
    case(UPPER_RIGHT):

      break;

    default:
      u_log_message(_("Found an unsupported text alignment [ %c %d %d %d %d %d %d %d %d ]\n"),
                    type, x, y, color, size, visibility, show_name_value, angle, alignment);
      u_log_message(_("Setting alignment to LOWER_LEFT\n"));
      alignment = LOWER_LEFT;
      break;
  }

  if (color < 0 || color > MAX_COLORS) {
    u_log_message(_("Found an invalid color [ %s ]\n"), first_line);
    u_log_message(_("Setting color to default color\n"));
    color = DEFAULT_TEXT_COLOR_INDEX;
  }
  /* g_assert(num_lines && num_lines > 0) */

  textstr = g_string_new ("");
  for (i = 0; i < num_lines; i++) {
    const char *line;

    line = s_textbuffer_next_line (tb);

    if (line == NULL) {
      g_string_free (textstr, TRUE);
      g_set_error(err, EDA_ERROR, EDA_ERROR_PARSE, _("Unexpected end-of-file after %d lines"), i);
      return NULL;
    }

    textstr = g_string_append (textstr, line);
  }
  /* retrieve the character string from the GString */
  string = g_string_free (textstr, FALSE);

  string = remove_last_nl(string);

  /* convert the character string to UTF-8 if necessary */
  if (!g_utf8_validate (string, -1, NULL)) {
    /* if it is not utf-8, it is ISO_8859-15 */
    char *tmp = g_convert (string, strlen (string),
                           "UTF-8", "ISO_8859-15",
                           NULL, NULL, NULL);
    if (tmp == NULL) {
      fprintf (stderr, "Failed to convert text string to UTF-8: %s.\n",
               string);
    } else {
      /* successfully converted string, now use tmp as string */
      GEDA_FREE (string);
      string = tmp;
    }
  }

  new_obj = o_text_new(color, x, y,
                       alignment, angle, string,
                       size, visibility, show_name_value);
  GEDA_FREE(string);

  return new_obj;
}

/*! \brief Create a string representation of the text object
 *  \par Function Description
 *  This function takes a text \a object and return a string
 *  according to the file format definition.
 *
 *  \param [in] object  a text Object
 *  \return the string representation of the text Object
 */
char *o_text_save(Object *object)
{
  char *buf;
  char *string;

  int   x, y;
  int   size;
  int   num_lines;
  int   visibility;

  x = object->text->x;
  y = object->text->y;

  string = object->text->string;
  size = object->text->size;

  /* string can have multiple lines (seperated by \n's) */
  num_lines = o_text_num_lines(string);

  /* Don't save invisible == 2 as visible */
  visibility = (object->visibility == VISIBLE) ? VISIBLE : INVISIBLE;

  buf = g_strdup_printf ("%c %d %d %d %d %d %d %d %d %d\n%s", object->type,
                         x, y, object->color, size, visibility,
                         object->show_name_value, object->text->angle,
                         object->text->alignment, num_lines, string);

  return(buf);
}

/*! \brief recreate the graphics of a text object
 *  \par Function Description
 *  This function updates the underlying primary of the text object
 *  \a o_current.
 *
 *  \param o_current The text object to update
 */
void o_text_recreate(Object *o_current)
{
  Page *page;

  o_emit_pre_change_notify(o_current);
  update_disp_string(o_current);

  if (!geda_object_bounds(o_current)) {
    o_current->w_bounds_valid_for = NULL;
  }

  o_emit_change_notify(o_current);

  page = geda_object_get_page(o_current);
  if (page) {
    page->CHANGED = 1;
  }

}

/*! \brief move a text object
 *
 *  \par Function Description
 *  This function changes the position of a text object \a o_current.
 *
 *  \param [in] dx           The x-distance to move the object
 *  \param [in] dy           The y-distance to move the object
 *  \param [in] o_current    The text Object to be moved
 */
void o_text_translate_world(int dx, int dy, Object *o_current)
{
  o_current->text->x = o_current->text->x + dx;
  o_current->text->y = o_current->text->y + dy;

  /* Update bounding box */
  o_current->w_bounds_valid_for = NULL;
}

/*! \brief create a copy of a text object
 *  \par Function Description
 *  This function creates a copy of the text object \a o_current.
 *
 *  \param [in] o_current    The object that is copied
 *  \return a new text object
 */
Object *o_text_copy(Object *o_current)
{
  Object *new_obj;
  Text   *text_obj;

  new_obj = o_text_new (o_current->color,
                        o_current->text->x, o_current->text->y,
                        o_current->text->alignment,
                        o_current->text->angle,
                        o_current->text->string,
                        o_current->text->size,
                        o_get_is_visible (o_current) ? VISIBLE : INVISIBLE,
                        o_current->show_name_value);

  text_obj = GEDA_TEXT(o_current);

  if (text_obj->rendered_text_bounds_func != NULL) {
    o_text_set_rendered_bounds_func (new_obj,
                                     text_obj->rendered_text_bounds_func,
                                     text_obj->rendered_text_bounds_data);
  }
  return new_obj;
}
/*! \brief Reset the refdes number back to a question mark
 *
 *  \par If this text object represents a refdes attribute,
 *  then this function resets the refdes number back to the
 *  question mark. In other cases, this function does
 *  nothing.
 *
 *  \param [in] object      The text object
 */
void o_text_reset_refdes(Object *object)
{
  int   len;
  int   index;
  char  buffer[16] = "refdes=\0";
  char *ptr;

  g_return_if_fail (GEDA_IS_TEXT(object));

  len   = 0;
  index = 7;

  ptr   = object->text->string;

  if ( strncmp ( ptr, &buffer[0], index) == 0 ) {

    len = strlen (object->text->string);

    for ( ; index < len; index++) {
      if ( isdigit(ptr[index]) ) {
        GEDA_FREE (object->text->string);
        buffer[index] = '?';
        buffer[++index] = '\0';
        object->text->string= strdup(&buffer[0]);
        o_text_recreate (object);
        break;
      }
      else
        buffer[index] = ptr[index];
    }
  }
}

/*! \brief write a text string to a postscript file
 *  \par Function Description
 *  This function writes the single \a string into the postscript file \a fp.
 *
 *  \param [in] fp           pointer to a FILE structure
 *  \param [in] string       The string to print
 *  \param [in] unicode_count Number of items in the unicode table
 *  \param [in] unicode_table Table of unicode items
 *
 *  \todo investigate whether the TAB character is handled correctly
 */
void o_text_print_text_string(FILE *fp, char *string, int unicode_count,
                              gunichar *unicode_table)
{
  int j;
  char *aux;
  gunichar current_char, c;

  if (!string)
  {
    return;
  }

  aux = string;

  fprintf(fp, "(");

  while (aux && ((gunichar) (*aux) != 0)) {
    current_char = g_utf8_get_char_validated(aux, -1);
    if (current_char == '(' || current_char == ')' || current_char == '\\') {
      fprintf(fp, "\\");
    }

    c = current_char;
    if (c >= 128) {
      current_char = '?';
      if (unicode_count)  {
        for (j = 0; j < unicode_count; j++)
          if (c == unicode_table[j]) {
            current_char = j + 128;
            break;
          }
      }
    }


    if (current_char == '\t') {
      /* Output eight spaces instead of the tab character */
      fprintf(fp, "       ");
    } else {
      fprintf(fp, "%c", current_char);
    }

    aux = g_utf8_find_next_char(aux, NULL);
  }

  fprintf(fp,") ");
}


/*! \brief print a text object into a postscript file
 *  \par Function Description
 *  This function writes the postscript representation of the text object
 *  \a o_current into the the file \a fp.
 *  \param [in] toplevel     The GedaToplevel object
 *  \param [in] fp           pointer to a FILE structure
 *  \param [in] o_current    The Object to print
 *  \param [in] origin_x     x-coord of the postscript origin
 *  \param [in] origin_y     y-coord of the postscript origin
 *  \param [in] unicode_count Number of items in the unicode table
 *  \param [in] unicode_table Table of unicode items
 */
void o_text_print(GedaToplevel *toplevel, FILE *fp, Object *o_current,
                  int origin_x, int origin_y,
                  int unicode_count, gunichar *unicode_table)
{
  int alignment;
  char *centering_control = NULL;
  char *p,*s;
  char *output_string = NULL;
  char *name = NULL;
  char *value = NULL;
  int x, y, angle, len;
  float font_size;


  if (!o_current->text->string) {
    return;
  }

  f_print_set_color(toplevel, fp, o_current->color);

  if (o_attrib_get_name_value (o_current, &name, &value)) {
    switch(o_current->show_name_value) {
      case(SHOW_NAME_VALUE):
        output_string = g_strdup(o_current->text->string);
        break;

      case(SHOW_NAME):
        if (name[0] != '\0') {
          output_string = g_strdup(name);
        } else {
          fprintf(stderr,"Got an improper attribute: %s\n",
                  o_current->text->string);
          output_string = g_strdup("invalid");
        }
        break;

      case(SHOW_VALUE):
        if (value[0] != '\0') {
          output_string = g_strdup(value);
        } else {
          /* you probably can remove this now... */
          /* since improper attributes will never get here */
          fprintf(stderr, "Got an improper attribute: %s\n",
                  o_current->text->string);
          output_string = g_strdup("invalid");
        }
        break;

    default:
      g_return_if_reached ();

    }
  } else {
    output_string = g_strdup(o_current->text->string);
  }

  /* Apply alignment map to apply when text is 180 degrees rotated.
   * We want the text on the printer to appear upside right, even
   * though mathematically it aught to be upside down.  To make this
   * work, we will reset the angle to 0, when it's equal to 180
   * degrees, then apply a transformation to the origin location as if
   * the text was rotated about that point.  E.g. if the text origin
   * was at the lower left, and the text was rotated by 180 degrees,
   * it would be as if the origin was at the upper right. The same
   * reasoning has been applied to all 8 other text origins.
   * MIDDLE_MIDDLE maps to itself.
   */
  alignment = o_current->text->alignment;
  angle = o_current->text->angle;
  if(angle == 180) {
    angle = 0;        /* reset angle to 0 to make text upright */
    switch(alignment) {
    case(LOWER_LEFT):    alignment = UPPER_RIGHT;
      break;
    case(MIDDLE_LEFT):   alignment = MIDDLE_RIGHT;
      break;
    case(UPPER_LEFT):    alignment = LOWER_RIGHT;
      break;
    case(LOWER_MIDDLE):  alignment = UPPER_MIDDLE;
      break;
    case(MIDDLE_MIDDLE): alignment = MIDDLE_MIDDLE;
      break;
    case(UPPER_MIDDLE):  alignment = LOWER_MIDDLE;
      break;
    case(LOWER_RIGHT):   alignment = UPPER_LEFT;
      break;
    case(MIDDLE_RIGHT):  alignment = MIDDLE_LEFT;
      break;
    case(UPPER_RIGHT):   alignment = LOWER_LEFT;
      break;
    }
  }

  /* Create an appropriate control string for the centering. */
  switch(alignment) {
                                       /* hcenter rjustify vcenter vjustify */
  case(LOWER_LEFT):    centering_control = "false false false false";
    break;
  case(MIDDLE_LEFT):   centering_control = "false false true false";
    break;
  case(UPPER_LEFT):    centering_control = "false false false true";
    break;
  case(LOWER_MIDDLE):  centering_control = "true false false false";
    break;
  case(MIDDLE_MIDDLE): centering_control = "true false true false";
    break;
  case(UPPER_MIDDLE):  centering_control = "true false false true";
    break;
  case(LOWER_RIGHT):   centering_control = "false true false false";
    break;
  case(MIDDLE_RIGHT):  centering_control = "false true true false";
    break;
  case(UPPER_RIGHT):   centering_control = "false true false true";
    break;
  }

  font_size = o_text_get_font_size_in_points (o_current)
                / 72.0 * 1000.0;
  fprintf(fp,"%s %f [",centering_control, font_size * PRINT_LINE_SPACING);

  /* split the line at each newline and print them */
  p = output_string;   /* Current point */
  s = output_string;   /* Start of the current string */
  len = strlen(output_string)+1;
  while(len != 0) {
    /* Have we reached the end of a line? */
    if((*p == '\n') || (*p == '\0')) {
      /* Yes, replace the newline with a NULL and output the string */
      *p = '\0';
      o_text_print_text_string(fp,s,unicode_count,unicode_table);
      /* Update output string start for next string */
      s = p+1; /* One past the current character. */
    }
    p++;   /* Advance to next character */
    len--; /* Keep track of how many characters left to process */
  }

  /* Finish up with the rest of the text print command */
  /* Collect pertinent info about the text location */
  x = o_current->text->x;
  y = o_current->text->y;
  fprintf(fp,"] %d %d %d %f text\n",angle,x,y,font_size);

  GEDA_FREE(output_string);
  GEDA_FREE(name);
  GEDA_FREE(value);
}


/*! \brief rotate a text object around a centerpoint
 *
 *  \par Function Description
 *  This function rotates a text \a object around the point
 *  (\a world_centerx, \a world_centery).
 *
 *  \param [in] world_centerx x-coord of the rotation center
 *  \param [in] world_centery y-coord of the rotation center
 *  \param [in] angle         The angle to rotate the text object
 *  \param [in] object        The text object
 *  \note only steps of 90 degrees are allowed for the \a angle
 */
void
o_text_rotate_world(int world_centerx, int world_centery, int angle, Object *object)
{
  int x, y;
  int newx, newy;

  if(GEDA_IS_TEXT(object)) {

    object->text->angle = ( object->text->angle + angle ) % 360;

    x = object->text->x + (-world_centerx);
    y = object->text->y + (-world_centery);

    rotate_point_90(x, y, angle, &newx, &newy);

    x = newx + (world_centerx);
    y = newy + (world_centery);

    o_text_translate_world(x-object->text->x, y-object->text->y, object);

    o_text_recreate(object);
  }
  else {
    BUG_MSG("o_glist_rotate_world");
  }
}

/*! \brief mirror a text object horizontaly at a centerpoint
 *
 *  \par Function Description
 *  This function mirrors a text \a object horizontaly at the point
 *  (\a world_centerx, \a world_centery).
 *
 *  \param [in] world_centerx x-coord of the mirror position
 *  \param [in] world_centery y-coord of the mirror position
 *  \param [in] object        The text object
 */
void
o_text_mirror_world(int world_centerx, int world_centery, Object *object)
{
  int origx, origy;
  int x, y;

  origx = object->text->x;
  origy = object->text->y;

  x = origx + (-world_centerx);
  y = origy + (-world_centery);

  if ((object->text->angle%180)==0) {
    switch(object->text->alignment) {
      case(LOWER_LEFT):
        object->text->alignment=LOWER_RIGHT;
        break;

      case(MIDDLE_LEFT):
        object->text->alignment=MIDDLE_RIGHT;
        break;

      case(UPPER_LEFT):
        object->text->alignment=UPPER_RIGHT;
        break;

      case(LOWER_RIGHT):
        object->text->alignment=LOWER_LEFT;
        break;

      case(MIDDLE_RIGHT):
        object->text->alignment=MIDDLE_LEFT;
        break;

      case(UPPER_RIGHT):
        object->text->alignment=UPPER_LEFT;
        break;

      default:
        break;
    }
  }
  else {
    switch(object->text->alignment) {
      case(LOWER_LEFT):
      object->text->alignment=UPPER_LEFT;
      break;

      case(UPPER_LEFT):
      object->text->alignment=LOWER_LEFT;
      break;

      case(LOWER_RIGHT):
      object->text->alignment=UPPER_RIGHT;
      break;

      case(UPPER_RIGHT):
      object->text->alignment=LOWER_RIGHT;
      break;

      case(LOWER_MIDDLE):
      object->text->alignment=UPPER_MIDDLE;
      break;

      case(UPPER_MIDDLE):
      object->text->alignment=LOWER_MIDDLE;
      break;

      default:
      break;
    }
  }

  object->text->x = -x + (world_centerx);
  object->text->y =  y + (world_centery);

  o_text_recreate(object);
}

/*! \brief Calculates the distance between the given point and the closest
 *  point on the text.
 *
 *  This function will calculate the distance to the text regardless
 *  if the text is visible or not.
 *
 *  \param [in] object       A text Object.
 *  \param [in] x            The x coordinate of the given point.
 *  \param [in] y            The y coordinate of the given point.
 *  \param [in] force_solid  If true, force treating the object as solid.
 *
 *  \return The shortest distance from the object to the point. If the
 *  distance cannot be calculated, this function returns a really large
 *  number (G_MAXDOUBLE).  With an invalid parameter, this funciton
 *  returns G_MAXDOUBLE.
 */
double o_text_shortest_distance (Object *object,
                                 int x, int y, int force_solid)
{
  int left, top, right, bottom;
  double dx, dy;

  g_return_val_if_fail (object->text != NULL, G_MAXDOUBLE);

  if (!world_get_single_object_bounds(object, &left, &top, &right, &bottom))
    return G_MAXDOUBLE;

  dx = min (x - left, right - x);
  dy = min (y - top, bottom - y);

  dx = min (dx, 0);
  dy = min (dy, 0);

  return sqrt ((dx * dx) + (dy * dy));
}

/*! \brief Return font size of a text object in postscript points.
 *
 *  \par Description
 *  gEDA fonts are specified in a non-standard unit. This
 *  function applies an appopriate scaling to return the
 *  font size in postscript points.
 *
 *  \param [in] object    The text Object whos font size to return
 *  \return The font size converted to postscript points.
 */
double o_text_get_font_size_in_points (Object *object)
{
  g_return_val_if_fail (object->type == OBJ_TEXT, 0.);

  return object->text->size * GEDA_FONT_FACTOR;
}

/*! \brief Get the string displayed by a text object
 *
 *  \par Function Description
 *  Retrieve the text string from a text object. The returned string
 *  should be treated as constant.
 *
 *  \param [in]  obj  The text object
 *
 *  \return The text object's string, or NULL on failure.
 */
const char *o_text_get_string (Object *obj)
{

  g_return_val_if_fail (GEDA_IS_TEXT(obj), NULL);
  return obj->text->string;
}

/*! \brief Set the font-renderer-specific bounds function.
 *  \par Function Description
 *  Set the function to be used to calculate text bounds for a given
 *  #Text Object. This allow a per text object renderer function to
 *  be defined. If the function is not defined the renderer for the
 *  Page will be used instead, if the Page rennderer is defined.
 *
 *  \param [in] object    The GedaToplevel object
 *  \param [in] func      Function to use.
 *  \param [in] user_data User data to be passed to the function.
 */
void o_text_set_rendered_bounds_func (Object *object,
                                      RenderedBoundsFunc func,
                                      void *user_data) {
  g_return_if_fail(GEDA_IS_TEXT(object));
  Text *text = GEDA_TEXT(object);
  text->rendered_text_bounds_func = func;
  text->rendered_text_bounds_data = user_data;
}

/*! \brief Set the string displayed by a text object.
 *  \par Function Description
 *  Updates the text object with a new text string.
 *
 *  \param [in]  object      The text object.
 *  \param [in]  new_string  The new value.
 */
void
o_text_set_string (Object *object, const char *new_string)
{
  g_return_if_fail(GEDA_IS_TEXT(object));
  g_return_if_fail (new_string != NULL);

  GEDA_FREE (object->text->string);
  object->text->string = g_strdup (new_string);

  o_text_recreate (object);

}