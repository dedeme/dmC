// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "runner/runner.h"
#include "heap.h"
#include "kut/DEFS.h"
#include "DEFS.h"
#include "fileix.h"
#include "bfunction.h"
#include "function.h"
#include "obj.h"
#include "runner/fail.h"
#include "runner/solver.h"

static Exp *freturn (Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  Exp *v = stat_get_return(st);
  return exp_is_empty(v)
    ? exp_empty_return()
    : solver_solve(is, h0, hs, v)
  ;
}

static void trace (Map *is, Heap0 *h0, Arr *hs, StatCode *st_cd) {
  //<int, Exp>
  Tp *v = stat_get_trace(stat_code_stat(st_cd));
  char *sexp = exp_to_js(solver_solve(is, h0, hs, tp_e2(v)));
  printf(
    "%s:%d: %s\n",
    fileix_to_str(stat_code_file_ix(st_cd)),
    stat_code_line(st_cd),
    *(int *)tp_e1(v)
      ? sexp
      : strlen(sexp) > 70
        ? str_f("%s...", str_left(sexp, 67))
        : sexp
  )
  ;
}

static void func_call (Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  // <Exp, Arr<Exp>>
  Tp *v = exp_get_pr(stat_get_func(st));
  Exp *fn = solver_solve(is, h0, hs, tp_e1(v));
  // <Exp>
  Arr *pars = exp_rget_array(solver_solve(is, h0, hs, exp_array(tp_e2(v))));
  if (obj_is_bfunction(fn)){
    obj_get_bfunction(fn)(pars);
  } else if (exp_is_function(fn)) {
    Exp *exp = function_run(exp_get_function(fn), pars);
    if (exp_is_break(exp)) {
      EXC_KUT(fail_add_stack(
        "'break' without 'while' or 'for'", exp_get_break(exp)
      ));
    }
    if (exp_is_continue(exp))
      EXC_KUT(fail_add_stack(
        "'continue' without 'while' or 'for'", exp_get_continue(exp)
      ));
  } else {
    EXC_KUT(fail_type("function", fn));
  }
}

static void assign (Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  // <Exp, Exp>
  Tp *v = stat_get_assign(st);
  Exp *left = tp_e1(v);
  if (exp_is_sym(left)) {
    char *sym = exp_rget_sym(left);
    int ok = heap_add(arr_get(hs, 0), sym, solver_solve(is, h0, hs, tp_e2(v)));
    if (!ok)
      EXC_KUT(str_f("Reassignation of symbol '%s'", sym));
    return;
  }
  if (exp_is_sq(left)) {
    // <Exp, Exp>
    Tp *v2 = exp_get_sq(left);
    Exp *ct = solver_solve(is, h0, hs, tp_e1(v2));
    if (exp_is_array(ct)) {
      arr_set(
        exp_rget_array(ct),
        exp_rget_int(solver_solve(is, h0, hs, tp_e2(v2))),
        solver_solve(is, h0, hs, tp_e2(v))
      );
      return;
    }
    if (exp_is_map(ct)) {
      // <Exp>
      Map *m = exp_rget_map(ct);
      char *key = exp_rget_string(solver_solve(is, h0, hs, tp_e2(v2)));
      if (!map_has_key(m, key))
        EXC_KUT(str_f("Key '%s' does not exist in dictionary", key));
      map_put(m, key, solver_solve(is, h0, hs, tp_e2(v)));
      return;
    }
  } else if (exp_is_pt(left)) {
    // <Exp, Exp>
    Tp *v2 = exp_get_sq(left);
    Exp *ct = solver_solve(is, h0, hs, tp_e1(v2));
    if (exp_is_map(ct)) {
      // <Exp>
      Map *m = exp_rget_map(ct);
      char *key = exp_rget_sym(tp_e2(v2));
      if (!map_has_key(m, key))
        EXC_KUT(str_f("Key '%s' does not exist in dictionary", key));
      map_put(m, key, solver_solve(is, h0, hs, tp_e2(v)));
      return;
    }
  }

  EXC_KUT(fail_type("symbol, array or dictionary", left));
}

// v is Tp<Exp, Exp>
static void assign_xxx (
  Map *is, Heap0 *h0, Arr *hs, Tp *v, Exp *(*fn)(Exp *, Exp *)
) {
  Exp *left = tp_e1(v);
  if (exp_is_sq(left)) {
    // <Exp, Exp>
    Tp *v2 = exp_get_sq(left);
    Exp *ct = solver_solve(is, h0, hs, tp_e1(v2));
    if (exp_is_array(ct)) {
      // <Exp>
      Arr *a = exp_rget_array(ct);
      int64_t ix = exp_rget_int(solver_solve(is, h0, hs, tp_e2(v2)));
      arr_set(a, ix, solver_solve_isolate(fn(
        arr_get(a, ix),
        solver_solve(is, h0, hs, tp_e2(v))
      )));
      return;
    }
    if (exp_is_map(ct)) {
      // <Exp>
      Map *m = exp_rget_map(ct);
      char *key = exp_rget_string(solver_solve(is, h0, hs, tp_e2(v2)));
      if (!map_has_key(m, key))
        EXC_KUT(str_f("Key '%s' does not exist in dictionary", key));
      map_put(m, key, solver_solve_isolate(fn(
        opt_get(map_get(m, key)),
        solver_solve(is, h0, hs, tp_e2(v))
      )));
      return;
    }
  } else if (exp_is_pt(left)) {
    // <Exp, Exp>
    Tp *v2 = exp_get_pt(left);
    Exp *ct = solver_solve(is, h0, hs, tp_e1(v2));
    if (exp_is_map(ct)) {
      // <Exp>
      Map *m = exp_rget_map(ct);
      char *key = exp_rget_sym(tp_e2(v2));
      if (!map_has_key(m, key))
        EXC_KUT(str_f("Key '%s' does not exist in dictionary", key));
      map_put(m, key, solver_solve_isolate(fn(
        opt_get(map_get(m, key)),
        solver_solve(is, h0, hs, tp_e2(v))
      )));
      return;
    }
  }

  EXC_KUT(fail_type("array or dictionary", left));
}

static Exp *try (Arr *stk, Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  // [<StatCode>, <char>, <StatCode>, <Opt<StatCode>>]
  Arr *ps = stat_get_try(st);
  Exp *r = exp_empty();
  TRY {
    r = runner_run_stat(arr_copy(stk), is, h0, hs, arr_get(ps, 0));
  } CATCH (e) {
    char *msg = exc_msg(e);
    Heap *h = heap_new();
    heap_add(
      h, arr_get(ps, 1), exp_string(str_right(msg, str_index(msg, ": ") + 2))
    );
    TRY {
      r = runner_run_stat(
        arr_copy(stk), is, h0, heap_add_to_arr(h, hs), arr_get(ps, 2)
      );
    } CATCH (e) {
      StatCode *finally = opt_get(arr_get(ps, 3));
      if (finally) runner_run_stat(arr_copy(stk), is, h0, hs, finally);
      char *msg = exc_msg(e);
      EXC_KUT(str_right(msg, str_index(msg, ": ") + 2));
    }_TRY
  }_TRY
  StatCode *finally = opt_get(arr_get(ps, 3));
  if (finally) {
    Exp *r2 = runner_run_stat(arr_copy(stk), is, h0, hs, finally);
    if (!exp_is_empty(r2)) r = r2;
  }

  return r;
}

static Exp *fwhile (Arr *stk, Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  // [Opt<Exp>, <StatCode>]
  Arr *ps = stat_get_while(st);

  Exp *r = exp_empty();
  Exp *cond = arr_get(ps, 0);
  StatCode *st_cd = arr_get(ps, 1);
  if (exp_is_empty(cond)) {
    for (;;) {
      Exp *r2 = runner_run_stat(arr_copy(stk), is, h0, hs, st_cd);
      if (exp_is_empty(r2)) continue;
      if (exp_is_continue(r2)) continue;
      if (exp_is_break(r2)) break;
      r = r2;
      break;
    }
  } else {
    for (;;) {
      int ok = FALSE;
      Exp *sv_cond = solver_solve(is, h0, hs, cond);
      if (exp_is_bool(sv_cond)) ok = exp_rget_bool(sv_cond);
      else if (exp_is_string(sv_cond)) ok = *exp_rget_string(sv_cond);
      else if (exp_is_array(sv_cond)) ok = arr_size(exp_rget_array(sv_cond));
      else EXC_KUT(fail_type("bool, string or array", sv_cond));

      if (ok) {
        Exp *r2 = runner_run_stat(arr_copy(stk), is, h0, hs, st_cd);
        if (exp_is_empty(r2)) continue;
        if (exp_is_continue(r2)) continue;
        if (exp_is_break(r2)) break;
        r = r2;
        break;
      }
      break;
    }
  }
  return r;
}

static Exp *fif (Arr *stk, Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  // [<Exp>, <StatCode>, Opt<StatCode>]
  Arr *ps = stat_get_if(st);

  Exp *cond = arr_get(ps, 0);
  StatCode *st1 = arr_get(ps, 1);
  int ok = FALSE;
  Exp *sv_cond = solver_solve(is, h0, hs, cond);
  if (exp_is_bool(sv_cond)) ok = exp_rget_bool(sv_cond);
  else if (exp_is_string(sv_cond)) ok = *exp_rget_string(sv_cond);
  else if (exp_is_array(sv_cond)) ok = arr_size(exp_rget_array(sv_cond));
  else EXC_KUT(fail_type("bool, string or array", cond));

  if (ok) return runner_run_stat(stk, is, h0, hs, st1);

  StatCode *st2 = opt_get(arr_get(ps, 2));

  if (st2) return runner_run_stat(stk, is, h0, hs, st2);

  return exp_empty();
}

static Exp *for_c (
  Arr *stk, Map *is, Heap0 *h0, Arr *hs, Stat *st, int with_ix
) {
  // [<char>, <char>, <Exp>, <StatCode>] or [<char>, <Exp>, <StatCode>]
  Arr *v = with_ix ? stat_get_for_ix(st) : stat_get_for(st);
  char *v1 = arr_get(v, 0);
  char *v2 = "";
  int ix = 1;
  if (with_ix) {
    v2 = arr_get(v, 1);
    ++ix;
  }
  Exp *ct = solver_solve(is, h0, hs, arr_get(v, ix++));
  StatCode *st_cd = arr_get(v, ix);

  if (exp_is_array(ct)) {
    // <Exp>
    Arr *a = exp_rget_array(ct);
    EACH(a, Exp, ex) {
      Heap *h = heap_new();
      heap_add(h, v1, ex);
      if (with_ix) heap_add(h, v2, exp_int(_i));
      Exp *r = runner_run_stat(stk, is, h0, heap_add_to_arr(h, hs), st_cd);
      if (exp_is_empty(r) || exp_is_continue(r)) continue;
      if (exp_is_break(r)) break;
      return r;
    }_EACH
    return exp_empty();
  }
  if (exp_is_map(ct)) {
    // <Exp>
    Map *m = exp_rget_map(ct);
    EACH(m, Kv, kv) {
      Heap *h = heap_new();
      heap_add(h, v1, kv_value(kv));
      if (with_ix) heap_add(h, v2, exp_string(kv_key(kv)));
      Exp *r = runner_run_stat(stk, is, h0, heap_add_to_arr(h, hs), st_cd);
      if (exp_is_empty(r) || exp_is_continue(r)) continue;
      if (exp_is_break(r)) break;
      return r;
    }_EACH
    return exp_empty();
  }
  if (exp_is_string(ct)) {
    char *s = exp_rget_string(ct);
    for (int64_t i = 0; i < strlen(s); ++i) {
      Heap *h = heap_new();
      heap_add(h, v1, exp_string(str_new_c(s[i])));
      if (with_ix) heap_add(h, v2, exp_int(i));
      Exp *r = runner_run_stat(stk, is, h0, heap_add_to_arr(h, hs), st_cd);
      if (exp_is_empty(r) || exp_is_continue(r)) continue;
      if (exp_is_break(r)) break;
      return r;
    }
    return exp_empty();
  }
  if (obj_is_iter(ct)) {
    It *it = obj_get_iter(ct);
    int64_t i = 0;
    while (it_has_next(it)) {
      Heap *h = heap_new();
      heap_add(h, v1, it_next(it));
      if (with_ix) heap_add(h, v2, exp_int(i++));
      Exp *r = runner_run_stat(stk, is, h0, heap_add_to_arr(h, hs), st_cd);
      if (exp_is_empty(r) || exp_is_continue(r)) continue;
      if (exp_is_break(r)) break;
      return r;
    }
    return exp_empty();
  }

  EXC_KUT(fail_type("string, array, map or <iter>", ct));
  return NULL; // Ureachable.
}

static Exp *for_r (
  Arr *stk, Map *is, Heap0 *h0, Arr *hs, Stat *st, int with_step
) {
  // [<char>, <Exp>, <Exp>, <Exp>, <StatCode>] or
  // [<char>, <Exp>, <Exp>, <StatCode>]
  Arr *v = with_step ? stat_get_for_rs(st) : stat_get_for_r(st);

  char *var = arr_get(v, 0);
  int64_t begin = exp_rget_int(solver_solve(is, h0, hs, arr_get(v, 1)));
  int64_t end = exp_rget_int(solver_solve(is, h0, hs, arr_get(v, 2)));
  int ix = 3;
  int64_t step = 0;
  if (with_step) {
    step = exp_rget_int(solver_solve(is, h0, hs, arr_get(v, ix++)));
    if (!step)
      EXC_KUT("Step is 0 in 'for'");
  }
  StatCode *st_cd = arr_get(v, ix);

  if (with_step) {
    if (step > 0) ++end;
    else --end;
  } else {
    step = 1;
  }

  if (step < 0) {
    for (int64_t i = begin; i > end; i += step) {
      Heap *h = heap_new();
      heap_add(h, var, exp_int(i));
      Exp *r = runner_run_stat(stk, is, h0, heap_add_to_arr(h, hs), st_cd);
      if (exp_is_empty(r) || exp_is_continue(r)) continue;
      if (exp_is_break(r)) break;
      return r;
    }
    return exp_empty();
  }
  for (int64_t i = begin; i < end; i += step) {
    Heap *h = heap_new();
    heap_add(h, var, exp_int(i));
    Exp *r = runner_run_stat(stk, is, h0, heap_add_to_arr(h, hs), st_cd);
    if (exp_is_empty(r) || exp_is_continue(r)) continue;
    if (exp_is_break(r)) break;
    return r;
  }
  return exp_empty();
}

static Exp *fswitch (Arr *stk, Map *is, Heap0 *h0, Arr *hs, Stat *st) {
  // [<Exp>, Arr<Tp<Exp, StatCode>>]
  Arr *v = stat_get_switch(st);
  Exp *exp1 = solver_solve(is, h0, hs, arr_get(v, 0));
  // [Tp<Exp, StatCode>]
  Arr *tps = arr_get(v, 1);
  EACH(tps, Tp, tp) {
    Exp *exp2 = tp_e1(tp);
    if (exp_is_sym(exp2) && !strcmp(exp_get_sym(exp2), "default"))
      return runner_run_stat(stk, is, h0, hs, tp_e2(tp));
    Exp *ok = solver_solve_isolate(exp_eq(
      exp1, solver_solve(is, h0, hs, exp2)
    ));
    if (exp_rget_bool(ok))
      return runner_run_stat(stk, is, h0, hs, tp_e2(tp));
  }_EACH
  return exp_empty();
}

// -----------------------------------------------------------------------------

// stk is Arr<StatCode>, is is Map<int>,
// hs is Arr<Heap>, code is Arr<StatCode>
Exp *runner_run(
  Arr *stack_trace, Map *imports, Heap0 *hp0, Arr *heaps, Arr *code
) {
  heaps = heap_add_to_arr(heap_new(), heaps);
  EACH(code, StatCode, st) {
    Exp *exp = runner_run_stat(stack_trace, imports, hp0, heaps, st);
    if (!exp_is_empty(exp)) return exp;
  }_EACH
  return exp_empty();
}

// stk is Arr<StatCode>, is is Map<int>, hs is Arr<Heap>
Exp *runner_run_stat(
  Arr *stk, Map *is, Heap0 *h0, Arr *hs, StatCode *st_cd
) {
  Stat *st = stat_code_stat(st_cd);
  arr_push(stk, st_cd);
  Exp *exp = exp_empty();
  TRY {
    if (stat_is_block_close(st)) EXC_KUT("'}' without '{'");
    else if (stat_is_break(st)) exp = exp_break(arr_copy(stk));
    else if (stat_is_continue(st)) exp = exp_continue(arr_copy(stk));
    else if (stat_is_return(st)) exp = freturn(is, h0, hs, st);
    else if (stat_is_trace(st)) trace(is, h0, hs, st_cd);
    else if (stat_is_func(st)) func_call(is, h0, hs, st);
    else if (stat_is_block(st))
      exp = runner_run(arr_copy(stk), is, h0, hs, stat_get_block(st));
    else if (stat_is_assign(st)) assign(is, h0, hs, st);
    else if (stat_is_add_as(st))
      assign_xxx(is, h0, hs, stat_get_add_as(st), exp_add);
    else if (stat_is_sub_as(st))
      assign_xxx(is, h0, hs, stat_get_sub_as(st), exp_sub);
    else if (stat_is_mul_as(st))
      assign_xxx(is, h0, hs, stat_get_mul_as(st), exp_mul);
    else if (stat_is_div_as(st))
      assign_xxx(is, h0, hs, stat_get_div_as(st), exp_div);
    else if (stat_is_mod_as(st))
      assign_xxx(is, h0, hs, stat_get_mod_as(st), exp_mod);
    else if (stat_is_and_as(st))
      assign_xxx(is, h0, hs, stat_get_and_as(st), exp_and);
    else if (stat_is_or_as(st))
      assign_xxx(is, h0, hs, stat_get_or_as(st), exp_or);
    else if (stat_is_try(st)) exp = try(arr_copy(stk), is, h0, hs, st);
    else if (stat_is_while(st)) exp = fwhile(arr_copy(stk), is, h0, hs, st);
    else if (stat_is_if(st)) exp = fif(arr_copy(stk), is, h0, hs, st);
    else if (stat_is_for(st)) exp = for_c(arr_copy(stk), is, h0, hs, st, FALSE);
    else if (stat_is_for_ix(st))
      exp = for_c(arr_copy(stk), is, h0, hs, st, TRUE);
    else if (stat_is_for_r(st))
      exp = for_r(arr_copy(stk), is, h0, hs, st, FALSE);
    else if (stat_is_for_rs(st)) exp =
      for_r(arr_copy(stk), is, h0, hs, st, TRUE);
    else if (stat_is_switch(st)) exp = fswitch(arr_copy(stk), is, h0, hs, st);
    else EXC_KUT(str_f("Not valid statement: '%s'", stat_to_str(st)));
  } CATCH (e) {
    char *msg = exc_msg(e);
    if (stat_is_block(st))
      EXC_KUT(str_right(msg, str_index(msg, ": ") + 2));

    EXC_KUT(fail_add_stack(str_right(msg, str_index(msg, ": ") + 2), stk));
  }_TRY
  arr_pop(stk);
  return exp;
}
