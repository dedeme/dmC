// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Result.h"

static void buy (Map *p, double *d) {
}

/*.
# Result of person_deals
Result:serial
  error: char *
  @msg: char *
===
Person: serial
  name: const char *
  age: int
  # Assets of:
  #   Person
  #   spouse
  @assets: double:: _d2
  -children: Arr *:: person
  deals: Map *:: result
  ---
  portfolio: Map *:map_new(free)
  ---
  void(*buy)(Map *portfolio, double ammount): buy
*/

/*--*/

struct result_Result {
  char *error;
  char *msg;
};

Result *result_new(char *error, char *msg) {
  Result *this = malloc(sizeof(Result));
  this->error = error;
  this->msg = msg;
  return this;
}

void result_free(Result *this) {
  free(error);
  free(msg);
  free(this);
};

char *result_error(Result *this) {
  return this->error;
}

char *result_msg(Result *this) {
  return this->msg;
}

void result_set_msg(Result *this, char *v) {
  this->msg = v;
}

Js *result_to_js_new(Result *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->error));
  arr_push(a, js_ws_new(this->msg));
  return js_wa_new(a);
}

Result *result_from_js_new(Js *js) {
  Result *this = malloc(sizeof(Result));
  // Arr[Js]
  Arr *a = js_ra_new(a);
  int i = 0;
  this->error = js_rs_new(arr_get(a, i++));
  this->msg = js_rs_new(arr_get(a, i++));
  return this;
}

struct person_Person {
  const char *name;
  int age;
  double assets;
  Arr *children;
  Map *deals;
  Map *portfolio;
  void(*buy)(Map *portfolio, double ammount);
};

Person *person_new(
  const char *name,
  int age,
  double assets,
  Arr *children,
  Map *deals
) {
  Person *this = malloc(sizeof(Person));
  this->name = name;
  this->age = age;
  this->assets = assets;
  this->children = children;
  this->deals = deals;
  this->portfolio = map_new(free);
  void(*this->buy)(Map *portfolio, double ammount) = buy;
  return this;
}

void person_free(Person *this) {
  free(name);
  arr_free(children);
  map_free(deals);
  map_free(portfolio);
  free(this);
};

const char *person_name(Person *this) {
  return this->name;
}

int person_age(Person *this) {
  return this->age;
}

double person_assets(Person *this) {
  return this->assets;
}

Map *person_deals(Person *this) {
  return this->deals;
}

Map *person_portfolio(Person *this) {
  return this->portfolio;
}

void(*person_buy(Person *this))(Map *portfolio, double ammount) {
  return this->buy;
}

void person_set_assets(Person *this, double v) {
  this->assets = v;
}

Js *person_to_js_new(Person *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->name));
  arr_push(a, js_wi_new(this->age));
  arr_push(a, js_wd_new(this->assets, 2));
  arr_push(a, arr_to_js_new(this->children, (FTO)person_to_js_new));
  arr_push(a, map_to_js_new(this->deals, (FTO)result_to_js_new));
  return js_wa_new(a);
}

Person *person_from_js_new(Js *js) {
  Person *this = malloc(sizeof(Person));
  // Arr[Js]
  Arr *a = js_ra_new(a);
  int i = 0;
  this->name = js_rs_new(arr_get(a, i++));
  this->age = js_ri(arr_get(a, i++));
  this->assets = js_rd(arr_get(a, i++));
  this->children = arr_from_js_new(arr_get(a, i++), (FFROM)person_from_js_new, (FPROC)person_free);
  this->deals = map_from_js_new(arr_get(a, i++), (FFROM)result_from_js_new, (FPROC)result_free);
  return this;
}

/*--*/
