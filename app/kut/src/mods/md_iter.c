// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_iter.h"
#include "exp.h"
#include "DEFS.h"
#include "obj.h"
#include "function.h"
#include "runner/fail.h"
#include "runner/runner.h"

// \<iter>, \*->b -> b
static Exp *all (Arr *exps) {
  CHECK_PARS ("iter.all", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      // <Exp>
      It *it = obj_rget_iter(arr_get(exps, 0));
      while (it_has_next(it)) {
        Exp *e = it_next(it);
        if (!exp_rget_as_bool(bf(arr_new_from(e, NULL))))
          return exp_bool(FALSE);
      }
      return exp_bool(TRUE);
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// \<iter>, \*->b -> b
static Exp *any (Arr *exps) {
  CHECK_PARS ("iter.any", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
        // <Exp>
        It *it = obj_rget_iter(arr_get(exps, 0));
        while (it_has_next(it)) {
          Exp *e = it_next(it);
          if (exp_rget_as_bool(bf(arr_new_from(e, NULL))))
            return exp_bool(TRUE);
        }
        return exp_bool(FALSE);
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// \<iter>, <iter> -> <iter>
static Exp *cat (Arr *exps) {
  CHECK_PARS ("iter.cat", 2, exps);
  // <Exp>
  It *it1 = obj_rget_iter(arr_get(exps, 0));
  // <Exp>
  It *it2 = obj_rget_iter(arr_get(exps, 1));
  return obj_iter(it_cat(it1, it2));
}

// \<iter> -> i
static Exp *count (Arr *exps) {
  CHECK_PARS ("iter.count", 1, exps);
  // <Exp>
  It *it = obj_rget_iter(arr_get(exps, 0));
  return exp_int(it_count(it));
}

//  \<iter>, i -> <iter>
static Exp *drop (Arr *exps) {
  CHECK_PARS ("iter.drop", 2, exps);
  // <Exp>
  It *it = obj_rget_iter(arr_get(exps, 0));
  return obj_iter(it_drop(it, exp_rget_int(arr_get(exps, 1))));
}

//  (\<iter>, \*->b) -> <iter>
static Exp *drop_while (Arr *exps) {
  CHECK_PARS ("iter.dropWhile", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      // <Exp>
      It *it = obj_rget_iter(arr_get(exps, 0));
        //--
        int fn2 (Exp *e) { return exp_rget_bool(bf(arr_new_from(e, NULL)));
        }
      return obj_iter(it_dropf(it, (FPRED)fn2));
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// (\<iter>, \*->()) -> ()
static Exp *each (Arr *exps) {
  CHECK_PARS ("iter.each", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      It *it = obj_rget_iter(arr_get(exps, 0));
        //--
        void fn2 (Exp *e) { bf(arr_new_from(e, NULL)); }
      it_each(it, (FPROC)fn2);
      return exp_empty();
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// (\<iter>, \*,i->()) -> ()
static Exp *each_ix (Arr *exps) {
  CHECK_PARS ("iter.eachIx", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      It *it = obj_rget_iter(arr_get(exps, 0));
        //--
        void fn2 (Exp *e, int ix) { bf(arr_new_from(e, exp_int(ix), NULL)); }
      it_each_ix(it, (void (*)(void *, int ix))fn2);
      return exp_empty();
    }
  return runner_fn(arr_get(exps, 1), fn);
}


// \-> <iter>
static Exp *empty (Arr *exps) {
  CHECK_PARS ("iter.empty", 0, exps);
  return obj_iter(it_empty());
}

  //--
  typedef struct {
    // <Exp>
    It *it;
    Bfunction fn;
  } it_filter_O;
  static Opt *filter_next (it_filter_O *o) {
    for(;;) {
      if (!it_has_next(o->it)) return opt_none();
      void *next = it_next(o->it);
      int ok = exp_rget_bool(o->fn(arr_new_from(next, NULL)));
      if (ok) return opt_some(next);
    }
  }
// (\<iter>, \*->b) -> <iter>
static Exp *filter (Arr *exps) {
  CHECK_PARS ("iter.filter", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      // <Exp>
      It *it = obj_rget_iter(arr_get(exps, 0));
      it_filter_O *o = MALLOC(it_filter_O);
      o->it = it;
      o->fn = bf;
      return obj_iter(it_new(o, (Opt *(*)(void *))filter_next));
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// (\<iter>, \*->b) -> ([*] | [])
static Exp *find (Arr *exps) {
  CHECK_PARS ("iter.find", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      It *it = obj_rget_iter(arr_get(exps, 0));
        //--
        int fn2 (Exp *e) { return exp_rget_bool(bf(arr_new_from(e, NULL))); }
      Exp *r = opt_get(it_find(it, (FPRED)fn2));
      if (r) return exp_array(arr_new_from(r, NULL));
      else return exp_array(arr_new());
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// (\<iter>, \*->b) -> i
static Exp *findex (Arr *exps) {
  CHECK_PARS ("iter.index", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      It *it = obj_rget_iter(arr_get(exps, 0));
        //--
        int fn2 (Exp *e) { return exp_rget_bool(bf(arr_new_from(e, NULL))); }
      return exp_int(it_index(it, (FPRED)fn2));
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// \<iter> -> b
static Exp *has_next (Arr *exps) {
  CHECK_PARS ("iter.hasNext", 1, exps);
  // <Exp>
  It *it = obj_rget_iter(arr_get(exps, 0));
  return exp_bool(it_has_next(it));
}

  //--
  typedef struct {
    // <Exp>
    It *it;
    Bfunction fn;
  } it_map_O;
  static Opt *map_next (it_map_O *o) {
    for(;;) {
      if (!it_has_next(o->it)) return opt_none();
      void *next = it_next(o->it);
      return opt_some(o->fn(arr_new_from(next, NULL)));
    }
  }
// (\<iter>, \*->*) -> <iter>
static Exp *map (Arr *exps) {
  CHECK_PARS ("iter.map", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      // <Exp>
      It *it = obj_rget_iter(arr_get(exps, 0));
      it_map_O *o = MALLOC(it_map_O);
      o->it = it;
      o->fn = bf;
      return obj_iter(it_new(o, (Opt *(*)(void *))map_next));
    }
  return runner_fn(arr_get(exps, 1), fn);
}

  //--
  // <Exp>, o is Tp<Function, Function>
  Opt *fn_new (Tp *o) {
    if (exp_rget_as_bool(function_run(tp_e1(o), arr_new()))) {
      return opt_some(function_run(tp_e2(o), arr_new()));
    }
    return opt_none();
  }
// (\\->b, \->*) -> <iter>
static Exp *new (Arr *exps) {
  CHECK_PARS ("iter.new", 2, exps);
  Function *fn_has = exp_rget_function(arr_get(exps, 0));
  Function *fn_next = exp_rget_function(arr_get(exps, 1));
  return obj_iter(it_new(tp_new(fn_has, fn_next), (Opt *(*)(void *))fn_new));
}

// \<iter> -> *
static Exp *next (Arr *exps) {
  CHECK_PARS ("iter.next", 1, exps);
  return it_next(obj_rget_iter(arr_get(exps, 0)));
}

// (\<iter>, (*1), \(*1), (*2) -> (*1)) -> (*1)
static Exp *reduce (Arr *exps) {
  CHECK_PARS ("iter.reduce", 3, exps);
    //--
    Exp *fn (Bfunction bf) {
      It *it = obj_rget_iter(arr_get(exps, 0));
      Exp *seed = arr_get(exps, 1);
      while (it_has_next(it))
        seed = bf(arr_new_from(seed, it_next(it), NULL));
      return seed;
    }
  return runner_fn(arr_get(exps, 2), fn);
}

//  \<iter>, i -> <iter>
static Exp *take (Arr *exps) {
  CHECK_PARS ("iter.take", 2, exps);
  // <Exp>
  It *it = obj_rget_iter(arr_get(exps, 0));
  return obj_iter(it_take(it, exp_rget_int(arr_get(exps, 1))));
}

  //--
  typedef struct {
    // <Exp>
    It *it;
    Bfunction fn;
  } it_take_O;
  static Opt *take_next (it_take_O *o) {
    if (!it_has_next(o->it)) return opt_none();
    void *next = it_next(o->it);
    int ok = exp_rget_bool(o->fn(arr_new_from(next, NULL)));
    return ok ? opt_some(next) : opt_none();
  }
// (\<iter>, \*->b) -> <iter>
static Exp *take_while (Arr *exps) {
  CHECK_PARS ("iter.takeWhile", 2, exps);
    //--
    Exp *fn (Bfunction bf) {
      // <Exp>
      It *it = obj_rget_iter(arr_get(exps, 0));
      it_take_O *o = MALLOC(it_take_O);
      o->it = it;
      o->fn = bf;
      return obj_iter(it_new(o, (Opt *(*)(void *))take_next));
    }
  return runner_fn(arr_get(exps, 1), fn);
}

// \<iter> -> s
static Exp *to_str (Arr *exps) {
  CHECK_PARS ("iter.toStr", 1, exps);
  // <Exp>
  It *it = obj_rget_iter(arr_get(exps, 0));
  return exp_string(str_f(
    "<iter>%s",
    exp_to_str(exp_array(arr_from_it(it)))
  ));
}

// \* -> <iter>
static Exp *unary (Arr *exps) {
  CHECK_PARS ("iter.unary", 1, exps);
  return obj_iter(it_unary(arr_get(exps, 0)));
}

Bfunction md_iter_get (char *fname) {
  if (!strcmp(fname, "all")) return all;
  if (!strcmp(fname, "any")) return any;
  if (!strcmp(fname, "cat")) return cat;
  if (!strcmp(fname, "count")) return count;
  if (!strcmp(fname, "drop")) return drop;
  if (!strcmp(fname, "dropWhile")) return drop_while;
  if (!strcmp(fname, "each")) return each;
  if (!strcmp(fname, "eachIx")) return each_ix;
  if (!strcmp(fname, "empty")) return empty;
  if (!strcmp(fname, "filter")) return filter;
  if (!strcmp(fname, "find")) return find;
  if (!strcmp(fname, "hasNext")) return has_next;
  if (!strcmp(fname, "index")) return findex;
  if (!strcmp(fname, "map")) return map;
  if (!strcmp(fname, "new")) return new;
  if (!strcmp(fname, "next")) return next;
  if (!strcmp(fname, "reduce")) return reduce;
  if (!strcmp(fname, "take")) return take;
  if (!strcmp(fname, "takeWhile")) return take_while;
  if (!strcmp(fname, "toStr")) return to_str;
  if (!strcmp(fname, "unary")) return unary;
  EXC_KUT(fail_bfunction("iter", fname));
  return NULL; // Unreachable
}
