// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "families/followMa.h"
#include <dm.h>
#include "DEFS.h"
#include "Gen.h"
#include "order/Buy.h"
#include "families/MaCalc2.h"

struct _Follow {
  size_t nick;
  MaCalc2 *calc;
  double inc;
};

static struct _Follow *_follow_new(
  size_t nick,
  size_t closes_short_size,
  size_t closes_long_size
) {
  struct _Follow *this = MALLOC(struct _Follow);
  this->nick = nick;
  this->calc = macalc2_new(closes_short_size, closes_long_size, QUOTES_NUMBER);
  this->inc = -10.0;
  return this;
}

struct followma_FollowMa {
  Gen *interval; // 5 + v
  Gen *len_long; // 5 + v
  Gen *len_short; // 5 + v
  Gen *level; // %

  size_t closes_short_size;
  size_t closes_long_size;
  double cash;
  size_t last_nick;
  size_t interval_value;
  size_t interval_counter;
  size_t level_value;
  Arr/*double*/ *extra;
};

typedef struct followma_FollowMa FollowMa;

static FollowMa *new(
  Gen *interval,
  Gen *len_short,
  Gen *len_long,
  Gen *level
) {
  FollowMa *this = MALLOC(FollowMa);
  this->interval = interval;
  this->len_long = len_long;
  this->len_short = len_short;
  this->level = level;

  this->closes_short_size = 5 + gen_actual(len_short);
  this->closes_long_size = 5 + gen_actual(len_long);
  this->cash = 0.0;
  this->last_nick = 0;
  this->interval_value = 5 + gen_actual(interval);
  this->interval_counter = 0;
  this->level_value = 0;
  this->extra = NULL;

  return this;
}

static bool gen_eq(FollowMa *this, FollowMa *other) {
  return gen_actual(this->interval) == gen_actual(other->interval) &&
    gen_actual(this->len_long) == gen_actual(other->len_long) &&
    gen_actual(this->len_short) == gen_actual(other->len_short) &&
    gen_actual(this->level) == gen_actual(other->level);
}

static void prepare(FollowMa *this) {
  if (this->extra) {
    THROW exc_not_null_pointer("this") _THROW
  }

  this->cash = 0.0;
  this->last_nick = NICKS_NUMBER - 1;
  this->interval_counter = 0;
  this->level_value = NICKS_NUMBER * gen_actual(this->level) / 100.0;

  Arr/*_Follow*/ *extra = arr_new();
  RANGE0(i, NICKS_NUMBER) {
    arr_add(extra, _follow_new(
      i, this->closes_short_size, this->closes_long_size
    ));
  }_RANGE

  this->extra = extra;
}

static void reset(FollowMa *this) {
    this->extra = NULL;
}

static void process(
  FollowMa *this,
  Flea *f,
  size_t nick,
  Quote *q
) {
  struct _Follow *follow = arr_get(this->extra, nick);
  double close = quote_close(q);
  if (!macalc2_add(&follow->inc, follow->calc, close)) {
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

static Json *trace_data(FollowMa *this, size_t nick) {
  struct _Follow *follow = arr_get(this->extra, nick);

  Arr/*Json*/ *jsr = arr_new();

  Arr/*Json*/ *closes = arr_new();
  double *cls = macalc2_short_values(follow->calc);
  REPEAT(this->closes_short_size){
    jarr_adouble(closes, (double)*cls++, 4);
  }_REPEAT
  jarr_aarray(jsr, closes);

  closes = arr_new();
  cls = macalc2_long_values(follow->calc);
  REPEAT(this->closes_long_size){
    jarr_adouble(closes, (double)*cls++, 4);
  }_REPEAT
  jarr_aarray(jsr, closes);

  jarr_adouble(jsr, (double)follow->inc, 4);

  return json_warray(jsr);
}

static FollowMa *mutate(FollowMa *this) {
  Gen *interval = gen_mutate(this->interval);
  Gen *len_short = gen_mutate(this->len_short);
  Gen *len_long = gen_mutate(this->len_long);
  while (gen_actual(len_long) == gen_actual(len_short)) {
    len_short = gen_mutate(this->len_short);
    len_long = gen_mutate(this->len_long);
  }
  if (gen_actual(len_long) < gen_actual(len_short)) {
    Gen *tmp = len_long;
    len_long = len_short;
    len_short = tmp;
  }
  Gen *level = gen_mutate(this->level);

  return new(interval, len_short, len_long, level);
}

static Json *serialize(FollowMa *this) {
  Arr/*Json*/ *a = arr_new();

  arr_add(a, gen_serialize(this->interval));
  arr_add(a, gen_serialize(this->len_short));
  arr_add(a, gen_serialize(this->len_long));
  arr_add(a, gen_serialize(this->level));

  return json_warray(a);
}

static Flea *mk_fextra(Flea *f, FollowMa *this) {
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

Flea *followma_new(Flea *f) {
  Gen *interval = gen_new(176);
  Gen *len_short = gen_new(176);
  Gen *len_long = gen_new(176);
  while (gen_actual(len_long) == gen_actual(len_short)) {
    len_short = gen_new(176);
    len_long = gen_new(176);
  }
  if (gen_actual(len_long) < gen_actual(len_short)) {
    Gen *tmp = len_long;
    len_long = len_short;
    len_short = tmp;
  }
  Gen *level = gen_new(26);

  return mk_fextra(f, new(interval, len_short, len_long, level));
}

Flea *followma_restore(Flea *f, Json *serial) {
  Arr/*Json*/ *a = json_rarray(serial);

  uint i = 0;
  Gen *interval = gen_restore(arr_get(a, i++));
  Gen *len_short = gen_restore(arr_get(a, i++));
  Gen *len_long = gen_restore(arr_get(a, i++));
  Gen *level = gen_restore(arr_get(a, i++));

  return mk_fextra(f, new(interval, len_short, len_long, level));
}

