// Copyright 05-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "mods/md_js.h"
#include "kut/js.h"
#include "exp.h"
#include "DEFS.h"
#include "runner/fail.h"

// \s -> b
static Exp *is_null (Arr *exps) {
  CHECK_PARS ("js.isNull", 1, exps);
  return exp_bool(js_is_null(exp_rget_string(arr_get(exps, 0))));
}

// \s -> b
static Exp *rb (Arr *exps) {
  CHECK_PARS ("js.rb", 1, exps);
  return exp_bool(js_rb(exp_rget_string(arr_get(exps, 0))));
}

// \s -> i
static Exp *ri (Arr *exps) {
  CHECK_PARS ("js.ri", 1, exps);
  return exp_int(js_rl(exp_rget_string(arr_get(exps, 0))));
}

// \s -> f
static Exp *rf (Arr *exps) {
  CHECK_PARS ("js.rf", 1, exps);
  return exp_float(js_rd(exp_rget_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *rs (Arr *exps) {
  CHECK_PARS ("js.rs", 1, exps);
  return exp_string(js_rs(exp_rget_string(arr_get(exps, 0))));
}

// \s -> a
static Exp *ra (Arr *exps) {
  CHECK_PARS ("js.ra", 1, exps);
  return exp_array(arr_map(
    js_ra(exp_rget_string(arr_get(exps, 0))), (FMAP)exp_string
  ));
}

// \s -> o
static Exp *ro (Arr *exps) {
  CHECK_PARS ("js.ro", 1, exps);
    //--
    Kv *fn (Kv *kv) { return kv_new(kv_key(kv), exp_string(kv_value(kv))); }
  return exp_map((Map *)arr_map(
    (Arr *)js_ro(exp_rget_string(arr_get(exps, 0))), (FMAP)fn
  ));
}

// \ -> s
static Exp *wn (Arr *exps) {
  CHECK_PARS ("js.wn", 0, exps);
  return exp_string("null");
}

// \b -> s
static Exp *wb (Arr *exps) {
  CHECK_PARS ("js.wb", 1, exps);
  return exp_string(exp_rget_bool(arr_get(exps, 0)) ? "true" : "false");
}

// \i -> s
static Exp *wi (Arr *exps) {
  CHECK_PARS ("js.wi", 1, exps);
  return exp_string(js_wl(exp_rget_int(arr_get(exps, 0))));
}

// \f -> s
static Exp *wf (Arr *exps) {
  CHECK_PARS ("js.wf", 1, exps);
  return exp_string(js_wf(exp_rget_float(arr_get(exps, 0)), 9));
}

// \f, i -> s
static Exp *wf2 (Arr *exps) {
  CHECK_PARS ("js.wf2", 2, exps);
  return exp_string(js_wf(
    exp_rget_float(arr_get(exps, 0)),
    exp_rget_int(arr_get(exps, 1))
  ));
}

// \s -> s
static Exp *ws (Arr *exps) {
  CHECK_PARS ("js.ws", 1, exps);
  return exp_string(js_ws(exp_rget_string(arr_get(exps, 0))));
}

// \a -> s
static Exp *wa (Arr *exps) {
  CHECK_PARS ("js.wa", 1, exps);
  return exp_string(js_wa(
    arr_map(exp_rget_array(arr_get(exps, 0)), (FMAP)exp_rget_string)
  ));
}

// \o -> s
static Exp *wo (Arr *exps) {
  CHECK_PARS ("js.wo", 1, exps);
    //--
    Kv *fn (Kv *kv) { return kv_new(kv_key(kv), exp_rget_string(kv_value(kv))); }
  return exp_string(js_wo(
    (Map *)arr_map((Arr *)exp_rget_map(arr_get(exps, 0)), (FMAP)fn)
  ));
}

Bfunction md_js_get (char *fname) {
  if (!strcmp(fname, "isNull")) return is_null;
  if (!strcmp(fname, "rb")) return rb;
  if (!strcmp(fname, "ri")) return ri;
  if (!strcmp(fname, "rf")) return rf;
  if (!strcmp(fname, "rs")) return rs;
  if (!strcmp(fname, "ra")) return ra;
  if (!strcmp(fname, "ro")) return ro;
  if (!strcmp(fname, "wn")) return wn;
  if (!strcmp(fname, "wb")) return wb;
  if (!strcmp(fname, "wi")) return wi;
  if (!strcmp(fname, "wf")) return wf;
  if (!strcmp(fname, "wf2")) return wf2;
  if (!strcmp(fname, "ws")) return ws;
  if (!strcmp(fname, "wa")) return wa;
  if (!strcmp(fname, "wo")) return wo;

  EXC_KUT(fail_bfunction("js", fname));
  return NULL; // Unreachable
}
