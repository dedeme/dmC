// Copyright 15-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

#include "families/follow.h"
#include <dm/dm.h>
#include "DEFS.h"
#include "Gen.h"
#include "order/Buy.h"
#include "families/UdCalc2.h"

struct _Follow {
  size_t nick;
  UdCalc2 *calc;
  double inc;
};

static struct _Follow *_follow_new(size_t nick, size_t closes_size) {
  struct _Follow *this = MALLOC(struct _Follow);
  this->nick = nick;
  this->calc = udcalc2_new(closes_size, QUOTES_NUMBER);
  this->inc = -10.0;
  return this;
}

struct follow_Follow {
  Gen *interval; // 5 + v
  Gen *length; // 5 + v
  Gen *level; // %

  size_t closes_size;
  double cash;
  size_t last_nick;
  size_t interval_value;
  size_t interval_counter;
  size_t level_value;
  Arr/*double*/ *extra;
};

typedef struct follow_Follow Follow;

static Follow *new(Gen *interval, Gen *length, Gen *level) {
  Follow *this = MALLOC(Follow);
  this->interval = interval;
  this->length = length;
  this->level = level;

  this->closes_size = 5 + gen_actual(length);
  this->cash = 0.0;
  this->last_nick = 0;
  this->interval_value = 5 + gen_actual(interval);
  this->interval_counter = 0;
  this->level_value = 0;
  this->extra = NULL;

  return this;
}

static bool gen_eq(Follow *this, Follow *other) {
  return gen_actual(this->interval) == gen_actual(other->interval) &&
    gen_actual(this->length) == gen_actual(other->length) &&
    gen_actual(other->level) == gen_actual(other->level);
}

static void prepare(Follow *this) {
  if (this->extra) {
    THROW exc_not_null_pointer("this") _THROW
  }
  this->cash = 0.0;
  this->last_nick = NICKS_NUMBER - 1;
  this->interval_counter = 0;
  this->level_value = NICKS_NUMBER * gen_actual(this->level) / 100.0;

  Arr/*_Follow*/ *extra = arr_new();
  RANGE0(i, NICKS_NUMBER) {
    arr_add(extra, _follow_new(i, this->closes_size));
  }_RANGE

  this->extra = extra;
}

static void reset(Follow *this) {
  this->extra = NULL;
}

static void process(
  Follow *this,
  Flea *f,
  size_t nick,
  Quote *q
) {
  struct _Follow *follow = arr_get(this->extra, nick);
  double close = quote_close(q);
  if (!udcalc2_add(&follow->inc, follow->calc, close)) {
    return;
  }

  if (this->interval_counter >= this->interval_value) {
    if (!nick) {
      this->cash = flea_cash(f);
    }

    if (close > 0) {
      size_t stocks = portfolio_get(flea_portfolio(f), nick);
      if (stocks) {
        this->cash += stocks * close;
      }
    }

    if (nick == this->last_nick) {
      this->interval_counter = 0;

      FNE(sort, struct _Follow, f1, f2) {
        return f2->inc > f1->inc;
      }_FN
      arr_sort(this->extra, sort);

      Arr/*size_t*/ *nicks_to_buy = arr_new();
      size_t *nick_to_buy;
      size_t up_level = this->level_value + (int)(this->cash / flea_bet(f));
      if (up_level > this->last_nick) {
        up_level = this->last_nick + 1;
      }
      if (up_level > arr_size(this->extra)) {
        THROW
          "up_level(%d) > arr_size(this->extra)(%d)",
          up_level, arr_size(this->extra)
        _THROW
      }
      RANGE(i, this->level_value, up_level) {
        struct _Follow *follow = arr_get(this->extra, i);
        if (follow->inc < -1) {
          break;
        }
        nick_to_buy = MALLOC(size_t);
        *nick_to_buy = follow->nick;
        arr_add(nicks_to_buy, nick_to_buy);
      }_RANGE

      EACH(flea_portfolio(f), Pentry, e) {
        bool contains = false;
        size_t pnick = portfolio_nick(e);
        EACH(nicks_to_buy, size_t, nick) {
          if (*nick == pnick) {
            contains = true;
            break;
          }
        }_EACH
        if (!contains) {
          sells_add(flea_sells(f), pnick, portfolio_stocks(e));
        }
      }_EACH

      EACH(nicks_to_buy, size_t, nick) {
        bool contains = false;
        EACH(flea_portfolio(f), Pentry, e) {
          if (*nick == portfolio_nick(e)) {
            contains = true;
            break;
          }
        }_EACH
        if (!contains) {
          buys_add(flea_buys(f), *nick, flea_bet(f));
        }
      }_EACH
    }
  } else {
    if (nick == this->last_nick) {
      ++this->interval_counter;
    }
  }
}

static Json *trace_data(Follow *this, size_t nick) {
  struct _Follow *follow = arr_get(this->extra, nick);

  Arr/*Json*/ *jsr = arr_new();

  Arr/*Json*/ *closes = arr_new();
  double *cls = udcalc2_values(follow->calc);
  REPEAT(this->closes_size){
    jarr_adouble(closes, *cls++, 4);
  }_REPEAT
  jarr_aarray(jsr, closes);

  jarr_adouble(jsr, follow->inc, 4);

  return json_warray(jsr);
}

static Follow *mutate(Follow *this) {
  Gen *interval = gen_mutate(this->interval);
  Gen *length = gen_mutate(this->length);
  Gen *level = gen_mutate(this->level);

  return new(interval, length, level);
}

static Json *serialize(Follow *this) {
  Arr/*Json*/ *a = arr_new();

  arr_add(a, gen_serialize(this->interval));
  arr_add(a, gen_serialize(this->length));
  arr_add(a, gen_serialize(this->level));

  return json_warray(a);
}

static Flea *mk_fextra(Flea *f, Follow *this) {
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

Flea *follow_new(Flea *f) {
  Gen *interval = gen_new(176);
  Gen *length = gen_new(176);
  Gen *level = gen_new(26);

  return mk_fextra(f, new(interval, length, level));
}

Flea *follow_restore(Flea *f, Json *serial) {
  Arr/*Json*/ *a = json_rarray(serial);

  uint i = 0;
  Gen *interval = gen_restore(arr_get(a, i++));
  Gen *length = gen_restore(arr_get(a, i++));
  Gen *level = gen_restore(arr_get(a, i++));

  return mk_fextra(f, new(interval, length, level));
}

