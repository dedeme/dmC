// Copyright 6-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rwith.h"
#include "lexer/token.h"
#include "lexer/rvalue.h"
#include "ast/Avalue.h"
#include "ast/ops.h"
#include "DEFS.h"

static Value *mk_or(Value *v0, Avalue *cs) {
  if (arr_size(cs) == 1) {
    Value *v1 = avalue_get(cs, 0);
    return value_new_binary(value_pos(v1), type_new_bool(), OPS_EQ, v0, v1);
  } else {
    Value *v1 = avalue_get(cs, 0);
    arr_remove(cs, 0);
    return value_new_binary(
      value_pos(v1), type_new_bool(), OPS_LOR,
      value_new_binary(value_pos(v1), type_new_bool(), OPS_EQ, v0, v1),
      mk_or(v0, cs)
    );
  }
}

static Value *mk_ternary(
  Value *v0, Arr/*Avalue*/ *conditions, Avalue *results
) {
  if (arr_size(results) == 1) {
    return avalue_get(results, 0);
  } else {
    Avalue *cs = arr_get(conditions, 0);
    Value *v2 = avalue_get(results, 0);
    arr_remove(conditions, 0);
    arr_remove(results, 0);

    Value *v1 = mk_or(v0, cs);
    Value *v3 = mk_ternary(v0, conditions, results);

    Type *tp = value_type(v2);
    Type *tp2 = value_type(v3);
    if (type_is_unknown(tp)) {
      tp = tp2;
    }

    return value_new_ternary(value_pos(v2), tp, v1, v2, v3);
  }
}

Tx *rwith(Value **v, Tx *tx) {
  Tx *r;
  Tx *start = tx;

  Value *v0;
  tx = rvalue(&v0, tx);

  Value *tmp;
  Arr/*Avalue*/ *conditions = arr_new();
  Avalue *results = avalue_new();
  for (;;) {
    if (tx_eq(tx, r = token_cconst(tx, '\\')))
      TH(tx) "Expected '\\'" _TH
    tx = r;

    if (tx_neq(tx, r = token_cconst(tx, ':'))) {
      tx = rvalue(&tmp, r);
      avalue_add(results, tmp);
      break;
    } else {
      Avalue *atmp;
      tx = token_list(&atmp, tx, ':', (Tx *(*)(void **, Tx *))rvalue);
      if (!arr_size(atmp))
        TH(tx) "Expected a value" _TH
      arr_add(conditions, atmp);

      tx = rvalue(&tmp, tx);
      avalue_add(results, tmp);
    }
  }

  if (!arr_size(conditions))
    TH(start) "'with' requires at least a condition" _TH

  *v = mk_ternary(v0, conditions, results);

  return tx;
}
