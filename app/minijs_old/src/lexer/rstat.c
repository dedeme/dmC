// Copyright 16-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/rstat.h"
#include "lexer/lex.h"
#include "lexer/Pos.h"
#include "lexer/rtype.h"
#include "lexer/rvalue.h"
#include "lexer/rblock.h"
#include "lexer/rcase.h"
#include "lexer/Tx.h"
#include "lexer/lex.h"
#include "ct/Astat.h"
#include "ct/Avalue.h"
#include "ct/Avatt.h"
#include "ct/Lstat.h"
#include "ct/Lvalue.h"
#include "ct/Aastat.h"
#include "ast/Type.h"
#include "ast/Value.h"
#include "ast/Vatt.h"
#include "ast/Stat.h"
#include "DEFS.h"

static Tx *rval_var(Stat **st, Tx *tx, Pos *pos, Type *tp, bool is_val) {
  Tx *r;

  char *id;
  if (tx_eq(tx, r = lex_valid_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = lex_blanks(r);

  Value *v;
  if (tx_eq(tx, r = lex_cconst(tx, '='))) {
    if (type_t(tp) == type_UNKNOWN)
      TH1(tx_path(tx), pos) "Type declaration is needed" _TH

    v = value_new_null(tx_pos(tx));
    value_set_type(v, tp);
  } else {
    tx = rvalue(&v, lex_blanks(r));

    if (type_t(tp) == type_UNKNOWN) {
      Type *vtp = value_type(v);
      if (type_t(vtp) == type_UNKNOWN)
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
  if (tx_eq(tx, r = lex_id(&mark, tx)))
    TH(tx) "Expected a mark" _TH
  tx = lex_blanks(r);

  char *text;
  tx = lex_native(&text, tx, mark);

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

  if (tx_neq(tx, r = lex_cconst(tx, ';'))) {
    *st = stat_new_return(pos, NULL);
    return r;
  }
  if (tx_neq(tx, r = lex_cconst(tx, '}'))) {
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

  if (tx_eq(tx, r = lex_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = lex_blanks(r);
  tx = rvalue(&v, tx);
  if (tx_eq(tx, r = lex_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = r;

  tx = rblock(&bl, lex_blanks(tx));

  *st = stat_new_while(pos, v, bl);
  return tx;
}

static Tx *rif(Stat **st, Tx *tx, Pos *pos) {
  Tx *r;

  Value *v;
  Astat *bl;
  Avalue *vs = avalue_new();
  Aastat *bls = aastat_new();

  if (tx_eq(tx, r = lex_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = lex_blanks(r);
  tx = rvalue(&v, tx);
  if (tx_eq(tx, r = lex_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = lex_blanks(r);

  tx = rblock(&bl, tx);
  tx = lex_blanks(tx);

  avalue_add(vs, v);
  aastat_add(bls, bl);

  char *id;
  while (tx_neq(tx, r = lex_id(&id, tx))) {
    if (str_cmp(id, "elif")) {
      break;
    }
    tx = lex_blanks(r);

    if (tx_eq(tx, r = lex_cconst(tx, '(')))
      TH(tx) "Expected '('" _TH
    tx = lex_blanks(r);
    tx = rvalue(&v, tx);
    if (tx_eq(tx, r = lex_cconst(tx, ')')))
      TH(tx) "Expected ')'" _TH
    tx = lex_blanks(r);

    tx = rblock(&bl, tx);
    avalue_add(vs, v);
    aastat_add(bls, bl);
  }

  if (str_eq(id, "else")) {
    tx = lex_blanks(r);

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

  if (tx_eq(tx, r = lex_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = lex_blanks(r);

  tx = lex_arr((Arr **)&bl1, tx, ';', (Tx *(*)(void **, Tx *))rblock);
  tx = lex_blanks(tx);
  EACH(bl1, Stat, s) {
    if (
      stat_t(s) != stat_VAL &&
      stat_t(s) != stat_VAR &&
      stat_t(s) != stat_ASSIGN
    )
      TH1(tx_path(tx), stat_pos(s))
        "Expected 'val', 'var' or assignement"
      _TH
  }_EACH

  tx = lex_arr((Arr **)&vs, tx, ';', (Tx *(*)(void **, Tx *))rvalue);
  tx = lex_blanks(tx);

  tx = lex_arr((Arr **)&bl2, tx, ')', (Tx *(*)(void **, Tx *))rblock);
  tx = lex_blanks(tx);

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
  if (tx_eq(tx, r = lex_id(&id, tx)) || str_cmp(id, "catch"))
    TH(tx) "Expected 'catch'" _TH
  tx = lex_blanks(r);

  if (tx_eq(tx, r = lex_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = lex_blanks(r);
  if (tx_eq(tx, r = lex_valid_id(&ex, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = lex_blanks(r);
  if (tx_eq(tx, r = lex_cconst(tx, ')')))
    TH(tx) "Expected ')'" _TH
  tx = lex_blanks(r);

  tx = rblock(&bl, tx);
  tx = lex_blanks(tx);
  aastat_add(bls, bl);

  if (tx_neq(tx, r = lex_id(&id, tx))) {
    if (str_eq(id, "finally")) {
      tx = lex_blanks(r);

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
  int size = avatt_size(atts);
  bool is_assign = true;
  if (*op == '=' && !*(op + 1) && size > 1) {
    Vatt *last = avatt_get(atts, size - 1);
    if (vatt_is_fn(last)) {
      Avalue *params = lvalue_to_arr(vatt_params(last));
      if (avalue_size(params) == 1) {
        Vatt *last1 = avatt_get(atts, size - 2);
        if (!vatt_is_fn(last1) && str_eq(vatt_id(last1), "get")) {
          avatt_set(atts, size - 2, vatt_new_id(vatt_pos(last1), "set"));
          avalue_add(params, rv);
          avatt_set(atts, size - 1, vatt_new_fn(
            vatt_pos(last), lvalue_from_arr(params)
          ));
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

  if (tx_neq(tx, r = lex_cconst(tx, '{'))) { // Block
    Astat *bl;
    tx = rblock(&bl, tx);
    *st = stat_new_block(pos, bl);
    return tx;
  }

  Type *tp = NULL;
  tx = rtype(&tp, tx);
  tx = lex_blanks(tx);

  char *id;
  if (tx_eq(tx, r = lex_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = lex_blanks(r);

  if (tp && str_cmp(id, "val") && str_cmp(id, "var"))
    TH(tx) "Expected 'val' or 'var'" _TH

  if (!tp) {
    tp = type_new_unknown();
  }

  if (str_eq(id, "val")) return rval_var(st, tx, pos, tp, true);
  else if (str_eq(id, "var")) return rval_var(st, tx, pos, tp, false);
  else if (str_eq(id, "native")) return rnative(st, tx, pos);
  else if (str_eq(id, "break")) return rbreak(st, tx, pos);
  else if (str_eq(id, "continue")) return rcontinue(st, tx, pos);
  else if (str_eq(id, "return")) return rreturn(st, tx, pos);
  else if (str_eq(id, "throw")) return rthrow(st, tx, pos);
  else if (str_eq(id, "while")) return rwhile(st, tx, pos);
  else if (str_eq(id, "if")) return rif(st, tx, pos);
  else if (str_eq(id, "for")) return rfor(st, tx, pos);
  else if (str_eq(id, "try")) return rtry(st, tx, pos);
  else if (str_eq(id, "case")) return rcase(st, tx, pos);

  Value *v;
  tx = rvalue(&v, start);
  tx = lex_blanks(tx);

  while (value_t(v) == value_VGROUP && !avatt_size(value_attachs(v))) {
    v = value_from_json((Ajson *)value_data(v));
  }

  char *op;
  if (tx_neq(tx, r = lex_assign(&op, tx))) {
    tx = lex_blanks(r);
    return rassign(st, tx, pos, v, op);
  }

  Avatt *atts = value_attachs(v);
  if (!avatt_size(atts) || !vatt_is_fn(avatt_get(atts, 0)))
    TH(tx) "Expected a function call" _TH

  *st = stat_new_fn(pos, v);
  return tx;
}
