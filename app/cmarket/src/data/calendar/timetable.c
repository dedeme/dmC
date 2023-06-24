// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/calendar/timetable.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Timetable *timetable_new (int hopen, int mopen, int hclose, int mclose) {
  Timetable *this = MALLOC(Timetable);
  this->hopen = hopen;
  this->mopen = mopen;
  this->hclose = hclose;
  this->mclose = mclose;
  return this;
}

char *timetable_to_js (Timetable *this) {
  return js_wa(arr_new_from(
    js_wi(this->hopen),
    js_wi(this->mopen),
    js_wi(this->hclose),
    js_wi(this->mclose),
    NULL
  ));
}

Timetable *timetable_from_js (char *js) {
  Arr *a = js_ra(js);
  return timetable_new(
    js_ri(arr_get(a, 0)),
    js_ri(arr_get(a, 1)),
    js_ri(arr_get(a, 2)),
    js_ri(arr_get(a, 3))
  );
}
