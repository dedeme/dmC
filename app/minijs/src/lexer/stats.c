// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/stats.h"
#include "DEFS.h"
#include "lexer/token.h"
#include "lexer/block.h"
#include "lexer/values.h"
#include "lexer/attachs.h"

Txpos *stats_read(Stat **stat, Txpos *tx) {
  if (txpos_at_end(tx))
    TH(tx) "End of file reached reading a block of statements" _TH

  Txpos *start = tx;
  Txpos *r;

  if (txpos_neq(tx, r = token_cconst(tx, '{'))) {
    tx = r;
    Arr/*Stat*/ *block;
    tx = stats_block(&block, tx);
    *stat = stat_new_block(txpos_pos(start), block);
    return tx;
  }

  bool end_at_block = false;

  bool is_typed = false;
  Type *tp;
  if (txpos_neq(tx, r = token_cconst(tx, '#'))) {
    tx = r;
    tx = block_type(&tp, tx);
    is_typed = true;
  } else {
    tp = type_new_unknown();
  }

  char *id;
  if (txpos_eq(tx, r = token_point_id(&id, tx)))
    TH(tx) "Expected an identifier" _TH
  tx = r;

  if (!strcmp(id, "val")) {

    Dvalue *dvalue;
    tx = block_declaration(&dvalue, tx, tp);

    if (value_vtype(dvalue_value(dvalue)) == VNULL)
        TH(tx) "'val %s' can not be null", dvalue_id(dvalue) _TH

    *stat = stat_new_val(txpos_pos(start), dvalue);
    return tx;

  }

  if (!strcmp(id, "var")) {

    Dvalue *dvalue;
    tx = block_declaration(&dvalue, tx, tp);
    *stat = stat_new_var(txpos_pos(start), dvalue);
    return tx;

  }

  if (is_typed)
    TH(tx) "Expected 'val' or' var'" _TH

  if (!strcmp(id, "break")) {

    stat_new_break(txpos_pos(start));

  } else if (!strcmp(id, "continue")) {

    stat_new_continue(txpos_pos(start));

  } else if (!strcmp(id, "throw")) {

    Value *value;
    tx = values_read(&value, tx, false);

    *stat = stat_new_throw(txpos_pos(start), value);

  } else if (!strcmp(id, "return")) {

    Value *value;
    if (txpos_neq(tx, r = token_cconst(tx, ';'))) {
      value = NULL;
    } else if (txpos_neq(tx, r = token_cconst(tx, '}'))) {
      value = NULL;
    } else {
      tx = values_read(&value, tx, false);
    }

    *stat = stat_new_return(txpos_pos(start), value);

  } else if (!strcmp(id, "while")) {

    Txpos *vstart = tx;
    Value *v;
    tx = values_read0(&v, tx);
    if (value_vtype(v) != VGROUP)
      TH(vstart) "Expected a value inside parenthesis" _TH

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    Arr/*Stat*/ *block;
    tx = stats_block(&block, tx);
    *stat = stat_new_while(txpos_pos(start), v, block);
    end_at_block = true;

  } else if (!strcmp(id, "do")) {

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    Arr/*Stat*/ *block;
    tx = stats_block(&block, tx);

    char *id2;
    if (txpos_eq(tx, r = token_id(&id2, tx)) || strcmp(id2, "while"))
      TH(tx) "Expected 'while'" _TH
    tx = r;

    Txpos *vstart = tx;
    Value *v;
    tx = values_read(&v, tx, false);
    if (value_vtype(v) != VGROUP)
      TH(vstart) "Expected a value inside parenthesis" _TH

    *stat = stat_new_do(txpos_pos(start), v, block);

  } else if (!strcmp(id, "for")) {

    if (txpos_eq(tx, r = token_cconst(tx, '(')))
      TH(tx) "Expected '('" _TH
    tx = r;

    char *id2;
    if (txpos_eq(tx, r = token_id(&id2, tx)))
      TH(tx) "Expected an identifier" _TH
    if (token_is_reserved(id2))
      TH(tx) "'%s' is a reserved word", id2 _TH
    tx = r;

    int kind_for = -1;
    Arr/*Value*/ *vs;
    if (txpos_neq(tx, r = token_cconst(tx, '='))) {
      tx = r;
      tx = token_list(
        &vs, tx, ')', (Txpos *(*)(void **, Txpos *)) values_read0
      );
      if (!arr_size(vs) || arr_size(vs) > 3)
        TH(tx) exc_range(1, 4, arr_size(vs)) _TH
      kind_for = 1;
    } else if (txpos_neq(tx, r = token_cconst(tx, ','))) {
      tx = r;
      tx = token_list(
        &vs, tx, ')', (Txpos *(*)(void **, Txpos *)) values_read0
      );
      if (!arr_size(vs) || arr_size(vs) > 3)
        TH(tx) exc_range(1, 3, arr_size(vs)) _TH
      kind_for = 0;
    } else if (txpos_neq(tx, r = token_const(tx, "::"))) {
      tx = r;
      tx = token_list(
        &vs, tx, ')', (Txpos *(*)(void **, Txpos *)) values_read0
      );
      if (!arr_size(vs) || arr_size(vs) > 3)
        TH(tx) exc_range(1, 2, arr_size(vs)) _TH
      kind_for = 2;
    } else
        TH(tx) "Expected '=', ',' or '::'" _TH

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    Arr/*Stat*/ *block;
    tx = stats_block(&block, tx);

    *stat = kind_for
      ? kind_for == 1
        ? stat_new_for(txpos_pos(start), vs, block)
        : stat_new_for_each(txpos_pos(start), arr_get(vs, 0), block)
      : stat_new_for0(txpos_pos(start), vs, block);
    end_at_block = true;

  } else if (!strcmp(id, "if")) {

    Txpos *vstart = tx;
    Arr/*Value*/ *vs = arr_new();
    Arr/*Arr[Stat]*/ *blocks = arr_new();

    Value *v;
    tx = values_read0(&v, tx);
    if (value_vtype(v) != VGROUP)
      TH(vstart) "Expected a value inside parenthesis" _TH

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    Arr/*Stat*/ *block;
    tx = stats_block(&block, tx);

    arr_add(vs, v);
    arr_add(blocks, block);

    for (;;) {
      char *id;
      if (txpos_eq(tx, r = token_id(&id, tx))) {
        break;
      }
      if (strcmp(id, "elif") && strcmp(id, "else")) {
        break;
      }
      tx = r;

      bool is_else = strcmp(id, "elif");
      if (!is_else) {
        tx = values_read0(&v, tx);
        if (value_vtype(v) != VGROUP)
          TH(vstart) "Expected a value inside parenthesis" _TH
        arr_add(vs, v);
      }

      if (txpos_eq(tx, r = token_cconst(tx, '{')))
        TH(tx) "Expected '{'" _TH
      tx = r;

      tx = stats_block(&block, tx);
      arr_add(blocks, block);

      if (is_else) {
        break;
      }
    }

    *stat = stat_new_if(txpos_pos(start), vs, blocks);
    end_at_block = true;

  } else if (!strcmp(id, "switch")) {

    Txpos *vstart = tx;
    Arr/*Value*/ *vs = arr_new();
    Arr/*Arr[Stat]*/ *blocks = arr_new();

    Value *v;
    tx = values_read0(&v, tx);
    if (value_vtype(v) != VGROUP)
      TH(vstart) "Expected a value inside parenthesis" _TH

    Value *vbase = v;

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    Arr/*Stat*/ *block;
    for (;;) {
      char *id;
      if (txpos_eq(tx, r = token_id(&id, tx))) {
        break;
      }
      if (strcmp(id, "case") && strcmp(id, "default")) {
        break;
      }
      tx = r;

      bool is_default = strcmp(id, "case");
      if (!is_default) {
        Arr/*Value*/ *list;
        Txpos *start2 = tx;
        tx = token_list(
          &list, tx, '{', (Txpos *(*)(void **, Txpos *)) values_read0
        );
        if (!arr_size(list))
          TH(start2) "Expected a value" _TH

        bool is_first = true;
        EACH(list, Value, lv) {
          if (is_first) {
            is_first = false;
            v = value_new_binary(value_pos(lv), "==", vbase, lv);
          } else {
            v = value_new_binary(
              value_pos(lv),
              "||",
              v,
              value_new_binary(value_pos(lv), "==", vbase, lv)
            );
          }
        }_EACH

        arr_add(vs, v);
      } else {
        if (txpos_eq(tx, r = token_cconst(tx, '{')))
          TH(tx) "Expected '{'" _TH
        tx = r;
      }

      tx = stats_block(&block, tx);
      arr_add(blocks, block);

      if (is_default) {
        break;
      }
    }


    if (txpos_eq(tx, r = token_cconst(tx, '}')))
      TH(tx) "Expected '}'" _TH
    tx = r;

    *stat = stat_new_if(txpos_pos(start), vs, blocks);
    end_at_block = true;

  } else if (!strcmp(id, "try")) {
    char *var = "";
    Arr/*Arr[Stat]*/ *blocks = arr_new();

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    Arr/*Stat*/ *block;
    tx = stats_block(&block, tx);
    arr_add(blocks, block);

    char *id;
    if (txpos_eq(tx, r = token_id(&id, tx)) || strcmp(id, "catch"))
      TH(tx) "Expected 'catch'" _TH
    tx = r;

    if (txpos_eq(tx, r = token_cconst(tx, '(')))
      TH(tx) "Expected '('" _TH
    tx = r;

    if (txpos_eq(tx, r = token_id(&var, tx)))
      TH(tx) "Expected an identifier" _TH
    tx = r;

    if (txpos_eq(tx, r = token_cconst(tx, ')')))
      TH(tx) "Expected ')'" _TH
    tx = r;

    if (txpos_eq(tx, r = token_cconst(tx, '{')))
      TH(tx) "Expected '{'" _TH
    tx = r;

    tx = stats_block(&block, tx);
    arr_add(blocks, block);

    if (txpos_neq(tx, r = token_id(&id, tx)) && !strcmp(id, "finally")) {
      tx = r;

      if (txpos_eq(tx, r = token_cconst(tx, '{')))
        TH(tx) "Expected '{'" _TH
      tx = r;

      tx = stats_block(&block, tx);
      arr_add(blocks, block);
    }

    *stat = stat_new_try(txpos_pos(start), var, blocks);
    end_at_block = true;

  } else if (!strcmp(id, "native")) {

    char *id2;
    if (txpos_eq(tx, r = token_id(&id2, tx)))
      TH(tx) "Expected an identifier" _TH
    tx = r;

    char *text;
    tx = token_native(&text, tx, id2);
    *stat = stat_new_native(txpos_pos(start), text);

  } else {

    if (token_is_reserved(id))
      TH(tx) "'%s' is a reserved word", id _TH

    Arr/*Attach*/ *atts = arr_new();
    tx = attachs_read_all(atts, tx);

    if (txpos_neq(tx, r = token_cconst(tx, '='))) {
      tx = r;
      if (
        arr_size(atts) &&
        attach_type(arr_get(atts, arr_size(atts) - 1)) == AFN
      )
        TH(tx) "Unexpected '='" _TH

      Value *lv = value_new_id(txpos_pos(start), atts, id);
      Value *rv;
      tx = values_read(&rv, tx, false);
      *stat = stat_new_assign(txpos_pos(start), lv, rv);
    } else {
      if (
        !arr_size(atts) ||
        attach_type(arr_get(atts, arr_size(atts) - 1)) != AFN
      )
        TH(start) "Expected a function call" _TH

      Value *value = value_new_id(txpos_pos(start), atts, id);
      *stat = stat_new_fn(txpos_pos(start), value);
    }
  }

  if (txpos_eq(tx, r = token_cconst(tx, ';'))) {
    if (*txpos_start(tx) != '}' && !end_at_block)
      TH(tx) "Expected ';'" _TH
  }
  tx = r;
  return tx;
}


Txpos *stats_block(Arr/*Stat*/ **stats, Txpos *tx) {
  Txpos *r;

  Arr/*Stat*/ *ss = arr_new();

  for (;;) {
    while (txpos_neq(tx, r = token_cconst(tx, ';'))) {
      tx = r;
    }

    if (txpos_neq(tx, r = token_cconst(tx, '}'))) {
      tx = r;
      break;
    }

    Stat *stat;
    tx = stats_read(&stat, tx);

    arr_add(ss, stat);
  }

  *stats = ss;
  return tx;
}
