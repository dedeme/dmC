// Copyright 01-Mar-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/Incr2.h"
#include "Flea.h"
#include "util.h"
#include "Order.h"
#include "io.h"
#include "DEFS.h"

enum {DAYS, STRIP};

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
  double ref;
  int closes_ix;
  int days_ix;
};

static CO *co_new(int to_sell, double ref, int closes_ix, int days_ix) {
  CO *this = malloc(sizeof(CO));
  this->to_sell = to_sell;
  this->ref = ref;
  this->closes_ix = closes_ix;
  this->days_ix = days_ix;
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

  return r;
}

// Arr[CO]
static Arr *cos_new(Darr *params, int qnicks, double *closes) {
  double strip_to_sell = darr_get(params, STRIP);
  Arr *r = arr_new((FPROC)co_free);
  double *p = closes;
  RANGE0(i, qnicks)
    arr_push(r, co_new(1, *p++ * (1 - strip_to_sell), i, 0));
  _RANGE
  return r;
}

static Order *order_new(Darr *params, void *company, double q) {
  double *closes = io_closes();
  int qnicks = io_qnicks();
  CO *co = (CO *)company;
  int days = darr_get(params, DAYS);

  if (co->days_ix < days) {
    co->days_ix += 1;
    return order_none_new();
  }

  co->closes_ix += qnicks;
  double new_ref = closes[co->closes_ix];
  if (co->to_sell) {
    if (q > 0) {
      if (new_ref > 0 || co->ref > 0) {
        if (co->ref < 0 || new_ref > co->ref) {
          co->ref = new_ref;
        }
        if (q <= (co->ref * (1 - darr_get(params, STRIP)))) {
          co->ref = new_ref;
          co->to_sell = 0;
          return order_sell_new();
        } else {
          return order_none_new();
        }
      } else {
        return order_none_new();
      }
    } else {
      if ((co->ref > 0 && new_ref > co->ref) || co->ref <= 0) {
        co->ref = new_ref;
      }
      return order_none_new();
    }
  } else {
    if (q > 0) {
      if (new_ref > 0 || co->ref > 0) {
        if (co->ref < 0 || new_ref < co->ref) {
          co->ref = new_ref;
        }
        double ref = co->ref * (1 + darr_get(params, STRIP));
        if (q >= ref) {
          double pond = (q - ref) / ref;
          co->ref = new_ref;
          co->to_sell = 1;
          return order_buy_new(BET / q, pond);
        } else {
          return order_none_new();
        }
      } else {
        return order_none_new();
      }
    } else {
      if ((co->ref > 0 && new_ref < co->ref) || co->ref <= 0) {
        co->ref = new_ref;
      }
      return order_none_new();
    }
  }
}

static double ref(Darr *params, void *company) {
  CO *co = (CO *)company;
  return co->to_sell
    ? co->ref * (1 - darr_get(params, STRIP))
    : co->ref * (1 + darr_get(params, STRIP))
  ;
}

Model *incr2_new() {
  // Varr[char]
  Varr *param_names = varr_new();
  varr_push(param_names, "Days");
  varr_push(param_names, "Strip");

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
  Js *param_jss = js_wa_new(param_jss_js);
  arr_free(param_jss_js);

  return model_new(
    str_new("Incr2"),
    param_names,
    param_jss,
    fparams_new,
    cos_new,
    order_new,
    ref
  );
}

#undef CO


