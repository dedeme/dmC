// Copyright 26-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/SupRes3.h"
#include "Flea.h"
#include "util.h"
#include "Order.h"
#include "io.h"
#include "DEFS.h"
#include "Qquotes.h"

enum {DAYS, STRIP_TO_BUY, STRIP_TO_SELL};

// Days is at d + d * x, where x >= MAX_DAYS && x <= MIN_DAYS
#define MAX_DAYS 50

// Days is at d + d * x, where x >= MAX_DAYS && x <= MIN_DAYS
#define MIN_DAYS 10

// Strip is d + d * x where x >= MAX_STRIP_TO_BUY && x <= MIN_STRIP_TO_BUY
#define MAX_STRIP_TO_BUY 0.2

// Strip is d + d * x where x >= MAX_STRIP_TO_BUY && x <= MIN_STRIP_TO_BUY
#define MIN_STRIP_TO_BUY 0.0001

// Strip is d + d * x where x >= MAX_STRIP_TO_SELL && x <= MIN_STRIP_TO_SELL
#define MAX_STRIP_TO_SELL 0.1

// Strip is d + d * x where x >= MAX_STRIP_TO_SELL && x <= MIN_STRIP_TO_SELL
#define MIN_STRIP_TO_SELL 0.0001

#define CO struct minMax_co
CO {
  int to_sell;
  Qquotes *qqs;
  double ref;
};

static CO *co_new(int days) {
  CO *this = malloc(sizeof(CO));
  this->to_sell = 1;
  this->qqs = qquotes_new(days);
  this->ref = MIN_TO_BET; // A big number
  return this;
}

static void co_free(CO *this) {
  qquotes_free(this->qqs);
  free(this);
}

static Darr *fparams_new(Flea *f) {
  Gen *g = flea_gen(f);
  double *p = gen_values(g);
  Darr *r = darr_new();

  darr_push(r, (double)(int)(util_param(MAX_DAYS, MIN_DAYS, *p++)));
  darr_push(r, util_param(MAX_STRIP_TO_BUY, MIN_STRIP_TO_BUY, *p++));
  darr_push(r, util_param(MAX_STRIP_TO_SELL, MIN_STRIP_TO_SELL, *p++));

  return r;
}

// Arr[CO]
static Arr *cos_new(Darr *params, int qnicks, double *closes) {
  Arr *r = arr_new((FPROC)co_free);
  RANGE0(i, qnicks)
    arr_push(r, co_new(darr_get(params, DAYS)));
  _RANGE
  return r;
}

static Order *order_new(Darr *params, void *company, double q) {
  CO *co = (CO *)company;
  Qquotes *qqs = co->qqs;

  if (qquotes_waiting(qqs)) {
    if (q > 0) {
      qquotes_add(qqs, q);
      if (q < co->ref) co->ref = q;
    }
    return order_none_new();
  }

  if (q > 0) {
    if (co->to_sell) {
      double ref = co->ref;
      double refs = ref * (1 - darr_get(params, STRIP_TO_SELL));
      if (q <= refs) {
        qquotes_add(qqs, q);
        co->ref = qquotes_max(qqs);
        co->to_sell = 0;
        return order_sell_new();
      } else {
        qquotes_add(qqs, q);
        double min = qquotes_min(qqs);
        if (min > ref) co->ref = min;
        return order_none_new();
      }
    } else {
      double ref = co->ref;
      double refs = ref * (1 + darr_get(params, STRIP_TO_BUY));
      if (q >= refs) {
        double pond = (q - refs) / refs;
        qquotes_add(qqs, q);
        co->ref = qquotes_min(qqs);
        co->to_sell = 1;
        return order_buy_new(BET / q, pond);
      } else {
        qquotes_add(qqs, q);
        double max = qquotes_max(qqs);
        if (max < ref) co->ref = max;
        return order_none_new();
      }
    }
  } else {
    return order_none_new();
  }
}

static double ref(Darr *params, void *company) {
  CO *co = (CO *)company;
  return co->to_sell
    ? co->ref  * (1 - darr_get(params, STRIP_TO_SELL))
    : co->ref * (1 + darr_get(params, STRIP_TO_BUY))
  ;
}

Model *supRes3_new() {
  // Varr[char]
  Varr *param_names = varr_new();
  varr_push(param_names, "Dias");
  varr_push(param_names, "Banda C");
  varr_push(param_names, "Banda V");

  // Arr[Js]
  Arr *param_jss_js = arr_new(free);
  Js *mk_pday_jss_new () {
    Arr *r = arr_new(free);
    arr_push(r, js_ws_new(""));
    arr_push(r, js_wi_new(1));
    arr_push(r, js_wi_new(0));
    arr_push(r, js_ws_new(""));
    Js *js = js_wa_new(r);
    arr_free(r);
    return js;
  }
  Js *mk_p_jss_new () {
    Arr *r = arr_new(free);
    arr_push(r, js_ws_new(""));
    arr_push(r, js_wi_new(100));
    arr_push(r, js_wi_new(4));
    arr_push(r, js_ws_new("%"));
    Js *js = js_wa_new(r);
    arr_free(r);
    return js;
  }
  arr_push(param_jss_js, mk_pday_jss_new());
  arr_push(param_jss_js, mk_p_jss_new());
  arr_push(param_jss_js, mk_p_jss_new());
  Js *param_jss = js_wa_new(param_jss_js);
  arr_free(param_jss_js);

  return model_new(
    str_new("SupRes3"),
    param_names,
    param_jss,
    fparams_new,
    cos_new,
    order_new,
    ref
  );
}

#undef CO

