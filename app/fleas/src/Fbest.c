// Copyright 21-Nov-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Fbest.h"

/* .
Fbest: serial
  date: char *
  fresults: Fresults *
  average: double:: _d2
  cv: double:: _d6
*/

/*--*/

struct fbest_Fbest {
  char *date;
  Fresults *fresults;
  double average;
  double cv;
};

Fbest *fbest_new(
  char *date,
  Fresults *fresults,
  double average,
  double cv
) {
  Fbest *this = malloc(sizeof(Fbest));
  this->date = date;
  this->fresults = fresults;
  this->average = average;
  this->cv = cv;
  return this;
}

void fbest_free(Fbest *this) {
  free(this->date);
  fresults_free(this->fresults);
  free(this);
};

char *fbest_date(Fbest *this) {
  return this->date;
}

Fresults *fbest_fresults(Fbest *this) {
  return this->fresults;
}

double fbest_average(Fbest *this) {
  return this->average;
}

double fbest_cv(Fbest *this) {
  return this->cv;
}

Js *fbest_to_js_new(Fbest *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->date));
  arr_push(a, fresults_to_js_new(this->fresults));
  arr_push(a, js_wd_new(this->average, 2));
  arr_push(a, js_wd_new(this->cv, 6));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Fbest *fbest_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *date = js_rs_new(arr_get(a, i++));
  Fresults *fresults = fresults_from_js_new(arr_get(a, i++));
  double average = js_rd(arr_get(a, i++));
  double cv = js_rd(arr_get(a, i++));
  arr_free(a);
  return fbest_new(date, fresults, average, cv);
}

/*--*/

Fbest *fbest_copy_new(Fbest *this) {
  return fbest_new(
    str_new(this->date), fresults_copy_new(this->fresults),
    this->average, this->cv
  );
}
