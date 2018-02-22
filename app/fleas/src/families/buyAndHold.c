// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "families/buyAndHold.h"
#include <dm.h>
#include "order/Buy.h"

struct _BuyAndHold {
};

typedef struct _BuyAndHold BuyAndHold;

static BuyAndHold *new(void) {
  return MALLOC(BuyAndHold);
}

static bool gen_eq(BuyAndHold *this, BuyAndHold *other) {
  return false;
}

static void prepare(BuyAndHold *this) {
}

static void reset(BuyAndHold *this) {
}

static BuyAndHold *mutate(BuyAndHold *this) {
  return MALLOC(BuyAndHold);
}

static void process(
  BuyAndHold *this,
  Flea *f,
  size_t nick,
  Quote *q
) {
  if (flea_bet(f) < flea_cash(f)) {
    buys_add(flea_buys(f), nick, flea_bet(f));
  }
}

static Json *trace_data(BuyAndHold *this) {
  return json_wnull();
}

static Json *serialize(BuyAndHold *this) {
  return json_warray(arr_new());
}

static Flea *mk_fextra(Flea *f, BuyAndHold *this) {
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

Flea *buyAndHold_new(Flea *f) {
  return mk_fextra(f, new());
}

inline
Flea *buyAndHold_restore(Flea *f, Json *serial) {
  return mk_fextra(f, new());
}
