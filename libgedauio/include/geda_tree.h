/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2 tab-width: 4 -*- */
/* vi: set et ts=4 sw=2 sts=2: */
/*
 * File: geda_threads.h
 *
 * gEDA - GPL Electronic Design Automation
 * libgedauio - gEDA's library for User Interface Objects
 *
 * Copyright (C) 2015 Wiley Edward Hill
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
 * Date: January 10, 2015
 * Contributing Author: Wiley Edward Hill <wileyhill@gmail.com>
 */
#ifndef __GEDA_TREE_H__
#define __GEDA_TREE_H__

#include <gtk/gtktreeview.h>
#include <gtk/gtktreemodel.h>

/* Utility functions */
void          geda_tree_copy_iter                (GtkTreeIter *iter1, GtkTreeIter *iter2);
bool          geda_tree_model_iter_previous      (GtkTreeModel *tree_model, GtkTreeIter *iter);

#endif /* __GEDA_TREE_H__ */