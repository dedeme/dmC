// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "password.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_checkbutton_new (Arr *params) {
  return exp_ext(gtk_check_button_new());
}

Exp *libkgtk_checkbutton_text (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkCheckButton *w = GTK_CHECK_BUTTON(exp_get_ext(aparams[0]));
  char *tx = exp_get_string(aparams[1]);
  gtk_check_button_set_label(w, tx);
  return exp_empty();
}

Exp *libkgtk_checkbutton_select (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkCheckButton *w = GTK_CHECK_BUTTON(exp_get_ext(aparams[0]));
  int sel = exp_get_int(aparams[1]);
  if (sel < 0 || sel > 2) sel = 1;
  if (sel == 2) {
    gtk_check_button_set_inconsistent(w, TRUE);
  } else {
    gtk_check_button_set_inconsistent(w, FALSE);
    gtk_check_button_set_active(w, sel);
  }
  return exp_empty();
}

Exp *libkgtk_checkbutton_get_select (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkCheckButton *w = GTK_CHECK_BUTTON(exp_get_ext(aparams[0]));
  int r = 2;
  if (!gtk_check_button_get_inconsistent(w))
    r = gtk_check_button_get_active(w);

  return exp_int(r);
}

Exp *libkgtk_checkbutton_group (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkCheckButton *w = GTK_CHECK_BUTTON(exp_get_ext(aparams[0]));
  GtkCheckButton *other = GTK_CHECK_BUTTON(exp_get_ext(aparams[1]));
  gtk_check_button_set_group(w, other);
  return exp_empty();
}
