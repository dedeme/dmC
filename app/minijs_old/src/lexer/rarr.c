// Copyright 14-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rarr.h"
#include "lexer/Tx.h"
#include "lexer/lex.h"
#include "lexer/rvalue.h"
#include "ct/Avalue.h"
#include "ct/Lvalue.h"
#include "ct/Avatt.h"
#include "ast/Value.h"

Tx *rarr(Value **v, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);

  if (tx_eq(tx, r = lex_cconst(tx, '['))) {
    return tx;
  }
  tx = lex_blanks(r);

  Avalue *vs = avalue_new();
  tx = lex_arr((Arr **)&vs, tx, ']', (Tx *(*)(void **, Tx *))rvalue);

  *v = value_new_arr(pos, avatt_new(), vs);
  return tx;
}
