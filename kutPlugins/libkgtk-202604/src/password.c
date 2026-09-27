// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "password.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_password_new (Arr *params) {
  return exp_ext(gtk_password_entry_new());
}

Exp *libkgtk_password_with_icon (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkPasswordEntry *w = GTK_PASSWORD_ENTRY(exp_get_ext(aparams[0]));
  int show = exp_get_bool(aparams[1]);
  gtk_password_entry_set_show_peek_icon(w, show);
  return exp_empty();
}

Exp *libkgtk_password_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkPasswordEntry *w = GTK_PASSWORD_ENTRY(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  gtk_editable_set_text(GTK_EDITABLE(w), tx);
  return exp_empty();
}

Exp *libkgtk_password_get_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkPasswordEntry *w = GTK_PASSWORD_ENTRY(exp_get_ext(aparams[0]));
  const char *bf = gtk_editable_get_text(GTK_EDITABLE(w));
  char *s = ATOMIC(strlen(bf) + 1);
  strcpy(s, bf);
  return exp_string(s);
}

