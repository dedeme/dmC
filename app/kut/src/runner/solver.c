// Copyright 22-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "runner/solver.h"
#include "kut/DEFS.h"
#include "kut/dec.h"
#include "DEFS.h"
#include "fileix.h"
#include "function.h"
#include "heap.h"
#include "modules.h"
#include "bmodule.h"
#include "obj.h"
#include "runner/fail.h"
#include "runner/solver.h"
#include "reader/reader.h"
#include "mods/md_str.h"
#include "mods/md_arr.h"
#include "mods/md_dic.h"

struct range_o {
  int64_t start;
  int64_t end;
  int64_t step;
};

static struct range_o *new_range_o(int64_t start, int64_t end, int64_t step) {
  struct range_o *this = MALLOC(struct range_o);
  this->start = start;
  this->end = end;
  this->step = step;
  return this;
}

// -----------------------------------------------------------------------------

// stk is Arr<StatCode>, is is Map<int>, hs is Arr<Heap>
Exp *solver_solve(Map *is, Heap0 *h0, Arr *hs, Exp *exp) {
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
      Exp *fsolve (Exp *exp) { return solver_solve(is, h0, hs, exp); }
    return exp_array(arr_map(exp_get_array(exp), (FMAP)fsolve));
  }
  if (exp_is_map(exp)) {
      //--
      Kv *fsolve (Kv *v) {
        return kv_new(kv_key(v), solver_solve(is, h0, hs, kv_value(v)));
      }
    return exp_map(map_from_array(
      arr_map(map_to_array(exp_get_map(exp)), (FMAP)fsolve)
    ));
  }
  if (exp_is_function(exp)) {
    return exp_function(function_set_context(
      exp_get_function(exp), is, h0, hs
    ));
  }
  if (exp_is_sym(exp)) {
    char *sym = exp_get_sym(exp);
    Exp *r = opt_get(heap_get_exp(hs, sym));

    // IN HEAP

    if (r) return r;

    Heap0Entry *e = opt_get(map_get(heap0_get(h0), sym));

    // IN HEAP0

    if (e) {
      r = solver_solve(is, h0, hs, heap0_entry_exp(e));
      map_put(arr_peek(hs), sym, r);
      return r;
    }

    int *md_ix = opt_get(map_get(is, sym));

    // IN IMPORTS

    if (md_ix) {
      int fix = *md_ix;
      Module *md = opt_get(modules_get_ok(fix));
      if (md) return obj_module(md);

      char *kut_code = fileix_read(fix);
      md = reader_read_main_block(cdr_new(fix, kut_code));
      modules_set(fix, md);
      return obj_module(md);
    }

    // IN BUILT-IN FUNCTIONS

    if (bmodule_exists(sym)) return obj_bmodule(sym);

    // NOT FOUND

    EXC_KUT(str_f("Symbol '%s' not found.", sym));
  }
  if (exp_is_range(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_range(exp);
    int64_t start = exp_rget_int(solver_solve(is, h0, hs, tp3_e1(v)));
    int64_t end = exp_rget_int(solver_solve(is, h0, hs, tp3_e2(v)));
    if (exp_is_empty(tp3_e3(v))) {
        //--
        // <Exp>
        Opt *fnext (void *o) {
          struct range_o *ro = (struct range_o *)o;
          if (ro->start >= ro->end) return opt_none();
          Exp *r = exp_int(ro->start);
          ++(ro->start);
          return opt_some(r);
        }
      return obj_iter(it_new(new_range_o(start, end, 1), fnext));
    }
    int64_t step = exp_rget_int(solver_solve(is, h0, hs, tp3_e3(v)));
    if (!step) EXC_KUT("Range can not have step 0");
    if (step > 0) {
        //--
        // <Exp>
        Opt *fnext (void *o) {
          struct range_o *ro = (struct range_o *)o;
          if (ro->start > ro->end) return opt_none();
          Exp *r = exp_int(ro->start);
          ro->start += ro->step;
          return opt_some(r);
        }
      return obj_iter(it_new(new_range_o(start, end, step), fnext));
    }
      //--
      // <Exp>
      Opt *fnext (void *o) {
        struct range_o *ro = (struct range_o *)o;
        if (ro->start < ro->end) return opt_none();
        Exp *r = exp_int(ro->start);
        ro->start += ro->step;
        return opt_some(r);
      }
    return obj_iter(it_new(new_range_o(start, end, step), fnext));
  }
  if (exp_is_pt(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_pt(exp);
    Exp *ex0 = solver_solve(is, h0, hs, tp_e1(v));
    if (obj_is_module(ex0)) {
      Module *md = obj_get_module(ex0);
      exp_rget_sym(tp_e2(v)); // test fi tp_e2(v) is symbol.
      return solver_solve(
        module_get_imports(md),
        module_get_heap0(md),
        arr_new_from(module_get_heap(md), NULL),
        tp_e2(v)
      );
    }
    if (obj_is_bmodule(ex0)) {
      return obj_bfunction(bmodule_get_function(
        obj_get_bmodule(ex0),
        exp_rget_sym(tp_e2(v))
      ));
    }
    if (exp_is_map(ex0)) {
      char *key = exp_rget_sym(tp_e2(v));
      return md_dic_fget(exp_rget_map(ex0), key);
    }
    EXC_KUT(fail_type("module or dictionary", ex0));
  }
  if (exp_is_sq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_sq(exp);
    Exp *ct = solver_solve(is, h0, hs, tp_e1(v));
    Exp *ix_key = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_string(ct))
      return md_str_at(exp_rget_string(ct), exp_rget_int(ix_key));
    if (exp_is_array(ct))
      return arr_get(exp_rget_array(ct), exp_rget_int(ix_key));
    if (exp_is_map(ct))
      return md_dic_fget(exp_rget_map(ct), exp_rget_string(ix_key));
    EXC_KUT(fail_type("string, array or dictionary", ct));
  }
  if (exp_is_slice(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_slice(exp);
    Exp *ct = solver_solve(is, h0, hs, tp3_e1(v));
    int64_t start = exp_is_empty(tp3_e2(v))
      ? 0
      : exp_rget_int(solver_solve(is, h0, hs, tp3_e2(v)))
    ;
    if (exp_is_string(ct)) {
      char *s = exp_rget_string(ct);
      int64_t end = exp_is_empty(tp3_e3(v))
        ? strlen(s)
        : exp_rget_int(solver_solve(is, h0, hs, tp3_e3(v)))
      ;
      return exp_string(str_sub(s, start, end));
    }
    if (exp_is_array(ct)) {
      // <Exp>
      Arr *a = exp_rget_array(ct);
      int64_t end = exp_is_empty(tp3_e3(v))
        ? arr_size(a)
        : exp_rget_int(solver_solve(is, h0, hs, tp3_e3(v)))
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
      Exp *r = obj_get_bfunction(ex0)(exp_rget_array(pars));
      if (exp_is_empty(r))
        EXC_KUT(str_f(
          "%s\n<bfunction> does not return any value", exp_to_str(exp)
        ));
      return r;
    }
    if (exp_is_function(ex0)) {
      Exp *r = function_run(exp_get_function(ex0), exp_rget_array(pars));
      if (exp_is_empty(r))
        EXC_KUT(str_f(
          "%s\n<function> does not return any value", exp_to_str(exp)
        ));
      return r;
    }
    EXC_KUT(fail_type("function or <bfunction>", ex0));
  }
  if (exp_is_switch(exp)) {
    // <Exp, Arr<Tp<Exp, Exp>>>
    Tp *v = exp_get_switch(exp);
    Exp *cond = solver_solve(is, h0, hs, tp_e1(v));
    // e is Tp<Exp, Exp>
    EACH(tp_e2(v), Tp, e) {
      Exp *exp1 = tp_e1(e);
      Exp *exp2 = solver_solve(is, h0, hs, tp_e2(e));
      if (exp_is_sym(exp1) && !strcmp(exp_rget_sym(exp1), "default"))
        return exp2;

      Exp *cond2 = solver_solve(is, h0, hs, exp1);
      if (exp_rget_bool(solver_solve_isolate(exp_eq(cond, cond2))))
        return exp2;
    }_EACH
    EXC_KUT(str_f("switch did not catch '%s'", exp_to_js(cond)));
  }
  if (exp_is_not(exp)) {
    Exp *v = solver_solve(is, h0, hs, exp_get_not(exp));
    if (exp_is_bool(v)) return exp_bool(!exp_rget_bool(v));
    if (exp_is_string(v)) return exp_bool(!*exp_rget_string(v));
    if (exp_is_array(v)) return exp_bool(!arr_size(exp_rget_array(v)));
    EXC_KUT(fail_type("bool, string or array", v));
  }
  if (exp_is_minus(exp)) {
    Exp *v = solver_solve(is, h0, hs, exp_get_minus(exp));
    if (exp_is_int(v)) return exp_int(- exp_rget_int(v));
    if (exp_is_float(v)) return exp_float(- exp_rget_float(v));
    EXC_KUT(fail_type("int or float", v));
  }
  if (exp_is_add(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_add(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_string(e1)) return exp_string(str_f(
      "%s%s", exp_rget_string(e1), exp_rget_string(e2)
    ));
    if (exp_is_int(e1)) return exp_int(exp_rget_int(e1) + exp_rget_int(e2));
    if (exp_is_float(e1)) return exp_float(
      exp_rget_float(e1) + exp_rget_float(e2)
    );
    if (exp_is_array(e1)) {
      // <Exp>
      Arr *a = arr_copy(exp_rget_array(e1));
      arr_cat(a, exp_rget_array(e2));
      return exp_array(a);
    }
    EXC_KUT(fail_type("int, float, string or array", e1));
  }
  if (exp_is_sub(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_sub(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) return exp_int(exp_rget_int(e1) - exp_rget_int(e2));
    if (exp_is_float(e1)) return exp_float(
      exp_rget_float(e1) - exp_rget_float(e2)
    );
    EXC_KUT(fail_type("int or float", e1));
  }
  if (exp_is_mul(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_mul(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) return exp_int(exp_rget_int(e1) * exp_rget_int(e2));
    if (exp_is_float(e1)) return exp_float(
      exp_rget_float(e1) * exp_rget_float(e2)
    );
    EXC_KUT(fail_type("int or float", e1));
  }
  if (exp_is_div(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_div(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    if (exp_is_int(e1)) {
      int64_t dv = exp_rget_int(e2);
      if (!dv)
        EXC_KUT("Integer division by 0");
      return exp_int(exp_rget_int(e1) / dv);
    }
    if (exp_is_float(e1)) {
      double dv = exp_rget_float(e2);
      if (dv == 0)
        EXC_KUT("Floating division by 0");
      return exp_float(exp_rget_float(e1) / dv);
    }
    EXC_KUT(fail_type("int or float", e1));
  }
  if (exp_is_mod(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_mod(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
    int64_t dv = exp_rget_int(e2);
    if (!dv)
      EXC_KUT("Integer division by 0");

    return exp_int(exp_rget_int(e1) % dv);
  }
  if (exp_is_and(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_and(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    if (exp_is_bool(e1)) {
      return exp_bool(
        exp_rget_bool(e1) &&
        exp_rget_bool(solver_solve(is, h0, hs, tp_e2(v)))
      );
    }
    if (exp_is_string(e1))
      return exp_bool(
        *exp_rget_string(e1) &&
        *exp_rget_string(solver_solve(is, h0, hs, tp_e2(v)))
      );
    if (exp_is_array(e1))
      return exp_bool(
        arr_size(exp_rget_array(e1)) &&
        arr_size(exp_rget_array(solver_solve(is, h0, hs, tp_e2(v))))
      );
    EXC_KUT(fail_type("bool, string or array", e1));
  }
  if (exp_is_or(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_or(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
    if (exp_is_bool(e1))
      return exp_bool(
        exp_rget_bool(e1) ||
        exp_rget_bool(solver_solve(is, h0, hs, tp_e2(v)))
      );
    if (exp_is_string(e1))
      return exp_bool(
        *exp_rget_string(e1) ||
        *exp_rget_string(solver_solve(is, h0, hs, tp_e2(v)))
      );
    if (exp_is_array(e1))
      return exp_bool(
        arr_size(exp_rget_array(e1)) ||
        arr_size(exp_rget_array(solver_solve(is, h0, hs, tp_e2(v))))
      );
    EXC_KUT(fail_type("bool, string or array", e1));
  }

    //--
    // v is Tp<Exp, Exp>
    Exp *is_eq(Tp *v) {
      Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
      Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
      if (exp_is_string(e1)) {
        if (exp_is_string(e2))
          return exp_bool(!strcmp(exp_rget_string(e1), exp_rget_string(e2)));
        else return exp_bool(FALSE);
      }
      if (exp_is_int(e1)) {
        if (exp_is_int(e2))
          return exp_bool(exp_rget_int(e1) == exp_rget_int(e2));
        else return exp_bool(FALSE);
      }
      if (exp_is_float(e1)) {
        if (exp_is_float(e2))
          return exp_bool(dec_eq(exp_rget_float(e1), exp_rget_float(e2)));
        else return exp_bool(FALSE);
      }
      if (exp_is_array(e1)) {
        if (exp_is_array(e2)) {
          // <Exp>
          Arr *a1 = exp_rget_array(e1);
          // <Exp>
          Arr *a2 = exp_rget_array(e2);

          if (arr_size(a1) != arr_size(a2)) return exp_bool(FALSE);
          for (int i = 0; i < arr_size(a1); ++i) {
            Exp *tmp = exp_eq(arr_get(a1, i), arr_get(a2, i));
            if (!exp_rget_bool(solver_solve_isolate(tmp))) {
              return exp_bool(FALSE);
            }
          }
          return exp_bool(TRUE);
        }
        else return exp_bool(FALSE);
      }
      if (exp_is_bool(e1)) {
        if (exp_is_bool(e2)) {
          int b1 = exp_rget_bool(e1) ? TRUE : FALSE;
          int b2 = exp_rget_bool(e2) ? TRUE : FALSE;
          return exp_bool(b1 == b2);
        }
        else return exp_bool(FALSE);
      }
      if (exp_is_map(e1)) {
        if (exp_is_map(e2)) {
          // <Exp>
          Map *m1 = exp_rget_map(e1);
          // <Exp>
          Map *m2 = exp_rget_map(e2);

          if (map_size(m1) != map_size(m2)) return exp_bool(FALSE);
          EACH(map_to_array(m1), Kv, kv) {
            char *key = kv_key(kv);
            Exp *v2 = opt_get(map_get(m2, key));
            if (!v2) return FALSE;
            Exp *tmp = exp_eq(kv_value(kv), v2);
            if (!exp_rget_bool(solver_solve_isolate(tmp))) {
              return exp_bool(FALSE);
            }
          }_EACH
          return exp_bool(TRUE);
        }
        else return exp_bool(FALSE);
      }

      EXC_KUT(fail_type("bool, int, float, string or array", e1));
      return NULL; // Unreachable
    }
    // v is Tp<Exp, Exp>
    Exp *is_greater(Tp *v) {
      // <Exp, Exp>
      Exp *e1 = solver_solve(is, h0, hs, tp_e1(v));
      Exp *e2 = solver_solve(is, h0, hs, tp_e2(v));
      if (exp_is_string(e1))
        return exp_bool(strcmp(exp_rget_string(e1), exp_rget_string(e2)) > 0);
      if (exp_is_int(e1))
        return exp_bool(exp_rget_int(e1) > exp_rget_int(e2));
      if (exp_is_float(e1))
        return exp_bool(exp_rget_float(e1) > exp_rget_float(e2));
      if (exp_is_bool(e1)) {
        return exp_bool(exp_rget_bool(e1) && !exp_rget_bool(e2));
      }

      EXC_KUT(fail_type("bool, int, float or string", e1));
      return NULL; // Unreachable
    }
  if (exp_is_eq(exp)) {
    return is_eq(exp_get_eq(exp));
  }
  if (exp_is_neq(exp)) {
    return exp_bool(!exp_rget_bool(is_eq(exp_get_neq(exp))));
  }
  if (exp_is_greater(exp)) {
    return is_greater(exp_get_greater(exp));
  }
  if (exp_is_greater_eq(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_greater_eq(exp);
    return exp_bool(
      exp_rget_bool(is_greater(v)) ||
      exp_rget_bool(is_eq(v))
    );
  }
  if (exp_is_less(exp)) {
    // <Exp, Exp>
    Tp *v = exp_get_less(exp);
    return exp_bool(
      !exp_rget_bool(is_greater(v)) &&
      !exp_rget_bool(is_eq(v))
    );
  }
  if (exp_is_less_eq(exp)) {
    return exp_bool(!exp_rget_bool(is_greater(exp_get_less_eq(exp))));
  }
  if (exp_is_ternary(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *v = exp_get_ternary(exp);
    Exp *e1 = solver_solve(is, h0, hs, tp3_e1(v));
    int cond = FALSE;
    if (exp_is_bool(e1)) cond = exp_rget_bool(e1);
    else if (exp_is_string(e1)) cond = *exp_rget_string(e1);
    else if (exp_is_array(e1)) cond = arr_size(exp_rget_array(e1));
    else EXC_KUT(fail_type("bool, string or array", e1));

    if (cond) return solver_solve(is, h0, hs, tp3_e2(v));
    else return solver_solve(is, h0, hs, tp3_e3(v));
  }

  EXC_KUT(str_f(
    "Unknown expression %s of type %s\n",
    exp_to_js(exp), exp_type_to_str(exp)
  ));
  return NULL; // Unreachable.
}

Exp *solver_solve_isolate(Exp *exp) {
  // <Heap>
  Arr *hs = arr_new_from(heap_new(), NULL);
  return solver_solve(map_new(), heap0_new(), hs, exp);
}
