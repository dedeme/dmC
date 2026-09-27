// Copyright 23-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "spinner.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_spinner_new (Arr *params) {
  GtkWidget* w = gtk_spinner_new();
  gtk_spinner_start(GTK_SPINNER(w));
  return exp_ext(w);
}
