// Copyright 6-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rnary.h"
#include "lexer/token.h"
#include "lexer/rvalue.h"
#include "ast/ops.h"
#include "ast/Avalue.h"
#include "Achar.h"
#include "DEFS.h"

static Value *inject(Pos *pos, char *op, Value *v1, Value *v2) {
  if (value_vtype(v2) == VBINARY) {
    int nop = ops_order(op);
    Achar *data = value_data(v2);
    char *op2 = achar_get(data, arr_size(data) - 1);
    int nop2 = ops_order(op2);
    if (nop2 > nop) {
      Achar *tmp = achar_copy(data);
      arr_remove(tmp, arr_size(tmp) - 1);
      Avalue *vs = avalue_restore(tmp);
      v1 = inject(pos, op, v1, avalue_get(vs, 0));
      v2 = avalue_get(vs, 1);
      op = op2;
    }
  } else if (value_vtype(v2) == VTERNARY) {
    Avalue *vs = avalue_restore(value_data(v2));
    v1 = value_new_binary(pos, type_new_unknown(), op, v1, avalue_get(vs, 0));
    return value_new_ternary(
      pos, value_type(v2), v1, avalue_get(vs, 1), avalue_get(vs, 2)
    );
  }

  Type *tp = type_new_unknown();
  if (!type_is_unknown(value_type(v1))) {
    tp = value_type(v1);
  } else if (!type_is_unknown(value_type(v2))) {
    tp = value_type(v2);
  }

  return value_new_binary(pos, tp, op, v1, v2);
}

Tx *rnary(Value **v, Tx *tx, Value *v1) {
  Tx *r;
  Pos *pos = tx_pos(tx);
  Value *v2;

  if (tx_neq(tx, r = token_cconst(tx, '?'))) {
    tx = r;

    if (tx_neq(tx, r = token_cconst(tx, ':'))) {
      tx = rvalue(&v2, tx);
      *v = value_new_binary(pos, value_type(v2), "?:", v1, v2);
      return tx;
    }

    Value *v3;

    tx = rvalue(&v2, tx);

    if (tx_eq(tx, r = token_cconst(tx, ':')))
      TH(tx) "Expected ':'" _TH
    tx = r;

    tx = rvalue(&v3, tx);

    Type *tp = type_new_unknown();
    if (!type_is_unknown(value_type(v2))) {
      tp = value_type(v2);
    } else if (!type_is_unknown(value_type(v3))) {
      tp = value_type(v3);
    }

    *v = value_new_ternary(pos, tp, v1, v2, v3);

    return tx;
  }

  char *op;
  if (tx_eq(tx, r = token_binary(&op, tx))) {
    *v = v1;
    return tx;
  }
  tx = r;

  tx = rvalue(&v2, tx);
  *v = inject(pos, op, v1, v2);

  return tx;
}
