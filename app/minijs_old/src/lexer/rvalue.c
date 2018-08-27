// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/ct/Achar.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "ast/Vatt.h"
#include "ast/Value.h"
#include "ct/Avatt.h"
#include "ct/Lvalue.h"
#include "ct/Astat.h"
#include "lexer/Tx.h"
#include "lexer/lex.h"
#include "lexer/rblock.h"
#include "lexer/rtype.h"
#include "lexer/rnary.h"
#include "lexer/rlunary.h"
#include "lexer/rnumber.h"
#include "lexer/rstring.h"
#include "lexer/rarr.h"
#include "lexer/rmap.h"
#include "lexer/rwith.h"
#include "lexer/rvalue.h"
#include "DEFS.h"

static Tx* add_attachs(Tx *tx, Avatt *avatt);

static Tx* add_pattach(Tx *tx, Avatt *avatt) {
  Tx *r;

  if (tx_neq(tx, r = lex_cconst(tx, '.'))) {
    tx = lex_blanks(r);
    char *id;
    if (tx_eq(tx, r = lex_id(&id, tx)))
      TH(tx) "Expected an identifier" _TH
    tx = lex_blanks(r);
    lex_test_reserved(tx, id);
    avatt_add(avatt, vatt_new_id(tx_pos(tx), id));
    return add_attachs(tx, avatt);
  }
  return tx;
}

static Tx* add_attachs(Tx *tx, Avatt *avatt) {
  Tx *r;

  if (tx_neq(tx, r = add_pattach(tx, avatt))) {
    return r;
  }

  if (tx_neq(tx, r = lex_cconst(tx, '('))) {
    Pos *pos = tx_pos(tx);
    tx = lex_blanks(r);

    Lvalue *vs;
    tx = lex_list((List **)&vs, tx, ')', (Tx *(*)(void **, Tx *))rvalue);
    avatt_add(avatt, vatt_new_fn(pos, vs));
    return add_attachs(lex_blanks(tx), avatt);
  }

  if (tx_neq(tx, r = lex_cconst(tx, '['))) {
    Pos *pos = tx_pos(tx);
    tx = lex_blanks(r);

    Value *v;
    if (tx_eq(tx, r = rvalue(&v, tx)))
      TH(tx) "Expected a value" _TH
    tx = lex_blanks(r);

    if (tx_eq(tx, r = lex_cconst(tx, ']')))
      TH(tx) "Expected ']'" _TH
    tx = lex_blanks(r);

    Lvalue *vs = lvalue_new();
    lvalue_cons(vs, v);

    avatt_add(avatt, vatt_new_id(pos, "get"));
    avatt_add(avatt, vatt_new_fn(pos, vs));
    return add_attachs(tx, avatt);
  }

  return tx;
}

Tx *rvalue(Value **v, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);

  if (tx_neq(tx, r = lex_const(tx, "->"))) { // Function
    tx = lex_blanks(r);
    Astat *sts;
    tx = rblock(&sts, tx);
    *v = value_new_fn(pos, achar_new(), sts);
    return tx;
  }

  if (tx_neq(tx, r = lex_cconst(tx, '('))) { // (value) (:cast)
    tx = r;

    if (tx_neq(tx, r = lex_cconst(tx, ':'))) {
      tx = lex_blanks(tx);

      Type *tp;
      tx = lex_blanks(rtype(&tp, tx));

      if (tx_eq(tx, r = lex_cconst(tx, ')')))
        TH(tx) "Expected ')'" _TH
      tx = lex_blanks(r);

      Value *v0;
      if (tx_eq(tx, r = rvalue(&v0, tx)))
        TH(tx) "Expected a value" _TH
      tx = r;

      *v = value_new_cast(pos, tp, v0);

      return tx;
    }

    tx = lex_blanks(tx);

    Value *v0;
    if (tx_eq(tx, r = rvalue(&v0, tx)))
      TH(tx) "Expected a value" _TH
    tx = lex_blanks(r);

    if (tx_eq(tx, r = lex_cconst(tx, ')')))
      TH(tx) "Expected ')'" _TH
    tx = r;

    v0 = value_new_group(pos, avatt_new(), v0);
    tx = add_attachs(tx, value_attachs(v0));
    tx = rnary(v, tx, v0);

    return tx;
  }

  char *id;
  if (tx_neq(tx, r = lex_lunary(&id, tx))) { // lunary
    tx = lex_blanks(r);

    return rlunary(v, id, tx);
  }

  Achar *generics;
  if (tx_eq(tx, r = lex_generic_id(&id, &generics, tx))) { // literal
    Value *v0;
    if (tx_neq(tx, r = rnumber(&v0, tx))) {
      return rnary(v, r, v0);
    } else if (tx_neq(tx, r = rstring(&v0, tx))) {
      tx = add_attachs(lex_blanks(r), value_attachs(v0));
      return rnary(v, tx, v0);
    } else if (tx_neq(tx, r = rarr(&v0, tx))) {
      tx = add_attachs(lex_blanks(r), value_attachs(v0));
      return rnary(v, tx, v0);
    } else if (tx_neq(tx, r = rmap(&v0, tx))) {
      tx = add_attachs(lex_blanks(r), value_attachs(v0));
      return rnary(v, tx, v0);
    }
    TH(tx) "Expected a value" _TH
  }
  tx = lex_blanks(r);

  if (achar_size(generics) == 0) {
    if (str_eq(id, "null")) { // Null
      *v = value_new_null(pos);
      return tx;
    }

    if (str_eq(id, "true") || str_eq(id, "false")) { // Bool
      Value *v0 = value_new_bool(pos, avatt_new(), id);
      tx = add_pattach(tx, value_attachs(v0));
      return rnary(v, tx, v0);
    }

    if (str_eq(id, "with")) { // with
      return rwith(v, tx);
    }

    lex_test_reserved(tx, id);

    if (tx_neq(tx, r = lex_cconst(tx, ','))) { // Function
      tx = lex_blanks(r);
      Achar *params;
      tx = lex_fn_list(&params, tx, lex_valid_id);
      achar_insert(params, 0, id);

      Astat *sts;
      tx = rblock(&sts, lex_blanks(tx));

      *v = value_new_fn(pos, params, sts);
      return tx;
    }

    if (tx_neq(tx, r = lex_const(tx, "->"))) { // Function
      tx = lex_blanks(r);
      Achar *params = achar_new();
      achar_add(params, id);

      Astat *sts;
      tx = rblock(&sts, tx);

      *v = value_new_fn(pos, params, sts);
      return tx;
    }
  }

  lex_test_reserved(tx, id);

  Value *v0 = value_new_id(pos, avatt_new(), id, generics);
  char *op;
  if (tx_neq(tx, r = lex_runary(&op, tx))) {
    tx = r;
    v0 = value_new_runary(tx_pos(tx), op, v0);
  } else {
    tx = add_attachs(lex_blanks(tx), value_attachs(v0));
  }
  return rnary(v, tx, v0);
}
