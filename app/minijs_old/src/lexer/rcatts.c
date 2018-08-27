// Copyright 06-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Achar.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "ast/Type.h"
#include "ast/Catt.h"
#include "ast/Value.h"
#include "ct/Lcatt.h"
#include "ct/Asym.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "lexer/rtype.h"
#include "lexer/rvalue.h"
#include "lexer/rnew.h"
#include "lexer/rcatts.h"
#include "DEFS.h"

Tx *rcatt(Catt **att, Tx *tx, bool is_public, bool is_static, Asym *symbols) {
  Tx *start = tx;
  Tx *r;

  Type *tp = type_new_unknown();
  if (tx_neq(tx, r = lex_cconst(tx, ':'))) {
    tx = lex_blanks(r);
    tx = lex_blanks(rtype(&tp, tx));
  }

  Tx *start2 = tx;
  char *val_var;
  if (tx_eq(tx, r = lex_id(&val_var, tx)))
    TH(tx) "Expected 'var', 'val', 'nat' ot 'new'" _TH
  tx = lex_blanks(r);

  if (type_t(tp) != type_UNKNOWN && str_eq(val_var, "nat")) {
    char *id;
    if (tx_eq(tx, r = lex_id(&id, tx)))
      TH(tx) "Expected an identifier" _TH

    lex_test_reserved(tx, id);
    asym_add_test(symbols, tx, id);
    *att = catt_new(
      is_public, is_static, catt_NAT, id, tp, value_new_null(tx_pos(tx))
    );
    return lex_blanks(r);
  }

  if (str_eq(val_var, "new")) {
    asym_add_test(symbols, tx, "new");
    *att = catt_new(
      is_public, is_static, catt_NEW, "", tp, value_new_null(tx_pos(tx))
    );
    return tx;
  }

  if (str_eq(val_var, "val")) {
    char *id;
    if (tx_eq(tx, r = lex_id(&id, tx)))
      TH(tx) "Expected an identifier" _TH
    tx = lex_blanks(r);

    lex_test_reserved(tx, id);
    asym_add_test(symbols, tx, id);

    if (tx_eq(tx, r = lex_cconst(tx, '=')))
      TH(tx) "Expectec '='" _TH
    tx = lex_blanks(r);

    Value *value;
    tx = rvalue(&value, tx);

    if (type_t(value_type(value)) != type_UNKNOWN) {
      if (type_t(tp) == type_UNKNOWN) {
        tp = value_type(value);
      }

      if (type_t(value_type(value)) != type_t(tp))
        TH(start) "Declared value and value type are different" _TH

    } else if (type_t(tp) == type_UNKNOWN)
      TH(start) "Expected a type declaration" _TH

    *att = catt_new(is_public, is_static, catt_VAL, id, tp, value);
    return tx;
  }

  if (str_cmp(val_var, "var"))
    TH(start2) "Expected 'var', 'val', 'nat' ot 'new'" _TH

  char *id;
  if (tx_eq(tx, r = lex_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = lex_blanks(r);

  lex_test_reserved(tx, id);
  asym_add_test(symbols, tx, id);

  if (tx_eq(tx, r = lex_cconst(tx, '='))) {
    if (type_t(tp) == type_UNKNOWN)
      TH(start) "Expected a type declaration" _TH

    *att = catt_new(
      is_public, is_static, catt_VAR, id, tp, value_new_null(tx_pos(tx))
    );
    return tx;
  }

  tx = lex_blanks(r);

  Value *value;
  tx = rvalue(&value, tx);

  if (type_t(value_type(value)) != type_UNKNOWN) {
    if (type_t(tp) == type_UNKNOWN) {
      tp = value_type(value);
    }

    if (type_t(value_type(value)) != type_t(tp))
      TH(start) "Declared value and value type are different" _TH

  } else if (type_t(tp) == type_UNKNOWN)
    TH(start) "Expected a type declaration" _TH

  *att = catt_new(is_public, is_static, catt_VAR, id, tp, value);
  return tx;
}

Tx *rcatts(
  Lcatt **atts, Tx *tx, bool is_public, bool is_static, Asym *symbols
) {
  Tx *r;

  if (tx_eq(tx, r = lex_const(tx, is_public ? "_public" : "_private"))) {
    return tx;
  }
  tx = lex_blanks(r);

  Lcatt *l = lcatt_new();
  for (;;) {
    if (!*(tx_start(tx)) || tx_neq(tx, r = lex_cconst(tx, '_'))) {
      if (lcatt_empty(l))
        TH(tx) "Expected at least one value declaration" _TH

      *atts = lcatt_cat(*atts, l);
      return tx;
    }

    Catt *att;
    tx = rcatt(&att, tx, is_public, is_static, symbols);
    tx = lex_blanks(tx);

    if (catt_t(att) == catt_NEW) {
      Lcatt *instances = lcatt_new();
      tx = rnew(&instances, &att, tx, symbols);
      l = lcatt_cat(l, instances);
    }

    l = lcatt_cons(l, att);
  }
}
