/* gEDA - GPL Electronic Design Automation
 * gschem - gEDA Schematic Capture
 * Copyright (C) 2013-2014 Ales Hvezda
 * Copyright (C) 2013-2014 gEDA Contributors (see ChangeLog for details)
 *
 * Adapted for gEDA by Wiley Edward Hill <wileyhill@gmail.com>
 *
 * This Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GEDA_FONT_BUTTON_H__
#define __GEDA_FONT_BUTTON_H__

#include <gtk/gtkbutton.h>

#ifndef DEFAULT_FONT_NAME
#define DEFAULT_FONT_NAME "ARIAL 10"
#endif
#ifndef DEFAULT_FONT_SIZE
#define DEFAULT_FONT_SIZE 10
#define MIN_FONT_SIZE      6
#define MAX_FONT_SIZE    256
#endif

#define DEFAULT_PREVIEW_TEXT _("abcdefghijk ABCDEFGHIJK")

G_BEGIN_DECLS

/* GedaFontButton is a button widget that allow user to select font properties. */

#define GEDA_TYPE_FONT_BUTTON             (geda_font_button_get_type ())
#define GEDA_FONT_BUTTON(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEDA_TYPE_FONT_BUTTON, GedaFontButton))
#define GEDA_FONT_BUTTON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  GEDA_TYPE_FONT_BUTTON, GedaFontButtonClass))
#define GEDA_IS_FONT_BUTTON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEDA_TYPE_FONT_BUTTON))
#define GEDA_IS_FONT_BUTTON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  GEDA_TYPE_FONT_BUTTON))
#define GEDA_FONT_BUTTON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  GEDA_TYPE_FONT_BUTTON, GedaFontButtonClass))

typedef struct _GedaFontButton        GedaFontButton;
typedef struct _GedaFontButtonClass   GedaFontButtonClass;
typedef struct _GedaFontButtonPrivate GedaFontButtonPrivate;

struct _GedaFontButton {

  GtkButton button;

  /*< private >*/
  GedaFontButtonPrivate *priv;

  PangoFontDescription    *font_desc;
  PangoFontFace           *font_face;

  char *title;

};

struct _GedaFontButtonClass {
  GtkButtonClass parent_class;

  /* font_set signal is emitted when font is chosen */
  void (* font_set ) (GedaFontButton *gfp);
  void (* size_set ) (GedaFontButton *gfp);
};

GType         geda_font_button_get_type       (void) G_GNUC_CONST;
GtkWidget    *geda_font_button_new            (void);
GtkWidget    *geda_font_button_new_with_font  (const char       *fontname);

const char   *geda_font_button_get_title      (GedaFontButton *font_button);
void          geda_font_button_set_title      (GedaFontButton *font_button,
                                               const char       *title);
bool          geda_font_button_get_use_font   (GedaFontButton *font_button);
void          geda_font_button_set_use_font   (GedaFontButton *font_button,
                                               bool              use_font);
const char   *geda_font_button_get_font_name  (GedaFontButton *font_button);
bool          geda_font_button_set_font_name  (GedaFontButton *font_button,
                                               const char       *fontname);
bool          geda_font_button_get_use_size   (GedaFontButton *font_button);
void          geda_font_button_set_use_size   (GedaFontButton *font_button,
                                               bool              use_size);
bool          geda_font_button_get_show_size  (GedaFontButton *font_button);
void          geda_font_button_set_show_size  (GedaFontButton *font_button,
                                               bool              show_size);
char         *geda_font_button_get_ascii_size (GedaFontButton *font_button);
int           geda_font_button_get_size       (GedaFontButton *font_button);
void          geda_font_button_set_size       (GedaFontButton *font_button,
                                               int               font_size);
bool          geda_font_button_get_show_style (GedaFontButton *font_button);
void          geda_font_button_set_show_style (GedaFontButton *font_button,
                                               bool              show_style);
bool        geda_font_button_get_show_preview (GedaFontButton *font_button);
void        geda_font_button_set_show_preview (GedaFontButton *font_button,
                                               bool              enable);
const char* geda_font_button_get_preview_text (GedaFontButton *font_button);
void        geda_font_button_set_preview_text (GedaFontButton *font_button,
                                               const char *text);

const PangoFontDescription *geda_font_button_get_font_desc (GedaFontButton *gfp);
void  geda_font_button_set_font_desc (GedaFontButton *font_button,
                                      PangoFontDescription * pfd);

G_END_DECLS

#endif /* __GEDA_FONT_BUTTON_H__ */