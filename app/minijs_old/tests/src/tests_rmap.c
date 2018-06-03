// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rmap.h"
#include "ast/Avalue.h"
#include "lexer/rvalue.h"
#include "ast/Atype.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rmap() {
  puts("Reader: rmap");

  Tx *tx;
  Value *v;
  Achar *a;
  Type *tp;
  Atype *params;

  tx = mk_tx("{  } ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VMAP);
  a = value_data(v);
  assert(!achar_size(a));
  assert(type_is_unknown(value_type(v)));

  tx = mk_tx("{ \"a\" : a == b } ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VMAP);
  a = value_data(v);
  assert(achar_size(a) == 2);
  tp = value_type(v);
  assert(!strcmp(type_id(tp), "Map"));
  params = type_params(tp);
  assert(arr_size(params) == 1);
  tp = atype_get(params, 0);
  assert(!strcmp(type_id(tp), "Bool"));

  tx = mk_tx("{ \"a\" : 5, \"b\":8 * 4 } ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VMAP);
  a = value_data(v);
  assert(achar_size(a) == 4);
  tp = value_type(v);
  assert(!strcmp(type_id(tp), "Map"));
  params = type_params(tp);
  assert(arr_size(params) == 1);
  tp = atype_get(params, 0);
  assert(!strcmp(type_id(tp), "Int"));

  puts("    Finished");
}
