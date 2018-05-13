// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rarr.h"
#include "ast/Avalue.h"
#include "lexer/rvalue.h"
#include "ast/Atype.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rarr() {
  puts("Reader: rarr");

  Tx *tx;
  Value *v;
  Achar *a;
  Type *tp;
  Atype *params;

  tx = mk_tx("[  ] ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VARR);
  a = value_data(v);
  assert(!arr_size(a));
  assert(type_is_unknown(value_type(v)));

  tx = mk_tx("[ a == b ] ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VARR);
  a = value_data(v);
  assert(arr_size(a) == 1);
  tp = value_type(v);
  assert(!strcmp(type_id(tp), "Arr"));
  params = type_params(tp);
  assert(arr_size(params) == 1);
  tp = atype_get(params, 0);
  assert(!strcmp(type_id(tp), "Bool"));

  tx = mk_tx("[ 5, 8 * 4 ] ");
  rvalue(&v, tx);

  assert(value_vtype(v) == VARR);
  a = value_data(v);
  assert(arr_size(a) == 2);
  tp = value_type(v);
  assert(!strcmp(type_id(tp), "Arr"));
  params = type_params(tp);
  assert(arr_size(params) == 1);
  tp = atype_get(params, 0);
  assert(!strcmp(type_id(tp), "Int"));

  puts("    Finished");
}
