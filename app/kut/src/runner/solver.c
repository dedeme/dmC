// Copyright 22-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "runner/solver.h"
#include "kut/math.h"
#include "kut/thread.h"
#include "DEFS.h"
#include "fileix.h"
#include "function.h"
#include "typed/tfunction.h"
#include "heap.h"
#include "modules.h"
#include "bmodule.h"
#include "imports.h"
#include "obj.h"
#include "symix.h"
#include "runner/fail.h"
#include "runner/solver.h"
#include "runner/runner.h"
#include "reader/reader.h"
#include "mods/md_str.h"
#include "mods/md_arr.h"
#include "mods/md_dic.h"


typedef struct {
  int64_t i;
  int64_t end;
} solver_range_O;
static Opt *range_next (solver_range_O *o) {
  int64_t i = (o->i)++;
  if (i >= o->end) return opt_none();

  Exp *r = exp_int(i);
  return opt_some(r);
}

typedef struct {
  int64_t i;
  int64_t step;
  int64_t end;
} solver_range2_O;
static Opt *range_next_up (solver_range2_O *o) {
  int64_t i = o->i;
  o->i = i + o->step;
  if (i > o->end) return opt_none();

  Exp *r = exp_int(i);
  return opt_some(r);
}
static Opt *range_next_down (solver_range2_O *o) {
  int64_t i = o->i;
  o->i = i + o->step;
  if (i < o->end) return opt_none();

  Exp *r = exp_int(i);
  return opt_some(r);
}

// ----------------------------------------------------------------------------

Exp *solver_solve(Imports *is, Heap0 *h0, Heaps *hs, Exp *exp) {
  if (exp_is_bool(exp)) {
    return exp;
  }
  if (exp_is_int(exp)) {
    return exp;
  }
  if (exp_is_float(exp)) {
    return exp;
  }
  if (exp_is_string(exp)) {
    return exp;
  }
  if (exp_is_some_object(exp)){
    return exp;
  }
  if (exp_is_array(exp)) {
      //--
      Exp *solve (Exp *e) { return solver_solve(is, h0, hs, e); }
    return exp_array(arr_map(exp_get_array(exp), (FMAP)solve));
  }
  if (exp_is_dic(exp)) {
      //--
      // <Exp>
      Kv *solve (Kv *kv) {
        return kv_new(kv_key(kv), solver_solve(is, h0, hs, kv_value(kv)));
      }
    return exp_dic((Map *)arr_map(map_to_array(exp_get_dic(exp)), (FMAP)solve));
  }
  if (exp_is_function(exp)) {
    return exp_function(function_set_context(
      exp_get_function(exp), is, h0, hs
    ));
  }
  if (exp_is_tfunction(exp)) {
    return exp;
  }
  if (exp_is_sym(exp)) {
    int sym = exp_get_sym(exp);

    // <Opt<Exp>, Heap|NULL>
    Tp *ex_hp = heaps_get(hs, sym);
    Exp *r = opt_get(tp_e1(ex_hp));

    // IN HEAP

    if (r) return r;

    Heap0Entry *e = opt_get(heap0_get(h0, sym));

    // IN HEAP0

    if (e) {
      Heap0 *new_h0 = h0;
      Exp *exp = heap0_entry_exp(e);

      if (heaps_is_initial(hs)) {
        if (exp_is_cyclic(exp))
          EXC_KUT(str_f("Cyclic symbol %s", symix_get(sym)));
        new_h0 = heap0_new_cyclic(h0, e);
      }

      r = solver_solve(is, new_h0, hs, exp);
      heap_add(tp_e2(ex_hp), sym, r);
      return r;
    }

    int fix = imports_get_fix(is, sym);

    // IN IMPORTS

    if (fix != -1) {
      Module *md;
        //--
        void fn (void) {
          md = opt_get(modules_get_ok(fix));
          if (md) return;

          char *kut_code = fileix_read(fix);
          md = reader_read_main_block(cdr_new(fix, kut_code));
          modules_set(fix, md);
          Exp *rs = runner_run(
            TRUE,
            stack_new(),
            module_get_imports(md), module_get_heap0(md),
            heaps_new(module_get_heap(md)),
            module_get_code(md)
          );
          if (exp_is_break(rs))
            EXC_KUT(fail_add_stack(
              "'break' without 'while' or 'for'", exp_get_break(rs)
            ));
          if (exp_is_continue(rs))
            EXC_KUT(fail_add_stack(
              "'continue' without 'while' or 'for'", exp_get_continue(rs)
            ));
        }
      thread_sync(fn);

      return obj_module(md);
    }

    // IN BUILT-IN FUNCTIONS

    if (bmodule_exists(sym)) return obj_bmodule(sym);

    // NOT FOUND

    EXC_KUT(str_f("Symbol '%s' not found.", symix_get(sym)));
  }
  if (exp_is_range(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_range(exp);
    int64_t start = exp_get_int(solver_solve(is, h0, hs, tp3_e1(v)));
    int64_t end = exp_get_int(solver_solve(is, h0, hs, tp3_e2(v)));
    if (exp_is_empty(tp3_e3(v))) {
      solver_range_O *o = MALLOC(solver_range_O);
      o->i = start;
      o->end = end;
      return obj_iter(it_new(o, (Opt *(*)(void *))range_next));
    }
    int64_t step = exp_get_int(solver_solve(is, h0, hs, tp3_e3(v)));
    if (!step) EXC_KUT("Range can not have step 0");
    if (step > 0) {
      solver_range2_O *o = MALLOC(solver_range2_O);
      o->i = start;
      o->step = step;
      o->end = end;
      return obj_iter(it_new(o, (Opt *(*)(void *))range_next_up));
    }
    solver_range2_O *o = MALLOC(solver_range2_O);
    o->i = start;
    o->step = step;
    o->end = end;
    return obj_iter(it_new(o, (Opt *(*)(void *))range_next_down));
  }
  if (exp_is_pt(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_pt(exp);
    Exp *ex0 = solver_solve(is, h0, hs, tp_e1(v));
    if (obj_is_module(ex0)) {
      Module *md = obj_get_module(ex0);
      int sym = exp_get_sym(tp_e2(v)); // test fi tp_e2(v) is symbol.
      if (exports_contains(module_get_exports(md), sym))
        return solver_solve(
          module_get_imports(md),
          module_get_heap0(md),
          heaps_new(module_get_heap(md)),
          tp_e2(v)
        );
      EXC_KUT(str_f("Symbol '%s' not found", symix_get(sym)));
    }
    if (obj_is_bmodule(ex0)) {
      return obj_bfunction(bmodule_get_function(
        obj_get_bmodule(ex0),
        exp_get_sym(tp_e2(v))
      ));
    }
    if (exp_is_dic(ex0)) {
      char *key = symix_get(exp_get_sym(tp_e2(v)));
      return md_dic_fget(exp_get_dic(ex0), key);
    }
    EXC_KUT(fail_type("module or dictionary", ex0));
  }
  if (exp_is_sq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_sq(exp);
    Exp *ct = solver_solve(is, h0, hs, tp_e1(v));
    Exp *ix_key = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_string(ct))
      return md_str_at(exp_get_string(ct), exp_get_int(ix_key));
    if (exp_is_array(ct))
      return arr_get(exp_get_array(ct), exp_get_int(ix_key));
    if (exp_is_dic(ct))
      return md_dic_fget(exp_get_dic(ct), exp_get_string(ix_key));
    EXC_KUT(fail_type("string, array or dictionary", ct));
  }
  if (exp_is_slice(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_slice(exp);
    Exp *ct = solver_solve(is, h0, hs, tp3_e1(v));
    int64_t start = exp_is_empty(tp3_e2(v))
      ? 0
      : exp_get_int(solver_solve(is, h0, hs, tp3_e2(v)))
    ;
    if (exp_is_string(ct)) {
      char *s = exp_get_string(ct);
      int64_t end = exp_is_empty(tp3_e3(v))
        ? strlen(s)
        : exp_get_int(solver_solve(is, h0, hs, tp3_e3(v)))
      ;
      return exp_string(str_sub(s, start, end));
    }
    if (exp_is_array(ct)) {
      // <Exp>
      Arr *a = exp_get_array(ct);
      int64_t end = exp_is_empty(tp3_e3(v))
        ? arr_size(a)
        : exp_get_int(solver_solve(is, h0, hs, tp3_e3(v)))
      ;
      if (start < 0) start = arr_size(a) + start;
      if (end < 0) end = arr_size(a) + end;
      return exp_array(arr_drop(arr_take(a, end), start));
    }
    EXC_KUT(fail_type("string or array", ct));
  }
  if (exp_is_pr(exp)) {
    // <Exp, Arr<Exp>>
    Tp *v = exp_get_pr(exp);
    Exp *ex0 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *pars = solver_solve(is, h0, hs, exp_array(tp_e2(v)));
    if (obj_is_bfunction(ex0)) {
      Exp *r = obj_get_bfunction(ex0)(exp_get_array(pars));
      if (exp_is_empty(r))
        EXC_KUT(str_f(
          "%s\n<bfunction> does not return any value", exp_to_str(exp)
        ));
      return r;
    }
    if (exp_is_function(ex0)) {
      Exp *r = function_run(exp_get_function(ex0), exp_get_array(pars));
      if (exp_is_empty(r))
        EXC_KUT(str_f(
          "%s\n<function> does not return any value", exp_to_str(exp)
        ));
      return r;
    }
    if (exp_is_tfunction(ex0)) {
      Exp *r = tfunction_run(exp_get_tfunction(ex0), exp_get_array(pars));
      if (exp_is_empty(r))
        EXC_KUT(str_f(
          "%s\n<function> does not return any value", exp_to_str(exp)
        ));
      return r;
    }
    EXC_KUT(fail_type("function or <bfunction>", ex0));
  }
  if (exp_is_switch(exp)) {
    // <Exp, Arr<Tp<Arr<Exp>, Exp>>>
    Tp *v = exp_get_switch(exp);
    Exp *cond = solver_solve(is, h0, hs, tp_e1(v));

    // e is Tp<Arr<Exp>, Exp>
    EACH(tp_e2(v), Tp, e) {
      // Exp
      Arr *exps = tp_e1(e);
      int ok = FALSE;
      EACH(exps, Exp, exp) {
        if (exp_is_sym(exp) && exp_get_sym(exp) == symix_DEFAULT)
          return solver_solve(is, h0, hs, tp_e2(e));

        Exp *cond2 = solver_solve(is, h0, hs, exp);
        if (exp_get_bool(solver_solve_isolate(exp_eq(cond, cond2)))) {
          ok = TRUE;
          break;
        }

      }_EACH

      if (ok)
        return solver_solve(is, h0, hs, tp_e2(e));
    }_EACH
    EXC_KUT(str_f("switch did not catch '%s'", exp_to_js(cond)));
  }
  if (exp_is_not(exp)) {
    Exp *v = solver_solve(is, h0, hs, exp_get_not(exp));
    return exp_bool(!exp_get_as_bool(v));
  }
  if (exp_is_minus(exp)) {
    Exp *v = solver_solve(is, h0, hs, exp_get_minus(exp));
    if (exp_is_int(v)) return exp_int(- exp_get_int(v));
    if (exp_is_float(v)) return exp_float(- exp_get_float(v));
    EXC_KUT(fail_type("int or float", v));
  }
  if (exp_is_untype(exp)) {
    Exp *v = solver_solve(is, h0, hs, exp_get_untype(exp));
    if (obj_is_typed(v))
      return tfunction_untype(
        obj_get_typed_type(v), obj_get_typed_value(v)
      );
    EXC_KUT(fail_type("object<typed>", v));
  }
  if (exp_is_add(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_add(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_string(e1)) return exp_string(str_f(
      "%s%s", exp_get_string(e1), exp_get_string(e2)
    ));
    if (exp_is_int(e1)) return exp_int(exp_get_int(e1) + exp_get_int(e2));
    if (exp_is_float(e1)) return exp_float(
      exp_get_float(e1) + exp_get_float(e2)
    );
    if (exp_is_array(e1)) {
      // <Exp>
      Arr *a = arr_copy(exp_get_array(e1));
      arr_cat(a, exp_get_array(e2));
      return exp_array(a);
    }
    EXC_KUT(fail_type("int, float, string, array or vec", e1));
  }
  if (exp_is_sub(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_sub(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) return exp_int(exp_get_int(e1) - exp_get_int(e2));
    if (exp_is_float(e1)) return exp_float(
      exp_get_float(e1) - exp_get_float(e2)
    );
    EXC_KUT(fail_type("int, float or vec", e1));
  }
  if (exp_is_mul(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_mul(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) return exp_int(exp_get_int(e1) * exp_get_int(e2));
    if (exp_is_float(e1)) return exp_float(
      exp_get_float(e1) * exp_get_float(e2)
    );
    EXC_KUT(fail_type("int, float or vec", e1));
  }
  if (exp_is_div(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_div(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) {
      int64_t dv = exp_get_int(e2);
      if (!dv)
        EXC_KUT("Integer division by 0");
      return exp_int(exp_get_int(e1) / dv);
    }
    if (exp_is_float(e1)) {
      double dv = exp_get_float(e2);
      if (dv == 0)
        EXC_KUT("Floating division by 0");
      return exp_float(exp_get_float(e1) / dv);
    }
    EXC_KUT(fail_type("int, float or vec", e1));
  }
  if (exp_is_mod(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_mod(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) {
      int64_t dv = exp_get_int(e2);
      if (!dv)
        EXC_KUT("Integer division by 0");
      return exp_int(exp_get_int(e1) % dv);
    }
    EXC_KUT(fail_type("int or vec", e1));
  }
  if (exp_is_and(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_and(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    int b1 = exp_get_bool(e1);
    if (b1)
      return exp_bool(exp_get_bool(solver_solve(is, h0, hs, tp_e2(v))));
    return exp_bool(FALSE);
  }
  if (exp_is_or(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_or(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    int b1 = exp_get_bool(e1);
    if (b1) return exp_bool(TRUE);
    return exp_bool(exp_get_bool(solver_solve(is, h0, hs, tp_e2(v))));
  }

    //--
    // v is Tp<Exp, Exp>
    Exp *is_eq(Tp *v) {
      Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
      Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
      if (exp_is_string(e1)) {
        if (exp_is_string(e2))
          return exp_bool(!strcmp(exp_get_string(e1), exp_get_string(e2)));
        return exp_bool(FALSE);
      }
      if (exp_is_int(e1)) {
        if (exp_is_int(e2))
          return exp_bool(exp_get_int(e1) == exp_get_int(e2));
        return exp_bool(FALSE);
      }
      if (exp_is_float(e1)) {
        if (exp_is_float(e2))
          return exp_bool(math_eq(exp_get_float(e1), exp_get_float(e2)));
        return exp_bool(FALSE);
      }
      if (exp_is_array(e1)) {
        if (!exp_is_array(e2)) return exp_bool(FALSE);
        // <Exp>
        Arr *a1 = exp_get_array(e1);
        // <Exp>
        Arr *a2 = exp_get_array(e2);

        if (arr_size(a1) != arr_size(a2)) return exp_bool(FALSE);
        for (int i = 0; i < arr_size(a1); ++i) {
          Exp *tmp = exp_eq(arr_get(a1, i), arr_get(a2, i));
          if (!exp_get_bool(solver_solve_isolate(tmp))) {
            return exp_bool(FALSE);
          }
        }
        return exp_bool(TRUE);
      }
      if (exp_is_bool(e1)) {
        if (exp_is_bool(e2)) {
          int b1 = exp_get_bool(e1) ? TRUE : FALSE;
          int b2 = exp_get_bool(e2) ? TRUE : FALSE;
          return exp_bool(b1 == b2);
        }
        return exp_bool(FALSE);
      }
      if (exp_is_dic(e1)) {
        if (!exp_is_dic(e2)) return exp_bool(FALSE);
        // <Exp>
        Map *m1 = exp_get_dic(e1);
        // <Exp>
        Map *m2 = exp_get_dic(e2);

        if (map_size(m1) != map_size(m2)) return exp_bool(FALSE);
        EACH(map_to_array(m1), Kv, kv) {
          char *key = kv_key(kv);
          Exp *v2 = opt_get(map_get(m2, key));
          if (!v2) return FALSE;
          Exp *tmp = exp_eq(kv_value(kv), v2);
          if (!exp_get_bool(solver_solve_isolate(tmp))) {
            return exp_bool(FALSE);
          }
        }_EACH
        return exp_bool(TRUE);
      }

      EXC_KUT(fail_type("bool, int, float, string, array or dic", e1));
      return NULL; // Unreachable
    }
    // v is Tp<Exp, Exp>
    Exp *is_greater(Tp *v) {
      // <Exp, Exp>
      Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
      Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
      if (exp_is_string(e1))
        return exp_bool(strcmp(exp_get_string(e1), exp_get_string(e2)) > 0);
      if (exp_is_int(e1))
        return exp_bool(exp_get_int(e1) > exp_get_int(e2));
      if (exp_is_float(e1))
        return exp_bool(exp_get_float(e1) > exp_get_float(e2));
      if (exp_is_bool(e1)) {
        return exp_bool(exp_get_bool(e1) && !exp_get_bool(e2));
      }

      EXC_KUT(fail_type("bool, int, float or string", e1));
      return NULL; // Unreachable
    }
  if (exp_is_eq(exp)) {
    return is_eq(exp_get_eq(exp));
  }
  if (exp_is_neq(exp)) {
    return exp_bool(!exp_get_bool(is_eq(exp_get_neq(exp))));
  }
  if (exp_is_greater(exp)) {
    return is_greater(exp_get_greater(exp));
  }
  if (exp_is_greater_eq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_greater_eq(exp);
    return exp_bool(
      exp_get_bool(is_greater(v)) ||
      exp_get_bool(is_eq(v))
    );
  }
  if (exp_is_less(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_less(exp);
    return exp_bool(
      !exp_get_bool(is_greater(v)) &&
      !exp_get_bool(is_eq(v))
    );
  }
  if (exp_is_less_eq(exp)) {
    return exp_bool(!exp_get_bool(is_greater(exp_get_less_eq(exp))));
  }
  if (exp_is_ternary(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_ternary(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp3_e1(v));
    if (exp_get_bool(e1)) return solver_solve(is, h0, hs, tp3_e2(v));
    else return solver_solve(is, h0, hs, tp3_e3(v));
  }

  EXC_KUT(str_f(
    "Unknown expression %s of type %s\n",
    exp_to_js(exp), exp_type_to_str(exp)
  ));
  return NULL; // Unreachable.
}

Exp *solver_solve_isolate(Exp *exp) {
  return solver_solve(imports_new(), heap0_new(), heaps_new(heap_new()), exp);
}
