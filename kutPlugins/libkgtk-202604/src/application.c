// Copyright 09-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>


#include "application.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include <gtk/gtk.h>

static void *run_closure;

void application_run_closure (Exp *cl, Arr *params) {
  ((void(*)(Exp *, Arr *))run_closure)(cl, params);
}

Exp *libkgtk_application_new (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *id = exp_get_string(aparams[0]);
  run_closure = exp_get_object("<plclosure>", aparams[1]);
  return exp_ext(
    gtk_application_new(id, G_APPLICATION_DEFAULT_FLAGS)
  );
}

Exp *libkgtk_application_close (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkApplication *app = GTK_APPLICATION(exp_get_ext(aparams[0]));
  g_object_unref(app);
  return exp_empty();
}

static void callback (GtkApplication *app, gpointer user_data) {
  application_run_closure(user_data, arr_new());
}
Exp *libkgtk_application_run (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkApplication *app = GTK_APPLICATION(exp_get_ext(aparams[0]));

  g_signal_connect (app, "activate", G_CALLBACK (callback), aparams[1]);

  g_application_run(G_APPLICATION (app), 0, NULL);
  return exp_empty();
}
