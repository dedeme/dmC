// Copyright 08-Mar-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "models/AppMMB.h"
#include "Flea.h"
#include "util.h"
#include "Order.h"
#include "DEFS.h"

enum {STRIP, STEP};

// Strip to buy is q * ( 1 + x), where x >= MIN_STRIP && x <= MAX_STRIP
#define MAX_STRIP 0.10

// Strip to buy is q * ( 1 + x), where x >= MIN_STRIP && x <= MAX_STRIP
#define MIN_STRIP 0.001

// Step to buy is q + (q - ref) * x, where x >= MIN_STEP && x <= MAX_STEP
#define MAX_STEP 0.10

// Step to buy is q + (q - ref) * x, where x >= MIN_STEP && x <= MAX_STEP
#define MIN_STEP 0.001

#define CO struct approx_co
CO {
  int to_sell;
  double ref;
  double mm;
};

static CO *co_new(int to_sell, double ref, double q) {
  CO *this = malloc(sizeof(CO));
  this->to_sell = to_sell;
  this->ref = ref;
  this->mm = q;
  return this;
}

static void co_free(CO *this) {
  free(this);
}

static Darr *fparams_new(Flea *f) {
  Gen *g = flea_gen(f);
  double *p = gen_values(g);
  Darr *r = darr_new();

  darr_push(r, util_param(MAX_STRIP, MIN_STRIP, *p++));
  darr_push(r, util_param(MAX_STEP, MIN_STEP, *p++));

  return r;
}

// Arr[CO]
static Arr *cos_new(Darr *params, int qnicks, double *closes) {
  Arr *r = arr_new((FPROC)co_free);
  double *p = closes;
  REPEAT(qnicks)
    double *q = p++;
    while (*q < 0) {
      q += qnicks;
    }
    arr_push(
      r,
      co_new(1, *q * (0.95) * (1 - darr_get(params, STRIP)) , *q)
    );
  _REPEAT
  return r;
}

static Order *order_new(Darr *params, void *company, double q) {
  CO *co = (CO *)company;
  if (q > 0) {
    if (co->to_sell) {
      co->ref = co->ref + (q - co->ref) * darr_get(params, STEP);
      if (q <= co->ref) {
        co->ref = co->mm * (1 + darr_get(params, STRIP));
        co->mm = q;
        co->to_sell = 0;
        return order_sell_new();
      } else {
        co->mm = q > co->mm ? q : co->mm;
        return order_none_new();
      }
    } else {
      co->ref = co->ref - (co->ref - q) * darr_get(params, STEP);
      if (q >= co->ref) {
        double pond = (q - co->ref) / co->ref;
        co->ref = co->mm * (1 -darr_get(params, STRIP));
        co->mm = q;
        co->to_sell = 1;
        return order_buy_new(BET / q, pond);
      } else {
        co->mm = q < co->mm ? q : co->mm;
        return order_none_new();
      }
    }
  } else {
    return order_none_new();
  }
}

static double ref(Darr *params, void *co) {
  return ((CO *)co)->ref;
}

Model *appMMB2_new() {
  // Varr[char]
  Varr *param_names = varr_new();
  varr_push(param_names, "Banda");
  varr_push(param_names, "Paso");

  // Arr[Js]
  Arr *param_jss_js = arr_new(free);
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
  arr_push(param_jss_js, mk_p_jss_new());
  arr_push(param_jss_js, mk_p_jss_new());
  Js *param_jss = js_wa_new(param_jss_js);
  arr_free(param_jss_js);

  return model_new(
    str_new("AppMMB2"),
    param_names,
    param_jss,
    fparams_new,
    cos_new,
    order_new,
    ref
  );
}

#undef CO
