// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "label.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_entry_new (Arr *params) {
  return exp_ext(gtk_entry_new());
}

Exp *libkgtk_entry_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkEntry *w = GTK_ENTRY(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  GtkEntryBuffer *bf = gtk_entry_get_buffer(w);
  gtk_entry_buffer_set_text(bf, tx, arr_size(str_runes(tx)));
  return exp_empty();
}

Exp *libkgtk_entry_get_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkEntry *w = GTK_ENTRY(exp_get_ext(aparams[0]));
  GtkEntryBuffer *bf = gtk_entry_get_buffer(w);
  const char *bfs = gtk_entry_buffer_get_text(bf);
  char *s = ATOMIC(strlen(bfs) + 1);
  strcpy(s, bfs);
  return exp_string(s);
}

Exp *libkgtk_entry_icon (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkEntry *w = GTK_ENTRY(exp_get_ext(aparams[0]));
  GtkWidget *img = GTK_WIDGET(exp_get_ext(aparams[1]));
  int isLeft = exp_get_bool(aparams[2]);
  char *tooltip = exp_get_string(aparams[3]);

  gtk_entry_set_icon_from_paintable(
    w,
    isLeft ? GTK_ENTRY_ICON_PRIMARY : GTK_ENTRY_ICON_SECONDARY,
    gtk_image_get_paintable(GTK_IMAGE(img))
  );
  if (*tooltip)
    gtk_entry_set_icon_tooltip_markup(
      w,
      isLeft ? GTK_ENTRY_ICON_PRIMARY : GTK_ENTRY_ICON_SECONDARY,
      tooltip
    );
  return exp_empty();
}

Exp *libkgtk_entry_focus (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkEntry *w = GTK_ENTRY(exp_get_ext(aparams[0]));
  gtk_entry_grab_focus_without_selecting(GTK_ENTRY(w));
  return exp_empty();
}
