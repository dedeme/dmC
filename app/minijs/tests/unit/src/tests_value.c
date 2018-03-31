// Copyright 24-Mar-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_value.h"
#include "ast/Value.h"
#include "DEFS.h"

static bool value_ok(Value *v) {
  char *serial = value_serialize(v);
  return !strcmp(serial, value_serialize(value_restore(serial)));
}

void tests_value() {
  printf("Value test\n");

  Value *v = value_new_null();
  assert(value_ok(v));

  v = value_new_bool("true");
  assert(value_ok(v));

  v = value_new_byte("12b");
  assert(value_ok(v));

  v = value_new_int("12345");
  assert(value_ok(v));

  v = value_new_float("12.34");
  assert(value_ok(v));

  v = value_new_char("'a'");
  assert(value_ok(v));

  v = value_new_str("\"abc\"");
  assert(value_ok(v));

  Arr/*char*/ *ks = arr_new();
  Arr/*Value*/ *a = arr_new();
  v = value_new_arr(a);
  assert(value_ok(v));
  v = value_new_map(ks, a);
  assert(value_ok(v));
  v = value_new_fn(ks, arr_new());
  assert(value_ok(v));
  v = value_new_fid("id", a);
  assert(value_ok(v));

  arr_add(a, value_new_str("\"abc\""));
  arr_add(ks, value_new_str("\"abc\""));
  v = value_new_arr(a);
  assert(value_ok(v));
  v = value_new_map(ks, a);
  assert(value_ok(v));
  v = value_new_fn(ks, arr_new());
  assert(value_ok(v));
  v = value_new_fid("id", a);
  assert(value_ok(v));

  arr_add(a, value_restore(value_serialize(v)));
  arr_add(ks, value_new_str("\"abc2\""));
  v = value_new_arr(a);
  assert(value_ok(v));
  v = value_new_map(ks, a);
  assert(value_ok(v));
  v = value_new_fn(ks, arr_new());
  assert(value_ok(v));
  v = value_new_fid("id", a);
  assert(value_ok(v));

  v = value_new_id("a");
  assert(value_ok(v));

  v = value_new_lunary("a", v);
  assert(value_ok(v));

  Value *v1 = value_new_runary("a", v);
  assert(value_ok(v1));

  Value *v2 = value_new_binary("a", v, v1);
  assert(value_ok(v2));

  v = value_new_ternary(v, v1, v2);
  assert(value_ok(v));

  printf( "    Finished\n");
}
