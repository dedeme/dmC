// Copyright 16-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/rcase.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "lexer/rblock.h"
#include "lexer/rvalue.h"
#include "ct/Avalue.h"
#include "ct/Lvalue.h"
#include "ct/Aastat.h"
#include "ct/Ltype.h"
#include "ast/ops.h"
#include "ast/Value.h"
#include "ast/Type.h"
#include "ast/Stat.h"
#include "DEFS.h"

inline
static Type *new_bool() {
  return type_new_data("Bool", ltype_new());
}

static Value *mk_or(Value *v0, Avalue *cs) {
  if (avalue_size(cs) == 1) {
    Value *v1 = avalue_get(cs, 0);
    return value_new_binary(value_pos(v1), new_bool(), OPS_EQ, v0, v1);
  } else {
    Value *v1 = avalue_get(cs, 0);
    avalue_remove(cs, 0);
    return value_new_binary(
      value_pos(v1), new_bool(), OPS_LOR,
      value_new_binary(value_pos(v1), new_bool(), OPS_EQ, v0, v1),
      mk_or(v0, cs)
    );
  }
}

static Stat *mk_if(
  Value *v0, Arr/*Avalue*/ *conditions, Aastat *blocks, Pos *pos
) {
  Avalue *values = avalue_new();
  EACH(conditions, Avalue, cs) {
    avalue_add(values, mk_or(v0, cs));
  }_EACH

  return stat_new_if(pos, values, blocks);
}

Tx *rcase(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;
  Tx *start = tx;

  Value *v0;
  tx = rvalue(&v0, tx);

  Astat *tmp;
  Arr/*Avalue*/ *conditions = arr_new();
  Aastat *blocks = aastat_new();
  for (;;) {
    if (tx_eq(tx, r = lex_cconst(tx, '\\'))) {
      break;
    }
    tx = lex_blanks(r);

    if (tx_neq(tx, r = lex_cconst(tx, ':'))) {
      tx = rblock(&tmp, lex_blanks(r));
      aastat_add(blocks, tmp);
      break;
    } else {
      Avalue *atmp = avalue_new();
      tx = lex_arr((Arr **)&atmp, tx, ':', (Tx *(*)(void **, Tx *))rvalue);
      tx = lex_blanks(tx);

      if (!avalue_size(atmp))
        TH(tx) "Expected a value" _TH
      arr_add(conditions, atmp);

      tx = rblock(&tmp, tx);
      aastat_add(blocks, tmp);
    }
  }

  if (!arr_size(conditions))
    TH(start) "'with' requires at least a condition" _TH

  *st = mk_if(v0, conditions, blocks, pos);

  return tx;
}
