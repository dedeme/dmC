// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "stName_tests.h"
#include <assert.h>
#include "StName.h"

void stName_tests(void) {
  puts("StName tests:");

  StName *name;
  char *error;

  stName_from_str(&error, &name, " Person ");
  assert(str_eq(stName_id(name), "Person"));
  assert(str_eq(stName_prefix(name), "person"));
  assert(stName_public(name));
  assert(stName_stype(name) == stName_NONE);

  Buf *bf = buf_new();
  stName_w_prefix(name, bf);
  assert(str_ends(buf_str(bf), "person_"));
  stName_w_struct(name, bf);
  assert(str_ends(buf_str(bf), "struct person_Person {\n"));
  stName_w_constructor_header(name, bf);
  assert(str_ends(buf_str(bf), "Person *person_new("));
  stName_w_constructor_body(name, bf);
  assert(str_ends(buf_str(bf), "  Person *this = malloc(sizeof(Person));\n"));
  stName_w_destructor(name, bf);
  assert(str_ends(buf_str(bf), "void person_free(Person *this) {\n"));
  stName_w_to_header(name, bf);
  assert(str_ends(buf_str(bf), "Js *person_to_js_new(Person *this) {\n"));
  stName_w_from_header(name, bf);
  assert(str_ends(buf_str(bf), "Person *person_from_js_new(Js *js) {\n"));
  stName_w_from_body(name, bf);
  assert(str_ends(buf_str(bf), "return person_new("));
  stName_wh_struct(name, bf);
  assert(str_ends(buf_str(bf), "typedef struct person_Person Person;\n"));
  stName_wh_constructor(name, bf);
  assert(str_ends(buf_str(bf), "Person *person_new("));
  stName_wh_destructor(name, bf);
  assert(str_ends(buf_str(bf), "void person_free(Person *this);\n"));
  stName_wh_to(name, bf);
  assert(str_ends(buf_str(bf), "Js *person_to_js_new(Person *this);\n"));
  stName_wh_from(name, bf);
  assert(str_ends(buf_str(bf), "Person *person_from_js_new(Js *js);\n"));


  buf_free(bf);
  stName_free(name);

  stName_from_str(&error, &name, " - Person : serial ");
  assert(str_eq(stName_id(name), "Person"));
  assert(str_eq(stName_prefix(name), "person"));
  assert(!stName_public(name));
  assert(stName_stype(name) == stName_SERIAL);

  bf = buf_new();

  stName_w_constructor_header(name, bf);
  assert(str_ends(buf_str(bf), "Person *_person_new("));

  buf_free(bf);
  stName_free(name);

  stName_from_str(&error, &name, "-Person:to");
  assert(str_eq(stName_id(name), "Person"));
  assert(str_eq(stName_prefix(name), "person"));
  assert(!stName_public(name));
  assert(stName_stype(name) == stName_TO);
  stName_free(name);

  stName_from_str(&error, &name, "Person:from");
  assert(str_eq(stName_id(name), "Person"));
  assert(str_eq(stName_prefix(name), "person"));
  assert(stName_public(name));
  assert(stName_stype(name) == stName_FROM);
  stName_free(name);

  stName_from_str(&error, &name, "Person:frxom");
  assert(*error);
  free(error);

  puts("    Finished");
}
