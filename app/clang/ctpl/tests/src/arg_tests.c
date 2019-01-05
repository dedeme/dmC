// Copyright 22-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "arg_tests.h"
#include <assert.h>
#include "Arg.h"

void arg_tests(void) {
  puts("Arg tests:");

  Arg *arg;
  char *error;

  arg_from_str(&error, &arg, "name : char  *");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "name"));
  assert(str_eq(arg_type(arg), "char  *"));
  assert(str_eq(arg_ffree(arg), "_s"));
  assert(str_eq(arg_fserial(arg), "_s"));
  arg_free(arg);

  arg_from_str(&error, &arg, "@age:int");
  assert(!error);
  assert(arg_gstype(arg) == arg_SETTER);
  assert(str_eq(arg_id(arg), "age"));
  assert(str_eq(arg_type(arg), "int"));
  assert(str_eq(arg_ffree(arg), "_i"));
  assert(str_eq(arg_fserial(arg), "_i"));

  StName *stName;
  stName_from_str(&error, &stName, "Person : serial");
  Buf *bf = buf_new();
  arg_w_struct(arg, bf);
  assert(str_ends(buf_str(bf), "  int age;\n"));
  arg_w_constructor_head(arg, bf);
  assert(str_ends(buf_str(bf), "int age"));
  arg_w_constructor_body(arg, bf);
  assert(str_ends(buf_str(bf), "  this->age = age;\n"));
  arg_w_destructor(arg, bf);
  assert(str_ends(buf_str(bf), "  this->age = age;\n"));
  arg_w_get(arg, bf, stName);
  assert(str_ends(
    buf_str(bf), "int person_age(Person *this) {\n  return this->age;\n}\n"
  ));
  arg_w_set(arg, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void person_set_age(Person *this, int v) {\n  this->age = v;\n}\n"
  ));
  arg_w_to(arg, bf);
  assert(str_ends(buf_str(bf), "  arr_push(a, js_wi_new(this->age));\n"));
  arg_w_from(arg, bf);
  assert(str_ends(buf_str(bf), "  int age = js_ri(arr_get(a, i++));\n"));
  arg_wh_get(arg, bf, stName);
  assert(str_ends(
    buf_str(bf), "int person_age(Person *this);\n"
  ));
  arg_wh_set(arg, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void person_set_age(Person *this, int v);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  arg_free(arg);

  arg_from_str(&error, &arg, "-d:time_t::date");
  assert(!error);
  assert(arg_gstype(arg) == arg_NONE);
  assert(str_eq(arg_id(arg), "d"));
  assert(str_eq(arg_type(arg), "time_t"));
  assert(str_eq(arg_ffree(arg), "_i"));
  assert(str_eq(arg_fserial(arg), "date"));
  arg_free(arg);

  arg_from_str(&error, &arg, "sel:int:_i:_b");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "sel"));
  assert(str_eq(arg_type(arg), "int"));
  assert(str_eq(arg_ffree(arg), "_i"));
  assert(str_eq(arg_fserial(arg), "_b"));
  arg_free(arg);

  arg_from_str(&error, &arg, "amount : double :: _d2");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "amount"));
  assert(str_eq(arg_type(arg), "double"));
  assert(str_eq(arg_ffree(arg), "_i"));
  assert(str_eq(arg_fserial(arg), "_d2"));
  arg_free(arg);

  arg_from_str(&error, &arg, "@pets : Arr * :: _s");
  assert(!error);
  assert(arg_gstype(arg) == arg_SETTER);
  assert(str_eq(arg_id(arg), "pets"));
  assert(str_eq(arg_type(arg), "Arr *"));
  assert(str_eq(arg_ffree(arg), "_a"));
  assert(str_eq(arg_fserial(arg), "_a _s"));

  stName_from_str(&error, &stName, "Person : serial");
  bf = buf_new();
  arg_w_struct(arg, bf);
  assert(str_ends(buf_str(bf), "  Arr *pets;\n"));
  arg_w_constructor_head(arg, bf);
  assert(str_ends(buf_str(bf), "Arr *pets"));
  arg_w_constructor_body(arg, bf);
  assert(str_ends(buf_str(bf), "  this->pets = pets;\n"));
  arg_w_destructor(arg, bf);
  assert(str_ends(buf_str(bf), "  arr_free(this->pets);\n"));
  arg_w_get(arg, bf, stName);
  assert(str_ends(
    buf_str(bf), "Arr *person_pets(Person *this) {\n  return this->pets;\n}\n"
  ));
  arg_w_set(arg, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void person_set_pets(Person *this, Arr *v) {\n  this->pets = v;\n}\n"
  ));
  arg_w_to(arg, bf);
  assert(str_ends(
    buf_str(bf),
    "  arr_push(a, arr_to_js_new(this->pets, (FTO)js_ws_new));\n"
  ));
  arg_w_from(arg, bf);
  assert(str_ends(
    buf_str(bf),
    "  Arr *pets = arr_from_js_new(arr_get(a, i++), (FFROM)js_rs_new, free);\n"
  ));
  arg_wh_get(arg, bf, stName);
  assert(str_ends(
    buf_str(bf), "Arr *person_pets(Person *this);\n"
  ));
  arg_wh_set(arg, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void person_set_pets(Person *this, Arr *v);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  arg_free(arg);

  arg_from_str(&error, &arg, "pets : Map * : _m : pet");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "pets"));
  assert(str_eq(arg_type(arg), "Map *"));
  assert(str_eq(arg_ffree(arg), "_m"));
  assert(str_eq(arg_fserial(arg), "_m pet"));

  stName_from_str(&error, &stName, "Person : serial");
  bf = buf_new();
  arg_w_struct(arg, bf);
  assert(str_ends(buf_str(bf), "  Map *pets;\n"));
  arg_w_constructor_head(arg, bf);
  assert(str_ends(buf_str(bf), "Map *pets"));
  arg_w_constructor_body(arg, bf);
  assert(str_ends(buf_str(bf), "  this->pets = pets;\n"));
  arg_w_destructor(arg, bf);
  assert(str_ends(buf_str(bf), "  map_free(this->pets);\n"));
  arg_w_get(arg, bf, stName);
  assert(str_ends(
    buf_str(bf), "Map *person_pets(Person *this) {\n  return this->pets;\n}\n"
  ));
  arg_w_set(arg, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void person_set_pets(Person *this, Map *v) {\n  this->pets = v;\n}\n"
  ));
  arg_w_to(arg, bf);
  assert(str_ends(
    buf_str(bf),
    "  arr_push(a, map_to_js_new(this->pets, (FTO)pet_to_js_new));\n"
  ));
  arg_w_from(arg, bf);
  assert(str_ends(
    buf_str(bf),
    "  Map *pets = map_from_js_new(arr_get(a, i++), "
    "(FFROM)pet_from_js_new, (FPROC)pet_free);\n"
  ));
  arg_wh_get(arg, bf, stName);
  assert(str_ends(
    buf_str(bf), "Map *person_pets(Person *this);\n"
  ));
  arg_wh_set(arg, bf, stName);
  assert(str_ends(
    buf_str(bf),
    "void person_set_pets(Person *this, Map *v);\n"
  ));

  buf_free(bf);
  stName_free(stName);
  arg_free(arg);

  arg_from_str(&error, &arg, "rare : Rare * : !kill_rare : ad_hoc");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "rare"));
  assert(str_eq(arg_type(arg), "Rare *"));
  assert(str_eq(arg_ffree(arg), "!kill_rare"));
  assert(str_eq(arg_fserial(arg), "ad_hoc"));
  assert(!arg_doc_null(arg));
  arg_free(arg);

  arg_from_str(&error, &arg, "normal : Normal * : normal : normal");
  arg_set_doc(arg, "A doc");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "normal"));
  assert(str_eq(arg_type(arg), "Normal *"));
  assert(str_eq(arg_ffree(arg), "normal"));
  assert(str_eq(arg_fserial(arg), "normal"));
  assert(str_eq(arg_doc_null(arg), "A doc"));
  arg_free(arg);

  arg_from_str(&error, &arg, "normal : Normal *");
  assert(!error);
  assert(arg_gstype(arg) == arg_GETTER);
  assert(str_eq(arg_id(arg), "normal"));
  assert(str_eq(arg_type(arg), "Normal *"));
  assert(str_eq(arg_ffree(arg), "normal"));
  assert(str_eq(arg_fserial(arg), "normal"));
  arg_free(arg);



  puts("    Finished");
}

