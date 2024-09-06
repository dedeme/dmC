// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_dic.h"
#include "kut/it.h"
#include "exp.h"
#include "obj.h"
#include "runner/fail.h"

// \d -> d
static Exp *copy (Arr *exps) {
  CHECK_PARS ("dic.copy", 1, exps);
  return exp_dic((Map *)arr_copy((Arr *)exp_get_dic(arr_get(exps, 0))));
}

// \[[s,*]...] -> d
static Exp *from_arr (Arr *exps) {
  CHECK_PARS ("dic.fromArr", 1, exps);

  // <Exp>
  Map *m = map_new();
  EACH(exp_get_array(arr_get(exps, 0)), Exp, e) {
    // [Exp, Exp]
    Arr *a = exp_get_array(e);
    map_put(m, exp_get_string(arr_get(a, 0)), arr_get(a, 1));
  }_EACH
  return exp_dic(m);
}

// \<iter>[[s,*]...] -> d
static Exp *from_iter (Arr *exps) {
  CHECK_PARS ("dic.fromIter", 1, exps);

  // <Exp>
  Map *m = map_new();
    //--
    void f (Exp *e) {
      // [Exp, Exp]
      Arr *a = exp_get_array(e);
      map_put(m, exp_get_string(arr_get(a, 0)), arr_get(a, 1));
    }
  it_each(obj_get_iter(arr_get(exps, 0)), (FPROC)f);
  return exp_dic(m);
}

// \d, s -> []|[*]
static Exp *get (Arr *exps) {
  CHECK_PARS ("dic.get", 2, exps);
  Exp *r = opt_get(map_get(
    exp_get_dic(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  ));
  return r
    ? exp_array(arr_new_from(r, NULL))
    : exp_array(arr_new())
  ;
}

// \d, s -> b
static Exp *has_key (Arr *exps) {
  CHECK_PARS ("dic.hasKey", 2, exps);
  return exp_bool(map_has_key(
    exp_get_dic(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  ));
}

// \d -> [s...]
static Exp *keys (Arr *exps) {
  CHECK_PARS ("dic.keys", 1, exps);
    //--
    Exp *fn (char *key) { return exp_string(key); }
  return exp_array(arr_map(map_keys(exp_get_dic(arr_get(exps, 0))), (FMAP)fn));
}

// \d, s, * -> ()
static Exp *put (Arr *exps) {
  CHECK_PARS ("dic.put", 3, exps);
  map_put(
    exp_get_dic(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1)),
    arr_get(exps, 2)
  );
  return exp_empty();
}

// \d, s -> ()
static Exp *fremove (Arr *exps) {
  CHECK_PARS ("dic.remove", 2, exps);
  map_remove(
    exp_get_dic(arr_get(exps, 0)),
    exp_get_string(arr_get(exps, 1))
  );
  return exp_empty();
}

// \d -> i
static Exp *size (Arr *exps) {
  CHECK_PARS ("dic.size", 1, exps);
  return exp_int(map_size(exp_get_dic(arr_get(exps, 0))));
}

// |d -> [[s,*]...]
static Exp *to_arr (Arr *exps) {
  CHECK_PARS ("dic.toArr", 1, exps);
    //--
    void *f (Kv *kv) {
      return exp_array(arr_new_from(
        exp_string(kv_key(kv)), kv_value(kv), NULL
      ));
    }
  return exp_array(
    arr_map((Arr *)exp_get_dic(arr_get(exps, 0)), (FMAP)f)
  );
}

// |d -> <iter>[[s,*]...]
static Exp *to_iter (Arr *exps) {
  CHECK_PARS ("dic.toIter", 1, exps);
    //--
    void *f (Kv *kv) {
      return exp_array(arr_new_from(
        exp_string(kv_key(kv)), kv_value(kv), NULL
      ));
    }
  return obj_iter(arr_to_it(
    arr_map((Arr *)exp_get_dic(arr_get(exps, 0)), (FMAP)f)
  ));
}

// \d -> a
static Exp *values (Arr *exps) {
  CHECK_PARS ("dic.values", 1, exps);
  return exp_array(
    arr_map((Arr *)exp_get_dic(arr_get(exps, 0)), (FMAP)kv_value)
  );
}

Bfunction md_dic_get (char *fname) {
  if (!strcmp(fname, "copy")) return copy;
  if (!strcmp(fname, "fromArr")) return from_arr;
  if (!strcmp(fname, "fromIter")) return from_iter;
  if (!strcmp(fname, "get")) return get;
  if (!strcmp(fname, "hasKey")) return has_key;
  if (!strcmp(fname, "keys")) return keys;
  if (!strcmp(fname, "put")) return put;
  if (!strcmp(fname, "remove")) return fremove;
  if (!strcmp(fname, "size")) return size;
  if (!strcmp(fname, "toArr")) return to_arr;
  if (!strcmp(fname, "toIter")) return to_iter;
  if (!strcmp(fname, "values")) return values;
  EXC_KUT(fail_bfunction("dic", fname));
  return NULL; // Unreachable
}

Exp *md_dic_fget (Map *dic, char *key) {
  Exp *r = opt_get(map_get(dic, key));
  if (!r)
    EXC_KUT(str_f("Key '%s' not found in dictionary", key));
  return r;
}
