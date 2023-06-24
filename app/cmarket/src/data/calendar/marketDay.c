// Copyright 01-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/calendar/marketDay.h"
#include "kut/DEFS.h"
#include "kut/js.h"

MarketDay *marketDay_new (
  char *date, int hopen, int mopen, int hclose, int mclose
) {
  MarketDay *this = MALLOC(MarketDay);
  this->date = date;
  this->hopen = hopen;
  this->mopen = mopen;
  this->hclose = hclose;
  this->mclose = mclose;
  return this;
}

char *marketDay_to_js (MarketDay *this) {
  return js_wa(arr_new_from(
    js_ws(this->date),
    js_wi(this->hopen),
    js_wi(this->mopen),
    js_wi(this->hclose),
    js_wi(this->mclose),
    NULL
  ));
}

MarketDay *marketDay_from_js (char *js) {
  Arr *a = js_ra(js);
  return marketDay_new(
    js_rs(arr_get(a, 0)),
    js_ri(arr_get(a, 1)),
    js_ri(arr_get(a, 2)),
    js_ri(arr_get(a, 3)),
    js_ri(arr_get(a, 4))
  );
}
