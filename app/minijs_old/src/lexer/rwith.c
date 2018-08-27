// Copyright 14-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/rwith.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "lexer/rvalue.h"
#include "ct/Ltype.h"
#include "ct/Avalue.h"
#include "ct/Lvalue.h"
#include "ast/ops.h"
#include "ast/Value.h"
#include "ast/Type.h"
#include "DEFS.h"

static Value *mk_or(Value *v0, Avalue *cs) {
  if (avalue_size(cs) == 1) {
    Value *v1 = avalue_get(cs, 0);
    return value_new_binary(
      value_pos(v1), type_new_data("Bool", ltype_new()), OPS_EQ, v0, v1
    );
  } else {
    Value *v1 = avalue_get(cs, 0);
    avalue_remove(cs, 0);
    return value_new_binary(
      value_pos(v1), type_new_data("Bool", ltype_new()), OPS_LOR,
      value_new_binary(
        value_pos(v1), type_new_data("Bool", ltype_new()), OPS_EQ, v0, v1
      ),
      mk_or(v0, cs)
    );
  }
}

static Value *mk_ternary(
  Value *v0, Arr/*Avalue*/ *conditions, Avalue *results
) {
  if (avalue_size(results) == 1) {
    return avalue_get(results, 0);
  } else {
    Avalue *cs = arr_get(conditions, 0);
    Value *v2 = avalue_get(results, 0);
    arr_remove(conditions, 0);
    avalue_remove(results, 0);

    Value *v1 = mk_or(v0, cs);
    Value *v3 = mk_ternary(v0, conditions, results);

    Type *tp = value_type(v2);
    Type *tp2 = value_type(v3);
    if (type_t(tp) == type_UNKNOWN) {
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
    if (tx_eq(tx, r = lex_cconst(tx, '\\')))
      TH(tx) "Expected '\\'" _TH
    tx = lex_blanks(r);

    if (tx_neq(tx, r = lex_cconst(tx, ':'))) {
      tx = lex_blanks(r);
      tx = rvalue(&tmp, tx);
      avalue_add(results, tmp);
      break;
    } else {
      Avalue *atmp = avalue_new();
      tx = lex_arr((Arr **)&atmp, tx, ':', (Tx *(*)(void **, Tx *))rvalue);
      tx = lex_blanks(tx);

      if (!avalue_size(atmp))
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
