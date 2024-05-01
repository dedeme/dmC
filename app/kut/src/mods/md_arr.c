// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_arr.h"
#include "exp.h"
#include "function.h"
#include "obj.h"
#include "runner/fail.h"
#include "runner/solver.h"
#include "runner/runner.h"

// (\a, \*->b) -> b
static Exp *all (Arr *exps) {
  CHECK_PARS ("arr.all", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  return exp_bool(arr_all(a, (FPRED)fn2));
}

// (\a, \*->b) -> b
static Exp *any (Arr *exps) {
  CHECK_PARS ("arr.any", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  return exp_bool(arr_any(a, (FPRED)fn2));
}

// \a, a -> ()
static Exp *cat (Arr *exps) {
  CHECK_PARS ("arr.cat", 2, exps);
  arr_cat(exp_rget_array(arr_get(exps, 0)), exp_rget_array(arr_get(exps, 1)));
  return exp_empty();
}

// \a -> ()
static Exp *clear (Arr *exps) {
  CHECK_PARS ("arr.clear", 1, exps);
  arr_clear(exp_rget_array(arr_get(exps, 0)));
  return exp_empty();
}

// \a -> a
static Exp *copy (Arr *exps) {
  CHECK_PARS ("arr.copy", 1, exps);
  return exp_array(arr_copy(exp_rget_array(arr_get(exps, 0))));
}

// \a, i -> a
static Exp *drop (Arr *exps) {
  CHECK_PARS ("arr.drop", 2, exps);
  return exp_array(arr_drop(
    exp_rget_array(arr_get(exps, 0)), exp_rget_int(arr_get(exps, 1))
  ));
}

// (\a, \*->b) -> a
static Exp *drop_while (Arr *exps) {
  CHECK_PARS ("arr.dropWhile", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  return exp_array(arr_dropf(a, (FPRED)fn2));
}

// (\a, \*, *->b) -> [a, a]
static Exp *duplicates (Arr *exps) {
  CHECK_PARS ("arr.duplicates", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e1, Exp *e2) {
      Arr *ps = arr_new_from(e1, e2,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  // <Exp>
  Arr *rest = arr_duplicates(a, (FEQ)fn2);
  return exp_array(arr_new_from(
    exp_array(a), exp_array(rest), NULL
  ));
}

// (\a, \*->()) -> ()
static Exp *each (Arr *exps) {
  CHECK_PARS ("arr.each", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    void fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      if (exp_is_function(fn)) function_run(exp_rget_function(fn), ps);
      else if (obj_is_bfunction(fn)) obj_rget_bfunction(fn)(ps);
      else EXC_KUT(fail_type("function", fn));
    }
  arr_each(a, (FPROC)fn2);
  return exp_empty();
}

// (\a, \*,i->()) -> ()
static Exp *each_ix (Arr *exps) {
  CHECK_PARS ("arr.eachIx", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    void fn2 (Exp *e, int ix) {
      Arr *ps = arr_new_from(e, exp_int(ix),  NULL);
      if (exp_is_function(fn)) function_run(exp_rget_function(fn), ps);
      else if (obj_is_bfunction(fn)) obj_rget_bfunction(fn)(ps);
      else EXC_KUT(fail_type("function", fn));
    }
  arr_each_ix(a, (void (*)(void *, int))fn2);
  return exp_empty();
}

// (\a, \*->b) -> a
static Exp *filter (Arr *exps) {
  CHECK_PARS ("arr.filter", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  return exp_array(arr_filter_to(a, (FPRED)fn2));
}

// (\a, \*->b) -> ()
static Exp *filter_in (Arr *exps) {
  CHECK_PARS ("arr.filterIn", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  arr_filter_in(a, (FPRED)fn2);
  return exp_empty();
}

// (\a, \*->b) -> ([] | [*])
static Exp *find (Arr *exps) {
  CHECK_PARS ("arr.find", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  Exp *e = opt_get(arr_find(a, (FPRED)fn2));
  if (e) return exp_array(arr_new_from(e, NULL));
  return exp_array(arr_new());
}

// \<iter> -> a
static Exp *from_iter (Arr *exps) {
  CHECK_PARS ("arr.fromIter", 1, exps);
  // <Exp>
  It *it = obj_rget_iter(arr_get(exps, 0));
  return exp_array(arr_from_it(it));
}

// (\a, \*->b) -> i
static Exp *findex (Arr *exps) {
  CHECK_PARS ("arr.index", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  return exp_int(arr_index(a, (FPRED)fn2));
}

// \a, i, * -> ()
static Exp *insert (Arr *exps) {
  CHECK_PARS ("arr.insert", 3, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  Exp *e = arr_get(exps, 2);
  arr_insert(a, ix, e);
  return exp_empty();
}

// \a, i, a -> ()
static Exp *insert_arr (Arr *exps) {
  CHECK_PARS ("arr.inserArr", 3, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  // <Exp>
  Arr *a2 = exp_rget_array(arr_get(exps, 2));
  arr_insert_arr(a, ix, a2);
  return exp_empty();
}

// \[s...], s -> s
static Exp *join (Arr *exps) {
  CHECK_PARS ("arr.join", 2, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  char *sep = exp_rget_string(arr_get(exps, 1));
    //--
    char *fn (Exp *e) { return exp_rget_string(e); }
  return exp_string(arr_join(arr_map(a, (FMAP)fn), sep));
}

// (\a, \*->*) -> a
static Exp *map (Arr *exps) {
  CHECK_PARS ("arr.map", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    Exp *fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return rs;
    }
  return exp_array(arr_map(a, (FMAP)fn2));
}

// \i, b | i | f | s -> a
static Exp *new (Arr *exps) {
  CHECK_PARS ("arr.new", 2, exps);
  int64_t n = exp_rget_int(arr_get(exps, 0));
  if (n < 0) n = 0;
  Exp *e = arr_get(exps, 1);
  // <Exp>
  Arr *r = arr_new_bf(n + 1);
  if (exp_is_bool(e)) {
    int v = exp_get_bool(e);
    for (int64_t i = 0; i < n ; ++i) arr_push(r, exp_bool(v));
  } else if (exp_is_int(e)) {
    int64_t v = exp_get_int(e);
    for (int64_t i = 0; i < n ; ++i) arr_push(r, exp_int(v));
  } else if (exp_is_float(e)) {
    double v = exp_get_float(e);
    for (int64_t i = 0; i < n ; ++i) arr_push(r, exp_float(v));
  } else if (exp_is_string(e)) {
    char *v = exp_get_string(e);
    for (int64_t i = 0; i < n ; ++i) arr_push(r, exp_string(v));
  } else {
    EXC_ILLEGAL_ARGUMENT(
      "Bad expression type",
      "bool, int, float or string",
      str_f("%s (%s)", exp_type_to_str(e), exp_to_str(e))
    );
  }
  return exp_array(r);
}

// \a -> *
static Exp *peek (Arr *exps) {
  CHECK_PARS ("arr.peek", 1, exps);
  return arr_peek(exp_rget_array(arr_get(exps, 0)));
}

// \a -> *
static Exp *pop (Arr *exps) {
  CHECK_PARS ("arr.pop", 1, exps);
  return arr_pop(exp_rget_array(arr_get(exps, 0)));
}

// \a, * -> ()
static Exp *push (Arr *exps) {
  CHECK_PARS ("arr.push", 2, exps);
  arr_push(exp_rget_array(arr_get(exps, 0)), arr_get(exps, 1));
  return exp_empty();
}

// \(a, *, \*,*->*) -> *
static Exp *reduce (Arr *exps) {
  CHECK_PARS ("arr.reduce", 3, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *e = arr_get(exps, 1);
  Exp *fn = arr_get(exps, 2);
    //--
    Exp *fn2 (Exp *e1, Exp *e2) {
      Arr *ps = arr_new_from(e1, e2,  NULL);
      FRUN(rs, fn, ps);
      return rs;
    }
  return arr_reduce(a, e, (void*(*)(void *, void *))fn2);
}

// \a, i -> *
static Exp *fremove (Arr *exps) {
  CHECK_PARS ("arr.remove", 2, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int64_t ix = exp_rget_int(arr_get(exps, 1));
  Exp *e = arr_get(a, ix);
  arr_remove(a, ix);
  return e;
}

// \a, i, i -> a
static Exp *remove_range (Arr *exps) {
  CHECK_PARS ("arr.remove_range", 3, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int64_t begin = exp_rget_int(arr_get(exps, 1));
  int64_t end = exp_rget_int(arr_get(exps, 2));
  // <Exp>
  Arr *r = arr_take(arr_drop(a, begin), end - begin);
  arr_remove_range(a, begin, end);
  return exp_array(r);
}

// \a -> a
static Exp *reverse (Arr *exps) {
  CHECK_PARS ("arr.reverse", 1, exps);
  // <Exp>
  Arr *a = arr_copy(exp_rget_array(arr_get(exps, 0)));
  arr_reverse(a);
  return exp_array(a);
}

// \a -> ()
static Exp *reverse_in (Arr *exps) {
  CHECK_PARS ("arr.reverseIn", 1, exps);
  arr_reverse(exp_rget_array(arr_get(exps, 0)));
  return exp_empty();
}

// \a -> *
static Exp *shift (Arr *exps) {
  CHECK_PARS ("arr.shift", 1, exps);
  // <Exp>
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *e = NULL;
  TRY {
    e = arr_get(a, 0);
  } CATCH (ex) {
    ex = ex;
  }_TRY
  if (!e) EXC_ILLEGAL_STATE("Array is empty");
  arr_remove(a, 0);
  return e;
}

// \a -> ()
static Exp *shuffle (Arr *exps) {
  CHECK_PARS ("arr.shuffle", 1, exps);
  arr_shuffle(exp_rget_array(arr_get(exps, 0)));
  return exp_empty();
}

// \a -> i
static Exp *size (Arr *exps) {
  CHECK_PARS ("arr.size", 1, exps);
  return exp_int(arr_size(exp_rget_array(arr_get(exps, 0))));
}

// (\a, \*,*->b) -> ()
static Exp *sort (Arr *exps) {
  CHECK_PARS ("arr.sort", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e1, Exp *e2) {
      // e2 and e1 are transposed because 'arr_sort' use 'greater' insted 'less'.
      Arr *ps = arr_new_from(e2, e1,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  arr_sort(a, (FEQ)fn2);
  return exp_empty();
}

// \a, i -> a
static Exp *take (Arr *exps) {
  CHECK_PARS ("arr.take", 2, exps);
  return exp_array(arr_take(
    exp_rget_array(arr_get(exps, 0)), exp_rget_int(arr_get(exps, 1))
  ));
}

// (\a, \*->b) -> a
static Exp *take_while (Arr *exps) {
  CHECK_PARS ("arr.takeWhile", 2, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  Exp *fn = arr_get(exps, 1);
    //--
    int fn2 (Exp *e) {
      Arr *ps = arr_new_from(e,  NULL);
      FRUN(rs, fn, ps);
      return exp_get_bool(rs);
    }
  return exp_array(arr_takef(a, (FPRED)fn2));
}

// \a -> <iter>
static Exp *to_iter (Arr *exps) {
  CHECK_PARS ("arr.toIter", 1, exps);
  return obj_iter(arr_to_it(exp_rget_array(arr_get(exps, 0))));
}

// \a, * -> ()
static Exp *unshift (Arr *exps) {
  CHECK_PARS ("arr.unshift", 2, exps);
  arr_insert(exp_rget_array(arr_get(exps, 0)), 0, arr_get(exps, 1));
  return exp_empty();
}

// \[[*.].] -> [[*.].]
static Exp *unzip (Arr *exps) {
  CHECK_PARS ("arr.unzip", 1, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));

  int nels = arr_size(a);
  if (nels == 0) return exp_array(a);

  // <Arr>
  Arr *r = arr_new();
  int nas = arr_size(exp_rget_array(arr_get(a, 0)));
  RANGE0(ias, nas) {
    arr_push(r, arr_new());
  }_RANGE

  RANGE0(iels, nels) {
    // <Exp>
    Arr *els = exp_rget_array(arr_get(a, iels));
    RANGE0(ias, nas) {
      Arr *col = arr_get(r, ias);
      arr_push(col, arr_get(els, ias));
    }_RANGE
  }_RANGE

  // <Exp>
  Arr *exp_r = arr_new();
  RANGE0(ias, nas) {
    arr_push(exp_r, exp_array(arr_get(r, ias)));
  }_RANGE

  return exp_array(exp_r);
}

// \[[*.].] -> [[*.].]
static Exp *zip (Arr *exps) {
  CHECK_PARS ("arr.zip", 1, exps);
  Arr *a = exp_rget_array(arr_get(exps, 0));
  int nas = arr_size(a);
  if (nas < 2) return exp_array(a);

  // Exp
  Arr *as = arr_new();
  RANGE0(i, nas) {
    arr_push(as, exp_rget_array(arr_get(a, i)));
  }_RANGE
  int nels = arr_size(arr_get(as, 0));
  RANGE(ias, 1, nas) {
    int sz = arr_size(arr_get(as, ias));
    if (sz < nels) nels = sz;
  }_RANGE
  if (nels == 0) return exp_array(arr_new());

  // Exp
  Arr *r = arr_new();
  RANGE0(iels, nels) {
    // Exp
    Arr *row = arr_new();
    RANGE0(ias, nas) {
      arr_push(row, arr_get(arr_get(as, ias), iels));
    }_RANGE
    arr_push(r, exp_array(row));
  }_RANGE
  return exp_array(r);
}

Bfunction md_arr_get (char *fname) {
  if (!strcmp(fname, "all")) return all;
  if (!strcmp(fname, "any")) return any;
  if (!strcmp(fname, "cat")) return cat;
  if (!strcmp(fname, "clear")) return clear;
  if (!strcmp(fname, "copy")) return copy;
  if (!strcmp(fname, "drop")) return drop;
  if (!strcmp(fname, "dropWhile")) return drop_while;
  if (!strcmp(fname, "duplicates")) return duplicates;
  if (!strcmp(fname, "each")) return each;
  if (!strcmp(fname, "eachIx")) return each_ix;
  if (!strcmp(fname, "filter")) return filter;
  if (!strcmp(fname, "filterIn")) return filter_in;
  if (!strcmp(fname, "find")) return find;
  if (!strcmp(fname, "fromIter")) return from_iter;
  if (!strcmp(fname, "index")) return findex;
  if (!strcmp(fname, "insert")) return insert;
  if (!strcmp(fname, "insertArr")) return insert_arr;
  if (!strcmp(fname, "join")) return join;
  if (!strcmp(fname, "map")) return map;
  if (!strcmp(fname, "new")) return new;
  if (!strcmp(fname, "peek")) return peek;
  if (!strcmp(fname, "pop")) return pop;
  if (!strcmp(fname, "push")) return push;
  if (!strcmp(fname, "reduce")) return reduce;
  if (!strcmp(fname, "remove")) return fremove;
  if (!strcmp(fname, "removeRange")) return remove_range;
  if (!strcmp(fname, "reverse")) return reverse;
  if (!strcmp(fname, "reverseIn")) return reverse_in;
  if (!strcmp(fname, "shift")) return shift;
  if (!strcmp(fname, "shuffle")) return shuffle;
  if (!strcmp(fname, "size")) return size;
  if (!strcmp(fname, "sort")) return sort;
  if (!strcmp(fname, "take")) return take;
  if (!strcmp(fname, "takeWhile")) return take_while;
  if (!strcmp(fname, "toIter")) return to_iter;
  if (!strcmp(fname, "unshift")) return unshift;
  if (!strcmp(fname, "unzip")) return unzip;
  if (!strcmp(fname, "zip")) return zip;
  EXC_KUT(fail_bfunction("arr", fname));
  return NULL; // Unreachable
}
