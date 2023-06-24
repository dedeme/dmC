// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "pgs/settings/calendarPg/calendarPg.h"
#include "pgs.h"
#include "db/calendarTb.h"
#include "data/calendar/marketDay.h"

// mrq is Map<char>
void calendarPg_process (Map *mrq) {
  char *rq = pgs_rs(mrq, "rq");
  if (str_eq(rq, "idata")) {
    //<char>
    Map *rp = map_new();
    map_add(rp, "calendar", calendarTb_read_js());
    pgs_print(rp);
  } else if (str_eq(rq, "setGeneral")) {
    calendarTb_write(calendar_mk_general(
      calendarTb_read(), timetable_from_js(pgs_r(mrq, "timetable"))
    ));
    pgs_print_empty();
  } else if (str_eq(rq, "setHolidays")) {
    calendarTb_write(calendar_mk_holidays(
      calendarTb_read(),
      arr_from_js(pgs_r(mrq, "holidays"), (FFROM) js_rs)
    ));
    pgs_print_empty();
  } else if (str_eq(rq, "setSpecialDays")) {
    calendarTb_write(calendar_mk_holidays(
      calendarTb_read(),
      arr_from_js(pgs_r(mrq, "specialDays"), (FFROM) marketDay_from_js)
    ));
    pgs_print_empty();
  } else {
    EXC_GENERIC(str_f(
      "Value '%s' not found for 'rq' in '%s'", rq, js_wo(mrq)
    ));
  }
}
