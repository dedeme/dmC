// Copyright 27-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "grid.h"
#include "kut/DEFS.h"
#include <gtk/gtk.h>


/// Creates a new Grid.
/// \ -> <gwg>
Exp *libkgtk_grid_new (Arr *params) {
  GtkGrid *w = GTK_GRID(gtk_grid_new());
  gtk_grid_set_row_spacing(w, 5);
  return exp_ext(w);
}

Exp *libkgtk_grid_put_at (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkGrid *w = GTK_GRID(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));
  int row = exp_get_int(aparams[2]);
  int col = exp_get_int(aparams[3]);
  int rowspan = exp_get_int(aparams[4]);
  int colspan = exp_get_int(aparams[5]);

  gtk_grid_attach(w, child, col, row, colspan, rowspan);
  return exp_empty();
}

Exp *libkgtk_grid_set (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkGrid *w = GTK_GRID(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));

  GtkWidget *pchild = gtk_grid_get_child_at(w, 0, 0);
  if (pchild) gtk_grid_remove(w, pchild);

  gtk_grid_attach(w, child, 0, 0, 1, 1);
  return exp_empty();
}

Exp *libkgtk_grid_remove_at (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkGrid *w = GTK_GRID(exp_get_ext(aparams[0]));
  int row = exp_get_int(aparams[1]);
  int col = exp_get_int(aparams[2]);

  GtkWidget *child = gtk_grid_get_child_at(w, col, row);
  gtk_grid_remove(w, child);
  return exp_empty();
}

/// Removes 'child' of 'w'.
///   w      : Grid to set.
///   child  : Widget to remove.
/// \<wg> -> <wg>
Exp *libkgtk_grid_remove (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkGrid *w = GTK_GRID(exp_get_ext(aparams[0]));
  GtkWidget *child = GTK_WIDGET(exp_get_ext(aparams[1]));
  gtk_grid_remove(w, child);
  return exp_empty();
}

