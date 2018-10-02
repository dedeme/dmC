// Copyright 28-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "families/UpDown.h"
#include "Flea.h"
#include "data/BuyRank.h"

enum GenKey {BET, DAYS, BUY_STRIP, SELL_STRIP};

/* .+.
-struct: -Calculator
  -closes: double *
  -max_len: int
  -len: int
  -status: enum flea_BuySellStatus

-struct: -UpDown
  -calculators: Calculator **
  -companies_number: int
  -bet: double
  -buy_strip: double
  -sell_strip: double
*/


/*.-.*/
#include "dmc/ct/Ajson.h"

struct calculator_Calculator {
  double *closes;
  int max_len;
  int len;
  enum flea_BuySellStatus status;
};

typedef struct calculator_Calculator Calculator;

Calculator *calculator_new(
  double *closes,
  int max_len,
  int len,
  enum flea_BuySellStatus status
) {
  Calculator *this = MALLOC(Calculator);
  XNULL(closes)
  this->closes = closes;
  this->max_len = max_len;
  this->len = len;
  this->status = status;
  return this;
}

#include "dmc/ct/Ajson.h"

struct upDown_UpDown {
  Calculator **calculators;
  int companies_number;
  double bet;
  double buy_strip;
  double sell_strip;
};

typedef struct upDown_UpDown UpDown;

UpDown *upDown_new(
  Calculator **calculators,
  int companies_number,
  double bet,
  double buy_strip,
  double sell_strip
) {
  UpDown *this = MALLOC(UpDown);
  XNULL(calculators)
  this->calculators = calculators;
  this->companies_number = companies_number;
  this->bet = bet;
  this->buy_strip = buy_strip;
  this->sell_strip = sell_strip;
  return this;
}
/*.-.*/

UpDown *operator = NULL;

Gen *upDown_prototype(void) {
  double gens[4] = {1000, 20, 0.005, 0.005};
  return gen_new(4, gens);
}

void upDown_init(Gen *gens, int companies_number) {
  int days = (int)gen_get(gens, DAYS);
  Calculator ** calcs = GC_MALLOC(companies_number * sizeof(Calculator *));
  Calculator **p = calcs;
  REPEAT(companies_number) {
    double *closes = ATOMIC(days * sizeof(double));
    *p++ = calculator_new(closes, days, 0, BUY_SELL_NONE);
  }_REPEAT
  double bet = (double)(int)gen_get(gens, BET);
  double buy_strip = gen_get(gens, BUY_STRIP);
  double sell_strip = gen_get(gens, SELL_STRIP);
  operator = upDown_new(calcs, companies_number, bet, buy_strip, sell_strip);
}

void upDown_operate(
  Abuy **buys,
  int **nicks_to_sell,
  int *nicks_to_sel_len,
  Quote **quotes
) {
  int companies_number = operator->companies_number;

  BuyRank *bs = buyRank_new();
  int *ss = ATOMIC(companies_number * sizeof(int));
  int ss_len = 0;

  RANGE0(nick, companies_number){
    double cl = quote_close(quotes[nick]);

    Calculator *calc = operator->calculators[nick];
    int len = calc->len;
    int max_len = calc->max_len;
    double *closes = calc->closes;

    if (len < max_len) {
      closes[len] = cl;
      calc->len = len + 1;
      continue;
    }
    double *p = closes;
    REPEAT(len - 1) {
      *p = *(p + 1);
      ++p;
    }_REPEAT
    *p = cl;

    double before = *closes;
    if (before <= 0 || cl <= 0) {
      continue;
    }

    enum flea_BuySellStatus status = calc->status;

    if (
      status == BUY_SELL_NONE || status == BUY_SELL_BUY
    ) {
      if (cl > before){
        double buy_strip = operator->buy_strip;
        double df = (cl - before) / before;
        if (df > buy_strip) {
          double bet = operator->bet;
          double stocks = buy_calc(bet, cl);
          if (stocks) {
            buyRank_add(bs, buy_new(nick, stocks, cl), df);
          }
          calc->status = BUY_SELL_SELL;
        }
        continue;
      }
      if (status == BUY_SELL_BUY) {
        continue;
      }
    }

    if (before > cl) {
      double sell_strip = operator->sell_strip;
      double df = (before - cl) / before;
      if (df > sell_strip) {
        ss[ss_len++] = nick;
        calc->status = BUY_SELL_BUY;
      }
    }

  }_RANGE

  *buys = buyRank_list(bs);
  *nicks_to_sell = ss;
  *nicks_to_sel_len = ss_len;
}
