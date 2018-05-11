// Copyright 6-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rlunary.h"
#include "lexer/token.h"
#include "lexer/rvalue.h"
#include "ast/ops.h"
#include "ast/Avalue.h"
#include "DEFS.h"

Tx *rlunary(Value **v, char *op, Tx *tx) {
  Pos *pos = tx_pos(tx);

  Value *v0;
  tx = rvalue(&v0, tx);

  *v = value_new_lunary(pos, op, v0);

  return tx;
}
