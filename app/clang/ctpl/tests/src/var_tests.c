// Copyright 23-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "var_tests.h"
#include <assert.h>
#include "Var.h"

void var_tests(void) {
  puts("Var tests:");

  Var *var;
  char *error;

  var_from_str(&error, &var, "name : char  * : \"\"");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "name"));
  assert(str_eq(var_type(var), "char  *"));
  assert(str_eq(var_ffree(var), "_s"));
  assert(str_eq(var_value(var), "\"\""));
  var_free(var);

  var_from_str(&error, &var, "age:int:34");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "age"));
  assert(str_eq(var_type(var), "int"));
  assert(str_eq(var_ffree(var), "_i"));
  assert(str_eq(var_value(var), "34"));

  StName *stName;
  stName_from_str(&error, &stName, "Person : serial");
  Buf *bf = buf_new();
  var_w_struct(var, bf);
  assert(str_ends(buf_str(bf), "  int age;\n"));
  var_w_constructor(var, bf);
  assert(str_ends(buf_str(bf), "  this->age = 34;\n"));
  var_w_destructor(var, bf);
  assert(str_ends(buf_str(bf), "  this->age = 34;\n"));
  var_w_get(var, bf, stName);
  assert(str_ends(
    buf_str(bf), "int person_age(Person *this) {\n  return this->age;\n}\n"
  ));
  var_wh_get(var, bf, stName);
  assert(str_ends(
    buf_str(bf), "int person_age(Person *this);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  var_free(var);

  var_from_str(&error, &var, "-d:time_t:date_now():_i");
  assert(!error);
  assert(var_gstype(var) == var_NONE);
  assert(str_eq(var_id(var), "d"));
  assert(str_eq(var_type(var), "time_t"));
  assert(str_eq(var_ffree(var), "_i"));
  assert(str_eq(var_value(var), "date_now()"));
  var_free(var);

  var_from_str(&error, &var, "sel:int:1:_i");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "sel"));
  assert(str_eq(var_type(var), "int"));
  assert(str_eq(var_ffree(var), "_i"));
  assert(str_eq(var_value(var), "1"));
  var_free(var);

  var_from_str(&error, &var, "- amount : double : 12.14 : _i");
  assert(!error);
  assert(var_gstype(var) == var_NONE);
  assert(str_eq(var_id(var), "amount"));
  assert(str_eq(var_type(var), "double"));
  assert(str_eq(var_ffree(var), "_i"));
  assert(str_eq(var_value(var), "12.14"));
  var_free(var);

  var_from_str(&error, &var, "pets : Arr * : arr_new(free)");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "pets"));
  assert(str_eq(var_type(var), "Arr *"));
  assert(str_eq(var_ffree(var), "_a"));
  assert(str_eq(var_value(var), "arr_new(free)"));

  stName_from_str(&error, &stName, "Person : serial");
  bf = buf_new();
  var_w_struct(var, bf);
  assert(str_ends(buf_str(bf), "  Arr *pets;\n"));
  var_w_constructor(var, bf);
  assert(str_ends(buf_str(bf), "  this->pets = arr_new(free);\n"));
  var_w_destructor(var, bf);
  assert(str_ends(buf_str(bf), "  arr_free(this->pets);\n"));
  var_w_get(var, bf, stName);
  assert(str_ends(
    buf_str(bf), "Arr *person_pets(Person *this) {\n  return this->pets;\n}\n"
  ));
  var_wh_get(var, bf, stName);
  assert(str_ends(
    buf_str(bf), "Arr *person_pets(Person *this);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  var_free(var);

  var_from_str(&error, &var, "pets : Map * : map_new((FPROC)pet_free) : _m");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "pets"));
  assert(str_eq(var_type(var), "Map *"));
  assert(str_eq(var_ffree(var), "_m"));
  assert(str_eq(var_value(var), "map_new((FPROC)pet_free)"));

  stName_from_str(&error, &stName, "Person : serial");
  bf = buf_new();
  var_w_struct(var, bf);
  assert(str_ends(buf_str(bf), "  Map *pets;\n"));
  var_w_constructor(var, bf);
  assert(str_ends(buf_str(bf), "  this->pets = map_new((FPROC)pet_free);\n"));
  var_w_destructor(var, bf);
  assert(str_ends(buf_str(bf), "  map_free(this->pets);\n"));
  var_w_get(var, bf, stName);
  assert(str_ends(
    buf_str(bf), "Map *person_pets(Person *this) {\n  return this->pets;\n}\n"
  ));
  var_wh_get(var, bf, stName);
  assert(str_ends(
    buf_str(bf), "Map *person_pets(Person *this);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  var_free(var);

  var_from_str(&error, &var, "rare : Rare * : rare_new(20) : !kill_rare");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "rare"));
  assert(str_eq(var_type(var), "Rare *"));
  assert(str_eq(var_ffree(var), "!kill_rare"));
  assert(str_eq(var_value(var), "rare_new(20)"));
  assert(!var_doc_null(var));
  var_free(var);

  var_from_str(&error, &var, "normal : Normal * : normal_new() : normal");
  var_set_doc(var, "A doc");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "normal"));
  assert(str_eq(var_type(var), "Normal *"));
  assert(str_eq(var_ffree(var), "normal"));
  assert(str_eq(var_value(var), "normal_new()"));
  assert(str_eq(var_doc_null(var), "A doc"));
  var_free(var);

  var_from_str(&error, &var, "normal : Normal * : normal_new()");
  assert(!error);
  assert(var_gstype(var) == var_GETTER);
  assert(str_eq(var_id(var), "normal"));
  assert(str_eq(var_type(var), "Normal *"));
  assert(str_eq(var_ffree(var), "normal"));
  assert(str_eq(var_value(var), "normal_new()"));
  var_free(var);

  puts("    Finished");
}

