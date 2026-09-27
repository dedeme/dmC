// Copyright 14-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "separator.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_separator_new (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  int is_vertical = exp_get_bool(aparams[0]);
  return exp_ext(gtk_separator_new(
    is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL
  ));
}
