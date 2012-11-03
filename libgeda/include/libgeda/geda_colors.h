/* gEDA - GPL Electronic Design Automation
 * libgeda - gEDA's Library 
 * Copyright (C) 1998-2012 Ales Hvezda
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

/*! \file geda_colors.h
 *  \brief define some constants for the colors
 */

#ifndef _COLORS_H_INCL
#define _COLORS_H_INCL

#define BACKGROUND_COLOR                0
#define PIN_COLOR                       1
#define NET_ENDPOINT_COLOR              2
#define GRAPHIC_COLOR                   3
#define NET_COLOR                       4
#define ATTRIBUTE_COLOR                 5
#define LOGIC_BUBBLE_COLOR              6
#define DOTS_GRID_COLOR                 7
#define DETACHED_ATTRIBUTE_COLOR        8
#define TEXT_COLOR                      9
#define BUS_COLOR                       10
#define SELECT_COLOR                    11
#define BOUNDINGBOX_COLOR               12
#define ZOOM_BOX_COLOR                  13
#define STROKE_COLOR                    14
#define LOCK_COLOR 	                15
#define OUTPUT_BACKGROUND_COLOR         16     
#define FREESTYLE1_COLOR         	17     
#define FREESTYLE2_COLOR         	18     
#define FREESTYLE3_COLOR         	19     
#define FREESTYLE4_COLOR         	20     
#define JUNCTION_COLOR         		21     
#define MESH_GRID_MAJOR_COLOR           22
#define MESH_GRID_MINOR_COLOR           23

#define DEFAULT_COLOR_INDEX             GRAPHIC_COLOR
/*
#define OVERRIDE_NET_COLOR              4
#define OVERRIDE_PIN_COLOR              1
#define OVERRIDE_BUS_COLOR              10

*/
#define RGB_NOCOLOR	{0xff, 0xff, 0xff, 0xff, FALSE}
#define RGB_BLACK	{0x00, 0x00, 0x00, 0xff, TRUE}
#define RGB_BLUE	{0x00, 0x00, 0xff, 0xff, TRUE}
#define RGB_CYAN	{0x00, 0xff, 0xff, 0xff, TRUE} /* aqua */
#define RGB_GRAY	{0x88, 0x88, 0x88, 0xff, TRUE}
#define RGB_GREY	{0x88, 0x88, 0x88, 0xff, TRUE}
#define RGB_GREEN	{0x00, 0xff, 0x00, 0xff, TRUE}
#define RGB_MAGENTA	{0xff, 0x00, 0xff, 0xff, TRUE} /* fuchsia */
#define RGB_ORANGE	{0xff, 0xa5, 0xff, 0xff, TRUE}
#define RGB_RED 	{0xff, 0x00, 0x00, 0xff, TRUE}
#define RGB_YELLOW	{0xff, 0xff, 0x00, 0xff, TRUE}
#define RGB_WHITE	{0xff, 0xff, 0xff, 0xff, TRUE}
#define RGB_ENDMAP	{0x00, 0x00, 0x00, 0x00, FALSE}
#endif
