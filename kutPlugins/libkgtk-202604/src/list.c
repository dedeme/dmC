// Copyright 25-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "image.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>
#include "application.h"

static void setup (
  GtkSignalListItemFactory *f, GtkListItem *item, gpointer user_data
) {
  gtk_list_item_set_child(
    item, GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10))
  );
}

static void bind (
  GtkSignalListItemFactory *f, GtkListItem *item, gpointer user_data
) {
  GtkWidget *child = gtk_list_item_get_child(item);
  GtkStringObject *strobj = gtk_list_item_get_item(item);
  char *text = (char *)gtk_string_object_get_string(strobj);
  if (!text)
    EXC_KUT("libgtk.list.bind: item is null");

  application_run_closure(user_data, arr_new_from(
    exp_ext(child), exp_string(text), NULL
  ));
}
Exp *libkgtk_list_new (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  int isSelectable = exp_get_bool(aparams[0]);
  Arr *valuesJs = exp_get_array(aparams[1]);
  int size = arr_size(valuesJs);
  const char* values[size + 1];
  EACH(valuesJs, Exp, v) {
    values[_i] = exp_get_string(v);
  }_EACH
  values[size] = NULL;
  GtkStringList* strings = gtk_string_list_new(values);
  GtkSelectionModel *model = isSelectable
    ? GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(strings)))
    : GTK_SELECTION_MODEL(gtk_no_selection_new(G_LIST_MODEL(strings)))
  ;

  GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
  g_signal_connect (factory, "setup", G_CALLBACK(setup), NULL);
  g_signal_connect (factory, "bind", G_CALLBACK(bind), aparams[2]);

  GtkWidget *r = gtk_list_view_new(
    GTK_SELECTION_MODEL(model),
    factory
  );
  return exp_ext(r);
}

Exp *libkgtk_list_scroll (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkListView *w = GTK_LIST_VIEW(exp_get_ext(aparams[0]));
  int ix = exp_get_int(aparams[1]);

  GtkScrollInfo *scroll_info = gtk_scroll_info_new ();
  gtk_scroll_info_set_enable_vertical (scroll_info, TRUE);
  gtk_list_view_scroll_to(w, ix, GTK_LIST_SCROLL_SELECT, scroll_info);
  return exp_empty();
}
