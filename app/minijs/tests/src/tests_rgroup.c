// Copyright 7-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rgroup.h"
#include "ast/Value.h"
#include "lexer/rvalue.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rgroup() {
  puts("Reader: rgroup");
  Tx *tx;
  Value *v, *v0;

  tx = mk_tx("(210)");
  rvalue(&v, tx);
  assert(value_vtype(v) == VGROUP);
  v0 = value_restore((Arr *)value_data(v));
  assert(value_vtype(v0) == VINT);
  assert(!strcmp(type_to_str(value_type(v)), "Int"));

  tx = mk_tx("(  210.03 )");
  rvalue(&v, tx);
  assert(value_vtype(v) == VGROUP);
  v0 = value_restore((Arr *)value_data(v));
  assert(value_vtype(v0) == VFLOAT);
  assert(!strcmp(type_to_str(value_type(v)), "Float"));

  puts("    Finished");
}
