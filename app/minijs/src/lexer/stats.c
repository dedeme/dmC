// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/stats.h"
#include "DEFS.h"
#include "lexer/token.h"
#include "lexer/block.h"

Txpos *stats_read(Stat **stat, Txpos *tx) {
  if (txpos_at_end(tx))
    TH(tx) "End of file reached reading a block of statements" _TH

  Txpos *r;
  Stat *s;

  bool is_typed = false;
  Type *tp;
  if (txpos_neq(tx, r = token_cconst(tx, '#'))) {
    tx = r;
    tx = block_type(&tp, tx);
    is_typed = true;
  } else {
    tp = type_new_unknown();
  }

  char *id;
  if (txpos_neq(tx, r = token_id(&id, tx))) {
    tx = r;
    if (!strcmp(id, "val")) {
      Dvalue *dvalue;
      tx = block_declaration(&dvalue, tx, tp);

      if (value_vtype(dvalue_value(dvalue)) == VNULL)
          TH(tx) "'val %s' can not be null", dvalue_id(dvalue) _TH

      s = stat_new_val(dvalue);
    } else {
      THROW "Without implementation" _THROW
    }
  } else {
    if (is_typed)
      TH(tx) "Expected a typed declaration" _TH
    THROW "Without implementation" _THROW
  }

  *stat = s;
  return tx;
}

Txpos *stats_block(Arr/*Stat*/ **stats, Txpos *tx) {
  Txpos *r;

  Arr/*Stat*/ *ss = arr_new();

  if (txpos_eq(tx, r = token_cconst(tx, '{'))) {
    Stat *stat;
    tx = stats_read(&stat, tx);

    arr_add(ss, stat);
  } else {
    tx = r;
    for (;;) {
      if (txpos_neq(tx, r = token_cconst(tx, '}'))) {
        tx = r;
        break;
      }

      Stat *stat;
      tx = stats_read(&stat, tx);

      arr_add(ss, stat);
    }
  }

  *stats = ss;
  return tx;
}
