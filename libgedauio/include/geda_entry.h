/* gEDA - GPL Electronic Design Automation
 *
 * Copyright (C) 2012-2014 Ales Hvezda
 * Copyright (C) 2012-2014 Wiley Edward Hill <wileyhill@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Boston, MA 02110-1301 USA
 *
 * Date: December 31, 2012
 * Contributing Author: Wiley Edward Hill <wileyhill@gmail.com>
 *
 */
#ifndef _GEDA_ENTRY_H
#define _GEDA_ENTRY_H

/*
 *
 */
#define NO_HISTORY (void*) -1
#define NO_COMPLETION (void*) -1

#ifndef MAX_ENTRY_HISTORY /* Make sure this gets defined */
  #define MAX_ENTRY_HISTORY 22
#endif
#ifndef max_command_length
  #define max_command_length 256
#endif

#include <gtk/gtkentrybuffer.h>

#include "geda_completion.h"

G_BEGIN_DECLS

typedef struct
{
  char  *name;
  void (*func)(void*);
  void  *arg;
  char  *help;
} commands;

#define GEDA_TYPE_ENTRY            (geda_entry_get_type ())
#define GEDA_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEDA_TYPE_ENTRY, GedaEntry))
#define GEDA_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GEDA_TYPE_ENTRY, GedaEntryClass))
#define IS_GEDA_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEDA_TYPE_ENTRY))
#define IS_GEDA_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GEDA_TYPE_ENTRY))
#define GEDA_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GEDA_TYPE_ENTRY, GedaEntryClass))

#define DISABLE (GList **)-1

typedef struct _GedaEntry      GedaEntry;
typedef struct _GedaEntryClass GedaEntryClass;
typedef struct _GedaEntryPriv  GedaEntryPriv;

typedef enum {
  ACCEPT_ALL_ASCII,
  ACCEPT_ALPHANUMERIC,
  ACCEPT_COORDINATE,
  ACCEPT_NUMERIC,
  ACCEPT_NUMBER,
  ACCEPT_INTEGER,
  ACCEPT_REAL,
} GedaEntryAccept;

struct _GedaEntry
{
        GtkEntry          parent;
        GedaEntryAccept   validation_mode;
        volatile int      enable_drag_n_drop   : 1;
        volatile int      activates_default    : 1;
        volatile int      auto_complete        : 1;
        volatile int      history_index;
        volatile int      text_case;
        volatile int      max_history;
        GedaEntryPriv    *priv;
};

struct _GedaEntryClass
{
        GtkEntryClass   parent_class;

  /* Action signals */
  void (* activate)           (GedaEntry        *entry);

  /* Hook to customize right-click popup */
  void (* populate_popup)     (GedaEntry         *entry,
                               GtkMenu           *menu);

  /* Source side drag signals */
  void (* drag_begin)         (GtkWidget         *widget,
                               GdkDragContext    *context);
  void (* drag_end)           (GtkWidget         *widget,
                               GdkDragContext    *context);
  void (* drag_data_get)      (GtkWidget         *widget,
                               GdkDragContext    *context,
                               GtkSelectionData  *selection_data,
                               unsigned int       info,
                               unsigned int       time_);
  void (* drag_data_delete)   (GtkWidget         *widget,
                               GdkDragContext    *context);

  /* Target side drag signals */
  void (* drag_leave)         (GtkWidget         *widget,
                               GdkDragContext    *context,
                               unsigned int       time_);
  bool (* drag_motion)        (GtkWidget         *widget,
                               GdkDragContext    *context,
                               int                x,
                               int                y,
                               unsigned int       time_);
  bool (* drag_drop)          (GtkWidget         *widget,
                               GdkDragContext    *context,
                               int                x,
                               int                y,
                               unsigned int       time_);
  void (* drag_data_received) (GtkWidget         *widget,
                               GdkDragContext    *context,
                               int                x,
                               int                y,
                               GtkSelectionData  *selection_data,
                               unsigned int       info,
                               unsigned int       time_);
};

GType      geda_entry_get_type               (void) G_GNUC_CONST;
GtkWidget *geda_entry_new                    (GList** history, GList** complete);
GtkWidget *geda_visible_entry_new            (GList** history, GList** complete);
GtkWidget *geda_entry_new_with_buffer        (GtkEntryBuffer *buffer);

void geda_entry_set_max_history              (GedaEntry *entry, int value);
int  geda_entry_get_max_history              (GedaEntry *entry);

void gtk_entry_set_completion                (GtkEntry *entry,
                                              GtkEntryCompletion *completion);
GtkEntryCompletion *gtk_entry_get_completion (GtkEntry *entry);

bool geda_entry_completion_get_case          (GedaEntry *entry);
void geda_entry_completion_set_case          (GedaEntry *entry, bool sensitive);

void geda_entry_set_input_case               (GedaEntry *entry, int mode);
bool geda_entry_get_input_case               (GedaEntry *entry);

void geda_entry_set_activates_default        (GedaEntry *entry, bool  setting);
bool geda_entry_get_activates_default        (GedaEntry *entry);

void geda_entry_widget_modify_color          (GtkWidget    *widget,
                                              GtkRcFlags    component,
                                              GtkStateType  state,
                                        const GdkColor     *color);
void geda_entry_modify_fg                    (GedaEntry    *entry,
                                              GtkStateType  state,
                                        const GdkColor     *color);
void geda_entry_modify_bg                    (GedaEntry    *entry,
                                              GtkStateType  state,
                                        const GdkColor     *color);

void geda_entry_set_valid_input     (GedaEntry *entry, GedaEntryAccept mode);
void geda_entry_set_attributes      (GedaEntry *entry, PangoAttrList *attrs);
PangoAttrList  *geda_entry_get_attributes (GedaEntry *entry);

G_END_DECLS

#endif /* __GTK_ENTRY_H__ */