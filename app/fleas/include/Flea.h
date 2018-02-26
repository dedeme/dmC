// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Conspicuous investor

#ifndef FLEA_H
  # define FLEA_H

#include <stdlib.h>
#include "DEFS.h"
#include "Quote.h"
#include "Stat.h"
#include "order/Buy.h"
#include "order/Sell.h"
#include "Portfolio.h"

typedef struct db_Db Db;

///
typedef struct flea_Flea Flea;

///
Flea *flea_new(size_t id, size_t cycle);

/// Returns true if 'this' and 'other' have the same gens.
bool flea_gen_eq(Flea *this, Flea *other);

///
Flea *flea_mutate(Flea *this, size_t id, size_t cycle);

/// Returns this modified with family parameters
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
);

/// flea_prepare prepares 'this' to work
void flea_prepare(Flea *this, size_t cycle);

/// fles_reset set memory used by 'flea_prepare' to NULL
void flea_reset(Flea *this);

///
size_t flea_id(Flea *this);

/// flea_cycle is the cycle when 'this' was created
size_t flea_cycle(Flea *this);

///
size_t flea_family(Flea *this);

///
double flea_bet(Flea *this);

/// Values are 0 -> Not in Ibex, 1 -> Yes in Ibex, 2 -> Indifferent
size_t flea_ibex(Flea *this);

///
Stat *flea_stats(Flea *this);

///
Buys *flea_buys(Flea *this);

///
Sells *flea_sells(Flea *this);

///
double flea_cash(Flea *this);

///
Portfolio *flea_portfolio(Flea *this);

///
size_t flea_nbuys(Flea *this);

///
size_t flea_nsells(Flea *this);

///
void flea_process(
  Flea *this,
  Db *db,
  char *date,
  Quote **day,
  int traced,
  Arr/*Trace*/ *traces
);

///
Json *flea_serialize(Flea *this);

///
Flea *flea_restore(Json *serial);


// ---------------------------------------------------------

typedef struct fleas_Fleas Fleas;

///
Fleas *fleas_new(size_t size);

/// fleas_kill set the flea at position 'ix' to NULL. There is no control of
/// range.
void fleas_kill(Fleas *this, size_t ix);

///
size_t fleas_size(Fleas *this);

/// fleas_get returns the flea at position 'ix'. There is no control of range.
Flea *fleas_get(Fleas *this, size_t ix);

///
void fleas_set(Fleas *this, size_t ix, Flea *f);

///
void fleas_sort(Fleas *this);

///
Json *fleas_serialize(Fleas *this);

#endif


