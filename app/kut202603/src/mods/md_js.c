// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "mods/md_js.h"
#include "kut/js.h"
#include "exp.h"
#include "runner/fail.h"

// \s -> b
static Exp *is_null (Arr *exps) {
  CHECK_PARS ("js.isNull", 1, exps);
  return exp_bool(js_is_null(exp_get_string(arr_get(exps, 0))));
}

// \s -> b
static Exp *rb (Arr *exps) {
  CHECK_PARS ("js.rb", 1, exps);
  return exp_bool(js_rb(exp_get_string(arr_get(exps, 0))));
}

// \s -> i
static Exp *ri (Arr *exps) {
  CHECK_PARS ("js.ri", 1, exps);
  return exp_int(js_rl(exp_get_string(arr_get(exps, 0))));
}

// \s -> f
static Exp *rf (Arr *exps) {
  CHECK_PARS ("js.rf", 1, exps);
  return exp_float(js_rd(exp_get_string(arr_get(exps, 0))));
}

// \s -> s
static Exp *rs (Arr *exps) {
  CHECK_PARS ("js.rs", 1, exps);
  return exp_string(js_rs(exp_get_string(arr_get(exps, 0))));
}

// \s -> a
static Exp *ra (Arr *exps) {
  CHECK_PARS ("js.ra", 1, exps);
  return exp_array(arr_map(
    js_ra(exp_get_string(arr_get(exps, 0))), (FMAP)exp_string
  ));
}

// \s -> o
static Exp *ro (Arr *exps) {
  CHECK_PARS ("js.ro", 1, exps);
    //--
    Kv *fn (Kv *kv) { return kv_new(kv_key(kv), exp_string(kv_value(kv))); }
  return exp_dic((Map *)arr_map(
    (Arr *)js_ro(exp_get_string(arr_get(exps, 0))), (FMAP)fn
  ));
}


static Exp *raux (char *js) {
  js = str_trim(js);
  Exp *rt;
  if (*js == '"') rt = exp_string(js_rs(js));
  else if (*js == '[') rt = exp_array(arr_from_js(js, (FFROM)raux));
  else if (*js == '{') rt = exp_dic(map_from_js(js, (FFROM)raux));
  else if (*js == '-' || (*js >= '0' && *js <= '9')) rt = exp_float(js_rd(js));
  else if (!strcmp(js, "true")) rt = exp_bool(TRUE);
  else if (!strcmp(js, "false")) rt = exp_bool(FALSE);
  else EXC_ILLEGAL_ARGUMENT("Bad JSON", "JSON string", js);
  return rt;
}

// \s -> * (JSONizable)
static Exp *r (Arr *exps) {
  CHECK_PARS ("js.r", 1, exps);
  return raux(exp_get_string(arr_get(exps, 0)));
}

// \ -> s
static Exp *wn (Arr *exps) {
  CHECK_PARS ("js.wn", 0, exps);
  return exp_string("null");
}

// \b -> s
static Exp *wb (Arr *exps) {
  CHECK_PARS ("js.wb", 1, exps);
  return exp_string(exp_get_bool(arr_get(exps, 0)) ? "true" : "false");
}

// \i -> s
static Exp *wi (Arr *exps) {
  CHECK_PARS ("js.wi", 1, exps);
  return exp_string(js_wl(exp_get_int(arr_get(exps, 0))));
}

// \f -> s
static Exp *wf (Arr *exps) {
  CHECK_PARS ("js.wf", 1, exps);
  return exp_string(js_wf(exp_get_float(arr_get(exps, 0)), 9));
}

// \f, i -> s
static Exp *wf2 (Arr *exps) {
  CHECK_PARS ("js.wf2", 2, exps);
  return exp_string(js_wf(
    exp_get_float(arr_get(exps, 0)),
    exp_get_int(arr_get(exps, 1))
  ));
}

// \s -> s
static Exp *ws (Arr *exps) {
  CHECK_PARS ("js.ws", 1, exps);
  return exp_string(js_ws(exp_get_string(arr_get(exps, 0))));
}

// \a -> s
static Exp *wa (Arr *exps) {
  CHECK_PARS ("js.wa", 1, exps);
  return exp_string(js_wa(
    arr_map(exp_get_array(arr_get(exps, 0)), (FMAP)exp_get_string)
  ));
}

// \o -> s
static Exp *wo (Arr *exps) {
  CHECK_PARS ("js.wo", 1, exps);
    //--
    Kv *fn (Kv *kv) { return kv_new(kv_key(kv), exp_get_string(kv_value(kv))); }
  return exp_string(js_wo(
    (Map *)arr_map((Arr *)exp_get_dic(arr_get(exps, 0)), (FMAP)fn)
  ));
}

static char *waux (Exp *exp) {
  char *rt;
  if (exp_is_string(exp)) rt = js_ws(exp_get_string(exp));
  else if (exp_is_array(exp)) rt = arr_to_js(exp_get_array(exp), (FTO)waux);
  else if (exp_is_dic(exp)) rt = map_to_js(exp_get_dic(exp), (FTO)waux);
  else if (exp_is_int(exp)) rt = js_wl(exp_get_int(exp));
  else if (exp_is_float(exp)) rt = js_wf(exp_get_float(exp), 9);
  else if (exp_is_bool(exp)) rt = js_wb(exp_get_bool(exp));
  else EXC_ILLEGAL_ARGUMENT(
      "Data not JSONizable",
      "JSONizable data",
      str_f("%s(%s)", exp_to_js(exp), exp_type_to_str(exp))
    );
  return rt;
}

// \*JSONizable -> s
static Exp *w (Arr *exps) {
  CHECK_PARS ("js.w", 1, exps);
  return exp_string(waux(arr_get(exps, 0)));
}

Bfunction md_js_get (char *fname) {
  if (!strcmp(fname, "isNull")) return is_null;
  if (!strcmp(fname, "r")) return r;
  if (!strcmp(fname, "rb")) return rb;
  if (!strcmp(fname, "ri")) return ri;
  if (!strcmp(fname, "rf")) return rf;
  if (!strcmp(fname, "rs")) return rs;
  if (!strcmp(fname, "ra")) return ra;
  if (!strcmp(fname, "ro")) return ro;
  if (!strcmp(fname, "w")) return w;
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
