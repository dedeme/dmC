// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "box.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_box_new (Arr *params) {
  return exp_ext(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10));
}

Exp *libkgtk_box_new_v (Arr *params) {
  return exp_ext(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
}

Exp *libkgtk_box_add (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkBox *w = GTK_BOX(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_box_append(w, child);
  return exp_empty();
}

Exp *libkgtk_box_adds (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkBox *w = GTK_BOX(exp_get_ext(aparams[0]));
  // <Exp>
  Arr *a = exp_get_array(aparams[1]);
  EACH(a, Exp, e) {
    GtkWidget *child = GTK_WIDGET(exp_get_ext(e));
    gtk_box_append(w, child);
  }_EACH
  return exp_empty();
}

Exp *libkgtk_box_remove (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkBox *w = GTK_BOX(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_box_remove(w, child);
  return exp_empty();
}

Exp *libkgtk_box_removes (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkBox *w = GTK_BOX(exp_get_ext(aparams[0]));
  // <Exp>
  Arr *a = exp_get_array(aparams[1]);
  EACH(a, Exp, e) {
    GtkWidget *child = GTK_WIDGET(exp_get_ext(e));
    gtk_box_remove(w, child);
  }_EACH
  return exp_empty();
}

Exp *libkgtk_box_spacing (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkBox *w = GTK_BOX(exp_get_ext(aparams[0]));
  int spacing = exp_get_int(aparams[1]);
  gtk_box_set_spacing(w, spacing);
  return exp_empty();
}
