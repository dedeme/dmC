// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "password.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_combo_new (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  Arr *a = exp_get_array(aparams[0]);
  int ix = exp_get_int(aparams[1]);
  int size = arr_size(a);
  const char* strings[size + 1];
  EACH(a, Exp, e) {
    strings[_i] = exp_get_string(e);
  }_EACH
  strings[size] = NULL;

  ix = ix < 0
    ? 0
    : ix >= size
      ? size - 1
      : ix
  ;

  GtkWidget *r = gtk_drop_down_new_from_strings((const char* const*)strings);
  gtk_drop_down_set_selected(GTK_DROP_DOWN(r), ix);
  return exp_ext(r);
}

Exp *libkgtk_combo_select (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkDropDown *w = GTK_DROP_DOWN(exp_get_ext(aparams[0]));
  int ix = exp_get_int(aparams[1]);

  GListModel *md = gtk_drop_down_get_model(w);
  int size = g_list_model_get_n_items(md);
  ix = ix < 0
    ? 0
    : ix >= size
      ? size - 1
      : ix
  ;
  gtk_drop_down_set_selected(w, ix);


  return exp_empty();
}

Exp *libkgtk_combo_get_select (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkDropDown *w = GTK_DROP_DOWN(exp_get_ext(aparams[0]));
  int r = gtk_drop_down_get_selected(w);
  if (r == GTK_INVALID_LIST_POSITION) r = -1;
  return exp_int(r);
}
