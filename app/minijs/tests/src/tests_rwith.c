// Copyright 11-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rwith.h"
#include "ast/Avalue.h"
#include "lexer/rvalue.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rwith() {
  puts("Reader: rwith");
  Tx *tx;
  Value *v, *v0, *v1, *v2;
  Avalue *vs;
  Achar *a;

  tx = mk_tx("with a \\ : 4 ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VINT);
  assert(!strcmp(value_data(v), "4"));

  tx = mk_tx("with a \\ 5 : true \\ : false ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore(value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v2 = avalue_get(vs, 2);

  assert(value_vtype(v0) == VBINARY);
  a = achar_restore(value_data(v0));
  assert(!strcmp(achar_get(a, 0), "=="));
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
  a = achar_restore(value_data(v0));
  assert(!strcmp(achar_get(a, 0), "=="));
  assert(value_vtype(v1) == VBOOL);

  assert(value_vtype(v) == VTERNARY);
  vs = avalue_restore(value_data(v));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  v = avalue_get(vs, 2);
  assert(value_vtype(v0) == VBINARY);
  a = achar_restore(value_data(v0));
  assert(!strcmp(achar_get(a, 0), "||"));
  assert(value_vtype(v1) == VBOOL);
  assert(value_vtype(v2) == VBOOL);

  vs = avalue_restore(achar_get(a, 1));
  v0 = avalue_get(vs, 0);
  v1 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VBINARY);
  a = achar_restore(value_data(v0));
  assert(!strcmp(achar_get(a, 0), "=="));
  vs = avalue_restore(achar_get(a, 1));
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VID);
  a = achar_restore(value_data(v0));
  assert(!strcmp(achar_get(a, 0), "a"));
  v0 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(value_data(v0), "1"));

  assert(value_vtype(v1) == VBINARY);
  a = achar_restore(value_data(v1));
  assert(!strcmp(achar_get(a, 0), "=="));
  vs = avalue_restore(achar_get(a, 1));
  v0 = avalue_get(vs, 0);
  assert(value_vtype(v0) == VID);
  a = achar_restore(value_data(v0));
  assert(!strcmp(achar_get(a, 0), "a"));
  v0 = avalue_get(vs, 1);
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(value_data(v0), "2"));



  puts("    Finished");
}
