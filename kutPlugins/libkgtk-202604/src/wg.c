// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "wg.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include "application.h"
#include <gtk/gtk.h>

Exp *libkgtk_wg_setCss (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  char *style = exp_get_string(aparams[0]);
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_string(provider, style);
  gtk_style_context_add_provider_for_display(
    gdk_display_get_default(),
    GTK_STYLE_PROVIDER(provider),
    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
  );
  return exp_empty();
}

Exp *libkgtk_wg_css (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *css = exp_get_string(aparams[1]);
  gtk_widget_add_css_class(w, css);
  return exp_empty();
}

Exp *libkgtk_wg_cursor (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *cursor = exp_get_string(aparams[1]);
  gtk_widget_set_cursor_from_name(w, cursor);
  return exp_empty();
}

Exp *libkgtk_wg_ch_css (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *before = exp_get_string(aparams[1]);
  char *after = exp_get_string(aparams[2]);
  gtk_widget_remove_css_class(w, before);
  if (*after) gtk_widget_add_css_class(w, after);
  return exp_empty();
}

Exp *libkgtk_wg_focus (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  gtk_widget_grab_focus(w);
  return exp_empty();
}

Exp *libkgtk_wg_halign (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *align = exp_get_string(aparams[1]);
  int al = GTK_ALIGN_CENTER;
  if (!strcmp(align, "left")) al = GTK_ALIGN_START;
  if (!strcmp(align, "right")) al = GTK_ALIGN_END;
  gtk_widget_set_halign(w, al);
  return exp_empty();
}

Exp *libkgtk_wg_hexpand (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  int value = exp_get_bool(aparams[1]);
  gtk_widget_set_hexpand(w, value);
  return exp_empty();
}

static void callback (GtkWidget *widget, gpointer user_data) {
  application_run_closure(user_data, arr_new());
}
static void callback2 (GtkWidget *widget, char *p1, gpointer user_data) {
  application_run_closure(user_data, arr_new_from(exp_string(p1), NULL));
}
// p1 is not used by the function pointed by user_data.
static void callback3 (GtkWidget *widget, int p1, gpointer user_data) {
  application_run_closure(user_data, arr_new_from(exp_int(p1), NULL));
}
Exp *libkgtk_wg_on (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *event = exp_get_string(aparams[1]);
  if (
    !strcmp(event, "clicked") ||
    !strcmp(event, "activate") ||
    !strcmp(event, "toggled") ||
    !strcmp(event, "day-selected")
  ) {
    g_signal_connect(w, event, G_CALLBACK(callback), aparams[2]);
  } else if (
    !strcmp(event, "activate-link")
  ) {
    g_signal_connect(w, event, G_CALLBACK(callback2), aparams[2]);
  } else if (
    !strcmp(event, "icon-press")
  ) {
    g_signal_connect(w, event, G_CALLBACK(callback3), aparams[2]);
  } else {
    EXC_KUT(str_f("Unknown signal '%s'", event));
  }
  return exp_empty();
}

static void callback4 (
  GtkGestureClick *controller,
  int n_press, double x, double y,
  gpointer user_data
) {
  application_run_closure(
    user_data,
    arr_new_from(exp_int(n_press), exp_float(x), exp_float(y), NULL)
  );
}
Exp *libkgtk_wg_on_mouse (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *event = exp_get_string(aparams[1]);
  GtkGesture *gesture = gtk_gesture_click_new();
  g_signal_connect(gesture, event, G_CALLBACK(callback4), aparams[2]);
  gtk_widget_add_controller(GTK_WIDGET(w), GTK_EVENT_CONTROLLER(gesture));
  return exp_empty();
}

Exp *libkgtk_wg_set_focusable (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  int value = exp_get_bool(aparams[1]);
  gtk_widget_set_can_focus(w, value);
  return exp_empty();
}

Exp *libkgtk_wg_set_focusable_on_click (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  int value = exp_get_bool(aparams[1]);
  gtk_widget_set_focus_on_click(w, value);
  return exp_empty();
}

Exp *libkgtk_wg_valign (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  char *align = exp_get_string(aparams[1]);
  int al = GTK_ALIGN_CENTER;
  if (!strcmp(align, "top")) al = GTK_ALIGN_START;
  if (!strcmp(align, "bottom")) al = GTK_ALIGN_END;
  gtk_widget_set_valign(w, al);
  return exp_empty();
}

Exp *libkgtk_wg_vexpand (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkWidget *w = exp_get_ext(aparams[0]);
  int value = exp_get_bool(aparams[1]);
  gtk_widget_set_vexpand(w, value);
  return exp_empty();
}
