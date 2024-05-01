// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "strategies/open.h"
#include <math.h>
#include "kut/DEFS.h"
#include "kut/js.h"
#include "vec.h"
#include "cts.h"
#include "broker.h"

#include "kut/file.h"

static char *mkOrder(char *date, char *nick, int type, int stocks, double price) {
  // <char>
  Arr *a = arr_new_from(
    js_ws(date),
    js_ws(nick),
    js_wi(type),
    js_wi(stocks),
    js_wf(price, 5),
    NULL
  );
  return js_wa(a);
}

char *open_process (Arr *dates, Arr *cos, Arr *opens, Arr *closes, Arr *refs) {
  int ndates = arr_size(dates);
  int ncos = arr_size(cos);

  // Global simulation.

  double cash = cts_initial_capital;
  double withdrawals = 0;
  // <JS>
  Arr *orders = arr_new();
  // <JS>
  Arr *hreals = arr_new_bf(ndates);
  // <JS>
  Arr *haccs = arr_new_bf(ndates);
  // <JS>
  Arr *hrefs = arr_new_bf(ndates);
  // <JS>
  Arr *hwithdrawals = arr_new_bf(ndates);
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

  // <Arr<JS>>
  Arr *all_buys = arr_new_bf(ncos);
  // <Arr<JS>>
  Arr *all_sales = arr_new_bf(ncos);
  double *prf_cashes = ATOMIC(ncos * sizeof(double));
  int *prf_stocks = ATOMIC(ncos * sizeof(int));
  double *prf_prices = ATOMIC(ncos * sizeof(double));
  int *prf_days_traps = ATOMIC(ncos * sizeof(int));
  double *pprf_cashes = prf_cashes;
  int *pprf_stocks = prf_stocks;
  double *pprf_prices = prf_prices;
  int *pprf_days_traps = prf_days_traps;
  REPEAT(ncos) {
    arr_push(all_buys, arr_new());
    arr_push(all_sales, arr_new());
    *pprf_cashes++ = cts_bet;
    *pprf_stocks++ = 0;
    *pprf_prices++ = 0.0;
    *pprf_days_traps++ = 0;
  }_REPEAT
  // <JS>
  Arr **all_buys_begin = (Arr **)arr_begin(all_buys);
  // <JS>
  Arr **all_sales_begin = (Arr **)arr_begin(all_sales);

  // START -----------------------------

  char **pdates = (char **)arr_begin(dates);
  char **begin_cos = (char **)arr_begin(cos);
  Vec **popens = (Vec **)arr_begin(opens);
  Vec **pcloses = (Vec **)arr_begin(closes);
  Vec **prefs = (Vec **)arr_begin(refs);
  REPEAT(ndates) {
    char *date = *pdates++;
    double *ops = (*popens++)->vs;
    double *cls = (*pcloses++)->vs;
    double *rfs = (*prefs++)->vs;

    double realA = 0;
    double accA = 0;
    double refA = 0;
    char **pcos = begin_cos;
    Arr **pall_buys = all_buys_begin;
    Arr **pall_sales = all_sales_begin;
    RANGE0(ico, ncos) {
      char *nk = *pcos++;
      Arr *buys = *pall_buys++;
      Arr *sales = *pall_sales++;
      double op = *ops++;
      double cl = *cls++;
      double rf = *rfs++;

      --days_traps[ico];
      --prf_days_traps[ico];

      if (to_dos[ico]) {
        if (to_sells[ico]) { // there is buy order.
          // Global simulation.
          if (days_traps[ico] < 1) {
            if (cash > cts_min_to_bet) {
              int stocks_v = (int)(cts_bet / op);
              stocks[ico] = stocks_v;
              prices[ico] = op;
              cash -= broker_buy(stocks_v, op);
              arr_push(orders, mkOrder(date, nk, cts_order_buy, stocks_v, op));
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
            arr_push(buys, js_ws(date));
          }
        } else {
          // Global simulation.
          int stocks_v = stocks[ico];
          if (stocks_v > 0) {
            stocks[ico] = 0;
            cash += broker_sell(stocks_v, op);
            arr_push(orders, mkOrder(date, nk, cts_order_sell, stocks_v, op));

            if (op < prices[ico] * cts_no_loss_multiplicator)
              days_traps[ico] = cts_days_loss;
          }
          // Profits simulation.
          stocks_v = prf_stocks[ico];
          if (stocks_v > 0) {
            prf_stocks[ico] = 0;
            prf_cashes[ico] += broker_sell(stocks_v, op);
            arr_push(sales, js_ws(date));

            if (op < prf_prices[ico] * cts_no_loss_multiplicator)
              prf_days_traps[ico] = cts_days_loss;
          }
        }
        to_dos[ico] = FALSE;
      }

      int stocks_v = stocks[ico];
      if (stocks_v > 0) {
        realA += broker_sell(stocks_v, cl);
        accA += broker_sell(stocks_v, prices[ico]);
        refA += broker_sell(stocks_v, rf < cl ? rf : cl);
      }

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

    double total = cash + realA;
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

    arr_push(hreals, js_wf(cash + withdrawals + realA, 3));
    arr_push(haccs, js_wf(cash + withdrawals + accA, 3));
    arr_push(hrefs, js_wf(cash + withdrawals + refA, 3));
    arr_push(hwithdrawals, js_wf(withdrawals, 3));
  }_REPEAT

  // <JS>
  Arr *prf_ratios = arr_new_bf(ncos);
  double *pcls = ((Vec *)arr_peek(closes))->vs;
  double *pcashes = prf_cashes;
  int *pstks = prf_stocks;
  REPEAT(ncos) {
    arr_push(prf_ratios, js_wf(
      (*pcashes++ + *pstks++ * *pcls++ - cts_bet) / cts_bet, 5
    ));
  }_REPEAT

  cash = cash + withdrawals;

  return js_wo(map_from_array(arr_new_from(
    kv_new("Refs", vec_matrix_to_js(refs, 5)),
    kv_new("Orders", js_wa(orders)),
    kv_new("Hreals", js_wa(hreals)),
    kv_new("Haccs", js_wa(haccs)),
    kv_new("Hrefs", js_wa(hrefs)),
    kv_new("Hwithdrawals", js_wa(hwithdrawals)),
    kv_new("cash", js_wf(cash, 3)),
    kv_new("Buys", js_wa(arr_map(all_buys, (FMAP)js_wa))),
    kv_new("Sales", js_wa(arr_map(all_sales, (FMAP)js_wa))),
    kv_new("Profits", js_wa(prf_ratios)),
    NULL
  )));
}

