// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef RESULT_H
  #define RESULT_H

#include "dmc/std.h"

/*--*/

///  Result of person_deals
typedef struct result_Result Result;

///
Result *result_new(char *error, char *msg);

///
void result_free(Result *this);

///
char *result_error(Result *this);

///
char *result_msg(Result *this);

///
void result_set_msg(Result *this, char *v);

///
Js *result_to_js_new(Result *this);

///
Result *result_from_js_new(Js *js);

///
typedef struct person_Person Person;

///
Person *person_new(
  const char *name,
  int age,
  double assets,
  Arr *children,
  Map *deals
);

///
void person_free(Person *this);

///
const char *person_name(Person *this);

///
int person_age(Person *this);

///  Assets of:
///    Person
///    spouse
double person_assets(Person *this);

///
Map *person_deals(Person *this);

///
Map *person_portfolio(Person *this);

///
void(*person_buy(Person *this))(Map *portfolio, double ammount);

///  Assets of:
///    Person
///    spouse
void person_set_assets(Person *this, double v);

///
Js *person_to_js_new(Person *this);

///
Person *person_from_js_new(Js *js);

/*--*/

#endif
