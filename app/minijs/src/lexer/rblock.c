// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rblock.h"
#include "lexer/rstat.h"
#include "lexer/token.h"
#include "DEFS.h"

Tx *rblock(Astat **block, Tx *tx) {
  Tx *r;

  if (tx_eq(tx, r = token_cconst(tx, '{'))) {
    return rsblock(block, tx);
  }
  return rmblock(block, r);
}

Tx *rsblock(Astat **block, Tx *tx) {
  Tx *r;

  Stat *st;
  tx = rstat(&st, tx);
  if (tx_eq(tx, r = token_cconst(tx, ';')))
    TH(tx) "Expected ';' (semicolon)" _TH

  return r;
}

Tx *rmblock(Astat **block, Tx *tx) {
  Tx *r;
  Astat *bl = astat_new();

  for (;;) {
    if (tx_at_end(tx))
      TH(tx) "Expected '}' and found end of file" _TH

    if (tx_neq(tx, r = token_cconst(tx, '}'))) {
      *block = bl;
      return r;
    }

    Stat *st;
    tx = rstat(&st, tx);
    astat_add(bl, st);
  }
}
