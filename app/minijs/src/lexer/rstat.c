// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rstat.h"
#include "lexer/token.h"
#include "lexer/rtype.h"
#include "lexer/rvalue.h"
#include "lexer/rblock.h"
#include "lexer/rcase.h"
#include "ast/Avalue.h"
#include "ast/Aastat.h"
#include "Pos.h"
#include "DEFS.h"

static Tx *rval_var(Stat **st, Tx *tx, Pos *pos, Type *tp, bool is_val) {
  Tx *r;

  char *id;
  if (tx_eq(tx, r = token_valid_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  Value *v;
  if (tx_eq(tx, r = token_cconst(tx, '='))) {
    if (type_is_unknown(tp))
      TH1(tx_path(tx), pos) "Type declaration is needed" _TH

    v = value_new_null(tx_pos(tx));
    value_set_type(v, tp);
  } else {
    tx = rvalue(&v, r);

    if (type_is_unknown(tp)) {
      Type *vtp = value_type(v);
      if (type_is_unknown(vtp))
        TH1(tx_path(tx), pos) "Type declaration is needed" _TH
    } else {
      value_set_type(v, tp);
    }
  }

  if (is_val) {
    *st = stat_new_val(pos, id, v);
  } else {
    *st = stat_new_var(pos, id, v);
  }

  return tx;
}

static Tx *rnative(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;

  char *mark;
  if (tx_eq(tx, r = token_id(&mark, tx)))
    TH(tx) "Expected a mark" _TH
  tx = r;

  char *text;
  tx = token_native(&text, tx, mark);

  *st = stat_new_native(pos, text);
  return tx;
}

static Tx *rbreak(Stat **st, Tx *tx, Pos *pos) {
  *st = stat_new_break(pos);
  return tx;
}

static Tx *rcontinue(Stat **st, Tx *tx, Pos *pos) {
  *st = stat_new_continue(pos);
  return tx;
}

static Tx *rreturn(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;

  if (tx_neq(tx, r = token_cconst(tx, ';'))) {
    *st = stat_new_return(pos, NULL);
    return r;
  }
  if (tx_neq(tx, r = token_cconst(tx, '}'))) {
    *st = stat_new_return(pos, NULL);
    return tx;
  }

  Value *v;
  tx = rvalue(&v, tx);
  *st = stat_new_return(pos, v);
  return tx;
}

static Tx *rthrow(Stat **st, Tx *tx, Pos *pos) {
  Value *v;
  tx = rvalue(&v, tx);
  *st = stat_new_throw(pos, v);
  return tx;
}

static Tx *rwhile(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;
  Value *v;
  Astat *bl;

  if (tx_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;
  tx = rvalue(&v, tx);
  if (tx_eq(tx, r = token_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = r;

  tx = rblock(&bl, tx);

  *st = stat_new_while(pos, v, bl);
  return tx;
}

static Tx *rif(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;

  Value *v;
  Astat *bl;
  Avalue *vs = avalue_new();
  Aastat *bls = aastat_new();

  if (tx_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;
  tx = rvalue(&v, tx);
  if (tx_eq(tx, r = token_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = r;

  tx = rblock(&bl, tx);
  avalue_add(vs, v);
  aastat_add(bls, bl);

  char *id;
  while (tx_neq(tx, r = token_id(&id, tx))) {
    if (strcmp(id, "elif")) {
      break;
    }
    tx = r;

    if (tx_eq(tx, r = token_cconst(tx, '(')))
      TH(tx) "Expected '('" _TH
    tx = r;
    tx = rvalue(&v, tx);
    if (tx_eq(tx, r = token_cconst(tx, ')')))
      TH(tx) "Expected ')'" _TH
    tx = r;

    tx = rblock(&bl, tx);
    avalue_add(vs, v);
    aastat_add(bls, bl);
  }

  if (!strcmp(id, "else")) {
    tx = r;

    tx = rblock(&bl, tx);
    aastat_add(bls, bl);
  }

  *st = stat_new_if(pos, vs, bls);
  return tx;
}

static Tx *rfor(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;

  Astat *bl1, *bl2, *bl3;
  Avalue *vs;

  if (tx_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;

  tx = token_list(&bl1, tx, ';', (Tx *(*)(void **, Tx *))rblock);
  EACH(bl1, Stat, s) {
    if (
      stat_type(s) != SVAL &&
      stat_type(s) != SVAR &&
      stat_type(s) != SASSIGN
    )
      TH1(tx_path(tx), stat_pos(s))
        "Expected 'val', 'var' or assignement"
      _TH
  }_EACH

  tx = token_list(&vs, tx, ';', (Tx *(*)(void **, Tx *))rvalue);
  tx = token_list(&bl2, tx, ')', (Tx *(*)(void **, Tx *))rblock);

  tx = rblock(&bl3, tx);

  Aastat *bls = aastat_new();
  aastat_add(bls, bl1);
  aastat_add(bls, bl2);
  aastat_add(bls, bl3);

  *st = stat_new_for(pos, vs, bls);
  return tx;
}

static Tx *rtry(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;

  char *ex;
  Astat *bl;
  Aastat *bls = aastat_new();

  tx = rblock(&bl, tx);
  aastat_add(bls, bl);

  char *id;
  if (tx_eq(tx, r = token_id(&id, tx)) || strcmp(id, "catch"))
    TH(tx) "Expected 'catch'" _TH
  tx = r;

  if (tx_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;
  if (tx_eq(tx, r = token_valid_id(&ex, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;
  if (tx_eq(tx, r = token_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = r;

  tx = rblock(&bl, tx);
  aastat_add(bls, bl);

  if (tx_neq(tx, r = token_id(&id, tx))) {
    if (!strcmp(id, "finally")) {
      tx = r;

      tx = rblock(&bl, tx);
      aastat_add(bls, bl);
    }
  }

  *st = stat_new_try(pos, ex, bls);
  return tx;
}

static Tx *rassign(Stat **st, Tx *tx, Pos *pos, Value *lv, char *op) {
  Value *rv;
  tx = rvalue(&rv, tx);

  Avatt *atts = value_attachs(lv);
  int size = arr_size(atts);
  bool is_assign = true;
  if (*op == '=' && !*(op + 1) && size > 1) {
    Vatt *last = avatt_get(atts, size - 1);
    if (vatt_is_fn(last)) {
      Avalue *params = vatt_params(last);
      if (arr_size(params) == 1) {
        Vatt *last1 = avatt_get(atts, size - 2);
        if (!vatt_is_fn(last1) && !strcmp(vatt_id(last1), "get")) {
          arr_set(atts, size - 2, vatt_new_id(vatt_pos(last1), "set"));
          avalue_add(params, rv);
          is_assign = false;
        }
      }
    }
  }

  if (is_assign) {
    *st = stat_new_assign(pos, op, lv, rv);
  } else {
    *st = stat_new_fn(pos, lv);
  }

  return tx;
}

Tx *rstat(Stat **st, Tx *tx) {
  Tx *r;
  Pos *pos = tx_pos(tx);
  Tx *start = tx;

  if (tx_neq(tx, r = token_cconst(tx, '{'))) { // Block
    Astat *bl;
    tx = rblock(&bl, tx);
    *st = stat_new_block(pos, bl);
    return tx;
  }

  Type *tp = NULL;
  tx = rtype(&tp, tx);

  char *id;
  if (tx_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  if (tp && strcmp(id, "val") && strcmp(id, "var"))
    TH(tx) "Expected 'val' or 'var'" _TH

  if (!tp) {
    tp = type_new_unknown();
  }

  if (!strcmp(id, "val")) return rval_var(st, tx, pos, tp, true);
  else if (!strcmp(id, "var")) return rval_var(st, tx, pos, tp, false);
  else if (!strcmp(id, "native")) return rnative(st, tx, pos);
  else if (!strcmp(id, "break")) return rbreak(st, tx, pos);
  else if (!strcmp(id, "continue")) return rcontinue(st, tx, pos);
  else if (!strcmp(id, "return")) return rreturn(st, tx, pos);
  else if (!strcmp(id, "throw")) return rthrow(st, tx, pos);
  else if (!strcmp(id, "while")) return rwhile(st, tx, pos);
  else if (!strcmp(id, "if")) return rif(st, tx, pos);
  else if (!strcmp(id, "for")) return rfor(st, tx, pos);
  else if (!strcmp(id, "try")) return rtry(st, tx, pos);
  else if (!strcmp(id, "case")) return rcase(st, tx, pos);

  Value *v;
  tx = rvalue(&v, start);

  while (value_vtype(v) == VGROUP && !arr_size(value_attachs(v))) {
    v = value_restore((Arr *)value_data(v));
  }

  char *op;
  if (tx_neq(tx, r = token_assign(&op, tx))) {
    return rassign(st, r, pos, v, op);
  }

  Avatt *atts = value_attachs(v);
  if (!arr_size(atts) || !vatt_is_fn(avatt_get(atts, 0)))
    TH(tx) "Expected a function call" _TH

  *st = stat_new_fn(pos, v);
  return tx;
}
