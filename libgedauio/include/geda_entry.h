/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2 tab-width: 4 -*- */
/* vi: set et ts=4 sw=2 sts=2: */
/*
 * File: geda_entry.h
 * libgedauio - gEDA's library for User Interface Objects
 *
 * gEDA - GPL Electronic Design Automation
 *
 * Copyright (C) 2012-2015 Wiley Edward Hill <wileyhill@gmail.com>
 *
 * This Library is free software; you can redistribute it and or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 3 of the
 * License.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA <http://www.gnu.org/licenses/>.
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

BEGIN_DECLS

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
        volatile int      have_history;
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

GedaType   geda_entry_get_type               (void) GEDA_CONST;
GtkWidget *geda_entry_new                    (GList** history, GList** complete);
GtkWidget *geda_visible_entry_new            (GList** history, GList** complete);
GtkWidget *geda_entry_new_with_buffer        (GtkEntryBuffer *buffer);
GtkWidget *geda_entry_new_with_max_length    (int length);

PangoAttrList*
     geda_entry_get_attributes               (GedaEntry *entry);

int  geda_entry_get_max_history              (GedaEntry *entry);
void geda_entry_set_max_history              (GedaEntry *entry,
                                              int value);
void geda_entry_set_completion               (GedaEntry *entry,
                                              GedaCompletion *completion);
GedaCompletion*
     geda_entry_get_completion               (GedaEntry *entry);

bool geda_entry_completion_get_case          (GedaEntry *entry);
void geda_entry_completion_set_case          (GedaEntry *entry,
                                              bool sensitive);
bool geda_entry_get_input_case               (GedaEntry *entry);
void geda_entry_set_input_case               (GedaEntry *entry,
                                              int mode);
bool geda_entry_get_activates_default        (GedaEntry *entry);
void geda_entry_set_activates_default        (GedaEntry *entry,
                                              bool  setting);

void geda_entry_set_attributes               (GedaEntry *entry,
                                              PangoAttrList *attrs);
void geda_entry_set_valid_input              (GedaEntry *entry,
                                              GedaEntryAccept mode);

void geda_entry_modify_fg                    (GedaEntry    *entry,
                                              GtkStateType  state,
                                        const GdkColor     *color);
void geda_entry_modify_bg                    (GedaEntry    *entry,
                                              GtkStateType  state,
                                        const GdkColor     *color);

void geda_entry_widget_modify_color          (GtkWidget    *widget,
                                              GtkRcFlags    component,
                                              GtkStateType  state,
                                        const GdkColor     *color);
PangoAttrList*
     geda_entry_widget_get_attributes        (GtkWidget *entry);

int  geda_entry_widget_get_max_history       (GtkWidget *entry);
void geda_entry_widget_set_max_history       (GtkWidget *entry,
                                              int value);
void geda_entry_widget_set_completion        (GtkWidget *entry,
                                              GedaCompletion *completion);
GedaCompletion *
     geda_entry_widget_get_completion        (GtkWidget *entry);

bool geda_entry_widget_completion_get_case   (GtkWidget *entry);
void geda_entry_widget_completion_set_case   (GtkWidget *entry,
                                              bool sensitive);
bool geda_entry_widget_get_input_case        (GtkWidget *entry);
void geda_entry_widget_set_input_case        (GtkWidget *entry,
                                              int mode);
bool geda_entry_widget_get_activates_default (GtkWidget *entry);
void geda_entry_widget_set_activates_default (GtkWidget *entry,
                                              bool  setting);
void geda_entry_widget_set_attributes        (GtkWidget *entry,
                                              PangoAttrList *attrs);
void geda_entry_widget_set_valid_input       (GtkWidget *entry,
                                              GedaEntryAccept mode);

void geda_entry_widget_modify_fg             (GtkWidget    *entry,
                                              GtkStateType  state,
                                        const GdkColor     *color);
void geda_entry_widget_modify_bg             (GtkWidget    *entry,
                                              GtkStateType  state,
                                        const GdkColor     *color);
END_DECLS

#endif /* __GTK_ENTRY_H__ */
