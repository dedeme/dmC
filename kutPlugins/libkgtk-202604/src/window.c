// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "window.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_window_new (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkApplication *app = GTK_APPLICATION(exp_get_ext(aparams[0]));
  return exp_ext(gtk_application_window_new(app));
}

Exp *libkgtk_window_add (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_window_set_child(w, child);
  return exp_empty();
}

Exp *libkgtk_window_close (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  gtk_window_close(w);
  return exp_empty();
}

Exp *libkgtk_window_get_app (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  return exp_ext(gtk_window_get_application(w));
}

Exp *libkgtk_window_maximize (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  int value = exp_get_bool(aparams[1]);
  if (value) gtk_window_maximize(w);
  else gtk_window_unmaximize(w);
  return exp_empty();
}

Exp *libkgtk_window_minimize (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  int value = exp_get_bool(aparams[1]);
  if (value) gtk_window_minimize(w);
  else gtk_window_unminimize(w);
  return exp_empty();
}

Exp *libkgtk_window_modal (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  GtkWindow *parent = GTK_WINDOW(exp_get_ext(aparams[1]));
  gtk_window_set_modal(w, TRUE);
  gtk_window_set_transient_for(w, parent);
  return exp_empty();
}

Exp *libkgtk_window_present (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  gtk_window_present(w);
  return exp_empty();
}

Exp *libkgtk_window_focus_visible (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  int value = exp_get_bool(aparams[1]);
  gtk_window_set_focus_visible(w, value);
  return exp_empty();
}

Exp *libkgtk_window_size (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  int width = exp_get_int(aparams[1]);
  int height = exp_get_int(aparams[2]);
  gtk_window_set_default_size(w, width, height);
  return exp_empty();
}

Exp *libkgtk_window_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWindow *w = GTK_WINDOW(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  gtk_window_set_title(w, tx);
  return exp_empty();
}
