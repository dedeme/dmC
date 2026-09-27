// Copyright 27-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "overlay.h"
#include "kut/DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_overlay_new (Arr *params) {
  return exp_ext(gtk_overlay_new());
}

Exp *libkgtk_overlay_set (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkOverlay *w = GTK_OVERLAY(exp_get_ext(aparams[0]));
  GtkWidget *back = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_overlay_set_child(w, back);
  return exp_empty();
}

Exp *libkgtk_overlay_add (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkOverlay *w = GTK_OVERLAY(exp_get_ext(aparams[0]));
  GtkWidget *fore = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_overlay_add_overlay(w, fore);
  return exp_empty();
}

Exp *libkgtk_overlay_remove (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkOverlay *w = GTK_OVERLAY(exp_get_ext(aparams[0]));
  GtkWidget *fore = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_overlay_remove_overlay(w, fore);
  return exp_empty();
}
