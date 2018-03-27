// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/block.h"
#include "lexer/token.h"
#include "lexer/Filepos.h"
#include "ast/Cvalue.h"
#include "DEFS.h"

Class *block_top_class(Txpos *tx, Cpath *cpath) {
  Class *c = class_mk(cpath);
  Txpos *r;

  while (!txpos_at_end(tx)) {
//    Filepos *fpos = txpos_filepos(tx);

    Type *tp;
    if (txpos_neq(tx, r = token_cconst(tx, '#'))) {
      tx = r;
      tx = block_type(&tp, tx);
    } else {
      tp = type_new(UNKNOWN, "", arr_new());
    }

    bool is_public = true;
    if (txpos_neq(tx, r = token_cconst(tx, '-'))) {
      tx = r;
      is_public = false;
    }

    bool is_getter_setter = false;
    if (txpos_neq(tx, r = token_cconst(tx, '='))) {
      tx = r;
      is_getter_setter = true;
    }

    bool is_static = true;
    if (txpos_neq(tx, r = token_cconst(tx, '@'))) {
      tx = r;
      is_static = false;
    }

    bool is_val = false;
    bool is_var = false;
    if (txpos_neq(tx, r = token_const(tx, "val"))) {
      tx = r;
      is_val = true;
    } else if (txpos_neq(tx, r = token_const(tx, "var"))) {
      tx = r;
      is_var = true;
    }

    if (is_val || is_var) {
      if (is_val) {
        if (is_var) TH(tx) "Declarations val and var are incompatible" _TH
        if (is_getter_setter) TH(tx) "'=' is incompatible with 'val'" _TH
        if (!is_static) TH(tx) "'@' is incompatible with 'val'" _TH
      } else {
        if (is_getter_setter) TH(tx) "'=' is incompatible with 'var'" _TH
        if (!is_static) TH(tx) "'@' is incompatible with 'var'" _TH
      }

      Dvalue *dvalue;
      tx = block_declaration(&dvalue, tx, tp);

      if (is_val) {
        arr_add(
          class_cvalues(c),
          cvalue_new(is_public, is_static, VAL, dvalue)
        );
      } else {
        arr_add(
          class_cvalues(c),
          cvalue_new(is_public, is_static, VAR, dvalue)
        );
      }
    } else {


      char *id;
      if (txpos_eq(tx, r = token_id(&id, tx)))
        TH(tx) "Expected an identifier" _TH
      tx = r;
    }

  }

  return c;
}

Txpos *block_type(Type **type, Txpos *tx) {
  Type *tp;
  Txpos *r;
  char *id;

  if (txpos_neq(tx, r = token_cconst(tx, '('))) {
    Arr/*Type*/ *params;
    r = token_list(&params, r, ':', (Txpos *(*)(void **, Txpos *))block_type);
    tx = r;

    if (txpos_neq(tx, r = token_cconst(tx, ')'))) {
      arr_add(params, NULL);
    } else {
      tx = block_type(&tp, tx);
      arr_add(params, tp);
      if (txpos_eq(tx, r = token_cconst(tx, ')')))
        TH(tx) "Expected ')'" _TH
    }

    tp = type_new(FN, "", params);
  } else if (txpos_neq(tx, r = token_cconst(tx, '['))) {
    tx = r;

    tx = block_type(&tp, tx);
    if (txpos_eq(tx, r = token_cconst(tx, ']')))
      TH(tx) "Expected ']'" _TH

    Arr/*Type*/ *params = arr_new();
    arr_add(params, tp);
    tp = type_new(ARR, "", params);
  } else if (txpos_neq(tx, r = token_cconst(tx, '{'))) {
    tx = r;

    tx = block_type(&tp, tx);
    if (txpos_eq(tx, r = token_cconst(tx, '}')))
      TH(tx) "Expected '}'" _TH

    Arr/*Type*/ *params = arr_new();
    arr_add(params, tp);
    tp = type_new(MAP, "", params);
  } else if (txpos_neq(tx, r = token_cconst(tx, '*'))) {
    tp = type_new(ANY, "", arr_new());
  } else if (txpos_neq(tx, r = token_id(&id, tx))) {
    tx = r;

    if (txpos_neq(tx, r = token_cconst(tx, '<'))) {
      tx = r;

      Arr/*Type*/ *params;
      r = token_list(&params, r, '>', (Txpos *(*)(void **, Txpos *))block_type);
      if (arr_size(params) == 0)
        TH(tx) "Expected at least one parameter" _TH

      tp = type_new(DATA, id, params);
    } else {
      tp = type_new(DATA, id, arr_new());
    }
  } else {
    TH(tx) "Wrong type definition" _TH
    assert(false);
  }

  *type = tp;
  return r;
}

Txpos *block_declaration(Dvalue **d, Txpos *tx, Type *tp) {
  Filepos *fpos = txpos_filepos(tx);
  Txpos *r;

  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
    Dvalue *dvalue = dvalue_new(fpos, id);
    if (type_type(tp) != UNKNOWN) {
      dvalue_set_type(dvalue, tp);
      dvalue_set_tpos(dvalue, fpos);
    }
    *d = dvalue;
    return r;
  }

  return tx;
}
