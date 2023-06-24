// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/calendar/calendar.h"
#include "data/calendar/marketDay.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Calendar *calendar_new (Timetable *general, Arr *holidays, Arr *special_days) {
  Calendar *this = MALLOC(Calendar);
  this->general = general;
  this->holidays = holidays;
  this->special_days = special_days;
  return this;
}

Calendar *calendar_mk_general (Calendar *this, Timetable *general) {
  return calendar_new(general, this->holidays, this->special_days);
}

Calendar *calendar_mk_holidays (Calendar *this, Arr *holidays) {
  return calendar_new(this->general, holidays, this->special_days);
}

Calendar *calendar_mk_special_days (Calendar *this, Arr *special_days) {
  return calendar_new(this->general, this->holidays, special_days);
}

char *calendar_to_js (Calendar *this) {
  return js_wa(arr_new_from(
    timetable_to_js(this->general),
    arr_to_js(this->holidays, (FTO) js_ws),
    arr_to_js(this->special_days, (FTO) marketDay_to_js),
    NULL
  ));
}

Calendar *calendar_from_js (char *js) {
  Arr *a = js_ra(js);
  return calendar_new(
    timetable_from_js(arr_get(a, 0)),
    arr_from_js(arr_get(a, 1), (FFROM) js_rs),
    arr_from_js(arr_get(a, 2), (FFROM) marketDay_from_js)
  );
}
