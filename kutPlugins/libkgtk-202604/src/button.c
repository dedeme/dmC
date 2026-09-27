// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "button.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_button_new (Arr *params) {
  return exp_ext(gtk_button_new());
}

Exp *libkgtk_button_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkButton *w = GTK_BUTTON(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  gtk_button_set_label(w, tx);
  return exp_empty();
}
