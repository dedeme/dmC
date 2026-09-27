// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "label.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_label_new (Arr *params) {
  GtkWidget *lb = gtk_label_new("");
  gtk_label_set_wrap(GTK_LABEL(lb), TRUE);
  return exp_ext(lb);
}

Exp *libkgtk_label_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkLabel *w = GTK_LABEL(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  gtk_label_set_text(w, tx);
  return exp_empty();
}

Exp *libkgtk_label_html (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkLabel *w = GTK_LABEL(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  gtk_label_set_markup(w, tx);
  return exp_empty();
}

Exp *libkgtk_label_selectable (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkLabel *w = GTK_LABEL(exp_get_ext(aparams[0]));
  int value = exp_get_bool(aparams[1]);
  gtk_label_set_selectable(w, value);
  return exp_empty();
}
