// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "db/calendarTb.h"
#include "db.h"

static char *fpath = "calendar.tb";

void calendarTb_init (void) {
  if (!db_exists(fpath))
    calendarTb_write(calendar_new(
      timetable_new(0, 0, 23, 55),
      arr_new(),
      arr_new()
    ));
}

///
char *calendarTb_read_js (void) {
  return db_read(fpath);
}

///
Calendar *calendarTb_read (void) {
  return calendar_from_js(db_read(fpath));
}

///
void calendarTb_write_js (char *js) {
  db_write(fpath, js);
}

///
void calendarTb_write (Calendar *data) {
  db_write(fpath, calendar_to_js(data));
}
