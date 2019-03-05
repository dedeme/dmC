// Copyright 25-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "calcProfits.h"
#include "io.h"
#include "DEFS.h"

static double normalize(double min, double dif, double value) {
  return (value - min) / dif;
}

void calcProfits(Model *fmodel, Rs *rs) {
  int qnicks = io_qnicks();
  double *closes = io_closes();
  double *opens = io_opens();

  Darr *params = rs_params(rs);
  Arr *cos = model_cos_new(fmodel, params, qnicks, closes);

  double sum = 0;
  double mx = -1000;
  double mn = 1000;
  Darr *ratios = darr_new();
  EACH_IX(cos, void, co, ixco)
    double cash = 0;
    int stocks = 0;
    Order *order = order_none_new();
    int ixq = ixco;
    REPEAT(io_qdays())
      if (order_is_sell(order)) {
        double price = opens[ixq];
        if (price < 0) {
          price = closes[ixq - qnicks];
        }
        cash += stocks * price;
        stocks = 0;
      } else if (order_is_buy(order)) {
        BuyData *data = order_buy_data(order);
        stocks = buyData_stocks(data);
        double price = opens[ixq];
        if (price < 0) {
          price = closes[ixq - qnicks];
        }
        cash -= stocks * price;
      }
      order_free(order);
      order = model_order_new(fmodel, params, co, closes[ixq]);
      ixq += qnicks;
    _REPEAT
    order_free(order);
    ixq -= qnicks;
    if (stocks > 0) {
      cash += BET;
    }

    double ratio = cash / BET;
    darr_push(ratios, ratio);
    sum += ratio;
    if (ratio > mx) {
      mx = ratio;
    }
    if(ratio < mn) {
      mn = ratio;
    }
  _EACH

  int cos_size = arr_size(cos);
  double avg = sum / cos_size;
  double dif = mx - mn;

  double avg_n = normalize(mn, dif, avg);

  double sum_n = 0;
  DEACH(ratios, r)
    double d = normalize(mn, dif, r) - avg_n;
    if (d < 0) {
      sum_n -= d;
    } else {
      sum_n += d;
    }
  _EACH
  double mdv = sum_n / cos_size;

  double sel = avg * (1 - mdv);

  rs_set_profits_avg(rs, avg);
  rs_set_profits_mdv(rs, mdv);
  rs_set_profits_sel(rs, sel);

  darr_free(ratios);
  arr_free(cos);
}
