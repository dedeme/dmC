// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rvalue.h"
#include "lexer/token.h"
#include "lexer/rnumber.h"
#include "lexer/rstring.h"
#include "lexer/rarr.h"
#include "lexer/rmap.h"
#include "lexer/rwith.h"
#include "lexer/rlunary.h"
#include "lexer/rnary.h"
#include "lexer/rblock.h"
#include "lexer/rtype.h"
#include "ast/Avalue.h"
#include "DEFS.h"

static Tx* add_attachs(Tx *tx, Avatt *avatt);

static Tx* add_pattach(Tx *tx, Avatt *avatt) {
  Tx *r;

  if (tx_neq(tx, r = token_cconst(tx, '.'))) {
    tx = r;
    char *id;
    if (tx_eq(tx, r = token_valid_id(&id, tx)))
      TH(tx) "Expected an identifier" _TH
    tx = r;
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

  if (tx_neq(tx, r = token_cconst(tx, '('))) {
    Pos *pos = tx_pos(tx);
    tx = r;

    Avalue *vs;
    tx = token_list(&vs, tx, ')', (Tx *(*)(void **, Tx *))rvalue);
    avatt_add(avatt, vatt_new_fn(pos, vs));
    return add_attachs(tx, avatt);
  }

  if (tx_neq(tx, r = token_cconst(tx, '['))) {
    Pos *pos = tx_pos(tx);
    tx = r;

    Value *v;
    if (tx_eq(tx, r = rvalue(&v, tx)))
      TH(tx) "Expected a value" _TH
    tx = r;

    if (tx_eq(tx, r = token_cconst(tx, ']')))
      TH(tx) "Expected ']'" _TH
    tx = r;

    Avalue *vs = avalue_new();
    avalue_add(vs, v);

    avatt_add(avatt, vatt_new_id(pos, "get"));
    avatt_add(avatt, vatt_new_fn(pos, vs));
    return add_attachs(tx, avatt);
  }

  return tx;
}

Tx *rvalue(Value **v, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);

  if (tx_neq(tx, r = token_const(tx, "->"))) { // Function
    tx = r;
    Astat *sts;
    tx = rblock(&sts, tx);
    *v = value_new_fn(pos, achar_new(), sts);
    return tx;
  }

  if (tx_neq(tx, r = token_cconst0(tx, '('))) { // (value) (:cast)
    tx = r;

    if (tx_neq(tx, r = token_cconst(tx, ':'))) {
      // tx does not advance because rtype must start with ':'
      Type *tp;
      tx = rtype(&tp, tx);

      if (tx_eq(tx, r = token_cconst(tx, ')')))
        TH(tx) "Expected ')'" _TH
      tx = r;

      Value *v0;
      if (tx_eq(tx, r = rvalue(&v0, tx)))
        TH(tx) "Expected a value" _TH
      tx = r;

      *v = value_new_cast(pos, tp, v0);

      return tx;
    }

    tx = token_blanks(tx);

    Value *v0;
    if (tx_eq(tx, r = rvalue(&v0, tx)))
      TH(tx) "Expected a value" _TH
    tx = r;

    if (tx_eq(tx, r = token_cconst(tx, ')')))
      TH(tx) "Expected ')'" _TH
    tx = r;

    v0 = value_new_group(pos, avatt_new(), v0);
    tx = add_attachs(tx, value_attachs(v0));
    tx = rnary(v, tx, v0);

    return tx;
  }

  char *id;
  if (tx_neq(tx, r = token_lunary(&id, tx))) { // lunary
    tx = r;

    return rlunary(v, id, tx);
  }

  Achar *generics;
  if (tx_eq(tx, r = token_generic_id(&id, &generics, tx))) { // literal
    Value *v0;
    if (tx_neq(tx, r = rnumber(&v0, tx))) {
      return rnary(v, r, v0);
    } else if (tx_neq(tx, r = rstring(&v0, tx))) {
      tx = add_attachs(r, value_attachs(v0));
      return rnary(v, tx, v0);
    } else if (tx_neq(tx, r = rarr(&v0, tx))) {
      tx = add_attachs(r, value_attachs(v0));
      return rnary(v, tx, v0);
    } else if (tx_neq(tx, r = rmap(&v0, tx))) {
      tx = add_attachs(r, value_attachs(v0));
      return rnary(v, tx, v0);
    }
    TH(tx) "Expected a value" _TH
  }
  tx = r;

  if (arr_size(generics) == 0) {
    if (!strcmp(id, "null")) { // Null
      *v = value_new_null(pos);
      return tx;
    }

    if (!strcmp(id, "true") || !strcmp(id, "false")) { // Bool
      Value *v0 = value_new_bool(pos, avatt_new(), id);
      tx = add_pattach(tx, value_attachs(v0));
      return rnary(v, tx, v0);
    }

    if (!strcmp(id, "with")) { // with
      return rwith(v, tx);
    }

    if (token_is_reserved(id))
      TH(tx) "'%s' is a reserved word", id _TH

    if (tx_neq(tx, r = token_cconst(tx, ','))) { // Function
      tx = r;
      Achar *params;
      tx = token_fn_list(&params, tx, token_valid_id);
      arr_insert(params, 0, id);

      Astat *sts;
      tx = rblock(&sts, tx);

      *v = value_new_fn(pos, params, sts);
      return tx;
    }

    if (tx_neq(tx, r = token_const(tx, "->"))) { // Function
      tx = r;
      Achar *params = achar_new();
      achar_add(params, id);

      Astat *sts;
      tx = rblock(&sts, tx);

      *v = value_new_fn(pos, params, sts);
      return tx;
    }
  }

  if (token_is_reserved(id))
    TH(tx) "'%s' is a reserved word", id _TH

  Value *v0 = value_new_id(pos, avatt_new(), id, generics);
  char *op;
  if (tx_neq(tx, r = token_runary(&op, tx))) {
    tx = r;
    v0 = value_new_runary(tx_pos(tx), op, v0);
  } else {
    tx = add_attachs(tx, value_attachs(v0));
  }
  return rnary(v, tx, v0);
}
