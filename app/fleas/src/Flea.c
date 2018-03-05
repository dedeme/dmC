// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>
#include "market/Buy.h"
#include "market/Sell.h"
#include "market/Pf.h"
#include "Flea.h"
#include "DEFS.h"
#include "Db.h"
#include "Gen.h"
#include "Options.h"
#include "Family.h"
#include "Trace.h"
#include "fextra.h"

struct _Flea {
  size_t age;
  Arr/*Buy*/ *buys;
  Arr/*Sell*/ *sells;
  double cash;
  Pf *portfolio;
  size_t nbuys;
  size_t nsells;
};

struct flea_Flea {
  size_t id;
  size_t cycle;
  Family *family;
  Gen *bet; // 5000 + v * 1000
  Gen *ibex; // 0 -> Selected; 1 -> Ibex; 2-> No ibex; 3 -> Mix
  Stat *stats;

  struct _Flea *extra;

  void *family_extra;
  bool (*gen_eq)(void *, void *);
  void (*prepare)(void *);
  void (*reset)(void *);
  void (*process)(void *, Flea *, size_t, Quote *);
  Json *(*trace_data)(void *, size_t);
  void *(*mutate)(void *);
  Json *(*serialize)(void *);
};

static bool gen_eq(void *this_family_extra, void *other_family_extra) {
  THROW "prepare is not initialized" _THROW
  return false;
}

static void prepare(void *family_extra) {
  THROW "prepare is not initialized" _THROW
}

static void reset(void *family_extra) {
  THROW "reset is not initialized" _THROW
}

static void process(void *family_extra, Flea *f, size_t s, Quote *q) {
  THROW "process is not initialized" _THROW
}

static Json *trace_data(void *family_extra, size_t s) {
  THROW "trace_data is not initialized" _THROW
  return NULL;
}

static void *mutate(void *family_extra) {
  THROW "mutate is not initialized" _THROW
  return NULL;
}

static Json *serialize(void *family_extra) {
  THROW "serialize is not initialized" _THROW
  return NULL;
}

static Flea *new(
  size_t id,
  size_t cycle,
  Family *family,
  Gen *bet,
  Gen *ibex,
  Stat *stats
) {
  Flea *this = MALLOC(Flea);
  this->id = id;
  this->cycle = cycle;
  this->family = family;
  this->bet = bet;
  this->ibex = ibex;
  this->stats = stats;

  this->extra = NULL;

  this->family_extra = NULL;
  this->gen_eq = gen_eq;
  this->prepare = prepare;
  this->reset = reset;
  this->process = process;
  this->trace_data = trace_data;
  this->mutate = mutate;
  this->serialize = serialize;

  return this;
}

Flea *flea_new(size_t id, size_t cycle) {
  Options *opts = options_get();
  Flea *r = new(
    id,
    cycle,
    options_best(opts)
      ? options_sel(opts)
        ? family_new(FAMILY_SEL_BEST)
        : family_new(FAMILY_BEST)
      : family_new(FAMILIES_ALL),
    gen_new(11),            // bet
    options_sel(opts) ? gen_new(1) : gen_new(4),
    stat_new()
  );

  return fextra_new(r);
}

bool flea_gen_eq(Flea *this, Flea *other) {
  return family_eq(this->family, other->family) &&
    gen_actual(this->bet) == gen_actual(other->bet) &&
    gen_actual(this->ibex) == gen_actual(other->ibex) &&
    this->gen_eq(this->family_extra, other->family_extra);
}

Flea *flea_mutate(Flea *this, size_t id, size_t cycle) {
  Family *family = family_copy(this->family);
  Gen *bet = gen_mutate(this->bet);
  Gen *ibex = gen_mutate(this->ibex);
  Stat *stats = stat_new();

  Flea *f = fextra_new(new(
    id,
    cycle,
    family,
    bet,
    ibex,
    stats
  ));
  f->family_extra = f->mutate(f->family_extra);
  return f;
}

Flea *flea_fextra(
  Flea *this,
  void *data,
  bool (*gen_eq)(void *, void *),
  void (*prepare)(void *),
  void (*reset)(void *),
  void (*process)(void *, Flea *, size_t, Quote *),
  Json *(*trace_data)(void *, size_t),
  void *(*mutate)(void *),
  Json *(*serialize)(void *)
) {
  this->family_extra = data;
  this->gen_eq = gen_eq;
  this->prepare = prepare;
  this->reset = reset;
  this->process = process;
  this->trace_data = trace_data;
  this->mutate = mutate;
  this->serialize = serialize;
  return this;
}

void flea_prepare(Flea *this, size_t cycle){
  if (this->extra) {
    THROW exc_not_null_pointer("this") _THROW
  }
  struct _Flea *extra = MALLOC(struct _Flea);
  extra->age = cycle - this->cycle;
  extra->buys = arr_new();
  extra->sells = arr_new();
	extra->cash = INITIAL_CASH;
  extra->portfolio = pf_new();
	extra->nbuys = 0;
	extra->nsells = 0;

  this->extra = extra;

  this->prepare(this->family_extra);
}

void flea_reset(Flea *this) {
  if (this->extra) {
    this->extra = NULL;
  }
  this->reset(this->family_extra);
}

inline
size_t flea_id(Flea *this) {
  return this->id;
}

inline
size_t flea_cycle(Flea *this) {
  return this->cycle;
}

inline
Family *flea_family(Flea *this) {
  return this->family;
}

inline
double flea_bet(Flea *this) {
  return 5000 + gen_actual(this->bet) * 1000;
}

inline
size_t flea_ibex(Flea *this) {
  return gen_actual(this->ibex);
}

inline
Stat *flea_stats(Flea *this) {
  return this->stats;
}

inline
Arr/*Buy*/ *flea_buys(Flea *this) {
  return this->extra->buys;
}

inline
Arr/*Sell*/ *flea_sells(Flea *this){
  return this->extra->sells;
}

inline
double flea_cash(Flea *this) {
  return this->extra->cash;
}

inline
Pf *flea_portfolio(Flea *this) {
  return this->extra->portfolio;
}

inline
size_t flea_nbuys(Flea *this) {
  return this->extra->nbuys;
}

inline
size_t flea_nsells(Flea *this) {
  return this->extra->nsells;
}

inline
static Json *family_trace_data(Flea *this, size_t nick) {
  return this->trace_data(this->family_extra, nick);
}

void flea_process(
  Flea *this,
  Db *db,
  char *date,
  Quote **day,
  int traced,
  Arr/*Trace*/ *traces
) {
  Nicks *nicks = db_nicks(db);

  if (traced != this->id) {
    traced = 0;
  }

  EACH(this->extra->sells, Sell, s) {
    double cash = 0;
    double open = quote_open(day[sell_nick(s)]);
    if (open > 0) {
      cash = sell_do(s, open);
    }
    if (cash > 0) {
      size_t nick = sell_nick(s);
      Pf *portfolio = this->extra->portfolio;

      double beforeCash;
      Pf *beforePortfolio;
      if (traced) {
        beforeCash = this->extra->cash;
        beforePortfolio = pf_copy(portfolio);
      }

      pf_remove(this->extra->portfolio, nick, sell_stocks(s));
      this->extra->cash += cash;
      ++this->extra->nsells;

      if (traced) {
        Pf *afterPortfolio;
        afterPortfolio = pf_copy(portfolio);

        Json *fdata = family_trace_data(this, nick);

        Trace *tr = trace_new(
          date,
          day[nick],
          beforeCash,
          beforePortfolio,
          nick_id(nicks_get(nicks, nick)),
          0,
          sell_stocks(s),
          this->extra->cash,
          afterPortfolio,
          fdata
        );
        arr_add(traces, tr);
      }
    }
  }_EACH
  this->extra->sells = arr_new();

  arr_shuffle(this->extra->buys);
  EACH(this->extra->buys, Buy, b) {
    if (buy_money(b) <= this->extra->cash) {
      size_t stocks = 0;
      double cost = 0.0;
      double open = quote_open(day[buy_nick(b)]);
      if (open > 0) {
        buy_do(&stocks, &cost, b, open);
      }
      if (stocks > 0) {
        size_t nick = buy_nick(b);
        Pf *portfolio = this->extra->portfolio;

        double beforeCash;
        Pf *beforePortfolio;
        if (traced) {
          beforeCash = this->extra->cash;
          beforePortfolio = pf_copy(portfolio);
        }

        pf_add(portfolio, nick, stocks);
        this->extra->cash -= cost;
        ++this->extra->nbuys;

        if (traced) {
          Pf *afterPortfolio;
          afterPortfolio = pf_copy(portfolio);

          Json *fdata = family_trace_data(this, nick);

          Trace *tr = trace_new(
            date,
            day[nick],
            beforeCash,
            beforePortfolio,
            nick_id(nicks_get(nicks, nick)),
            buy_money(b),
            0,
            this->extra->cash,
            afterPortfolio,
            fdata
          );
          arr_add(traces, tr);
        }
      }
    }
  }_EACH
  this->extra->buys = arr_new();

  RANGE0(nick_ix, NICKS_NUMBER) {
    if (
      (gen_actual(this->ibex) == 0 && !nick_sel(nicks_get(nicks, nick_ix)))
      ||
      (gen_actual(this->ibex) == 1 && !nick_in_ibex(nicks_get(nicks, nick_ix)))
      ||
      (gen_actual(this->ibex) == 2 && nick_in_ibex(nicks_get(nicks, nick_ix)))
    ) {
      continue;
    }

    Quote *quote = day[nick_ix];
    this->process(this->family_extra, this, nick_ix, quote);
  }_RANGE
}

Json *flea_serialize(Flea *this) {
  Arr/*Json*/ *serial = arr_new();

  jarr_auint(serial, this->id);
  jarr_auint(serial, this->cycle);
  arr_add(serial, family_serialize(this->family));
  arr_add(serial, gen_serialize(this->bet));
  arr_add(serial, gen_serialize(this->ibex));
  arr_add(serial, stat_serialize(this->stats));
  arr_add(serial, this->serialize(this->family_extra));

  return json_warray(serial);
}

Flea *flea_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  size_t i = 0;
  size_t id = jarr_guint(serial, i++);
  size_t cycle = jarr_guint(serial, i++);
  Family *family = family_restore(arr_get(serial, i++));
  Gen *bet = gen_restore(arr_get(serial, i++));
  Gen *ibex = gen_restore(arr_get(serial, i++));
  Stat *stats = stat_restore(arr_get(serial, i++));
  Json *fextra = arr_get(serial, i++);
  Flea *r = new(
    id,
    cycle,
    family,
    bet,
    ibex,
    stats
  );

  return fextra_restore(r, fextra);
}

// ---------------------------------------------------------

struct fleas_Fleas {
  size_t size;
  Flea **fs;
};

Fleas *fleas_new(size_t size) {
  Fleas *this = MALLOC(Fleas);
  this->fs = GC_MALLOC(size * sizeof(Flea *));
  Flea **fs = this->fs;
  REPEAT(size) {
    *fs++ = NULL;
  }_REPEAT
  this->size = size;
  return this;
}

inline
size_t fleas_size(Fleas *this) {
  return this->size;
}

inline
void fleas_kill(Fleas *this, size_t ix) {
  *(this->fs + ix) = NULL;
}

inline
Flea *fleas_get(Fleas *this, size_t ix) {
  return *(this->fs + ix);
}

inline
void fleas_set(Fleas *this, size_t ix, Flea *f) {
  *(this->fs + ix) = f;
}

void fleas_sort(Fleas *this) {
  int i, j;
  Flea **f1, **f2, *tmp;
  f1 = this->fs;
  for (i = 0; i < this->size - 1; ++i, ++f1) {
    f2 = this->fs + i + 1;
    for (j = i + 1; j < this->size; ++j, ++f2) {
      if (!*f1) {
        if (*f2) {
          tmp = *f1; *f1 = *f2; *f2 = tmp;
        }
      } else if (
        *f2 &&
        stat_cash(flea_stats(*f2)) > stat_cash(flea_stats(*f1))
      ) {
        tmp = *f1; *f1 = *f2; *f2 = tmp;
      }
    }
  }
}

Json *fleas_serialize(Fleas *this) {
  Arr/*Json*/ *jsa = arr_new();
  RANGE0(i, this->size) {
    Flea *f = fleas_get(this, i);
    if (f) {
      arr_add(jsa, flea_serialize(f));
    } else {
      jarr_anull(jsa);
    }
  }_RANGE
  return json_warray(jsa);
}
