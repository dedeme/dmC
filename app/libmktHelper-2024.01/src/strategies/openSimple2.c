// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "strategies/openSimple.h"
#include <math.h>
#include "kut/DEFS.h"
#include "kut/js.h"
#include "vec.h"
#include "cts.h"
#include "broker.h"

// Returns [sales, assets, accs, rf_assets, profits, rf_profits]
double *openSimple2_calc (Arr *opens, Arr *closes, Arr *refs) {
  int ndates = arr_size(opens);
  int ncos = ((Vec *)arr_peek(opens))->size;

  // Global simulation.

  double cash = cts_initial_capital;
  double withdrawals = 0;
  int nsales = 0;
  int *stocks = ATOMIC(ncos * sizeof(int));
  double *prices = ATOMIC(ncos * sizeof(double));
  int *to_dos = ATOMIC(ncos * sizeof(int));
  int *to_sells = ATOMIC(ncos * sizeof(int));
  int *days_traps = ATOMIC(ncos * sizeof(int));
  int *pstocks = stocks;
  double *pprices = prices;
  int *pto_dos = to_dos;
  int *pto_sells = to_sells;
  int *pdays_traps = days_traps;
  REPEAT(ncos) {
    *pstocks++ = 0;
    *pprices++ = 0.0;
    *pto_dos++ = FALSE;
    *pto_sells++ = TRUE;
    *pdays_traps++ = 0;
  }_REPEAT

  // Profits simulation.

  double *prf_cashes = ATOMIC(ncos * sizeof(double));
  int *prf_stocks = ATOMIC(ncos * sizeof(int));
  double *prf_prices = ATOMIC(ncos * sizeof(double));
  int *prf_days_traps = ATOMIC(ncos * sizeof(int));
  double *pprf_cashes = prf_cashes;
  int *pprf_stocks = prf_stocks;
  double *pprf_prices = prf_prices;
  int *pprf_days_traps = prf_days_traps;
  REPEAT(ncos) {
    *pprf_cashes++ = cts_bet;
    *pprf_stocks++ = 0;
    *pprf_prices++ = 0.0;
    *pprf_days_traps++ = 0;
  }_REPEAT

  // START -----------------------------

  Vec **popens = (Vec **)arr_begin(opens);
  Vec **pcloses = (Vec **)arr_begin(closes);
  Vec **prefs = (Vec **)arr_begin(refs);
  REPEAT(ndates) {
    double *ops = (*popens++)->vs;
    double *cls = (*pcloses++)->vs;
    double *rfs = (*prefs++)->vs;

    double assets = 0;
    RANGE0(ico, ncos) {
      double op = *ops++;
      double cl = *cls++;
      double rf = *rfs++;

      days_traps[ico] -= 1;
      prf_days_traps[ico] -= 1;

      if (to_dos[ico]) {
        if (to_sells[ico]) { // there is buy order.
          // Global simulation.
          if (days_traps[ico] < 1) {
            if (cash > cts_min_to_bet) {
              int stocks_v = (int)(cts_bet / op);
              stocks[ico] = stocks_v;
              prices[ico] = op;
              cash -= broker_buy(stocks_v, op);
            }
          }
          // Profits simulation.
          if (prf_days_traps[ico] < 1) {
            double prf_cash = prf_cashes[ico];
            int stocks = (int)((prf_cash - broker_buy_fees(prf_cash)) / op);
            double cost = broker_buy(stocks, op);
            while (cost > prf_cash) {
              stocks -= 1;
              cost = broker_buy(stocks, op);
            }
            prf_stocks[ico] = stocks;
            prf_prices[ico] = op;
            prf_cashes[ico] = prf_cash - cost;
          }
        } else {
          // Global simulation.
          int stocks_v = stocks[ico];
          if (stocks_v > 0) {
            stocks[ico] = 0;
            cash += broker_sell(stocks_v, op);
            ++nsales;

            if (op < prices[ico] * cts_no_loss_multiplicator)
              days_traps[ico] = cts_days_loss;
          }
          // Profits simulation.
          stocks_v = prf_stocks[ico];
          if (stocks_v > 0) {
            prf_stocks[ico] = 0;
            prf_cashes[ico] += broker_sell(stocks_v, op);

            if (op < prf_prices[ico] * cts_no_loss_multiplicator)
              prf_days_traps[ico] = cts_days_loss;
          }
        }
        to_dos[ico] = FALSE;
      }

      int stocks_v = stocks[ico];
      if (stocks_v > 0) assets += broker_sell(stocks_v, cl);

      if (to_sells[ico]) {
        if (rf > cl) {
          to_dos[ico] = TRUE;
          to_sells[ico] = FALSE;
        }
      } else if (rf < cl) {
        to_dos[ico] = TRUE;
        to_sells[ico] = TRUE;
      }
    }_RANGE

    double total = cash + assets;
    if (total > cts_withdrawal_limit) {
      double dif = total - cts_initial_capital - cts_bet;
      double securAmount = cts_min_to_bet - cts_bet;
      double withdraw = -1.0;
      if (cash > dif + securAmount) {
        withdraw = dif;
      } else if (cash < cts_min_to_bet) {
        withdraw = floor((cash - securAmount) / cts_bet) * cts_bet;
      }
      if (withdraw > 0) {
        withdrawals += withdraw;
        cash -= withdraw;
      }
    }

  }_REPEAT

  // Global simulation.
  cash += withdrawals;
  double assets = cash;
  double accs = cash;
  double rf_assets = cash;
  int *pstks = stocks;
  double *last_cls = ((Vec *)arr_peek(closes))->vs;
  double *last_prices = prices;
  double *last_rfs = ((Vec *)arr_peek(refs))->vs;
  REPEAT(ncos) {
    int stk = *pstks++;
    double cl = *last_cls++;
    double pr = *last_prices++;
    double rf = *last_rfs++;
    if (stk) {
      assets += broker_sell(stk, cl);
      accs += broker_sell(stk, pr);
      rf_assets += broker_sell(stk, rf);
    }
  }_REPEAT

  // Profits simulation.
  double profits = 0;
  double rf_profits = 0;
  double *pcashes = prf_cashes;
  pstks = prf_stocks;
  last_cls = ((Vec *)arr_peek(closes))->vs;
  last_rfs = ((Vec *)arr_peek(refs))->vs;
  REPEAT(ncos) {
    double cash = *pcashes++;
    int stks = *pstks++;
    double cl = *last_cls++;
    double rf = *last_rfs++;
    if (stks) {
      profits += (cash + stks * cl - cts_bet) / cts_bet;
      rf_profits += (cash + stks * rf - cts_bet) / cts_bet;
    }
  }_REPEAT

  double *r = ATOMIC(5 * sizeof(double));
  r[0] = (double)nsales;
  r[1] = assets;
  r[2] = accs;
  r[3] = rf_assets;
  r[4] = profits / ncos;
  r[5] = rf_profits / ncos;
  return r;
}

char *openSimple2_process (Arr *opens, Arr *closes, Arr *refs) {
  double *r = openSimple2_calc(opens, closes, refs);
  return js_wo(map_from_array(arr_new_from(
    kv_new("sales", js_wf(r[0], 0)),
    kv_new("assets", js_wf(r[1], 3)),
    kv_new("accs", js_wf(r[2], 3)),
    kv_new("rfAssets", js_wf(r[3], 3)),
    kv_new("profits", js_wf(r[4], 5)),
    kv_new("rfProfits", js_wf(r[5], 5)),
    NULL
  )));
}
