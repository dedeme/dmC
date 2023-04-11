// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "checker/checker.h"
#include "kut/DEFS.h"
#include "stat.h"
#include "fileix.h"
#include "function.h"
#include "modules.h"
#include "bmodule.h"
#include "checker/layers.h"

// 'code' is Arr<StatCode>
static void check_block (int isInternal, Layers *layers, Map *is, Arr *code);
// 'is' is Map<int>
static void check_st(Layers *layers, Map* is, StatCode *st_cd);

// 'is' is Map<int>
static void check_exp(Layers *layers, int fix, int nline, Map *is, Exp *exp) {
  if (exp_is_array(exp)) {
    EACH(exp_rget_array(exp), Exp, e) {
      check_exp(layers, fix, nline, is, e);
    }_EACH
  } else if (exp_is_map(exp)) {
    // v is Tp<char, Exp>
    EACH(map_to_array(exp_rget_map(exp)), Tp, v) {
      check_exp(layers, fix, nline, is, tp_e2(v));
    }_EACH
  } else if (exp_is_function(exp)) {
    Function *fn = exp_get_function(exp);
    StatCode *st_cd = function_get_stat(fn);
    //Cksym
    Arr *syms = arr_new();
    EACH(function_get_pars(fn), char, p) {
      arr_push(syms, cksym_new(p, fix, stat_code_line(st_cd)));
    }_EACH
    layers = layers_add(layers, layer_new(FALSE, syms));

    check_st(layers, is, st_cd);
  } else if (exp_is_sym(exp)) {
    layers_err_if_not_found(
      layers, is, cksym_new(exp_rget_sym(exp), fix, nline)
    );
  } else if (exp_is_range(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_range(exp);
    check_exp(layers, fix, nline, is, tp3_e1(v));
    check_exp(layers, fix, nline, is, tp3_e2(v));
    check_exp(layers, fix, nline, is, tp3_e3(v));
  } else if (exp_is_pt(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_pt(exp);
    Exp *exp1 = tp_e1(v);
    char *sym2 = exp_rget_sym(tp_e2(v));
    if (exp_is_sym(exp1)) {
      char *sym1 = exp_rget_sym(exp1);
      if (bmodule_exists(sym1)) {
        TRY {
          bmodule_get_function(sym1, sym2);
        } CATCH (e) {
          if (e) printf(
            "%s:%d: Function '%s.%s' not found\n",
            fileix_to_fail(fix), nline, sym1, sym2
          );
        }_TRY
      } else {
        int r = layers_err_if_not_found(
          layers, is, cksym_new(sym1, fix, nline)
        );
        if (r >= 0) {
          Module *md = opt_get(modules_get_ok(r));
          int ok = FALSE;
          // 'e' is Map<Heap0Entry>
          EACH(map_to_array(heap0_get(module_get_heap0(md))), Tp, e) {
            if (!strcmp(tp_e1(e), sym2)) {
              ok = TRUE;
              break;
            }
          }_EACH
          if (!ok)
            printf(
              "%s:%d: Symbol '%s.%s' not found\n",
              fileix_to_fail(fix), nline, sym1, sym2
            );
        }
      }
    } else {
      check_exp(layers, fix, nline, is, exp1);
    }
  } else if (exp_is_sq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_sq(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_slice(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_slice(exp);
    check_exp(layers, fix, nline, is, tp3_e1(v));
    check_exp(layers, fix, nline, is, tp3_e2(v));
    check_exp(layers, fix, nline, is, tp3_e3(v));
  } else if (exp_is_pr(exp)) {
    // <Exp, Arr<Exp>>
    Tp *v = exp_get_pr(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    EACH(tp_e2(v), Exp, e) {
      check_exp(layers, fix, nline, is, e);
    }_EACH
  } else if (exp_is_switch(exp)) {
    // <Exp, Arr<Tp<Exp, Exp>>>
    Tp *v = exp_get_switch(exp);

    check_exp(layers, fix, nline, is, tp_e1(v));
    // 'e' is Tp<Exp, Exp>
    EACH(tp_e2(v), Tp, e) {
      Exp *exp = tp_e1(e);
      if (!exp_is_sym(exp) || strcmp(exp_rget_sym(exp), "default"))
        check_exp(layers, fix, nline, is, tp_e1(e));
      check_exp(layers, fix, nline, is, tp_e2(e));
    }_EACH
  } else if (exp_is_not(exp)) {
    check_exp(layers, fix, nline, is, exp_get_not(exp));
  } else if (exp_is_minus(exp)) {
    check_exp(layers, fix, nline, is, exp_get_minus(exp));
  } else if (exp_is_add(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_add(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_sub(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_sub(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_mul(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_mul(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_div(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_div(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_mod(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_mod(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_and(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_and(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_or(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_or(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_greater(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_greater(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_greater_eq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_greater_eq(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_less(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_less(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_less_eq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_less_eq(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_eq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_eq(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_neq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_neq(exp);
    check_exp(layers, fix, nline, is, tp_e1(v));
    check_exp(layers, fix, nline, is, tp_e2(v));
  } else if (exp_is_ternary(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_ternary(exp);
    check_exp(layers, fix, nline, is, tp3_e1(v));
    check_exp(layers, fix, nline, is, tp3_e2(v));
    check_exp(layers, fix, nline, is, tp3_e3(v));
  }
}

// 'is' is Map<int>
static void check_st(Layers *layers, Map *is, StatCode *st_cd) {
  int fix = stat_code_file_ix(st_cd);
  int line = stat_code_line(st_cd);
  Stat *st = stat_code_stat(st_cd);
  if (stat_is_assign(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_assign(st);
    Exp *left = tp_e1(v);
    Exp *right = tp_e2(v);
    if (exp_is_sym(left)) {
      layers_add_symbol(layers, fix, line, exp_rget_sym(left));
    } else {
      check_exp(layers, fix, line, is, left);
    }
    check_exp(layers, fix, line, is, right);
  } else if (stat_is_add_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_add_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_sub_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_sub_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_mul_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_mul_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_div_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_div_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_mod_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_mod_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_and_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_and_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_or_as(st)) {
    // <Exp, Exp>
    Tp *v = stat_get_or_as(st);
    check_exp(layers, fix, line, is, tp_e1(v));
    check_exp(layers, fix, line, is, tp_e2(v));
  } else if (stat_is_func(st)) {
    check_exp(layers, fix, line, is, stat_get_func(st));
  } else if (stat_is_block(st)) {
    check_block(TRUE, layers, is, stat_get_block(st));
  } else if (stat_is_trace(st)) {
    check_exp(layers, fix, line, is, tp_e2(stat_get_trace(st)));
  } else if (stat_is_return(st)) {
    check_exp(layers, fix, line, is,stat_get_return(st));
  } else if (stat_is_try(st)) {
    // [<StatCode>, <char>, <StatCode>, <Opt<StatCode>>]
    Arr *v = stat_get_try(st);
    check_st(layers, is, arr_get(v, 0));
    char *param = arr_get(v, 1);
    StatCode *catch_st = arr_get(v, 2);
    Cksym *new_sym = cksym_new(param, fix, stat_code_line(catch_st));
    check_st(
      layers_add(layers, layer_new(FALSE, arr_new_from(new_sym, NULL))),
      is, catch_st
    );
    StatCode *finally_st = opt_get(arr_get(v, 3));
    if (finally_st) check_st(layers, is, finally_st);
  } else if (stat_is_while(st)) {
    // [<Exp>, <StatCode>]
    Arr *v = stat_get_while(st);
    Exp *cond = arr_get(v, 0);
    if (!exp_is_empty(cond)) check_exp(layers, fix, line, is, cond);
    check_st(layers, is, arr_get(v, 1));
  } else if (stat_is_if(st)) {
    // [<Exp>, <StatCode>, Opt<StatCode>]
    Arr *v = stat_get_if(st);
    Exp *cond = arr_get(v, 0);
    if (cond) check_exp(layers, fix, line, is, cond);
    check_st(layers, is, arr_get(v, 1));
    StatCode *else_st = opt_get(arr_get(v, 2));
    if (else_st) check_st(layers, is, else_st);
  } else if (stat_is_for(st)) {
    // [<char>, <Exp>, <StatCode>]
    Arr *v = stat_get_for(st);
    check_exp(layers, fix, line, is, arr_get(v, 1));
    StatCode *for_st = arr_get(v, 2);
    Cksym *new_sym = cksym_new(arr_get(v, 0), fix, stat_code_line(for_st));
    check_st(
      layers_add(layers, layer_new(FALSE, arr_new_from(new_sym, NULL))),
      is, for_st
    );
  } else if (stat_is_for_ix(st)) {
    // [<char>, <char>, <Exp>, <StatCode>]
    Arr *v = stat_get_for_ix(st);
    check_exp(layers, fix, line, is, arr_get(v, 2));
    StatCode *for_st = arr_get(v, 3);
    Cksym *new_sym1 = cksym_new(arr_get(v, 0), fix, stat_code_line(for_st));
    Cksym *new_sym2 = cksym_new(arr_get(v, 1), fix, stat_code_line(for_st));
    check_st(
      layers_add(layers, layer_new(FALSE, arr_new_from(new_sym1, new_sym2, NULL))),
      is, for_st
    );
  } else if (stat_is_for_r(st)) {
    // [<char>, <Exp>, <Exp>, <StatCode>]
    Arr *v = stat_get_for_r(st);
    check_exp(layers, fix, line, is, arr_get(v, 1));
    check_exp(layers, fix, line, is, arr_get(v, 2));
    StatCode *for_st = arr_get(v, 3);
    Cksym *new_sym = cksym_new(arr_get(v, 0), fix, stat_code_line(for_st));
    check_st(
      layers_add(layers, layer_new(FALSE, arr_new_from(new_sym, NULL))),
      is, for_st
    );
  } else if (stat_is_for_rs(st)) {
    // [<char>, <Exp>, <Exp>, <Exp>, <StatCode>]
    Arr *v = stat_get_for_rs(st);
    check_exp(layers, fix, line, is, arr_get(v, 1));
    check_exp(layers, fix, line, is, arr_get(v, 2));
    check_exp(layers, fix, line, is, arr_get(v, 3));
    StatCode *for_st = arr_get(v, 4);
    Cksym *new_sym = cksym_new(arr_get(v, 0), fix, stat_code_line(for_st));
    check_st(
      layers_add(layers, layer_new(FALSE, arr_new_from(new_sym, NULL))),
      is, for_st
    );
  } else if (stat_is_switch(st)) {
    // [<Exp>, Arr<Tp<Exp, StatCode>>]
    Arr *v = stat_get_switch(st);
    check_exp(layers, fix, line, is, arr_get(v, 0));
    // exp_st is Tp<Exp, StatCode>
    EACH(arr_get(v, 1), Tp, exp_st) {
      StatCode *st_cd = tp_e2(exp_st);
      Exp *exp = tp_e1(exp_st);
      if (!exp_is_sym(exp) || strcmp(exp_rget_sym(exp), "default"))
        check_exp(layers, fix, stat_code_line(st_cd), is, tp_e1(exp_st));
      check_st(layers, is, st_cd);
    }_EACH
  }
}

// 'is' is Map<int>, 'code' is Arr<StatCode>
static void check_block (int isInternal, Layers *layers, Map *is, Arr *code) {
  if (isInternal) layers = layers_add(layers, layer_new(TRUE, arr_new()));

  EACH(code, StatCode, st_cd) {
    check_st(layers, is, st_cd);
  }_EACH

  if (isInternal) {
    EACH(layers_not_used(layers), Cksym, sym) {
      printf(
        "%s:%d: Symbol '%s' not used\n",
        fileix_to_fail(cksym_get_fix(sym)), cksym_get_nline(sym),
        cksym_get_id(sym)
      );
    }_EACH
  }
}

void checker_run (int fix, Module *md) {
  // Cksym
  Arr *syms = arr_new();
  // v is a Tp<char, Heap0Entry>
  EACH(map_to_array(heap0_get(module_get_heap0(md))), Tp, v) {
    arr_push(syms, cksym_new(tp_e1(v), fix, heap0_entry_nline(tp_e2(v))));
  }_EACH

  Layer *layer = layer_new(FALSE, syms);
  check_block (
    FALSE, layers_new(layer), module_get_imports(md), module_get_code(md)
  );
}
