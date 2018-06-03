// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rarr.h"
#include "lexer/token.h"
#include "lexer/rvalue.h"
#include "ast/Avalue.h"

Tx *rarr(Value **v, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);

  if (tx_eq(tx, r = token_cconst(tx, '['))) {
    return tx;
  }
  Avalue *vs;
  tx = token_list(&vs, r, ']', (Tx *(*)(void **, Tx *))rvalue);

  *v = value_new_arr(pos, arr_new(), vs);
  return tx;
}
