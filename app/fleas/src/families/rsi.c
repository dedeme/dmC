// Copyright 15-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/rsi.h"
#include <dm/dm.h>
#include "Gen.h"
#include "order/Buy.h"

struct _Rsi {
  size_t ix;
  double ups_avg;
  double downs_avg;
  double last_close;
  bool can_buy;
  bool can_sell;
};

static struct _Rsi *_rsi_new(void) {
  struct _Rsi *this = MALLOC(struct _Rsi);
  this->ix = 0;
  this->ups_avg = 0.0;
  this->downs_avg = 0.0;
  this->last_close = -1.0;
  this->can_buy = false;
  this->can_sell = false;
  return this;
}

struct rsi_Rsi {
  Gen *length; // v + 5
  Gen *up_level; // 50 + (v + 1) * 0.01
  Gen *down_level; // 50 - (v + 1) * 0.01

  size_t avgs_size;
  double up;
  double down;
  Arr/*_Rsi*/ *extra;
};

typedef struct rsi_Rsi Rsi;

static Rsi *new(Gen *length, Gen *up_level, Gen *down_level) {
  Rsi *this = MALLOC(Rsi);
  this->length = length;
  this->up_level = up_level;
  this->down_level = down_level;

  this->avgs_size = 5 + gen_actual(length);
  this->up = 50.0 + ((double)gen_actual(up_level) + 1) * 0.01;
  this->down = 50.0 - ((double)gen_actual(down_level) + 1) * 0.01;
  this->extra = NULL;

  return this;
}

static bool gen_eq(Rsi *this, Rsi *other) {
  return gen_actual(this->length) == gen_actual(other->length) &&
    gen_actual(this->up_level) == gen_actual(other->up_level) &&
    gen_actual(other->down_level) == gen_actual(other->down_level);
}

static void prepare(Rsi *this) {
  if (this->extra) {
    THROW exc_not_null_pointer("this") _THROW
  }

  Arr/*_Rsi*/ *extra = arr_new();
  REPEAT(NICKS_NUMBER) {
    arr_add(extra, _rsi_new());
  }_REPEAT

  this->extra = extra;
}

static void reset(Rsi *this) {
    this->extra = NULL;
}

static void process(
  Rsi *this,
  Flea *f,
  size_t nick,
  Quote *q
) {
  double close = quote_close(q);
  if (close <= 0) {
    return;
  }
  struct _Rsi *rsi = arr_get(this->extra, nick);

  if (!rsi->ix) {
    rsi->last_close = close;
    ++rsi->ix;
    return;
  }

  double ix = (double)rsi->ix;
  double last_close = rsi->last_close;
  double up = 0.0;
  double down = 0.0;
  if (close > last_close) {
    up = close - last_close;
  } else {
    down = last_close - close;
  }

  rsi->last_close = close;
  rsi->ups_avg = (rsi->ups_avg * (ix - 1) + up) / ix;
  rsi->downs_avg = (rsi->downs_avg * (ix - 1) + down) / ix;
  ++rsi->ix;

  if (ix < this->avgs_size) {
    return;
  }

  double rsiv = rsi->downs_avg < 0.00001
    ? 100.0
    : 100.0 - 100.0 / (1 + rsi->ups_avg / rsi->downs_avg);

  size_t stocks = portfolio_get(flea_portfolio(f), nick);
  if (rsiv > this->up) {
    if (rsi->can_buy && !stocks) {
      buys_add(flea_buys(f), nick, flea_bet(f));
    }
    rsi->can_buy = false;
    rsi->can_sell = true;
  } else if (rsiv < this->down) {
    if (rsi->can_sell && stocks) {
      sells_add(flea_sells(f), nick, stocks);
    }
    rsi->can_sell = false;
    rsi->can_buy = true;
  } else {
    rsi->can_buy = true;
    rsi->can_sell = true;
  }
}

static Json *trace_data(Rsi *this, size_t nick) {
  struct _Rsi *rsi = arr_get(this->extra, nick);

  Arr/*Json*/ *jsr = arr_new();

  jarr_auint(jsr, rsi->ix);
  jarr_adouble(jsr, (double)rsi->ups_avg, 4);
  jarr_adouble(jsr, (double)rsi->downs_avg, 4);
  jarr_adouble(jsr, (double)rsi->last_close, 4);
  jarr_abool(jsr, rsi->can_buy);
  jarr_abool(jsr, rsi->can_sell);

  return json_warray(jsr);
}

static Rsi *mutate(Rsi *this) {
  Gen *length = gen_mutate(this->length);
  Gen *up_level = gen_mutate(this->up_level);
  Gen *down_level = gen_mutate(this->down_level);

  return new(length, up_level, down_level);
}

static Json *serialize(Rsi *this) {
  Arr/*Json*/ *a = arr_new();

  arr_add(a, gen_serialize(this->length));
  arr_add(a, gen_serialize(this->up_level));
  arr_add(a, gen_serialize(this->down_level));

  return json_warray(a);
}

static Flea *mk_fextra(Flea *f, Rsi *this) {
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


Flea *rsi_new(Flea *f) {
  Gen *length = gen_new(146);
  Gen *up_level = gen_new(200);
  Gen *down_level = gen_new(200);

  return mk_fextra(f, new(length, up_level, down_level));
}

Flea *rsi_restore(Flea *f, Json *serial) {
  Arr/*Json*/ *a = json_rarray(serial);

  uint i = 0;
  Gen *length = gen_restore(arr_get(a, i++));
  Gen *up_level = gen_restore(arr_get(a, i++));
  Gen *down_level = gen_restore(arr_get(a, i++));

  return mk_fextra(f, new(length, up_level, down_level));
}
