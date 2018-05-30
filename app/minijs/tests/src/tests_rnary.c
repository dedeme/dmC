// Copyright 7-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rnary.h"
#include "ast/Avalue.h"
#include "lexer/rvalue.h"

static char *rm_last(Achar *achar) {
  int ix = achar_size(achar) - 1;
  char *r = achar_get(achar, ix);
  achar_remove(achar, ix);
  return r;
}

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rnary() {
  puts("Reader: rnary");
  Tx *tx;
  Value *v, *v0, *v1, *v2;
  Avalue *vs;
  char *op;
  Achar *a;

  puts("--> lunary");

  tx = mk_tx("-34");
  tx = rvalue(&v, tx);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "-"));
  v0 = value_restore((Arr *)a);
  assert(!strcmp(achar_get(value_data(v0), 0), "34"));
  assert(type_eq(value_type(v0), value_type(v)));
  assert(!strcmp(type_id(value_type(v0)), "Int"));

  tx = mk_tx("++34");
  tx = rvalue(&v, tx);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "++"));
  v0 = value_restore((Arr *)a);
  assert(!strcmp(achar_get(value_data(v0), 0), "34"));
  assert(type_eq(value_type(v0), value_type(v)));
  assert(!strcmp(type_id(value_type(v0)), "Int"));

  tx = mk_tx("---34");
  tx = rvalue(&v, tx);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "--"));
  v = value_restore((Arr *)a);

  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "-"));
  v0 = value_restore((Arr *)a);
  assert(!strcmp(achar_get(value_data(v0), 0), "34"));
  assert(type_eq(value_type(v0), value_type(v)));
  assert(!strcmp(type_id(value_type(v0)), "Int"));

  puts("--> runary");

  tx = mk_tx("x++");
  tx = rvalue(&v, tx);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "++"));
  v0 = value_restore((Arr *)a);
  assert(value_vtype(v0) == VID);
  a = value_data(v0);
  assert(!strcmp(achar_get(a, 0), "x"));
  assert(type_eq(value_type(v0), value_type(v)));
  assert(type_is_unknown(value_type(v0)));

  puts("--> binary");

  tx = mk_tx("1 + 2");
  tx = rvalue(&v, tx);
  assert(value_vtype(v) == VBINARY);
  assert(!strcmp(type_to_str(value_type(v)), "Int"));
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "+"));
  vs = avalue_restore((Arr *)a);
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  assert(!strcmp(type_to_str(value_type(v0)), "Int"));
  assert(!strcmp(achar_get(value_data(v0), 0), "1"));
  assert(!strcmp(type_to_str(value_type(v1)), "Int"));
  assert(!strcmp(achar_get(value_data(v1), 0), "2"));

  tx = mk_tx("1 * 2 + 5");
  tx = rvalue(&v, tx);
  assert(value_vtype(v) == VBINARY);
  assert(!strcmp(type_to_str(value_type(v)), "Int"));
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "+"));
  vs = avalue_restore((Arr *)a);
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VBINARY);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "*"));
  v1 = avalue_get(vs, 1);
  assert(!strcmp(type_to_str(value_type(v1)), "Int"));
  assert(!strcmp(achar_get(value_data(v1), 0), "5"));

  tx = mk_tx("1 + 2 * 5");
  tx = rvalue(&v, tx);
  assert(value_vtype(v) == VBINARY);
  assert(!strcmp(type_to_str(value_type(v)), "Int"));
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "+"));
  vs = avalue_restore((Arr *)a);
  v0 = avalue_get(vs, 0);
  assert(!strcmp(type_to_str(value_type(v0)), "Int"));
  assert(!strcmp(achar_get(value_data(v0), 0), "1"));
  v1 = avalue_get(vs, 1);
  assert(value_vtype(v1) == VBINARY);
  a = value_data(v1);
  op = rm_last(a);
  assert(!strcmp(op, "*"));

  tx = mk_tx("(1 + 2) * 5");
  tx = rvalue(&v, tx);
  assert(value_vtype(v) == VBINARY);
  assert(!strcmp(type_to_str(value_type(v)), "Int"));
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "*"));
  vs = avalue_restore((Arr *)a);

  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VGROUP);
  assert(!strcmp(type_to_str(value_type(v0)), "Int"));
  v0 = value_restore((Arr *)value_data(v0));
  assert(value_vtype(v0) == VBINARY);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "+"));

  v1 = avalue_get(vs, 1);
  assert(!strcmp(type_to_str(value_type(v1)), "Int"));
  assert(!strcmp(achar_get(value_data(v1), 0), "5"));

  puts("--> ternary");

  tx = mk_tx("a?1:2.0");
  tx = rvalue(&v, tx);

  vs = avalue_restore((Arr *)value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v2 = avalue_get(vs, 2);
  assert(value_vtype(v) == VTERNARY);
  assert(value_vtype(v0) == VID);
  assert(value_vtype(v1) == VINT);
  assert(value_vtype(v2) == VFLOAT);

  tx = mk_tx("(5 == y) ? 1 + 4 : 2 * 5 ");
  tx = rvalue(&v, tx);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore((Arr *)value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v2 = avalue_get(vs, 2);

  assert(value_vtype(v1) == VBINARY);
  assert(value_vtype(v2) == VBINARY);

  assert(value_vtype(v0) == VGROUP);
  v = value_restore((Arr *)value_data(v0));
  assert(value_vtype(v) == VBINARY);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  vs = avalue_restore((Arr *)a);
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(achar_get(value_data(v0), 0), "5"));
  v0 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VID);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "y"));

  tx = mk_tx("5 == y ? 1 + 4 : 2 * 5 ");
  tx = rvalue(&v, tx);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore((Arr *)value_data(v));
  v = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v2 = avalue_get(vs, 2);

  assert(value_vtype(v1) == VBINARY);
  assert(value_vtype(v2) == VBINARY);

  assert(value_vtype(v) == VBINARY);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  vs = avalue_restore((Arr *)a);
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(achar_get(value_data(v0), 0), "5"));
  v0 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VID);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "y"));

  tx = mk_tx("5 == (y ? 1 + 4 : 2 * 5) ");
  tx = rvalue(&v, tx);

  assert(value_vtype(v) == VBINARY);
  a = value_data(v);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  vs = avalue_restore((Arr *)a);
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(achar_get(value_data(v0), 0), "5"));
  assert(value_vtype(v1) == VGROUP);

  puts("    Finished");
}
