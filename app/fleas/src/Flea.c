// Copyright 27-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Flea.h"
#include "dmc/Iarr.h"
#include "market/Quote.h"
#include "market/Pf.h"
#include "market/Buy.h"
#include "market/Sell.h"
#include "market/fees.h"
#include "Co.h"
#include "Fresults.h"
#include "DEFS.h"
#include "BuySorter.h"

#include <assert.h>

/* .
-Flea: serial
  id: char *
  gen: Gen *: _s
  ---
  ndays: int: 20 + 80 * gen_get(gen, gen_DAYS)
  buy_strip: double: gen_get(gen, gen_BUY) * 0.2
  sell_strip: double: gen_get(gen, gen_SELL) * 0.2
  # Arr[Co]
  -cos: Arr *: arr_new((FPROC)co_free)
*/

/*--*/

struct flea_Flea {
  char *id;
  Gen *gen;
  int ndays;
  double buy_strip;
  double sell_strip;
  // Arr[Co]
  Arr *cos;
};

Flea *_flea_new(char *id, Gen *gen) {
  Flea *this = malloc(sizeof(Flea));
  this->id = id;
  this->gen = gen;
  this->ndays = MIN_DAYS + (MAX_DAYS - MIN_DAYS) * gen_get(gen, gen_DAYS);
  this->buy_strip = gen_get(gen, gen_BUY) * 0.2;
  this->sell_strip = gen_get(gen, gen_SELL) * 0.2;
  this->cos = arr_new((FPROC)co_free);
  return this;
}

void flea_free(Flea *this) {
  free(this->id);
  free(this->gen);
  arr_free(this->cos);
  free(this);
};

char *flea_id(Flea *this) {
  return this->id;
}

Gen *flea_gen(Flea *this) {
  return this->gen;
}

int flea_ndays(Flea *this) {
  return this->ndays;
}

double flea_buy_strip(Flea *this) {
  return this->buy_strip;
}

double flea_sell_strip(Flea *this) {
  return this->sell_strip;
}

int flea_eq_gen(Flea *this, Flea *other) {
  return gen_eq(this->gen, other->gen);
}

Js *flea_to_js_new(Flea *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->id));
  arr_push(a, gen_to_js_new(this->gen));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Flea *flea_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *id = js_rs_new(arr_get(a, i++));
  Gen *gen = gen_from_js_new(arr_get(a, i++));
  arr_free(a);
  return _flea_new(id, gen);
}

/*--*/

Flea *flea_new(char *id) {
  Gen *g0 = gen_new();
  Gen *g = gen_mutate_new(g0);
  free(g0);
  return _flea_new(id, g);
}

Flea *flea_gen_new(char *id, Gen *gen) {
  return _flea_new(id, gen);
}

Flea *flea_copy_new(Flea *this) {
  return _flea_new(str_new(this->id), gen_copy_new(this->gen));
}

Flea *flea_mutate_new(Flea *this, char *new_id) {
  Gen *g = gen_mutate_new(this->gen);
  return _flea_new(new_id, g);
}

void flea_init(Flea *this, int companies_number) {
  int ndays = this->ndays;
  // Arr[Co]
  Arr *cos = arr_new((FPROC)co_free);
  REPEAT(companies_number)
    arr_push(cos, co_new(ndays));
  _REPEAT
  arr_free(this->cos);
  this->cos = cos;
}

Fresults *flea_process_new(Flea *this, Quote **quotes, int q_ix, int q_end) {
  // Arr[Co]
  Arr *cos = this->cos;
  int size_cos = arr_size(cos);

  if ((q_end - q_ix) % size_cos != 0) {
    FAIL("Wrong blocks in quotes")
  }

  double cash = INITIAL_CAPITAL;
  Pf *pf = pf_new();
  BuySorter *buys = buySorter_new(size_cos);
  Iarr *sells = iarr_new();
  int nbuys = 0;
  int nsells = 0;


  int days_waiting = 0;
  while (q_ix < q_end) {
    if (days_waiting < DAYS_STAND_BY) {
      EACH(cos, Co, co)
        if (co_add(co, quote_close(quotes[q_ix++]))){
          if (co_buying(co)) {
            if (co_up(co, this->buy_strip) > 0){
              co_set_buying(co, 0);
              co_set_ref(co, *co_closes(co));
            }
          } else {
            if (co_down(co, this->sell_strip) > 0){
              co_set_buying(co, 1);
              co_set_ref(co, *co_closes(co));
            }
          }
        }
      _EACH
      ++days_waiting;
      continue;
    }

    // Buy and sell --------------------------------------------------
    // Arr[Buy]
    Arr *foreseen_buys = buySorter_free_new(buys);
    buys = buySorter_new(size_cos);

    // Varr[Buy]
    Varr *actual_buys = varr_new();
    double tmp_cash = cash;
    EACH(foreseen_buys, Buy, b)
      tmp_cash -= buy_do(b, buy_price(b));
      if (tmp_cash < 1000) {
        break;
      }
      varr_push(actual_buys, b);
    _EACH
    EACH(actual_buys, Buy, b)
      double open = quote_open(quotes[q_ix + buy_nick(b)]);
      if (open > 0) {
        cash -= buy_do(b, open);
        pf_add(pf, buy_nick(b), buy_stocks(b), open);
        ++nbuys;
      }
    _EACH

    arr_free(foreseen_buys);
    varr_free(actual_buys);

    Iarr *to_do = iarr_new();
    IEACH(sells, nick)
      int stocks = pf_stocks(pf, nick);
      if (stocks) {
        double open = quote_open(quotes[q_ix + nick]);
        if (open > 0) {
          cash += sell_income(stocks, open);
          pf_remove(pf, nick);
          ++nsells;
        } else {
          iarr_push(to_do, nick);
        }
      }
    _EACH
    iarr_free(sells);
    sells = to_do;

    // Make new orders -----------------------------------------------
    EACH_IX(cos, Co, co, nick)
      double close = quote_close(quotes[q_ix++]);
      if (co_add(co, close)){
        if (co_buying(co)) {
          double ponderation = co_up(co, this->buy_strip);
          if (ponderation > 0){
            co_set_buying(co, 0);
            co_set_ref(co, *co_closes(co));
            buySorter_add(
              buys,
              buy_new(nick, buy_calc(BET, close), close),
              1 / ponderation
            );
          }
        } else {
          if (co_down(co, this->sell_strip) > 0){
            co_set_buying(co, 1);
            co_set_ref(co, *co_closes(co));
            iarr_push(sells, nick);
          }
        }
      }
    _EACH

  }

  // Returns resuls --------------------------------------------------

  int last_day = q_end - size_cos;

  Iarr *pf_nicks = pf_nicks_new(pf);
  IEACH(pf_nicks, nick)
    double last_close = quote_close(quotes[last_day + nick]);
    if (last_close > 0) {
      cash += last_close * pf_stocks(pf, nick);
    } else if (last_close > -1.5) {
      int stocks;
      double price;
      pf_get(&stocks, &price, pf, nick);
      cash += price * stocks;
    }
  _EACH
  iarr_free(pf_nicks);

  Fresults *r = fresults_new(this, cash, nbuys, nsells);

  pf_free(pf);
  // Arr[Buy]
  Arr *tmp = buySorter_free_new(buys);
  arr_free(tmp);
  iarr_free(sells);

  return r;
}
