// Copyright 7-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rcast.h"
#include "ast/Value.h"
#include "lexer/rvalue.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rcast() {
  puts("Reader: rcast");
  Tx *tx;
  Value *v, *v0;

  tx = mk_tx("(:Str) m");
  rvalue(&v, tx);
  assert(value_vtype(v) == VCAST);
  v0 = value_restore(value_data(v));
  assert(value_vtype(v0) == VID);
  assert(!strcmp(type_to_str(value_type(v)), "Str"));

  tx = mk_tx("(:   Int  )m");
  rvalue(&v, tx);
  assert(value_vtype(v) == VCAST);
  v0 = value_restore(value_data(v));
  assert(value_vtype(v0) == VID);
  assert(!strcmp(type_to_str(value_type(v)), "Int"));

  puts("    Finished");
}
