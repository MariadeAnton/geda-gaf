/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2 tab-width: 4 -*- */

/* System headers which libgeda headers rely on */

#include <glib.h>
#include <glib-object.h>
#include <libguile.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib/gstdio.h>

#include <geda.h>

/* Public libgeda headers */
#include <defines.h>

#include <g_types.h>
#include <o_types.h>

#include <s_struct.h>
#include <g_struct.h>

#include <f_types.h>

#include <geda_config.h>
#include <geda_errors.h>

#include <globals.h>

#include <geda_color.h>
#include <geda_colors.h>
#include <papersizes.h>
#include <prototype.h>

#include <geda_list.h>
#include <geda_notify.h>
#include <geda_object.h>
#include <geda_page.h>
#include <geda_line.h>
#include <geda_arc.h>
#include <geda_box.h>
#include <geda_bus.h>
#include <geda_circle.h>
#include <geda_complex.h>
#include <geda_net.h>
#include <geda_path.h>
#include <geda_picture.h>
#include <geda_pin.h>
#include <geda_text.h>
#include <geda_toplevel.h>

/* Private libgeda headers */
#include "globals.h"
#include "prototype_priv.h"
#include "i_vars_priv.h"

/* Gettext translation */
#include "gettext_priv.h"
