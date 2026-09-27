// Copyright 14-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "image.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_image_new (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *name = exp_get_string(aparams[0]);

  if (arr_size(params) == 1)
    return exp_ext(gtk_image_new_from_file(name));

  int size = exp_get_int(aparams[1]);
  return exp_ext(
    gtk_image_new_from_paintable(GDK_PAINTABLE(
      gtk_icon_theme_lookup_icon(
        gtk_icon_theme_get_for_display(gdk_display_get_default()),
        name,
        NULL,
        size,
        1, 0, 0
      )
    )
  ));
}
