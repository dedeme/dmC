// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/block.h"
#include "lexer/token.h"
#include "lexer/head.h"
#include "lexer/Pos.h"
#include "lexer/values.h"
#include "ast/Cvalue.h"
#include "DEFS.h"

Class *block_top_class(Txpos *tx, Cpath *cpath) {
  Class *c = class_mk(cpath);
  Txpos *r;

  if (txpos_neq(tx, r = head_local(tx))) {
    tx = r;
    class_set_public(c, false);
  }

  Arr/*char*/ *generics = arr_new();
  tx = head_generics(&generics, tx);
  class_set_generics(c, generics);

  Arr/*Import*/ *imports = arr_new();
  tx = head_imports(&imports, tx);
  class_set_imports(c, imports);

  Implement *extends = implement_new(txpos_pos(tx), "", arr_new());
  tx = head_extends(&extends, tx);
  class_set_extends(c, extends);

  Arr/*Implement*/ *implements = arr_new();
  tx = head_implements(&implements, tx);
  class_set_implements(c, implements);


  bool fail = false;
  bool is_public = true;
  while (!txpos_at_end(tx)) {
    if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
      tx = r;
      continue;
    }
    TRY {
      Txpos *start = tx;

      if (txpos_neq(tx, r = token_directive(tx, "_private"))) {
        if (is_public) {
          is_public = false;
          tx = r;
          while (txpos_neq(tx, r = token_cconst(tx, ';'))) {
            tx = r;
          }
        } else
          TH(tx) "Duplicate directive '_private'" _TH
      }

      bool is_typed = false;
      Type *tp;
      if (txpos_neq(tx, r = token_cconst(tx, ':'))) {
        tx = r;
        tx = block_type(&tp, tx);
        is_typed = true;
      } else {
        tp = type_new_unknown();
      }

      char *id;
      if (txpos_eq(tx, r = token_id(&id, tx)))
        TH(tx) "Expected an identifier" _TH
      tx = r;

      if (!strcmp(id, "new")) {
        if (!is_typed)
          TH(tx) "Method type is missing (%s)", id _TH
        if (type_type(tp) != FN)
          TH(tx) "Method type must be function (%s)", id _TH

        if (txpos_eq(tx, r = token_cconst(tx, '=')))
          TH(tx) "Expected '='" _TH
        tx = r;


        Pos *pos = txpos_pos(tx);
        Dvalue *dvalue = dvalue_new(pos, id);
        dvalue_set_type(dvalue, tp);
        dvalue_set_tpos(dvalue, pos);

        Arr/*Cvalue*/ *cvalues;
        tx = values_new_read(&cvalues, tx, dvalue, is_public);

        while (txpos_neq(tx, r = token_cconst(tx, ';'))) {
          tx = r;
        }

        EACH(cvalues, Cvalue, v) {
          Pos *pos = cvalue_pos(v);
          TRY {
            class_add_cvalue(c, v);
          } CATCH(e) {
            printf(
              "%s:%zu[%zu]: %s\n",
              cpath_fpath(txpos_cpath(tx)), pos_nline(pos), pos_nchar(pos),
              str_sub_end(e, str_cindex(e, '\n') + 1)
            );
            THROW "\1" _THROW
          }_TRY
        }_EACH
      } else {
        if (!strcmp(id, "val")) {
          Dvalue *dvalue;
          tx = block_declaration(&dvalue, tx, tp);

          if (value_vtype(dvalue_value(dvalue)) == VNULL)
              TH(tx) "'val %s' can not be null", dvalue_id(dvalue) _TH

          TRY {
            class_add_cvalue(
              c, cvalue_new_val(txpos_pos(start), is_public, true, dvalue)
            );
          } CATCH(e) {
            TH(start) str_sub_end(e, str_cindex(e, '\n') + 1) _TH
          }_TRY
        } else if (!strcmp(id, "var")) {
          Dvalue *dvalue;
          tx = block_declaration(&dvalue, tx, tp);

          TRY {
            class_add_cvalue(
              c, cvalue_new_var(txpos_pos(start), is_public, true, dvalue)
            );
          } CATCH(e) {
            TH(start) str_sub_end(e, str_cindex(e, '\n') + 1) _TH
          }_TRY
        } else if (!strcmp(id, "enum")) {
          if (txpos_eq(tx, r = token_cconst(tx, '(')))
            TH(tx) "Expected '('" _TH
          tx = r;
          Txpos *start2 = tx;
          Arr/*char*/ *ids;
          tx = token_list(
            &ids, tx, ')', (Txpos *(*)(void **, Txpos *))token_valid_id
          );
          if (!arr_size(ids))
            TH(tx) "Variables are missing" _TH
          int n = 0;
          EACH(ids, char, id) {
            Dvalue *dvalue = dvalue_new(txpos_pos(start2), id);
            dvalue_set_tpos(dvalue, dvalue_pos(dvalue));
            dvalue_set_type(dvalue, type_new_int());
            dvalue_set_value(dvalue, value_new_int(
              dvalue_pos(dvalue),
              str_printf("%d", n++)
            ));
            TRY {
              class_add_cvalue(
                c, cvalue_new_val(txpos_pos(start2), is_public, true, dvalue)
              );
            } CATCH(e) {
              TH(start) str_sub_end(e, str_cindex(e, '\n') + 1) _TH
            }_TRY
          }_EACH
        } else {
          if (token_is_reserved(id))
            TH(tx) "'%s' is a reserved word", id _TH

          if (!is_typed)
            TH(tx) "Method type is missing (%s)", id _TH
          if (type_type(tp) != FN)
            TH(tx) "Method type must be function (%s)", id _TH

          if (txpos_eq(tx, r = token_cconst(tx, '=')))
            TH(tx) "Expected '='" _TH
          tx = r;

          Pos *pos = txpos_pos(tx);
          Value *val;
          tx = values_read(&val, tx, false);

          if (type_type(value_type(val)) != FN)
            TH(tx) "Method value must be function (%s)", id _TH

          Dvalue *dvalue = dvalue_new(pos, id);
          dvalue_set_type(dvalue, tp);
          dvalue_set_tpos(dvalue, pos);
          dvalue_set_value(dvalue, val);
          TRY {
            class_add_cvalue(
              c, cvalue_new_val(txpos_pos(start), is_public, true, dvalue)
            );
          } CATCH(e) {
            TH(start) str_sub_end(e, str_cindex(e, '\n') + 1) _TH
          }_TRY
        }
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
  if (token_is_reserved(id))
    TH(tx) "'%s' is a reserved word", id _TH
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
  tx = values_read(&val, tx, false);
/*
  if (txpos_eq(tx, r = token_cconst(tx, ';'))) {
    if (*txpos_start(tx) != '}')
      TH(tx) "Expectedxx ';'" _TH
  }
  tx = r;
*/
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
