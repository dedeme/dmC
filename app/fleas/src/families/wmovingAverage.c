// Copyright 22-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/wmovingAverage.h"
#include <dm/dm.h>
#include "Gen.h"
#include "order/Buy.h"
#include "families/WmaCalc.h"

struct _WmovingAverage {
  WmaCalc *calc;
  bool can_buy;
  bool can_sell;
};

static struct _WmovingAverage *_new(
  size_t avg_len,
  double mbuy,
  double msell
) {
  struct _WmovingAverage *this = MALLOC(struct _WmovingAverage);
  this->calc = wmacalc_new(avg_len, mbuy, msell);
  this->can_buy = false;
  this->can_sell = false;
  return this;
}

typedef struct wmovingAverage_WmovingAverage WmovingAverage;

struct wmovingAverage_WmovingAverage {
  Gen *length; // v + 5
  Gen *buy_strip; // v * 0.001
  Gen *sell_strip;  // v * 0.001

  size_t avg_len;
  double mbuy;
  double msell;
  Arr/*_WmovingAverage*/ *extra;
};

static WmovingAverage *new(
  Gen *length, Gen *buy_strip, Gen *sell_strip
) {
  WmovingAverage *this = MALLOC(WmovingAverage);
  this->length = length;
  this->buy_strip = buy_strip;
  this->sell_strip = sell_strip;

  this->avg_len = gen_actual(length) + 5;
  this->mbuy = 1 + (double)gen_actual(buy_strip) * 0.001;
  this->msell = 1 - (double)gen_actual(sell_strip) * 0.001;
  this->extra = NULL;

  return this;
}

static bool gen_eq(WmovingAverage *this, WmovingAverage *other) {
  return gen_actual(this->length) == gen_actual(other->length) &&
    gen_actual(this->buy_strip) == gen_actual(other->buy_strip) &&
    gen_actual(other->sell_strip) == gen_actual(other->sell_strip);
}

static void prepare(WmovingAverage *this) {
  if (this->extra) {
    THROW exc_not_null_pointer("this") _THROW
  }

  Arr/*_WmovingAverage*/ *extra = arr_new();
  REPEAT(NICKS_NUMBER) {
    arr_add(extra, _new(this->avg_len, this->mbuy, this->msell));
  }_REPEAT

  this->extra = extra;
}

static void reset(WmovingAverage *this) {
    this->extra = NULL;
}

static void process(
  WmovingAverage *this,
  Flea *f,
  size_t nick,
  Quote *q
) {
  struct _WmovingAverage *ma = arr_get(this->extra, nick);
  enum wmacalc_Result r = wmacalc_add(ma->calc, quote_close(q));

  if (r == WMACALC_NOT_VALID) {
    return;
  }

  if (r == WMACALC_BUY) {
    if (ma->can_buy) {
      buys_add(flea_buys(f), nick, flea_bet(f));
    }
    ma->can_buy = false;
    ma->can_sell = true;
  } else {
    ma->can_buy = true;
    if (r == WMACALC_SELL) {
      if (ma->can_sell) {
        size_t stocks = portfolio_get(flea_portfolio(f), nick);
        if (stocks) {
          sells_add(flea_sells(f), nick, stocks);
        }
      }
      ma->can_sell = false;
    } else {
      ma->can_sell = true;
    }
  }
}

static Json *trace_data(WmovingAverage *this, size_t nick) {
  struct _WmovingAverage *ma = arr_get(this->extra, nick);

  Arr/*Json*/ *jsr = arr_new();

  jarr_adouble(jsr, wmacalc_sum(ma->calc), 4);
  jarr_abool(jsr, ma->can_buy);
  jarr_abool(jsr, ma->can_sell);

  return json_warray(jsr);
}

static WmovingAverage *mutate(WmovingAverage *this) {
  Gen *length = gen_mutate(this->length);
  Gen *buy_strip = gen_mutate(this->buy_strip);
  Gen *sell_strip = gen_mutate(this->sell_strip);

  return new(length, buy_strip, sell_strip);
}

static Json *serialize(WmovingAverage *this) {
  Arr/*Json*/ *a = arr_new();

  arr_add(a, gen_serialize(this->length));
  arr_add(a, gen_serialize(this->buy_strip));
  arr_add(a, gen_serialize(this->sell_strip));

  return json_warray(a);
}

static Flea *mk_fextra(Flea *f, WmovingAverage *this) {
  return flea_fextra(
    f,
    this,
    (bool (*)(void *, void *))gen_eq,
    (void (*)(void *))prepare,
    (void (*)(void *))reset,
    (void (*)(void *, Flea *, size_t, Quote *))process,
    (Json *(*)(void *, size_t))trace_data,
    (void *(*)(void *))mutate,
    (Json *(*)(void *))serialize
  );
}


Flea *wmovingAverage_new(Flea *f) {
  Gen *length = gen_new(146);
  Gen *buy_strip = gen_new(301);
  Gen *sell_strip = gen_new(301);

  return mk_fextra(f, new(length, buy_strip, sell_strip));
}

Flea *wmovingAverage_restore(Flea *f, Json *serial) {
  Arr/*Json*/ *a = json_rarray(serial);
  uint i = 0;
  Gen *length = gen_restore(arr_get(a, i++));
  Gen *buy_strip = gen_restore(arr_get(a, i++));
  Gen *sell_strip = gen_restore(arr_get(a, i++));

  return mk_fextra(f, new(length, buy_strip, sell_strip));
}
