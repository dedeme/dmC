// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_type.h"
#include "ast/Type.h"
#include "ast/Imported.h"

static void test(Type *t, enum Type_t tt, char *ts) {
//  puts(type_to_str(t));
  assert(type_type(t) == tt);
  assert(!strcmp(type_to_str(t), ts));
  assert(type_eq(t, type_restore(type_serialize(t))));
  assert(type_child(t, t));
}

void tests_type() {
  puts("Type 1");

  test(type_new_unknown(), UNKNOWN, "?");
  test(type_new_void(), VOID, "");
  test(type_new_any(), ANY, "*");
  test(type_new_bool(), DATA, "Bool");
  test(type_new_byte(), DATA, "Byte");
  test(type_new_int(), DATA, "Int");
  test(type_new_float(), DATA, "Float");
  test(type_new_char(), DATA, "Char");
  test(type_new_str(), DATA, "Str");
  test(type_new_arr(type_new_int()), DATA, "Arr<Int>");
  test(type_new_map(type_new_int()), DATA, "Map<Int>");
  Atype *ts = atype_new();
  atype_add(ts, type_new_void());
  test(type_new_fn(ts), FN, "(:)");
  ts = atype_new();
  atype_add(ts, type_new_any());
  test(type_new_fn(ts), FN, "(:*)");
  ts = atype_new();
  atype_add(ts, type_new_any());
  atype_add(ts, type_new_str());
  test(type_new_fn(ts), FN, "(*:Str)");
  ts = atype_new();
  atype_add(ts, type_new_any());
  atype_add(ts, type_new_void());
  test(type_new_fn(ts), FN, "(*:)");
  ts = atype_new();
  atype_add(ts, type_new_int());
  atype_add(ts, type_new_any());
  atype_add(ts, type_new_void());
  test(type_new_fn(ts), FN, "(Int,*:)");
  ts = atype_new();
  atype_add(ts, type_new_int());
  atype_add(ts, type_new_bool());
  atype_add(ts, type_new_float());
  test(type_new_fn(ts), FN, "(Int,Bool:Float)");

  puts("Type 2");
  Imported *im = imported_get();
  Class *c0 = class_new("C0");
  imported_add(im, c0);
  Class *c1 = class_new("C1");
  class_set_super(c1, class_id(c0));
  imported_add(im, c1);
  Class *c2 = class_new("C2");
  class_set_super(c2, class_id(c1));
  imported_add(im, c2);

  test(class__type(c0, arr_new()), DATA, "C0");
  test(class__type(c1, arr_new()), DATA, "C1");
  test(class__type(c2, arr_new()), DATA, "C2");

  assert(type_child(class__type(c0, arr_new()), class__type(c1, arr_new())));
  assert(type_child(class__type(c0, arr_new()), class__type(c2, arr_new())));
  assert(type_child(class__type(c1, arr_new()), class__type(c2, arr_new())));
  assert(!type_child(class__type(c2, arr_new()), class__type(c1, arr_new())));

  assert(!type_child(type_new_unknown(), type_new_any()));
  assert(!type_child(type_new_void(), type_new_any()));
  assert(!type_child(type_new_any(), type_new_void()));
  assert(!type_child(type_new_any(), type_new_unknown()));
  assert(type_child(type_new_any(), type_new_str()));
  assert(!type_child(type_new_str(), type_new_any()));

  Atype *ts0 = atype_new();
  atype_add(ts0, type_new_any());
  atype_add(ts0, type_new_any());
  Atype *ts1 = atype_new();
  atype_add(ts1, type_new_any());
  atype_add(ts1, type_new_str());
  Atype *ts2 = atype_new();
  atype_add(ts2, type_new_str());
  atype_add(ts2, type_new_any());
  Atype *ts3 = atype_new();
  atype_add(ts3, type_new_str());
  atype_add(ts3, type_new_str());

  assert(type_child(type_new_fn(ts0), type_new_fn(ts0)));
  assert(type_child(type_new_fn(ts0), type_new_fn(ts1)));
  assert(type_child(type_new_fn(ts0), type_new_fn(ts2)));
  assert(type_child(type_new_fn(ts0), type_new_fn(ts3)));

  assert(!type_child(type_new_fn(ts1), type_new_fn(ts0)));
  assert(type_child(type_new_fn(ts1), type_new_fn(ts1)));
  assert(!type_child(type_new_fn(ts1), type_new_fn(ts2)));
  assert(type_child(type_new_fn(ts1), type_new_fn(ts3)));

  assert(!type_child(type_new_fn(ts2), type_new_fn(ts0)));
  assert(!type_child(type_new_fn(ts2), type_new_fn(ts1)));
  assert(type_child(type_new_fn(ts2), type_new_fn(ts2)));
  assert(type_child(type_new_fn(ts2), type_new_fn(ts3)));

  assert(!type_child(type_new_fn(ts3), type_new_fn(ts0)));
  assert(!type_child(type_new_fn(ts3), type_new_fn(ts1)));
  assert(!type_child(type_new_fn(ts3), type_new_fn(ts2)));
  assert(type_child(type_new_fn(ts3), type_new_fn(ts3)));

  puts("    Finished");
}
