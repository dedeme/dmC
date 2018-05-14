// Copyright 11-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rwith.h"
#include "ast/Avalue.h"
#include "lexer/rvalue.h"

static char *rm_last(Achar *achar) {
  int ix = arr_size(achar) - 1;
  char *r = arr_get(achar, ix);
  arr_remove(achar, ix);
  return r;
}

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rwith() {
  puts("Reader: rwith");
  Tx *tx;
  Value *v, *v0, *v1, *v2;
  Avalue *vs;
  Achar *a;
  char *op;

  tx = mk_tx("with a \\ 2 : 4 ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VINT);
  assert(!strcmp(achar_get(value_data(v), 0), "4"));

  tx = mk_tx("with a \\ 5 : true \\ : false ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore(value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v2 = avalue_get(vs, 2);

  assert(value_vtype(v0) == VBINARY);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  assert(value_vtype(v1) == VBOOL);
  assert(value_vtype(v2) == VBOOL);

  tx = mk_tx("with a \\ 5 : true \\ 1, 2 : true \\: false ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore(value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v = avalue_get(vs, 2);
  assert(value_vtype(v0) == VBINARY);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  assert(value_vtype(v1) == VBOOL);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore(value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v = avalue_get(vs, 2);
  assert(value_vtype(v0) == VBINARY);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "||"));
  assert(value_vtype(v1) == VBOOL);
  assert(value_vtype(v2) == VBOOL);

  vs = avalue_restore(a);
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VBINARY);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  vs = avalue_restore(a);
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VID);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "a"));
  v0 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(achar_get(value_data(v0), 0), "1"));

  assert(value_vtype(v1) == VBINARY);
  a = value_data(v1);
  op = rm_last(a);
  assert(!strcmp(op, "=="));
  vs = avalue_restore(a);
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VID);
  a = value_data(v0);
  op = rm_last(a);
  assert(!strcmp(op, "a"));
  v0 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(achar_get(value_data(v0), 0), "2"));



  puts("    Finished");
}
