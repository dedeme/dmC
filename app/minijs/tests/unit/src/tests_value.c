// Copyright 24-Mar-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_value.h"
#include "ast/Value.h"
#include "DEFS.h"

void tests_value() {
  printf("Value test\n");

  Value *v = value_new_null();
  Value *v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_bool("true");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_byte("12b");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_int("12345");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_float("12.34");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_char("'a'");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_str("\"abc\"");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  Arr/*char*/ *ks = arr_new();
  Arr/*Value*/ *a = arr_new();
  v = value_new_arr(a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_map(ks, a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_fn(ks, arr_new());
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_fid("id", a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  arr_add(a, value_new_str("\"abc\""));
  arr_add(ks, value_new_str("\"abc\""));
  v = value_new_arr(a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_map(ks, a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_fn(ks, arr_new());
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_fid("id", a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  arr_add(a, v2);
  arr_add(ks, value_new_str("\"abc2\""));
  v = value_new_arr(a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_map(ks, a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_fn(ks, arr_new());
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));
  v = value_new_fid("id", a);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_id("a");
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  v = value_new_lmonadic("a", v);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));

  Value *v1 = value_new_rmonadic("a", v);
  v2 = value_restore(value_serialize(v1));
  assert(value_eq(v1, v2));

  Value *v3 = value_new_binary("a", v, v1);
  v2 = value_restore(value_serialize(v3));
  assert(value_eq(v3, v2));

  v = value_new_ternary("a", v, v1, v3);
  v2 = value_restore(value_serialize(v));
  assert(value_eq(v, v2));


  printf( "    Finished\n");
}
