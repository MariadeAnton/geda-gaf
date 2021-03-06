/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 * Copyright (C) 1998-2015 Ales Hvezda
 * Copyright (C) 1998-2015 gEDA Contributors (see ChangeLog for details)
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
 * \file gschem_status_bar.c
 *
 * \brief A widget for the "status bar" at the bottom of the window
 *
 */

#include <config.h>

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <math.h>

#include <gschem.h>
#include "geda_widgets.h"

/** \defgroup Gschem-Status-Bar Gschem Status Bar
 *  @{ \brief This module defines the #GschemStatusBar class
*/

enum
{
  PROP_0,
  PROP_COORDINATES_MODE,
  PROP_GRID_MODE,
  PROP_GRID_SIZE,
  PROP_HEIGHT,
  PROP_LEFT_BUTTON_TEXT,
  PROP_MIDDLE_BUTTON_TEXT,
  PROP_RIGHT_BUTTON_TEXT,
  PROP_SNAP_MODE,
  PROP_SNAP_SIZE,
  PROP_STATUS_TEXT,
  PROP_STATUS_TEXT_COLOR,
};

enum {
  SET_MIDDLE_ACTION,
  SET_MIDDLE_MOUSEPAN,
  SET_MIDDLE_REPEAT,
#ifdef HAVE_LIBSTROKE
  SET_MIDDLE_STROKE,
#endif
  SET_THIRD_POPUP,
  SET_THIRD_MOUSEPAN,
  UPDATE_GRID_LABEL,
  LAST_SIGNAL
};

struct _GschemStatusBarBuffers
{
  char status_left_text_buffer   [STATUS_LEFT_TEXT_BUFFER_SIZE];
  char status_middle_text_buffer [STATUS_MIDDLE_TEXT_BUFFER_SIZE];
  char status_right_text_buffer  [STATUS_RIGHT_LEFT_TEXT_BUFFER_SIZE];
  char status_grid_text_buffer   [STATUS_GRID_TEXT_BUFFER_SIZE];
  char status_coord_text_buffer  [STATUS_COORD_TEXT_BUFFER_SIZE];
  char status_label_text_buffer  [STATUS_STATUS_TEXT_BUFFER_SIZE];
};

static unsigned int signals[LAST_SIGNAL] = { 0 };

static void
dispose (GObject *object);

static void
finalize (GObject *object);

static void
get_property (GObject *object, unsigned int param_id, GValue *value, GParamSpec *pspec);

static void
gschem_status_bar_class_init (GschemStatusBarClass *klass);

static void
gschem_status_bar_instance_init (GschemStatusBar *view);

static void
set_property (GObject *object, unsigned int param_id, const GValue *value, GParamSpec *pspec);

static void
update_grid_label (GschemStatusBar *widget);

static GObjectClass *gschem_status_bar_parent_class = NULL;

typedef struct st_popup_menu_entry {
  const char *text;
  const int   signal;
} StatusPopupEntry;

static StatusPopupEntry coord_popup_items[] = {

  { N_( COORD_DISPLAY_OFF ),    COORD_FORMAT_OFF   },
  { N_( COORD_DISPLAY_VEC180 ), COORD_FORMAT_V180  },
  { N_( COORD_DISPLAY_VEC360 ), COORD_FORMAT_V360  },
  { N_( COORD_DISPLAY_XY ),     COORD_FORMAT_XY    },
  { N_( COORD_DISPLAY_COORD ),  COORD_FORMAT_COORD },
  { N_( COORD_DISPLAY_COMMA ),  COORD_FORMAT_COMMA },
  { N_( COORD_DISPLAY_X ),      COORD_FORMAT_X     },
  { N_( COORD_DISPLAY_Y ),      COORD_FORMAT_Y     },
  { N_( COORD_DISPLAY_XONLY ),  COORD_FORMAT_XONLY },
  { N_( COORD_DISPLAY_YONLY ),  COORD_FORMAT_YONLY },
  {NULL} /* sentinel */
};

static StatusPopupEntry middle_popup_items[] = {

  { N_( RC_STR_MID_ACTION ),   SET_MIDDLE_ACTION   },
#ifdef HAVE_LIBSTROKE
  { N_( RC_STR_MID_STROKE ),   SET_MIDDLE_STROKE   },
#endif
  { N_( RC_STR_MID_REPEAT ),   SET_MIDDLE_REPEAT   },
  { N_( RC_STR_MID_MOUSEPAN ), SET_MIDDLE_MOUSEPAN },
  {NULL} /* sentinel */
};

static StatusPopupEntry third_popup_items[] = {

  { N_( RC_STR_3RD_POPUP ),    SET_THIRD_POPUP    },
  { N_( RC_STR_3RD_PAN ),      SET_THIRD_MOUSEPAN },
  {NULL} /* sentinel */
};

static void coord_options_popup_clicked (GtkMenuItem *menuitem, void *user_data)
{
  GschemStatusBar *bar;
  GtkWidget       *widget;

  unsigned mode = (unsigned)(long)user_data;

  bar    = GEDA_OBJECT_GET_DATA(menuitem, "status-bar");
  widget = (GtkWidget*)bar;

  gschem_status_bar_set_coord_mode (widget, mode);
  gschem_status_bar_set_coordinates (widget, bar->x0, bar->y0, bar->x1, bar->y1);
}

static void status_options_popup_clicked (GtkMenuItem *menuitem, void *user_data)
{
  GtkWidget *widget;
  unsigned   signal = (unsigned)(long)user_data;

  widget = GEDA_OBJECT_GET_DATA(menuitem, "status-bar");

  g_signal_emit (widget, signals[signal], 0);

}

/* -------------- Popup Menu for Mouse Middle Button Options  -------------- */

/*! \brief GschemStatusBar Show Coordinate Display Options Popup
 *
 *  \par Function Description
 *  This functions creates and displays a small pop-up menu on
 *  the coordinates display widget when the right mouse button
 *  is released on the widget.
 */
static void coord_display_options_popup (GtkWidget      *event_box,
                                         GdkEventButton *event,
                                         void           *user_data)
{
  GtkWidget *menu;
  GtkWidget *popup_item;
  int i;

  /* create the context menu */
  menu = gtk_menu_new();

  for (i = 0; coord_popup_items[i].text != NULL; i++) {

    StatusPopupEntry entry = coord_popup_items[i];

    popup_item = gtk_menu_item_new_with_label (entry.text);

    g_signal_connect (GTK_OBJECT(popup_item), "activate",
                     (GCallback)coord_options_popup_clicked,
                      GUINT_TO_POINTER(entry.signal));

    GEDA_OBJECT_SET_DATA (popup_item, user_data, "status-bar");

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), popup_item);
  }

  gtk_widget_show_all (menu);

  /* make menu a popup menu */
  gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                  (event != NULL) ? event->button : 0,
                  gdk_event_get_time ((GdkEvent*)event));
}

/*! \brief coordinate Display Indicator Button Released callback
 *  \par Function Description
 *  Called when a mouse button is release with the cursor over
 *  the coordinate display indicator. If the 3rd button was
 *  released, a small popup menu is displayed.
 *
 *  \sa middle_button_options_popup
 */
static bool coord_display_released (GtkWidget      *label,
                                    GdkEventButton *event,
                                    void           *user_data)
{
  bool ret_val;

  if (event->button == 3) {

    coord_display_options_popup(label, event, user_data);

    ret_val = TRUE;
  }
  else {
    ret_val = FALSE;
  }

  return ret_val;
}

/* -------------- Popup Menu for Mouse Middle Button Options  -------------- */

/*! \brief GschemStatusBar Show Middle Mouse Options Popup
 *
 *  \par Function Description
 *  This functions creates and displays a small pop-up menu on
 *  the middle-button status widget when the right mouse button
 *  is released on the widget.
 */
static void middle_button_options_popup (GtkWidget      *event_box,
                                         GdkEventButton *event,
                                         void           *user_data)
{
  GtkWidget *menu;
  GtkWidget *popup_item;
  int i;

  /* create the context menu */
  menu = gtk_menu_new();

  for (i = 0; middle_popup_items[i].text != NULL; i++) {

    StatusPopupEntry entry = middle_popup_items[i];

    popup_item = gtk_menu_item_new_with_label (entry.text);

    g_signal_connect (popup_item, "activate",
                      (GCallback)status_options_popup_clicked,
                      (void*)(long)(entry.signal));

    GEDA_OBJECT_SET_DATA (popup_item, user_data, "status-bar");

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), popup_item);
  }

  gtk_widget_show_all (menu);

  /* make menu a popup menu */
  gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                  (event != NULL) ? event->button : 0,
                  gdk_event_get_time ((GdkEvent*)event));
}

/*! \brief Middle Button Status Indicator Button Released callback
 *  \par Function Description
 *  Called when a mouse button is release with the cursor over
 *  the middle button status indicator. If the 3rd button was
 *  released, a small popup menu is displayed.
 *
 *  \sa middle_button_options_popup
 */
static bool middle_button_released (GtkWidget      *label,
                                    GdkEventButton *event,
                                    void           *user_data)
{
  bool ret_val;

  if (event->button == 3) {

    middle_button_options_popup(label, event, user_data);

    ret_val = TRUE;
  }
  else {
    ret_val = FALSE;
  }

  return ret_val;
}

/* --------------- Popup Menu for Mouse Third Button Options  -------------- */

/*! \brief GschemStatusBar Show Third Mouse Options Popup
 *
 *  \par Function Description
 *  This functions creates and displays a small pop-up menu on
 *  the third-button status widget when the right mouse button
 *  is released on the widget.
 */
static void third_button_options_popup (GtkWidget      *event_box,
                                        GdkEventButton *event,
                                        void           *user_data)
{
  GtkWidget *menu;
  GtkWidget *popup_item;
  int i;

  /* create the context menu */
  menu = gtk_menu_new();

  for (i = 0; third_popup_items[i].text != NULL; i++) {

    StatusPopupEntry entry = third_popup_items[i];

    popup_item = gtk_menu_item_new_with_label (entry.text);

    g_signal_connect(popup_item, "activate",
                     (GCallback)status_options_popup_clicked,
                     (void*)(long)(entry.signal));

    GEDA_OBJECT_SET_DATA (popup_item, user_data, "status-bar");

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), popup_item);
  }

  gtk_widget_show_all (menu);

  /* make menu a popup menu */
  gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                  (event != NULL) ? event->button : 0,
                  gdk_event_get_time ((GdkEvent*)event));
}

/*! \brief Third Button Status Indicator Button Released callback
 *  \par Function Description
 *  Called when a mouse button is release with the cursor over
 *  the third button status indicator. If the 3rd button was
 *  released, a small popup menu is displayed.
 *
 *  \sa middle_button_options_popup
 */
static bool third_button_released (GtkWidget      *label,
                                    GdkEventButton *event,
                                    void           *user_data)
{
  bool ret_val;

  if (event->button == 3) {

    third_button_options_popup(label, event, user_data);

    ret_val = TRUE;
  }
  else {
    ret_val = FALSE;
  }

  return ret_val;
}


/* ------------------------ End Popup Menu Callbacks  ---------------------- */

static void gschem_status_bar_reformat_coordinates (GschemStatusBar *gsb)
{
  gschem_status_bar_set_coordinates (GTK_WIDGET(gsb), gsb->x0, gsb->y0, gsb->x1, gsb->y1);
}

static void gschem_status_bar_style_set (GtkWidget *widget, GtkStyle *previous)
{
  int height;

  gtk_widget_style_get (GTK_WIDGET (widget), "height", &height, NULL);

  gschem_status_bar_set_height (widget, height);
}

/*! \brief Dispose of the object
 */
static void dispose (GObject *object)
{
  /* lastly, chain up to the parent dispose */
  g_return_if_fail (gschem_status_bar_parent_class != NULL);
  gschem_status_bar_parent_class->dispose (object);
}

/*! \brief Finalize object
 */
static void finalize (GObject *object)
{
  GschemStatusBar *status_bar = GSCHEM_STATUS_BAR (object);
  GEDA_FREE(status_bar->buffers);

  /* lastly, chain up to the parent finalize */
  g_return_if_fail (gschem_status_bar_parent_class != NULL);
  gschem_status_bar_parent_class->finalize (object);
}

/*! \brief Get a property
 *
 *  \param [in]     object
 *  \param [in]     param_id
 *  \param [in,out] value
 *  \param [in]     pspec
 */
static void
get_property (GObject *object, unsigned int param_id, GValue *value, GParamSpec *pspec)
{
  GtkWidget  *status_bar = (GtkWidget*)GSCHEM_STATUS_BAR (object);
  const char *string;

  switch (param_id) {
    case PROP_COORDINATES_MODE:
      g_value_set_int (value, gschem_status_bar_get_coord_mode(status_bar));
      break;

    case PROP_GRID_MODE:
      g_value_set_int (value, gschem_status_bar_get_grid_mode (status_bar));
      break;

    case PROP_GRID_SIZE:
      g_value_set_int (value, gschem_status_bar_get_grid_size (status_bar));
      break;

    case PROP_HEIGHT:
      g_value_set_int (value, gschem_status_bar_get_height (status_bar));
      break;

    case PROP_LEFT_BUTTON_TEXT:
      string = gschem_status_bar_get_left_button_text (status_bar);
      g_value_set_string (value, string);
      break;

    case PROP_MIDDLE_BUTTON_TEXT:
      string =  gschem_status_bar_get_middle_button_text (status_bar);
      g_value_set_string (value, string);
      break;

    case PROP_RIGHT_BUTTON_TEXT:
      string = gschem_status_bar_get_right_button_text (status_bar);
      g_value_set_string (value, string);
      break;

    case PROP_SNAP_MODE:
      g_value_set_int (value, gschem_status_bar_get_snap_mode (status_bar));
      break;

    case PROP_SNAP_SIZE:
      g_value_set_int (value, gschem_status_bar_get_snap_size (status_bar));
      break;

    case PROP_STATUS_TEXT:
      string = gschem_status_bar_get_status_text (status_bar);
      g_value_set_string (value, string);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
  }
}

/*! \brief Initialize GschemStatusBar class
 *
 *  \param [in] klass The class for the GschemStatusBar
 */
static void
gschem_status_bar_class_init (GschemStatusBarClass *klass)
{
  GObjectClass   *gobject_class   = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class    = (GtkWidgetClass*)klass;
  GParamSpec     *pspec;

  gschem_status_bar_parent_class  = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

  klass->reformat_coordinates     = gschem_status_bar_reformat_coordinates;

  gobject_class->dispose          = dispose;
  gobject_class->finalize         = finalize;

  gobject_class->get_property     = get_property;
  gobject_class->set_property     = set_property;

  widget_class->style_set          = gschem_status_bar_style_set;

  /* Register properties */
  pspec = g_param_spec_int ("coord-mode",
                          _("Coordinates Mode"),
                          _("Sets the Format Mode used to display coordinates on the status bar"),
                              COORD_FORMAT_OFF,
                              COORD_FORMAT_YONLY,
                              COORD_FORMAT_XY,
                             (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_COORDINATES_MODE, pspec);

  pspec = g_param_spec_int ("grid-mode",
                          _("Grid Mode"),
                          _("Sets the Gride Mode to display on the status bar"),
                              G_MININT,
                              G_MAXINT,
                              GRID_NONE,
                             (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_GRID_MODE, pspec);

  pspec = g_param_spec_int ("grid-size",
                          _("Grid Size"),
                          _("Sets the Grid Size to display on the status bar"),
                              G_MININT,
                              G_MAXINT,
                              0,
                             (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_GRID_SIZE, pspec);

  pspec = g_param_spec_int ("height",
                          _("height"),
                          _("Sets the height of the status bar"),
                              0,
                              25,
                              2,
                             (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_HEIGHT, pspec);

  pspec = g_param_spec_string ("left-text",
                             _("Left Button Text"),
                             _("Set the string for the Left Button Text"),
                               "none", (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_LEFT_BUTTON_TEXT, pspec);

  pspec = g_param_spec_string ("middle-text",
                             _("Middle Button Text"),
                             _("Set the string for the Middle Button Text"),
                               "none", (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_MIDDLE_BUTTON_TEXT, pspec);

  pspec = g_param_spec_string ("right-text",
                             _("Right Button Text"),
                             _("Set the string for the Right Button Text"),
                               "none", (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_RIGHT_BUTTON_TEXT, pspec);

  pspec = g_param_spec_int ("snap-mode",
                          _("Snap Mode"),
                          _("Sets the Snap Mode to display on the status bar"),
                             G_MININT,
                             G_MAXINT,
                             0,
                            (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_SNAP_MODE, pspec);

  pspec = g_param_spec_int ("snap-size",
                          _("Snap Size"),
                          _("Sets the Size Mode to display on the status bar"),
                             G_MININT,
                             G_MAXINT,
                             0,
                            (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_SNAP_SIZE, pspec);

  pspec = g_param_spec_string ("status-text",
                             _("Status Text"),
                             _("Set the string for the text on the Status Bar"),
                               "none", (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_STATUS_TEXT, pspec);

  pspec = g_param_spec_int ("status-text-color",
                          _("Status State"),
                          _("Status State"),
                             0,
                             MAX_COLORS,
                             0,
                            (G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_STATUS_TEXT_COLOR, pspec);

  /*!
  * GschemStatusBar:height:
  * Sets the status_height of the status bar widget.
  */
  pspec = g_param_spec_int ("height",
                          _("Status Bar Height"),
                          _("Sets or gets the height of the status bar"),
                             0,
                             25,
                             2,
                            (G_PARAM_READABLE));

  gtk_widget_class_install_style_property (widget_class, pspec);

  signals[SET_MIDDLE_ACTION]   = g_signal_new ("set-middle-action",
                                                gschem_status_bar_get_type(),
                                                G_SIGNAL_RUN_FIRST,
                                                G_STRUCT_OFFSET (GschemStatusBarClass,
                                                                 middle_action),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);

  signals[SET_MIDDLE_MOUSEPAN] = g_signal_new ("set-middle-pan",
                                                gschem_status_bar_get_type(),
                                                G_SIGNAL_RUN_LAST,
                                                G_STRUCT_OFFSET (GschemStatusBarClass,
                                                                 middle_pan),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);

  signals[SET_MIDDLE_REPEAT]   = g_signal_new ("set-middle-repeat",
                                                gschem_status_bar_get_type(),
                                                G_SIGNAL_RUN_LAST,
                                                G_STRUCT_OFFSET (GschemStatusBarClass,
                                                                 middle_repeat),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);
#ifdef HAVE_LIBSTROKE

  signals[SET_MIDDLE_STROKE]   = g_signal_new ("set-middle-stroke",
                                                gschem_status_bar_get_type(),
                                                G_SIGNAL_RUN_LAST,
                                                G_STRUCT_OFFSET (GschemStatusBarClass,
                                                                 middle_stroke),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);
#endif

  signals[SET_THIRD_POPUP]     = g_signal_new ("set-third-popup",
                                                gschem_status_bar_get_type(),
                                                G_SIGNAL_RUN_FIRST,
                                                G_STRUCT_OFFSET (GschemStatusBarClass,
                                                                 third_popup),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);

  signals[SET_THIRD_MOUSEPAN]  = g_signal_new ("set-third-pan",
                                                gschem_status_bar_get_type(),
                                                G_SIGNAL_RUN_LAST,
                                                G_STRUCT_OFFSET (GschemStatusBarClass,
                                                                 third_pan),
                                                NULL, NULL,
                                                g_cclosure_marshal_VOID__VOID,
                                                G_TYPE_NONE, 0);
}


/*! \brief Get the Status Bar grid mode
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The grid mode
 */
int
gschem_status_bar_get_grid_mode (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return (GSCHEM_STATUS_BAR(widget))->grid_mode;
#else
  int ret_val = -1;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      ret_val = gsb->grid_mode;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}


/*! \brief Get the Status Bar grid size
 *
 *  \param [in] widget This GschemStatusBar
 *  \return The grid size
 */
int
gschem_status_bar_get_grid_size (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return (GSCHEM_STATUS_BAR(widget))->grid_size;
#else
  int ret_val = -1;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      ret_val = gsb->grid_size;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}

/*! \brief Get the Height of the Status Bar
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The height
 */
int
gschem_status_bar_get_height (GtkWidget *widget)
{
  int ypad = -1;

#if defined (G_DISABLE_ASSERT)
  g_object_get (GTK_MISC (((GschemStatusBar*)widget)->left_label), "ypad", &ypad, NULL);
#else

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      g_object_get (GTK_MISC (((GschemStatusBar*)widget)->left_label), "ypad", &ypad, NULL);
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
  return ypad;
}

/*! \brief Get the Status Bar left button text
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The left button text
 */
const char*
gschem_status_bar_get_left_button_text (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return geda_label_get_text ((GedaLabel*)gsb->left_label);
#else
  const char *ret_val = NULL;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->left_label)) {
        ret_val = geda_label_widget_get_text (gsb->left_label);
      }
      else {
        BUG_MSG("left_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}

/*! \brief Get the Status Bar middle button text
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The middle button text
 */
const char*
gschem_status_bar_get_middle_button_text (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return geda_label_get_text ((GedaLabel*)gsb->middle_label);
#else
  const char *ret_val = NULL;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->middle_label)) {
        ret_val = geda_label_widget_get_text(gsb->middle_label);
      }
      else {
        BUG_MSG("middle_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}


/*! \brief Get the Status Bar right button text
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The right button text
 */
const char*
gschem_status_bar_get_right_button_text( GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return geda_label_get_text ((GedaLabel*)gsb->right_label);
#else
  const char *ret_val = NULL;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->right_label)) {
        ret_val = geda_label_widget_get_text (gsb->right_label);
      }
      else {
        BUG_MSG("right_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}

/*! \brief Get the Status Bar Coordinates Mode
 *
 *  \par Function Description
 *   This function returns the coordinates display mode used by
 *   the #GschemStatusBar. If \a widget is not GschemStatusBar,
 *   aka w_current->status_bar is NULL the function returns 0
 *   without squealing.
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The snap mode
 */
int
gschem_status_bar_get_coord_mode (GtkWidget *widget)
{
  if (widget && GSCHEM_IS_STATUS_BAR(widget)) {
    return ((GschemStatusBar*)widget)->coord_mode;
  }
  return 0;
}

/*! \brief Get the Status Bar  snap mode
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The snap mode
 */
int
gschem_status_bar_get_snap_mode (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return (GSCHEM_STATUS_BAR(widget))->snap_mode;
#else
  int ret_val = -1;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      ret_val = gsb->snap_mode;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}


/*! \brief Get the Status Bar snap size
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The snap size
 */
int
gschem_status_bar_get_snap_size (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return (GSCHEM_STATUS_BAR(widget))->snap_size;
#else
  int ret_val = -1;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      ret_val = gsb->snap_size;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}

/*! \brief Get the Status Bar status text
 *
 *  \param [in] widget This GschemStatusBar
 *
 *  \return The status text
 */
const char*
gschem_status_bar_get_status_text (GtkWidget *widget)
{
#if defined (G_DISABLE_ASSERT)
  return geda_label_get_text ((GedaLabel*)gsb->status_label);
#else
  const char *ret_val = NULL;

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->status_label)) {
        ret_val = geda_label_widget_get_text (gsb->status_label);
      }
      else {
        BUG_MSG("status_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
  return ret_val;
#endif
}


/*! \brief Get/register GschemStatusBar type.
 */
GedaType
gschem_status_bar_get_type (void)
{
  static GedaType type = 0;

  if (type == 0) {
    static const GTypeInfo info = {
      sizeof(GschemStatusBarClass),
      NULL,                                                    /* base_init */
      NULL,                                                    /* base_finalize */
      (GClassInitFunc) gschem_status_bar_class_init,
      NULL,                                                    /* class_finalize */
      NULL,                                                    /* class_data */
      sizeof(GschemStatusBar),
      0,                                                       /* n_preallocs */
      (GInstanceInitFunc) gschem_status_bar_instance_init,
    };

    type = g_type_register_static (GTK_TYPE_HBOX, "GschemStatusBar", &info, 0);
  }

  return type;
}

/* Macro for gschem_status_bar_setup_buffers. This macro is undefined immediately
 * after the function.
 */
#define TheTarget(target) (unsigned long int*)(dest + offsetof(GschemStatusBar, target))

/*! \brief Gschem Status Bar Setup Buffers
 *
 *  \par Function Description
 *  This function  dynamically allocates memory for a structure similar to
 *  the private structures used by GtkWidgets, except that we don't use the
 *  g_type_class_add_private() function. Currently, the structure contains
 *  only string buffers but other data could be added. The buffers are used
 *  to create compound strings for status-bar text rather than using strdup
 *  or derivitve to constantly allocate and de-allocate trival amounts of
 *  ram. In or to facilitates usage, the status-bar object provides puplic
 *  pointers to these buffers. The pointers are defined as char * const, so
 *  the content can be changed but the pointers can not. Since the pointers
 *  point to dynamicaly allocated ram, we must initially write to the read
 *  only pointers. This function does that.
 *
 *  \param [in] widget This GschemStatusBar
 */
static GschemStatusBarBuffers*
gschem_status_bar_setup_buffers (GschemStatusBar *widget)
{
  unsigned int nbytes = sizeof(GschemStatusBarBuffers);
  unsigned long int dest;

  GschemStatusBarBuffers *buffers = GEDA_MEM_ALLOC(nbytes);

  if (buffers) {

    /* zero-out the newly allocated memory */
    unsigned char *ptr = (unsigned char*)buffers;
    while (nbytes-- > 0)
      *ptr++ = 0;

    /* We can not change pointers directly because we defined as const,
     * so we address indirectly using structure offsets */
    dest                         = (long int)widget;
   *TheTarget(left_label_text)   = (long int)&(buffers->status_left_text_buffer[0]);
   *TheTarget(middle_label_text) = (long int)&(buffers->status_middle_text_buffer[0]);
   *TheTarget(right_label_text)  = (long int)&(buffers->status_right_text_buffer[0]);
   *TheTarget(grid_label_text)   = (long int)&(buffers->status_grid_text_buffer[0]);
   *TheTarget(coord_label_text)  = (long int)&(buffers->status_coord_text_buffer[0]);
   *TheTarget(status_label_text) = (long int)&(buffers->status_label_text_buffer[0]);

  }
  return buffers;
}
#undef TheTarget

/*! \brief Initialize GschemStatusBar instance
 *
 *  \param [in] widget This GschemStatusBar
 */
static void
gschem_status_bar_instance_init (GschemStatusBar *widget)
{
  EdaConfig  *cfg = eda_config_get_user_context ();

  GtkWidget  *coord_event;
  GtkWidget  *middle_event;
  GtkWidget  *third_event;
  GtkWidget  *separator;

  const char *coord_label_tip;
  const char *left_label_tip;
  const char *middle_label_tip;
  const char *right_label_tip;
  const char *grid_label_tip;
  const char *status_label_tip;
  const char *status_bar_tip;

  coord_label_tip  = _("Coordinate display, right click for format options");
  left_label_tip   = _("Left pointer button assignment");
  middle_label_tip = _("Middle pointer button assignment, right click for options");
  right_label_tip  = _("Right pointer button assignment, right click for options");
  grid_label_tip   = _("Indicates the current snap,grid units or states");
  status_label_tip = _("Indicates the current command state");
  status_bar_tip   = _("Gschem status bar");

  g_return_if_fail (widget != NULL);

  const char *grp;
  const char *key;

  cfg = eda_config_get_user_context();
  grp = WIDGET_CONFIG_GROUP;
  key = "status-coord-mode";
  i_var_restore_group_integer(cfg, grp, key, &widget->coord_mode, COORD_FORMAT_XY);

  widget->buffers = gschem_status_bar_setup_buffers (widget);

  gtk_widget_push_composite_child ();

  widget->left_label = geda_visible_label_new (NULL);
  gtk_misc_set_padding (GTK_MISC (widget->left_label), STATUS_XPAD, STATUS_YPAD);
  gtk_box_pack_start (GTK_BOX (widget), widget->left_label, FALSE, FALSE, 0);
  gtk_widget_set_tooltip_text (GTK_WIDGET(widget->left_label), left_label_tip);

  separator = gtk_vseparator_new ();
  g_object_set (separator, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), separator, FALSE, FALSE, 0);

  middle_event = gtk_event_box_new();
  g_object_set (middle_event, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), middle_event, FALSE, FALSE, 0);

  widget->middle_label = geda_visible_label_new (NULL);
  gtk_misc_set_padding (GTK_MISC (widget->middle_label), STATUS_XPAD, STATUS_YPAD);
  gtk_container_add(GTK_CONTAINER(middle_event), widget->middle_label);
  gtk_widget_set_tooltip_text (GTK_WIDGET(widget->middle_label), middle_label_tip);

  separator = gtk_vseparator_new ();
  g_object_set (separator, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), separator, FALSE, FALSE, 0);

  third_event = gtk_event_box_new();
  g_object_set (third_event, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), third_event, FALSE, FALSE, 0);

  widget->right_label = geda_visible_label_new (NULL);
  gtk_misc_set_padding (GTK_MISC (widget->right_label), STATUS_XPAD, STATUS_YPAD);
  gtk_container_add(GTK_CONTAINER(third_event), widget->right_label);
  gtk_widget_set_tooltip_text (GTK_WIDGET(widget->right_label), right_label_tip);

  separator = gtk_vseparator_new ();
  g_object_set (separator, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), separator, FALSE, FALSE, 0);

  widget->grid_label = geda_visible_label_new (NULL);
  gtk_misc_set_padding (GTK_MISC (widget->grid_label), STATUS_XPAD, STATUS_YPAD);
  gtk_box_pack_start (GTK_BOX (widget), widget->grid_label, FALSE, FALSE, 0);
  gtk_widget_set_tooltip_text (GTK_WIDGET(widget->grid_label), grid_label_tip);

  separator = gtk_vseparator_new ();
  g_object_set (separator, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), separator, FALSE, FALSE, 1);

  coord_event = gtk_event_box_new();
  g_object_set (coord_event, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), coord_event, FALSE, FALSE, 0);

  widget->coord_label = geda_visible_label_new (_(COORD_DISPLAY_OFF));
  gtk_misc_set_padding (GTK_MISC (widget->coord_label), STATUS_XPAD, STATUS_YPAD);
  gtk_container_add(GTK_CONTAINER(coord_event), widget->coord_label);
  gtk_widget_set_tooltip_text (GTK_WIDGET(widget->coord_label), coord_label_tip);

  separator = gtk_vseparator_new ();
  g_object_set (separator, "visible", TRUE, NULL);
  gtk_box_pack_start (GTK_BOX (widget), separator, FALSE, FALSE, 0);

  widget->status_label = geda_visible_label_new (NULL);
  gtk_misc_set_padding (GTK_MISC (widget->status_label), STATUS_XPAD, STATUS_YPAD);
  gtk_box_pack_end (GTK_BOX (widget), widget->status_label, FALSE, FALSE, 0);
  gtk_widget_set_tooltip_text (GTK_WIDGET(widget->status_label), status_label_tip);

  {
    AtkObject *obj;
    obj = gtk_widget_get_accessible(GTK_WIDGET(widget));
    atk_object_set_name (obj, status_bar_tip);
    atk_object_set_description(obj, status_bar_tip);
  }

  gtk_widget_pop_composite_child ();

  g_signal_connect (widget, "notify::coord-mode",
                    G_CALLBACK (G_STRUCT_OFFSET (GschemStatusBarClass,
                                                 reformat_coordinates)),
                    NULL);

  signals[UPDATE_GRID_LABEL] =
    g_signal_new_class_handler ("update-grid-label",
                                 gschem_status_bar_get_type (),
                                 G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                 G_CALLBACK (update_grid_label),
                                 NULL, NULL,
                                 gtk_marshal_VOID__VOID,
                                 G_TYPE_NONE, 0);

  g_signal_connect (coord_event, "button-release-event",
                    G_CALLBACK (coord_display_released),
                    widget);

  g_signal_connect (middle_event, "button-release-event",
                    G_CALLBACK (middle_button_released),
                    widget);

  g_signal_connect (third_event, "button-release-event",
                    G_CALLBACK (third_button_released),
                    widget);
}

/*! \brief Set the mode used to display coordinates on the status bar
 *
 *  \par Function Description
 *  This routine sets the coordinate display mode on the status bar,
 *  and, if the value is not a "solo" format, save the value to the
 *  key file. The "solo" formats are not saved because it is assumed
 *  users would not want to retain this state between sessions.
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] mode   The coordinate mode
 */
void
gschem_status_bar_set_coord_mode (GtkWidget *widget, int mode)
{
  GschemStatusBar *gsb;

  EdaConfig  *cfg = eda_config_get_user_context ();
  const char *grp = WIDGET_CONFIG_GROUP;
  const char *key = "status-coord-mode";

  inline unsigned get_coord_mode(int new_mode) {
    gsb->coord_mode &= ~COORD_FORMAT_VECTOR; /* Clear off any old vector bits */
    return ((new_mode & COORD_FORMAT_V180) || (new_mode & COORD_FORMAT_V360))
            ? gsb->coord_mode |= new_mode : new_mode;
  }

#if defined (G_DISABLE_ASSERT)
  gsb = GSCHEM_STATUS_BAR(widget)
  gsb->coord_mode = get_coord_mode(mode);
  if (coord_mode < COORD_FORMAT_X) {
    eda_config_set_integer (cfg, grp, key, gsb->coord_mode);
  }
#else

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      gsb = (GschemStatusBar*)widget;
      gsb->coord_mode = get_coord_mode(mode);
      if (mode < COORD_FORMAT_X) {
        eda_config_set_integer (cfg, grp, key, gsb->coord_mode);
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the grid mode displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] mode   The grid mode
 */
void
gschem_status_bar_set_grid_mode (GtkWidget *widget, int mode)
{
#if defined (G_DISABLE_ASSERT)
  (GSCHEM_STATUS_BAR(widget))->grid_mode = mode;
#else

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      ((GschemStatusBar*)widget)->grid_mode = mode;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the grid size displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] size   The grid size
 */
void
gschem_status_bar_set_grid_size (GtkWidget *widget, int size)
{
#if defined (G_DISABLE_ASSERT)
  (GSCHEM_STATUS_BAR(widget))->grid_size = size;
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      ((GschemStatusBar*)widget)->grid_size = size;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the Height of the Status Bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] height The new height
 */
void
gschem_status_bar_set_height (GtkWidget *widget, int height)
{
  GschemStatusBar *status_bar;

#if defined (G_DISABLE_ASSERT)

  status_bar = GSCHEM_STATUS_BAR(widget);
  gtk_misc_set_padding (GTK_MISC (status_bar->left_label), STATUS_XPAD, height);
  gtk_misc_set_padding (GTK_MISC (status_bar->middle_label), STATUS_XPAD, height);
  gtk_misc_set_padding (GTK_MISC (status_bar->right_label), STATUS_XPAD, height);
  gtk_misc_set_padding (GTK_MISC (status_bar->grid_label), STATUS_XPAD, height);
  gtk_misc_set_padding (GTK_MISC (status_bar->status_label), STATUS_XPAD, height);

#else

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      status_bar = (GschemStatusBar*)widget;
      gtk_misc_set_padding (GTK_MISC (status_bar->left_label), STATUS_XPAD, height);
      gtk_misc_set_padding (GTK_MISC (status_bar->middle_label), STATUS_XPAD, height);
      gtk_misc_set_padding (GTK_MISC (status_bar->right_label), STATUS_XPAD, height);
      gtk_misc_set_padding (GTK_MISC (status_bar->grid_label), STATUS_XPAD, height);
      gtk_misc_set_padding (GTK_MISC (status_bar->status_label), STATUS_XPAD, height);
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the left button text displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] text   The text
 */
void
gschem_status_bar_set_left_button_text (GtkWidget *widget, const char *text)
{
#if defined (G_DISABLE_ASSERT)
  geda_label_set_text ((GedaLabel *)(GSCHEM_STATUS_BAR(widget))->right_label, text);
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->left_label)) {
        geda_label_widget_set_text (gsb->left_label, text);
        //g_object_notify (G_OBJECT (widget), "left-text");
      }
      else {
        BUG_MSG("left_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}


/*! \brief Set the middle button text displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] text   The text
 */
void
gschem_status_bar_set_middle_button_text (GtkWidget *widget, const char *text)
{
#if defined (G_DISABLE_ASSERT)
  geda_label_set_text ((GedaLabel *)(GSCHEM_STATUS_BAR(widget))->right_label, text);
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->middle_label)) {
        geda_label_widget_set_text (gsb->middle_label, text);
      }
      else {
        BUG_MSG("middle_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}


/*! \brief Set the right button text displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] text   The text
 */
void
gschem_status_bar_set_right_button_text (GtkWidget *widget, const char *text)
{
#if defined (G_DISABLE_ASSERT)
  geda_label_set_text ((GedaLabel*)(GSCHEM_STATUS_BAR(widget))->right_label, text);
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->right_label)) {
        geda_label_widget_set_text ( gsb->right_label, text);
      }
      else {
        BUG_MSG("right_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the snap mode displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] mode   The snap mode
 */
void
gschem_status_bar_set_snap_mode (GtkWidget *widget, int mode)
{
#if defined (G_DISABLE_ASSERT)
  (GSCHEM_STATUS_BAR(widget))->snap_mode = mode;
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      ((GschemStatusBar*)widget)->snap_mode = mode;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the snap size displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] size   The snap size
 */
void
gschem_status_bar_set_snap_size (GtkWidget *widget, int size)
{
#if defined (G_DISABLE_ASSERT)
  (GSCHEM_STATUS_BAR(widget))->snap_size = size;
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      ((GschemStatusBar*)widget)->snap_size = size;
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
    }
  }
#endif
}

/*! \brief Set the Coordinate text displayed on the status bar
 *
 *  \par Function Description
 *   This routine sets the coordinate string displayed on the status bar.
 *   The x0 and y0 arguments are only used in vector mode, otherwise both
 *   are ignored. In vector mode, if the x0 argument is equal to negative
 *   zero, yes -0, the format specified prior to changing to vector mode
 *   is used. In our scheme, vector mode is bit 1 of mode, the other bits
 *   are used for the "other" formats. For the other formats we calculate
 *   the index to the format string by counting bit shifts after clearing
 *   bit 1, which may be set but gschem is not inside an action.
 *
 *  \param [in] widget  This GschemStatusBar
 *  \param [in] x0      First abscissa or -0 in not inside an action
 *  \param [in] y0      First ordinate, used if in vectored mode and x0
 *  \param [in] x1      Second abscissa, is X value of the cursor position
 *  \param [in] y1      Second ordinate, is Y value of the cursor position
 *
 *  \note All coordinates must be world (since this module has no w_current).
 *
 *  \sa gschem_status_bar_get_coord_mode gschem_status_bar_set_coord_mode
 */
void
gschem_status_bar_set_coordinates (GtkWidget *widget, int x0, int y0, int x1, int y1)
{
  GschemStatusBar *status_bar;
  char  *text;

  struct st_coordinate_formats {
    const char *text;
  } coordinate_formats[] = {
    {_(COORD_DISPLAY_OFF)},
    {"%d<%d"},                /* COORD_FORMAT_V180 */
    {"%d<<%d"},               /* COORD_FORMAT_V360 */
    {"X=%d, Y=%d"},           /* COORD_FORMAT_XY */
    {"(%d, %d)"},             /* COORD_FORMAT_COORD */
    {"%d, %d"},               /* COORD_FORMAT_COMMA */
    {"X=%d"},                 /* COORD_FORMAT_X */
    {"y=%d"},                 /* COORD_FORMAT_Y */
    {"X=%d"},                 /* COORD_FORMAT_XONLY */
    {"y=%d"},                 /* COORD_FORMAT_YONLY */
  };

  /* Save coodinates in GschemStatusBar, see gschem_status_bar.h */
  inline void save_coordinates(void) {
    status_bar->x0 = x0;
    status_bar->y0 = x0;
    status_bar->x1 = x1;
    status_bar->y1 = y1;
  }

  /* Returns string to display on the status bar */
  inline char *get_coordinates_text(unsigned mode) {

    char  *string;
    int    degrees;
    int    index;
    int    length;
    double radians;

    index = 0;

    /* Check if vector format and valid first abscissa */
    if ((mode & COORD_FORMAT_V180  ||
         mode & COORD_FORMAT_V360) && x0 != -0) {

      /* Get the magnitude*/
      length = m_line_length(x0, y0, x1, y1);

      /* Get the angle */
      radians = atan2((y1 - y0), (x1 - x0));
      degrees = radians * 180.0 / M_PI;

      if (mode & COORD_FORMAT_V360) {
        if (degrees < 0) {
          degrees = degrees + 360;
        }
        index = COORD_FORMAT_V360;
      }
      else {
        index = COORD_FORMAT_V180;
      }

      string = u_string_sprintf(coordinate_formats[index].text, length, degrees);
    }
    else {

      /* Clear the vector bits, for case not inside_action */
      mode &= ~COORD_FORMAT_VECTOR;

      /* Look for next bit, the shift count is index to the string */
      while(mode) {
        index++;
        mode = mode>>1;
      }

      string = u_string_sprintf(coordinate_formats[index].text, x1, y1);
    }

    return string;
  }

#if defined (G_DISABLE_ASSERT)

  status_bar = GSCHEM_STATUS_BAR(widget);
  text       = get_coordinates_text(status_bar->coord_mode);
  geda_label_set_text ((GedaLabel*)status_bar->coord_label, text);
  save_coordinates();

#else

  if (widget == NULL) {
    BUG_MSG("widget is NULL");
    text = NULL;
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      status_bar = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(status_bar->coord_label)) {
        text = get_coordinates_text(status_bar->coord_mode);
        geda_label_widget_set_text (status_bar->coord_label, text);
        save_coordinates();
      }
      else {
        BUG_MSG("coord_label is not a GedaLabel");
        text = NULL;
      }
    }
    else {
      BUG_MSG("widget is not a GschemStatusBar");
      text = NULL;
    }
  }

#endif

  GEDA_FREE(text);
}

/*! \brief Set the status text displayed on the status bar
 *
 *  \param [in] widget This GschemStatusBar
 *  \param [in] text   The status text
 */
void
gschem_status_bar_set_status_text (GtkWidget *widget, const char *text)
{
#if defined (G_DISABLE_ASSERT)
  geda_label_set_text ((GedaLabel *)(GSCHEM_STATUS_BAR(widget))->status_label, text);
#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->status_label)) {
        geda_label_widget_set_text (gsb->status_label, text);
      }
      else {
        BUG_MSG("status_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a ");
    }
  }
#endif
}

/*! \brief Set the status text color
 *
 * \par Function Description
 * Changes the status text color to show if the current editing
 * action is active or not.
 *
 * \param [in] widget This GschemStatusBar
 * \param [in] index  The state to visualise
 */
void
gschem_status_bar_set_status_text_color (GtkWidget *widget, int index)
{
  GdkColor *color = x_color_get_color_from_index(index);

#if defined (G_DISABLE_ASSERT)
  GschemStatusBar *gsb = (GschemStatusBar*)widget;
  gtk_widget_modify_fg (GTK_WIDGET (gsb->status_label), GTK_STATE_NORMAL, color);

#else
  if (widget == NULL) {
    BUG_MSG("widget is NULL");
  }
  else {
    if (GSCHEM_IS_STATUS_BAR(widget)) {
      GschemStatusBar *gsb = (GschemStatusBar*)widget;
      if (GEDA_IS_LABEL(gsb->status_label)) {
        gtk_widget_modify_fg (GTK_WIDGET (gsb->status_label), GTK_STATE_NORMAL, color);
      }
      else {
        BUG_MSG("status_label is not a GedaLabel");
      }
    }
    else {
      BUG_MSG("widget is not a ");
    }
  }
#endif
}

/*! \brief Set a gobject property
 */
static void
set_property (GObject *object, unsigned int param_id, const GValue *value, GParamSpec *pspec)
{
  GtkWidget *status_bar = (GtkWidget*)GSCHEM_STATUS_BAR (object);

  switch (param_id) {

    case PROP_COORDINATES_MODE:
      gschem_status_bar_set_coord_mode(status_bar, g_value_get_int (value));
      break;

    case PROP_GRID_MODE:
      gschem_status_bar_set_grid_mode (status_bar, g_value_get_int (value));
      break;

    case PROP_GRID_SIZE:
      gschem_status_bar_set_grid_size (status_bar, g_value_get_int (value));
      break;

    case PROP_HEIGHT:
      gschem_status_bar_set_height (status_bar, g_value_get_int (value));
      break;

    case PROP_LEFT_BUTTON_TEXT:
      gschem_status_bar_set_left_button_text (status_bar, g_value_get_string (value));
      break;

    case PROP_MIDDLE_BUTTON_TEXT:
      gschem_status_bar_set_middle_button_text (status_bar, g_value_get_string (value));
      break;

    case PROP_RIGHT_BUTTON_TEXT:
      gschem_status_bar_set_right_button_text (status_bar, g_value_get_string (value));
      break;

    case PROP_SNAP_MODE:
      gschem_status_bar_set_snap_mode (status_bar, g_value_get_int (value));
      break;

    case PROP_SNAP_SIZE:
      gschem_status_bar_set_snap_size (status_bar, g_value_get_int (value));
      break;

    case PROP_STATUS_TEXT:
      gschem_status_bar_set_status_text (status_bar, g_value_get_string (value));
      break;

    case PROP_STATUS_TEXT_COLOR:
      gschem_status_bar_set_status_text_color (status_bar, g_value_get_int (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
  }
}

/*! \brief Write the grid settings to the gschem "status bar."
 *
 *  \par Function Description
 *  This function creates the string for the Grid/Snap label on
 *  the status bar utilizing the classes internal scratch ram,
 *  widget->grid_label_text and local char array for integers.
 *  The assemulated string is uploaded to the grid label widget.
 *
 *  \param [in] widget This GschemStatusBar
 */
static void
update_grid_label (GschemStatusBar *widget)
{
  if (widget->grid_label != NULL) {

    char scratch[6]; /* tmp char used to convert large integers */

    char *ptr = strcpy (widget->grid_label_text,_("Grid"));

    ptr = strcat(ptr, "(");

    switch (widget->snap_mode) {
      case SNAP_OFF:
        ptr = strcat(ptr,_("OFF"));
        break;

      case SNAP_GRID:
        strcat (ptr, u_string_int2str(widget->snap_size, &scratch[0], 10));
        break;

      case SNAP_RESNAP:
        strcat (ptr, u_string_int2str(widget->snap_size, &scratch[0], 10));
        strcat (ptr, "R");
        break;

      default:
        ptr = strcat(ptr,_("Error"));
        fprintf(stderr, "%s: snap_mode out of range: %d\n", __func__,
                widget->snap_mode);
    }

    ptr = strcat(ptr, ", ");

    if (widget->grid_mode == GRID_NONE) {
        ptr = strcat(ptr,_("OFF"));
    }
    else {
      if (widget->grid_size <= 0) {
        ptr = strcat(ptr,_("NONE"));
      }
      else {
        strcat (ptr, u_string_int2str(widget->grid_size,&scratch[0], 10));
      }
    }

    ptr = strcat(ptr, ")");

    geda_label_widget_set_text(widget->grid_label, widget->grid_label_text);
  }
}

GtkWidget *gschem_status_bar_new (void)
{
  return GTK_WIDGET (g_object_new (GSCHEM_TYPE_STATUS_BAR, NULL));
}

/** @} endgroup Gschem-Status-Bar */
