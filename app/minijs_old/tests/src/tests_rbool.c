// Copyright 7-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tests_rbool.h"
#include "ast/Value.h"
#include "lexer/rvalue.h"

static Tx *mk_tx(char *t) {
  return tx_new("Empty", t, t, 1, 0);
}

void tests_rbool() {
  puts("Reader: rbool");
  Tx *tx;
  Value *v;

  tx = mk_tx("true");
  rvalue(&v, tx);
  assert(value_vtype(v) == VBOOL);
  assert(!strcmp(achar_get(value_data(v), 0), "true"));

  tx = mk_tx("false");
  rvalue(&v, tx);
  assert(value_vtype(v) == VBOOL);
  assert(!strcmp(achar_get(value_data(v), 0), "false"));

  puts("    Finished");
}
