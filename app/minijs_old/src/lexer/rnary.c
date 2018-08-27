// Copyright 14-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Achar.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/rnary.h"
#include "lexer/lex.h"
#include "lexer/Pos.h"
#include "lexer/Tx.h"
#include "lexer/rvalue.h"
#include "ast/ops.h"
#include "ast/Value.h"
#include "ast/Type.h"
#include "ct/Avalue.h"
#include "ct/Ltype.h"
#include "DEFS.h"

static Value *inject(Pos *pos, char *op, Value *v1, Value *v2) {
  if (value_t(v2) == value_VBINARY) {
    int nop = ops_order(op);
    Achar *data = value_data(v2);
    char *op2 = achar_get(data, achar_size(data) - 1);
    int nop2 = ops_order(op2);
    if (nop2 > nop) {
      Ajson *tmp = ajson_new();
      RANGE0(i, achar_size(data) - 1) {
        ajson_add(tmp, (Json *)achar_get(data, i));
      }_RANGE
      Avalue *vs = avalue_from_json(tmp, value_from_json);
      v1 = inject(pos, op, v1, avalue_get(vs, 0));
      v2 = avalue_get(vs, 1);
      op = op2;
    }
  } else if (value_t(v2) == value_VTERNARY) {
    Avalue *vs = avalue_from_json((Ajson *)value_data(v2), value_from_json);
    v1 = value_new_binary(pos, type_new_unknown(), op, v1, avalue_get(vs, 0));
    return value_new_ternary(
      pos, value_type(v2), v1, avalue_get(vs, 1), avalue_get(vs, 2)
    );
  }

  Type *tp = type_new_unknown();
  if (ops_is_bool(op)) {
    tp = type_new_data("Bool", ltype_new());
  } else if (type_t(value_type(v1)) != type_UNKNOWN) {
    tp = value_type(v1);
  } else if (type_t(value_type(v2)) != type_UNKNOWN) {
    tp = value_type(v2);
  }

  return value_new_binary(pos, tp, op, v1, v2);
}

Tx *rnary(Value **v, Tx *tx, Value *v1) {
  Tx *r;
  Pos *pos = tx_pos(tx);
  Value *v2;
  Value *v3;

  if (tx_neq(tx, r = lex_cconst(tx, '?'))) {
    if (tx_eq(r, lex_cconst(r, ':'))) {
      tx = lex_blanks(r);
      tx = lex_blanks(rvalue(&v2, tx));

      if (tx_eq(tx, r = lex_cconst(tx, ':')))
        TH(tx) "Expected ':'" _TH
      tx = lex_blanks(r);

      tx = rvalue(&v3, tx);

      Type *tp = type_new_unknown();
      if (type_t(value_type(v2)) != type_UNKNOWN) {
        tp = value_type(v2);
      } else if (type_t(value_type(v3)) != type_UNKNOWN) {
        tp = value_type(v3);
      }

      *v = value_new_ternary(pos, tp, v1, v2, v3);

      return tx;
    }
  }

  char *op;
  if (tx_eq(tx, r = lex_binary(&op, tx))) {
    *v = v1;
    return tx;
  }
  tx = lex_blanks(r);

  tx = rvalue(&v2, tx);
  *v = inject(pos, op, v1, v2);

  return tx;
}
