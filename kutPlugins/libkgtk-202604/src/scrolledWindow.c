// Copyright 25-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scrolledWindow.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_scrolled_window_new (Arr *params) {
  GtkScrolledWindow *w = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
  gtk_scrolled_window_set_has_frame(w, TRUE);

  return exp_ext(w);
};

Exp *libkgtk_scrolled_window_add (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkScrolledWindow *w = GTK_SCROLLED_WINDOW(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_scrolled_window_set_child(w, child);
  return exp_empty();
}
