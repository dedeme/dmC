// Copyright 7-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rnull.h"
#include "ast/Value.h"
#include "lexer/rvalue.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rnull() {
  puts("Reader: rnull");
  Tx *tx;
  Value *v;

  tx = mk_tx("null");
  rvalue(&v, tx);
  assert(value_vtype(v) == VNULL);

  puts("    Finished");
}
