// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/block.h"
#include "lexer/token.h"
#include "lexer/Pos.h"
#include "lexer/values.h"
#include "ast/Cvalue.h"
#include "DEFS.h"

Class *block_top_class(Txpos *tx, Cpath *cpath) {
  Class *c = class_mk(cpath);
  Txpos *r;

  bool fail = false;
  while (!txpos_at_end(tx)) {
    TRY {
      Txpos *start = tx;
      bool is_typed = false;
      Type *tp;
      if (txpos_neq(tx, r = token_cconst(tx, '#'))) {
        tx = r;
        tx = block_type(&tp, tx);
        is_typed = true;
      } else {
        tp = type_new_unknown();
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

      char *id;
      if (txpos_eq(tx, r = token_id(&id, tx)))
        TH(tx) "Expected an identifier" _TH
      tx = r;

      if (!strcmp(id, "val")) {
        if (is_getter_setter) TH(tx) "'=' is incompatible with 'val'" _TH
        if (!is_static) TH(tx) "'@' is incompatible with 'val'" _TH

        Dvalue *dvalue;
        tx = block_declaration(&dvalue, tx, tp);

        if (value_vtype(dvalue_value(dvalue)) == VNULL)
            TH(tx) "'val %s' can not be null", dvalue_id(dvalue) _TH

        TRY {
          class_add_cvalue(c, cvalue_new(is_public, is_static, VAL, dvalue));
        } CATCH(e) {
          TH(start) str_sub_end(e, str_cindex(e, '\n') + 1) _TH
        }_TRY
      } else if (!strcmp(id, "var")) {
        if (is_getter_setter) TH(tx) "'=' is incompatible with 'var'" _TH
        if (!is_static) TH(tx) "'@' is incompatible with 'var'" _TH

        Dvalue *dvalue;
        tx = block_declaration(&dvalue, tx, tp);

        TRY {
          class_add_cvalue(c, cvalue_new(is_public, is_static, VAR, dvalue));
        } CATCH(e) {
          TH(start) str_sub_end(e, str_cindex(e, '\n') + 1) _TH
        }_TRY
      } else {
        if (is_typed)
          TH(tx) "Expected a typed declaration" _TH
        THROW "Unknown dentifier '%s'", id _THROW
      }
    } CATCH(e) {
      if (e[strlen(e) - 1] == '\1') {
        fail = true;
        tx = token_skip_stat(tx);
      } else
        THROW e _THROW
    } _TRY
  }

  if (fail)
    THROW "\1" _THROW

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

    tp = type_new_fn(params);
  } else if (txpos_neq(tx, r = token_cconst(tx, '['))) {
    tx = r;

    tx = block_type(&tp, tx);
    if (txpos_eq(tx, r = token_cconst(tx, ']')))
      TH(tx) "Expected ']'" _TH

    tp = type_new_arr(tp);
  } else if (txpos_neq(tx, r = token_cconst(tx, '{'))) {
    tx = r;

    tx = block_type(&tp, tx);
    if (txpos_eq(tx, r = token_cconst(tx, '}')))
      TH(tx) "Expected '}'" _TH

    tp = type_new_map(tp);
  } else if (txpos_neq(tx, r = token_cconst(tx, '*'))) {
    tp = type_new_any();
  } else if (txpos_neq(tx, r = token_id(&id, tx))) {
    tx = r;

    if (txpos_neq(tx, r = token_cconst(tx, '<'))) {
      tx = r;

      Arr/*Type*/ *params;
      r = token_list(&params, r, '>', (Txpos *(*)(void **, Txpos *))block_type);
      if (arr_size(params) == 0)
        TH(tx) "Expected at least one parameter" _TH

      tp = type_new_data(id, params);
    } else {
      tp = type_new_data(id, arr_new());
    }
  } else {
    TH(tx) "Wrong type definition" _TH
    assert(false);
  }

  *type = tp;
  return r;
}

Txpos *block_declaration(Dvalue **d, Txpos *tx, Type *tp) {
  Pos *pos = txpos_pos(tx);
  Txpos *r;

  char *id;
  if (txpos_eq(tx, r = token_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
    Dvalue *dvalue = dvalue_new(pos, id);
    if (!type_is_unknown(tp)) {
      dvalue_set_type(dvalue, tp);
      dvalue_set_tpos(dvalue, pos);
    }
    *d = dvalue;
    return r;
  }

  if (txpos_eq(tx, r = token_cconst(tx, '=')))
    TH(tx) "Expected '='" _TH
  tx = r;

  pos = txpos_pos(tx);
  Value *val;
  if (!strcmp(id, "new")) {
    THROW "Without implementation" _THROW
  } else {
    tx = values_read(&val, tx, false);
  }

  if (txpos_eq(tx, r = token_cconst(tx, ';')))
    TH(tx) "Expected ';'" _TH
  tx = r;

  Dvalue *dvalue = dvalue_new(pos, id);
  if (type_is_unknown(tp)) {
    tp = value_type(val);
  }
  if (!type_is_unknown(tp)) {
    dvalue_set_type(dvalue, tp);
    dvalue_set_tpos(dvalue, pos);
  }
  dvalue_set_value(dvalue, val);

  *d = dvalue;
  return tx;
}
