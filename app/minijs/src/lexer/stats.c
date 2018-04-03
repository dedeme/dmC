// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/stats.h"
#include "DEFS.h"
#include "lexer/token.h"
#include "lexer/block.h"
#include "lexer/values.h"

Txpos *stats_read(Stat **stat, Txpos *tx) {
  if (txpos_at_end(tx))
    TH(tx) "End of file reached reading a block of statements" _TH

  Txpos *start = tx;
  Txpos *r;

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
  if (txpos_eq(tx, r = token_point_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  if (!strcmp(id, "val")) {
    Dvalue *dvalue;
    tx = block_declaration(&dvalue, tx, tp);

    if (value_vtype(dvalue_value(dvalue)) == VNULL)
        TH(tx) "'val %s' can not be null", dvalue_id(dvalue) _TH

    *stat = stat_new_val(txpos_pos(start), dvalue);
    return tx;
  }

  if (!strcmp(id, "var")) {
    Dvalue *dvalue;
    tx = block_declaration(&dvalue, tx, tp);
    *stat = stat_new_var(txpos_pos(start), dvalue);
    return tx;
  }

  if (is_typed)
    TH(tx) "Expected 'val' or' var'" _TH

  if (!strcmp(id, "return")) {
    Value *value;
    if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
      value = NULL;
    } else {
      tx = values_read(&value, tx, false);
    }

    *stat = stat_new_return(txpos_pos(start), value);
    return tx;
  } else {
    if (txpos_eq(tx, r = token_cconst(tx, '(')))
      TH(tx) "Expected '('" _TH
    tx = r;

    /**/ Txpos *value_read(Value **v, Txpos *tx) {
    /**/   return values_read(v, tx, false);
    /**/ }
    Arr/*Value*/ *vs;
    tx = token_list(&vs, tx, ')', (Txpos *(*)(void **, Txpos *))value_read);

    *stat = stat_new_fn(txpos_pos(start), vs);
  }

  if (txpos_eq(tx, r = token_cconst(tx, ';')))
    TH(tx) "Expected ';'" _TH
  tx = r;
  return tx;
}


Txpos *stats_block(Arr/*Stat*/ **stats, Txpos *tx) {
  Txpos *r;

  Arr/*Stat*/ *ss = arr_new();

  for (;;) {
    while (txpos_neq(tx, r = token_cconst(tx, ';'))) {
      tx = r;
    }

    if (txpos_neq(tx, r = token_cconst(tx, '}'))) {
      if (
        *txpos_start(r) == '}' ||
        *txpos_start(r) == ')' ||
        *txpos_start(r) == ']' ||
        *txpos_start(r) == ';' ||
        *txpos_start(r) == ','
      ) {
        tx = r;
      } else {
        *(txpos_start(tx)) = ';';
      }
      break;
    }

    Stat *stat;
    tx = stats_read(&stat, tx);

    arr_add(ss, stat);
  }

  *stats = ss;
  return tx;
}
