/* gEDA - GPL Electronic Design Automation
 * GEDA - gEDA Schematic Capture
 *
 * Copyright (C) 2014 Wiley Edward Hill <wileyhill@gmail.com>
 *
 * Date: November 24, 2014
 * Contributing Author: Wiley Edward Hill <wileyhill@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301 USA
 */
#ifndef __GEDA_THREADS_H__
#define __GEDA_THREADS_H__

#include <glib.h>

void          geda_main_context_invoke           (GMainContext   *context,
						  GSourceFunc     function,
						  void           *data);
void          geda_main_context_invoke_full      (GMainContext   *context,
						  int             priority,
						  GSourceFunc     function,
                                                  void           *data,
						  GDestroyNotify  notify);
			       
#endif /* __GEDA_THREADS_H__ */