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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA, <http://www.gnu.org/licenses/>.
 */
/*!
 * \file gschem_page_view.h
 *
 * \brief A widget for viewing a schematic page
 */

#define GSCHEM_TYPE_PAGE_VIEW (gschem_page_view_get_type())
#define GSCHEM_PAGE_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSCHEM_TYPE_PAGE_VIEW, GschemPageView))
#define GSCHEM_PAGE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GSCHEM_TYPE_PAGE_VIEW, GschemPageViewClass))
#define GSCHEM_IS_PAGE_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSCHEM_TYPE_PAGE_VIEW))
#define GSCHEM_PAGE_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GSCHEM_TYPE_PAGE_VIEW, GschemPageViewClass))

typedef struct _GschemPageViewClass GschemPageViewClass;
typedef struct _GschemPageView GschemPageView;

struct _GschemPageViewClass
{
  GtkWindowClass parent_class;
};

struct _GschemPageView
{
  GtkWindow parent;

  GtkAdjustment *hadjustment;
  GtkAdjustment *vadjustment;

  void *page;
};

GtkAdjustment*
gschem_page_view_get_hadjustment (GschemPageView *view);

void*
gschem_page_view_get_page (GschemPageView *view);

GedaType
gschem_page_view_get_type ();

GtkAdjustment*
gschem_page_view_get_vadjustment (GschemPageView *view);

GschemPageView*
gschem_page_view_new ();

void
gschem_page_view_set_hadjustment (GschemPageView *view, GtkAdjustment *hadjustment);

void
gschem_page_view_set_page (GschemPageView *view, void *page);

void
gschem_page_view_set_vadjustment (GschemPageView *view, GtkAdjustment *vadjustment);