// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rattribute.h"
#include "DEFS.h"
#include "lexer/token.h"
#include "lexer/rtype.h"
#include "lexer/rvalue.h"
#include "lexer/rnew.h"

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

  char *val_var_enum = NULL;
  if (tx_eq(tx, r = token_id(&val_var_enum, tx))) {
    if (type_is_unknown(tp)) {
      return tx;
    }
    TH(tx) "Expected 'val' or 'var'" _TH
  }

  if (!strcmp(val_var_enum, "val")) {
    is_val = true;
  } else if (!strcmp(val_var_enum, "var")) {
    is_val = false;
  } else if (!strcmp(val_var_enum, "enum")) {
    if (tx_eq(tx, r = token_cconst(tx, '(')))
      TH(tx) "Expected '('" _TH
    tx = r;
    Achar *ids;
    Tx *start = tx;
    tx = token_list(
      (Arr **)&ids, tx, ')', (Tx *(*)(void **, Tx *))token_valid_id
    );
    EACH((Arr *)ids, char, id) {
      if (class_contains_id(c, id))
        TH(tx) "Identifier '%s' is duplicated", id _TH

      aatt_add(
        class_statics(c),
        att_new(
          is_public, id, type_new_int(), true,
          value_new_int(tx_pos(start), str_printf("%d", _i))
        )
      );
    }_EACH
  } else {
    TH(tx) "Expected 'val', 'var' or 'enum'" _TH
  }

  tx = r;

  tx = token_id(&id, tx);

  if (class_contains_id(c, id))
    TH(tx) "Identifier '%s' is duplicated", id _TH

  if (!strcmp(id, "new")) {
    if (type_is_unknown(tp))
      TH(tx_type) "Type declaration is needed" _TH
    if (type_type(tp) != FN)
      TH(tx_type) "Type of new must be 'function'" _TH

    if (tx_eq(tx, r = token_cconst(tx, '=')))
      TH(tx) "Expected '='" _TH
    tx = r;

    Value *v;
    tx = rnew(&v, tx, c, tp, is_public);

    aatt_add(
      class_statics(c),
      att_new(is_public, "new", tp, true, v)
    );
    return tx;
  }


  if (token_is_reserved(id))
    TH(tx) "'%s' is a reserved word", id _TH

  if (tx_eq(tx, r = token_cconst(tx, '='))) {
    if (type_is_unknown(tp))
      TH(tx_type) "Type declaration is needed" _TH

    aatt_add(
      class_statics(c),
      att_new(is_public, id, tp, is_val, value_new_null(tx_pos(tx)))
    );
    return r;
  }
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
