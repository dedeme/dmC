// Copyright 01-Mar-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/MA2.h"
#include "Flea.h"
#include "util.h"
#include "Order.h"
#include "io.h"
#include "DEFS.h"

enum {DAYS, STRIP_TO_BUY, STRIP_TO_SELL};

// Days is at d + d * x, where x >= MAX_DAYS && x <= MIN_DAYS
#define MAX_DAYS 120

// Days is at d + d * x, where x >= MAX_DAYS && x <= MIN_DAYS
#define MIN_DAYS 20

// Strip is d + d * x where x >= MAX_STRIP && x <= MIN_STRIP
#define MAX_STRIP 0.2

// Strip is d + d * x where x >= MAX_STRIP && x <= MIN_STRIP
#define MIN_STRIP 0.0001

#define CO struct minMax_co
CO {
  int to_sell;
  double avg;
  int days_ix;
  double ref;
};

static CO *co_new() {
  CO *this = malloc(sizeof(CO));
  this->to_sell = 1;
  this->days_ix = 0;
  this->avg = 0;
  this->ref = 0;
  return this;
}

static void co_free(CO *this) {
  free(this);
}

static Darr *fparams_new(Flea *f) {
  Gen *g = flea_gen(f);
  double *p = gen_values(g);
  Darr *r = darr_new();

  darr_push(r, (double)(int)(util_param(MAX_DAYS, MIN_DAYS, *p++)));
  darr_push(r, util_param(MAX_STRIP, MIN_STRIP, *p++));
  darr_push(r, util_param(MAX_STRIP, MIN_STRIP, *p++));

  return r;
}

// Arr[CO]
static Arr *cos_new(Darr *params, int qnicks, double *closes) {
  Arr *r = arr_new((FPROC)co_free);
  RANGE0(i, qnicks)
    arr_push(r, co_new());
  _RANGE
  return r;
}

static Order *order_new(Darr *params, void *company, double q) {
  CO *co = (CO *)company;
  int days = darr_get(params, DAYS);

  if (co->days_ix < days) {
    if (q > 0) {
      co->avg = (co->avg * co->days_ix + q) / (co->days_ix + 1);
      co->days_ix += 1;
      co->ref = co->avg;
    }
    return order_none_new();
  }

  if (q > 0) {
    double avg = (co->avg * (days - 1) + q) / days;
    co->avg = avg;
    if (co->to_sell) {
      double ref = co->ref;
      if (avg > ref) {
        co->ref = avg;
      } else {
        avg = ref;
      }
      if (q <= avg * (1 - darr_get(params, STRIP_TO_SELL))) {
        co->ref = co->avg;
        co->to_sell = 0;
        return order_sell_new();
      } else {
        return order_none_new();
      }
    } else {
      double ref0 = co->ref;
      if (avg < ref0) {
        co->ref = avg;
      } else {
        avg = ref0;
      }
      double ref = avg * (1 + darr_get(params, STRIP_TO_BUY));
      if (q >= ref) {
        double pond = (q - ref) / ref;
        co->ref = co->avg;
        co->to_sell = 1;
        return order_buy_new(BET / q, pond);
      } else {
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

Model *mA2_new() {
  // Varr[char]
  Varr *param_names = varr_new();
  varr_push(param_names, "Days");
  varr_push(param_names, "Strip C");
  varr_push(param_names, "Strip V");

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
    str_new("MA2"),
    param_names,
    param_jss,
    fparams_new,
    cos_new,
    order_new,
    ref
  );
}

#undef CO
