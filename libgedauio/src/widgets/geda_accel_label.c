/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 * Copyright (C) 1998-2014 gEDA Contributors (see ChangeLog for details)
 *
 * Code based on GTK 2.14.5 gtk/gtkaccellabel.c (LGPL)
 *
 * GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GedaAccelLabel: GtkLabel with accelerator monitoring facilities.
 * Copyright (C) 1998 Tim Janik
 *
 * Modified by the GTK+ Team and others 1997-2001.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 *
 *  Adapted for gEDA by Peter Clifton <pcjc2@cam.ac.uk>
 *
 *  THIS FILE IS LGPL LICENSED, gEDA AS A WHOLE IS GPL LICENSED
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */

#include "config.h"

#include <geda.h>

#include <gtk/gtkaccellabel.h>
#include "geda_accel_label.h"

#ifdef HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

#define P_(x) (x)

/**
 * \brief GedaAccelLabel - A Button Widget for Menus
 * \par
 * A GedaAccelLabel is a variant of GtkAccelLabel, but allows menu items
 * with multi-key assignments.
 *
 * \defgroup GedaAccelLabel Accelerator Label
 * @{
 */

enum {
  PROP_0,
  PROP_ACCEL_CLOSURE,
  PROP_ACCEL_WIDGET,
  PROP_ACCEL_STRING,
};

G_DEFINE_TYPE (GedaAccelLabel, geda_accel_label, GTK_TYPE_ACCEL_LABEL)

bool
geda_accel_label_refetch (GedaAccelLabel *accel_label)
{
  bool enable_accels;

  g_return_val_if_fail (GEDA_IS_ACCEL_LABEL (accel_label), FALSE);

  g_object_get (gtk_widget_get_settings (GTK_WIDGET (accel_label)),
                "gtk-enable-accels", &enable_accels,
                NULL);

  if (!enable_accels || accel_label->accel_string == NULL) {
    if (accel_label->accel_string != NULL)
      g_free (accel_label->accel_string);

    accel_label->accel_string = g_strdup ("");
  }

  gtk_widget_queue_resize (GTK_WIDGET (accel_label));

  return FALSE;
}


static const char *
geda_accel_label_get_string (GedaAccelLabel *accel_label)
{
  if (!accel_label->accel_string)
    geda_accel_label_refetch (accel_label);

  return accel_label->accel_string;
}


static void
geda_accel_label_set_property (GObject      *object,
                               unsigned int  prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  GedaAccelLabel  *accel_label;

  accel_label = GEDA_ACCEL_LABEL (object);

  switch (prop_id) {
    /* Dummy properties from GtkAccelLabel */
    case PROP_ACCEL_CLOSURE:
    case PROP_ACCEL_WIDGET:
      break;

    case PROP_ACCEL_STRING:
      geda_accel_label_set_accel_string (accel_label, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
geda_accel_label_get_property (GObject      *object,
                               unsigned int  prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  GedaAccelLabel  *accel_label;

  accel_label = GEDA_ACCEL_LABEL (object);

  switch (prop_id) {
    /* Dummy property from GtkAccelLabel */
    case PROP_ACCEL_CLOSURE:
      g_value_set_boxed (value, NULL);
      break;

      /* Dummy property from GtkAccelLabel */
    case PROP_ACCEL_WIDGET:
      g_value_set_object (value, NULL);
      break;

    case PROP_ACCEL_STRING:
      g_value_set_string (value, accel_label->accel_string);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
geda_accel_label_init (GedaAccelLabel *accel_label)
{
  accel_label->accel_padding = 3;
  accel_label->accel_string = NULL;
}

static void
geda_accel_label_finalize (GObject *object)
{
  GedaAccelLabel *accel_label = GEDA_ACCEL_LABEL (object);

  g_free (accel_label->accel_string);

  G_OBJECT_CLASS (geda_accel_label_parent_class)->finalize (object);
}

unsigned int
geda_accel_label_get_accel_width (GedaAccelLabel *accel_label)
{
  g_return_val_if_fail (GEDA_IS_ACCEL_LABEL (accel_label), 0);

  return (accel_label->accel_string_width +
          (accel_label->accel_string_width ? accel_label->accel_padding : 0));
}

static void
geda_accel_label_size_request (GtkWidget      *widget,
                               GtkRequisition *requisition)
{
  GedaAccelLabel *accel_label = GEDA_ACCEL_LABEL (widget);
  GtkAccelLabel *gtk_accel_label = GTK_ACCEL_LABEL (widget);
  PangoLayout *layout;
  int width;

  GTK_WIDGET_CLASS (geda_accel_label_parent_class)->size_request (widget, requisition);

  layout = gtk_widget_create_pango_layout (widget, geda_accel_label_get_string (accel_label));
  pango_layout_get_pixel_size (layout, &width, NULL);
  accel_label->accel_string_width = width;
  gtk_accel_label->accel_string_width = width; /* HACK: This field is private to GtkAccelLabel */
  g_object_unref (layout);
}

static int
get_first_baseline (PangoLayout *layout)
{
  PangoLayoutIter *iter;
  int result;

  iter = pango_layout_get_iter (layout);
  result = pango_layout_iter_get_baseline (iter);
  pango_layout_iter_free (iter);

  return PANGO_PIXELS (result);
}

static bool
geda_accel_label_expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  GedaAccelLabel *accel_label = GEDA_ACCEL_LABEL (widget);
  GtkMisc *misc = GTK_MISC (accel_label);
  GtkTextDirection direction;

  direction = gtk_widget_get_direction (widget);

  if (GTK_WIDGET_DRAWABLE (accel_label)) {
    unsigned int ac_width;

    ac_width = geda_accel_label_get_accel_width (accel_label);

    if (widget->allocation.width >= widget->requisition.width + ac_width) {
      PangoLayout *label_layout;
      PangoLayout *accel_layout;
      GtkLabel *label = GTK_LABEL (widget);

      int x;
      int y;

      label_layout = gtk_label_get_layout (GTK_LABEL (accel_label));

      if (direction == GTK_TEXT_DIR_RTL) {
        widget->allocation.x += ac_width;
      }
      widget->allocation.width -= ac_width;
      if (gtk_label_get_ellipsize (label)) {
        pango_layout_set_width (label_layout,
                                pango_layout_get_width (label_layout)
                                - ac_width * PANGO_SCALE);
      }

      if (GTK_WIDGET_CLASS (geda_accel_label_parent_class)->expose_event) {
        GTK_WIDGET_CLASS (geda_accel_label_parent_class)->expose_event (widget, event);
      }

      if (direction == GTK_TEXT_DIR_RTL) {
        widget->allocation.x -= ac_width;
      }

      widget->allocation.width += ac_width;
      if (gtk_label_get_ellipsize (label)) {
        pango_layout_set_width (label_layout,
                                pango_layout_get_width (label_layout)
                                + ac_width * PANGO_SCALE);
      }

      if (direction == GTK_TEXT_DIR_RTL) {
        x = widget->allocation.x + misc->xpad;
      }
      else {
        x = widget->allocation.x + widget->allocation.width - misc->xpad - ac_width;
      }

      gtk_label_get_layout_offsets (GTK_LABEL (accel_label), NULL, &y);

      accel_layout = gtk_widget_create_pango_layout (widget, geda_accel_label_get_string (accel_label));

      y += get_first_baseline (label_layout) - get_first_baseline (accel_layout);

      gtk_paint_layout (widget->style,
                        widget->window,
                        GTK_WIDGET_STATE (widget),
                        FALSE,
                        &event->area,
                        widget,
                        "accellabel",
                        x, y,
                        accel_layout);

      g_object_unref (accel_layout);
    }
    else {
      if (GTK_WIDGET_CLASS (geda_accel_label_parent_class)->expose_event)
        GTK_WIDGET_CLASS (geda_accel_label_parent_class)->expose_event (widget, event);
    }
  }

  return FALSE;
}

/* Underscores in key names are better displayed as spaces
 * E.g., Page_Up should be "Page Up"
 */
static void
substitute_underscores (char *str)
{
  char *p;

  for (p = str; *p; p++)
    if (*p == '_')
      *p = ' ';
}


/**
 * geda_accel_label_set_accel_string:
 * \param accel_label a #GedaAccelLabel
 * \param accel_string the accelerator string.
 *
 * Sets the accelerator string for this accelerator label.
 **/
void
geda_accel_label_set_accel_string (GedaAccelLabel *accel_label,
                                   const char     *accel_string)
{
  g_return_if_fail (GEDA_IS_ACCEL_LABEL (accel_label));

  if (accel_label->accel_string)
    g_free (accel_label->accel_string);

  if (accel_string) {
    accel_label->accel_string = g_strdup (accel_string);
    substitute_underscores (accel_label->accel_string);
  }
  else {
    accel_label->accel_string = NULL;
  }

  g_object_notify (G_OBJECT (accel_label), "accel-string");
}

static void
geda_accel_label_class_init (GedaAccelLabelClass *class)
{
  GObjectClass *gobject_class  = G_OBJECT_CLASS (class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  gobject_class->finalize      = geda_accel_label_finalize;
  gobject_class->set_property  = geda_accel_label_set_property;
  gobject_class->get_property  = geda_accel_label_get_property;

  widget_class->size_request   = geda_accel_label_size_request;
  widget_class->expose_event   = geda_accel_label_expose_event;

  g_object_class_install_property (gobject_class,
                                   PROP_ACCEL_CLOSURE,
                                   g_param_spec_boxed ("accel-closure",
                                                       P_("Accelerator Closure"),
                                                       P_("The closure to be monitored for accelerator changes"),
                                                       G_TYPE_CLOSURE,
                                                       G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_ACCEL_WIDGET,
                                   g_param_spec_object ("accel-widget",
                                                        P_("Accelerator Widget"),
                                                        P_("The widget to be monitored for accelerator changes"),
                                                        GTK_TYPE_WIDGET,
                                                        G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_ACCEL_STRING,
                                   g_param_spec_string ("accel-string",
                                                        P_("Accelerator String"),
                                                        P_("The accelerator string to be displayed"),
                                                        NULL,
                                                        G_PARAM_READWRITE));
}
/*! @} endgroup GedaAccelLabel */