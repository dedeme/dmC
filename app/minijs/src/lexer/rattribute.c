// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rattribute.h"
#include "DEFS.h"
#include "lexer/token.h"
#include "lexer/rtype.h"
#include "lexer/rvalue.h"

Tx *rattribute(Tx *tx, Class *c, bool is_public) {
  Type *tp;
  char *id;
  bool is_val;
  Value *v;

  Tx *tx_type = tx;
  Tx *r;
  if (tx_eq(tx, r = rtype(&tp, tx))) {
    tp = type_new_unknown();
  }
  tx = r;

  char *val_var = NULL;
  if (tx_eq(tx, r = token_id(&val_var, tx)))
    if (type_is_unknown(tp)) {
      return tx;
    }
    TH(tx) "Expected 'val' or 'var'" _TH
  if (!strcmp(val_var, "val")) {
    is_val = true;
  } else if (!strcmp(val_var, "var")) {
    is_val = false;
  }  else {
    TH(tx) "Expected 'val' or 'var'" _TH
  }
  tx = r;

  tx = token_valid_id(&id, tx);

  if (tx_neq(tx, r = token_cconst(tx, ';'))) {
    if (type_is_unknown(tp))
      TH(tx_type) "Type declaration is needed" _TH

    aatt_add(
      class_statics(c),
      att_new(is_public, id, tp, is_val, value_new_null(tx_pos(tx)))
    );
    return r;
  }

  if (tx_eq(tx, r = token_cconst(tx, '=')))
    TH(tx) "Expected '='" _TH
  tx = r;

  tx = rvalue(&v, tx);

  if (type_is_unknown(tp)) {
    Type *vtp = value_type(v);
    if (type_is_unknown(vtp))
      TH(tx_type) "Type declaration is needed" _TH

    tp = vtp;
  }

  aatt_add(
    class_statics(c),
    att_new(is_public, id, tp, is_val, v)
  );
  return tx;
}
