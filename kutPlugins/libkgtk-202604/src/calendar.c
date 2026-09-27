// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "calendar.h"
#include "kut/DEFS.h"
#include "kut/time.h"
#include "DEFS.h"
#include <gtk/gtk.h>

Exp *libkgtk_calendar_new (Arr *params) {
  GtkCalendar *cl = GTK_CALENDAR(gtk_calendar_new());
  return exp_ext(cl);
}

Exp *libkgtk_calendar_get_date (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkCalendar *w = GTK_CALENDAR(exp_get_ext(aparams[0]));
  GDateTime* dt = gtk_calendar_get_date(w);
  int y;
  int m;
  int d;
  g_date_time_get_ymd(dt, &y, &m, &d);
  g_date_time_unref(dt);

  return exp_string(time_to_str(time_new(d, m, y)));
}

Exp *libkgtk_calendar_date (Arr *params) {
  Exp **aparams = (Exp **)arr_begin(params);
  GtkCalendar *cl = GTK_CALENDAR(exp_get_ext(aparams[0]));
  char *date = exp_get_string(aparams[1]);
  Time t = time_from_str(date);

  gtk_calendar_set_year(cl, time_year(t));
  gtk_calendar_set_month(cl, time_month(t));
  gtk_calendar_set_day(cl, time_day(t));
  return exp_empty();
}
