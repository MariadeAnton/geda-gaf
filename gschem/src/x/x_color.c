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
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <math.h>

#include "gschem.h"

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

COLOR display_colors[MAX_COLORS];
COLOR outline_colors[MAX_COLORS];

static GdkColor* gdk_colors[MAX_COLORS];
static GdkColor* gdk_outline_colors[MAX_COLORS];

static GdkColormap *colormap = NULL;

/*! \brief Initializes the color system for the application.
 *  \par Function Documentation
 *
 *  Initialises the color maps to defaults.
 */
void
x_color_init (void)
{
  colormap = gdk_colormap_get_system ();

  /* Initialise default color maps */
  u_color_map_defaults (display_colors);
  u_color_map_defaults (outline_colors);
}

/*! \brief Frees memory used by the color system.
 *  \par Function Documentation
 *  This function frees the colors from colormap along with
 *  \b black and \b white.
 */
void
x_color_free (void)
{
  int i;

  gdk_colormap_free_colors (colormap, &black, 1);
  gdk_colormap_free_colors (colormap, &white, 1);

  for (i = 0; i < MAX_COLORS; i++) {
    if (display_colors[i].enabled)
      gdk_colormap_free_colors (colormap, gdk_colors[i], 1);
    if (outline_colors[i].enabled)
      gdk_colormap_free_colors (colormap, gdk_outline_colors[i], 1);
  }
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Documentation
 *
 */
void x_color_allocate (void)
{
  int error;
  int i;
  COLOR c;

  gdk_color_parse ("black", &black);
  if (!gdk_colormap_alloc_color (colormap,
                                 &black,
                                 FALSE,
                                 TRUE)) {
    fprintf (stderr, _("Could not allocate the color %s!\n"), _("black"));
    exit (-1);
  }

  gdk_color_parse ("white", &white);
  if (!gdk_colormap_alloc_color (colormap,
                                 &white,
                                 FALSE,
                                 TRUE)) {
    fprintf (stderr, _("Could not allocate the color %s!\n"), _("white"));
    exit (-1);
  }

  for (i = 0; i < MAX_COLORS; i++) {

    if (display_colors[i].enabled) {
      gdk_colors[i] = (GdkColor *)
        g_malloc(sizeof(GdkColor));

      c = display_colors[i];

      /* Interpolate 8-bpp colours into 16-bpp GDK color
       * space. N.b. ignore transparency because GDK doesn't
       * understand it. */
      gdk_colors[i]->red = c.r + (c.r<<8);
      gdk_colors[i]->green = c.g + (c.g<<8);
      gdk_colors[i]->blue = c.b + (c.b<<8);

      error = gdk_color_alloc(colormap, gdk_colors[i]);

      if (error == FALSE) {
        g_error (_("Could not allocate display color %i!\n"), i);
      }
    } else {
      gdk_colors[i] = NULL;
    }

    if (outline_colors[i].enabled) {
      gdk_outline_colors[i] = (GdkColor *) g_malloc(sizeof(GdkColor));

      c = outline_colors[i];

      /* Interpolate 8-bpp colours into 16-bpp GDK color
       * space. N.b. ignore transparency because GDK doesn't
       * understand it. */
      gdk_outline_colors[i]->red = c.r + (c.r<<8);
      gdk_outline_colors[i]->green = c.g + (c.g<<8);
      gdk_outline_colors[i]->blue = c.b + (c.b<<8);

      error = gdk_color_alloc(colormap, gdk_outline_colors[i]);

      if (error == FALSE) {
        g_error (_("Could not allocate outline color %i!\n"), i);
      }
    } else {
      gdk_outline_colors[i] = NULL;
    }
  }
}

/*! \brief Get Pointer to GdkColor
 *  \par Function Documentation
 *   Returns a pointer to the active GdkColor object given the geda
 *  /gschem color index.
 */
GdkColor *x_get_color(int color)
{
  if ((color < 0) || (color >= MAX_COLORS)
      || (gdk_colors[color] == NULL)) {
    fprintf(stderr, _("Tried to get an invalid color: %d\n"), color);
    return(&white);
  } else {
    return(gdk_colors[color]);
  }
}

/*! \brief Get Display Color Map
 *  \par Function Documentation
 *   Returns a pointer to a new Garray containing a copy of the
 *   current color-map allocations.
 * 
 *  \note the returned color-map MUST be freed using g_array_free.
 */
GArray *x_color_get_display_color_map()
{
  GArray* color_map;
  color_map = g_array_sized_new (FALSE, FALSE, sizeof(COLOR), MAX_COLORS);
  color_map = g_array_append_vals (color_map, display_colors, MAX_COLORS);
  return color_map;
}

/*! \brief Get Display Outline Color Map
 *  \par Function Documentation
 *   Returns a pointer to a new Garray containing a copy of the
 *   current color-map allocations.
 *
 *  \note the returned color-map MUST be freed using g_array_free.
 */
GArray *x_color_get_outline_color_map()
{
  GArray* color_map;
  color_map = g_array_sized_new (FALSE, FALSE, sizeof(COLOR), MAX_COLORS);
  color_map = g_array_append_vals (color_map, outline_colors, MAX_COLORS);
  return color_map;
}
/*! \brief Get Display Color Map
 *  \par Function Documentation
 *   Returns a pointer to a new Garray containing a copy of the
 *   current color-map allocations.
 *
 *  \note the returned color-map MUST be freed using g_array_free.
 */
GArray   *x_color_get_print_color_map()
{
  GArray* color_map;
  color_map = g_array_sized_new (FALSE, FALSE, sizeof(COLOR), MAX_COLORS);
  color_map = g_array_append_vals (color_map, print_colors, MAX_COLORS);
  return color_map;
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Documentation
 *
 *  \todo this has to change... to the right code
 */
GdkColor *x_get_darkcolor(int color)
{
  if ((color < 0) || (color >= MAX_COLORS)
      || (gdk_outline_colors[color] == NULL)) {
    fprintf(stderr, _("Tried to get an invalid color: %d\n"), color);
    return(&white);
  } else {
    return(gdk_outline_colors[color]);
  }
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Documentation
 *
 */
COLOR *x_color_lookup (int color)
{
  if (color < 0 || color >= MAX_COLORS || !display_colors[color].enabled) {
    fprintf(stderr, _("Tried to get an invalid color: %d\n"), color);
    return &display_colors[DEFAULT_COLOR_INDEX];
  } else {
    return &display_colors[color];
  }
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Documentation
 *
 *  \todo this has to change... to the right code
 */
COLOR *x_color_lookup_dark (int color)
{
  if (color < 0 || color >= MAX_COLORS || !outline_colors[color].enabled) {
    fprintf(stderr, _("Tried to get an invalid outline color: %d\n"), color);
    return &outline_colors[DEFAULT_COLOR_INDEX];
  } else {
    return &outline_colors[color];
  }

}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Documentation
 *
 */
bool x_color_get_state (int color)
{
  return display_colors[color].enabled;
}

/*! \todo Finish function documentation!!!
 *  \brief
 *  \par Function Documentation
 *
 */
void x_color_set_state (int color, int state)
{
  display_colors[color].enabled = state != FALSE;
}

/*! \brief Return pointer to string name of the color
 *  \par Function Documentation
 *  The function obtains the RGB color at the given index
 *  position and calls library function u_color_rgba_encode
 *  to obtain the a pointer to string name of the color.
 */
char *x_color_get_name(int index)
{
  COLOR c;

  if ((index >= MAX_COLORS) || (index < 0)) {
    return(NULL);
  }

  if (display_colors[index].enabled) {
    c = display_colors[index];
    return u_color_rgba_encode (c.r, c.g, c.b, c.a);
  }

  /* didn't find a color, but there still might be more */
  return(NULL);
}

bool x_color_display_enabled (int index)
{
  return (gdk_colors[index] != NULL);
}

/*! \brief Loads and executes a color map scheme
 *  \par Function Documentation
 *       This function executes a color map scm file after
 *       verifying accessibility. The file must be referenced
 *       relative to the path returned by geda-rc-path. The
 *       current colors are free and the new color allocated.
 */
int x_load_color_scheme(char* scheme) {

  char *strBuffer;
  char *inputfile;
  char *rc_path;
  int result = FALSE;

  SCM s_result;

  strBuffer = GEDA_MEM_ALLOC( MAX_FILE ); /* be 255 */

  if (strBuffer) {
    rc_path = scm_2_cstring("geda-rc-path");
    inputfile = g_strconcat (rc_path, DIR_SEPARATOR_S, scheme, NULL);
    free(rc_path);
    if ((access (inputfile, R_OK)) == 0) {
      x_color_free();
      strcpy(strBuffer, "(load \"");
      strcat(strBuffer, inputfile);
      strcat(strBuffer, "\")");
      scm_dynwind_begin (0);
        scm_dynwind_free(inputfile);
        scm_dynwind_free(strBuffer);
        s_result = g_scm_c_eval_string_protected(strBuffer);
      scm_dynwind_end ();
      if ((result = scm_is_true(s_result)) ? 1 : 0) {
        q_log_message(_("Allocatating new color scheme\n"));
        x_color_allocate();
      }
    }
    else {
      GEDA_FREE(strBuffer);
      u_log_message (_("x_load_color_scheme: Could not locate file:%s\n"), scheme);
    }
  }
  else
    u_log_message(_("x_load_color_scheme: Memory allocation error\n"));
  return result;
}