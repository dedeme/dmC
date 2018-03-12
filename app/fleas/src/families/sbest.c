// Copyright 22-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/sbest.h"
#include <dmc/all.h>
#include "DEFS.h"
#include "Gen.h"
#include "market/Buy.h"
#include "families/UdCalc.h"

struct _Sbest {
  UdCalc *calc;
  bool can_buy;
  bool can_sell;
};

static struct _Sbest *_updown_new(
  size_t sbest_len,
  double mbuy,
  double msell
) {
  struct _Sbest *this = MALLOC(struct _Sbest);
  this->calc = udcalc_new(sbest_len, mbuy, msell, QUOTES_NUMBER);
  this->can_buy = false;
  this->can_sell = false;
  return this;
}

struct sbest_Sbest {
  Gen *length; // v + 61
  Gen *buy_strip; // v * 0.001
  Gen *sell_strip; // v * 0.001

  size_t sbest_len;
  double mbuy;
  double msell;
  Arr/*_MovingAverage*/ *extra;
};

typedef struct sbest_Sbest Sbest;

static Sbest *new(Gen *length, Gen *buy_strip, Gen *sell_strip) {
  Sbest *this = MALLOC(Sbest);
  this->length = length;
  this->buy_strip = buy_strip;
  this->sell_strip = sell_strip;

  this->sbest_len = gen_actual(this->length) + 61;
  this->mbuy = 1 + (double)gen_actual(buy_strip) * 0.001;
  this->msell = 1 - (double)gen_actual(sell_strip) * 0.001;
  this->extra = NULL;

  return this;

}

static bool gen_eq(Sbest *this, Sbest *other) {
  return gen_actual(this->length) == gen_actual(other->length) &&
    gen_actual(this->buy_strip) == gen_actual(other->buy_strip) &&
    gen_actual(other->sell_strip) == gen_actual(other->sell_strip);
}

static void prepare(Sbest *this) {
  if (this->extra) {
    THROW exc_not_null_pointer("this") _THROW
  }

  Arr/*_UpDown*/ *extra = arr_new();

  size_t best_len = this->sbest_len;
  REPEAT(NICKS_NUMBER) {
    arr_add(extra, _updown_new(best_len, this->mbuy, this->msell));
  }_REPEAT

  this->extra = extra;
}

static void reset(Sbest *this) {
  this->extra = NULL;
}

static Sbest *mutate(Sbest *this) {
  return new(
    gen_mutate(this->length),
    gen_mutate(this->buy_strip),
    gen_mutate(this->sell_strip)
  );
}

static void process(
  Sbest *this,
  Flea *f,
  size_t nick,
  Quote *q
) {
  double close = quote_close(q);
  struct _Sbest *ud = arr_get(this->extra, nick);
  enum udcalc_Result r = udcalc_add(ud->calc, close);

  if (r == UDCALC_NOT_VALID) {
    return;
  }

  if (r == UDCALC_BUY) {
    if (ud->can_buy) {
      size_t stocks = buy_calc(flea_bet(f), close);
      arr_add(flea_buys(f), buy_new_limit(nick, stocks, close));
    }
    ud->can_buy = false;
    ud->can_sell = true;
  } else {
    ud->can_buy = true;
    if (r == UDCALC_SELL) {
      if (ud->can_sell) {
        size_t stocks = pf_stocks(flea_portfolio(f), nick);
        if (stocks) {
          arr_add(flea_sells(f), sell_new(nick, stocks));
        }
      }
      ud->can_sell = false;
    } else {
      ud->can_sell = true;
    }
  }
}

static Json *trace_data(Sbest *this, size_t nick) {
  struct _Sbest *ud = arr_get(this->extra, nick);

  Arr/*Json*/ *jsr = arr_new();
  Arr/*Json*/ *closes = arr_new();

  double *cls = udcalc_values(ud->calc) - 1;
  REPEAT(this->sbest_len){
    jarr_adouble(closes, *cls++, 4);
  }_REPEAT
  jarr_aarray(jsr, closes);

  jarr_abool(jsr, ud->can_buy);
  jarr_abool(jsr, ud->can_sell);

  return json_warray(jsr);
}

static Json *serialize(Sbest *this) {
  Arr/*Json*/ *a = arr_new();

  arr_add(a, gen_serialize(this->length));
  arr_add(a, gen_serialize(this->buy_strip));
  arr_add(a, gen_serialize(this->sell_strip));

  return json_warray(a);
}

static Flea *mk_fextra(Flea *f, Sbest *this) {
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

Flea *sbest_new(Flea *f) {
  Gen *length = gen_new(9);
  Gen *buy_strip = gen_new(301);
  Gen *sell_strip = gen_new(301);

  return mk_fextra(f, new(length, buy_strip, sell_strip));
}

Flea *sbest_restore(Flea *f, Json *serial) {
  Arr/*Json*/ *a = json_rarray(serial);

  uint i = 0;
  Gen *length = gen_restore(arr_get(a, i++));
  Gen *buy_strip = gen_restore(arr_get(a, i++));
  Gen *sell_strip = gen_restore(arr_get(a, i++));

  return mk_fextra(f, new(length, buy_strip, sell_strip));
}
