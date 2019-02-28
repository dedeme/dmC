// Copyright 26-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "calcCharts.h"
#include "io.h"
#include "DEFS.h"

typedef struct chartEntry_ChartEntry ChartEntry;
struct chartEntry_ChartEntry {
  char *date;
  double close;
  double ref;
};

static ChartEntry *chartEntry_new(char *date, double close, double ref) {
  ChartEntry *this = malloc(sizeof(ChartEntry));
  this->date = date;
  this->close = close;
  this->ref = ref;
  return this;
}

static void chartEntry_free(ChartEntry *this) {
  free(this->date);
  free(this);
};

static Js *chartEntry_to_js_new(ChartEntry *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->date));
  arr_push(a, js_wd_new(this->close, 4));
  arr_push(a, js_wd_new(this->ref, 4));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

typedef struct chartsEntry_ChartsEntry ChartsEntry;

struct chartsEntry_ChartsEntry {
  char *nick;
  double profits;
  Arr *quotes;
};

static ChartsEntry *chartsEntry_new(char *nick, double profits, Arr *quotes) {
  ChartsEntry *this = malloc(sizeof(ChartsEntry));
  this->nick = nick;
  this->profits = profits;
  this->quotes = quotes;
  return this;
}

static void chartsEntry_free(ChartsEntry *this) {
  free(this->nick);
  arr_free(this->quotes);
  free(this);
};

static Js *chartsEntry_to_js_new(ChartsEntry *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->nick));
  arr_push(a, js_wd_new(this->profits, 4));
  arr_push(a, arr_to_js_new(this->quotes, (FTO)chartEntry_to_js_new));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

char *calcCharts(Model *fmodel, Rs *rs) {
  int qnicks = io_qnicks();
  double *closes = io_closes();
  double *opens = io_opens();
  // Arr[char]
  Arr *nicks = io_nicks();
  // Arr[char]
  Arr *dates = io_dates();

  Darr *params = rs_params(rs);
  Arr *cos = model_cos_new(fmodel, params, qnicks, closes);

  // Arr[ChartsEntry]
  Arr *charts = arr_new((FPROC)chartsEntry_free);
  EACH_IX(cos, void, co, ixco)
    // Arr[ChartEntry]
    Arr *qs = arr_new((FPROC)chartEntry_free);
    double cash = 0;
    int stocks = 0;
    Order *order = order_none_new();
    int ixq = ixco;
    RANGE0(day, io_qdays())
      char *date = str_new(arr_get(dates, day));
      double close = closes[ixq];
      double ref = model_ref(fmodel, params, co);
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

      if (close >= 0 && ref >= 0) {
        arr_push(qs, chartEntry_new(date, close, ref));
      } else {
        free(date);
      }
      ixq += qnicks;
    _RANGE
    order_free(order);
    ixq -= qnicks;
    if (stocks > 0) {
      cash += BET;
    }

    arr_push(charts, chartsEntry_new(str_new(
      arr_get(nicks, ixco)), cash / BET, qs));
  _EACH

  Js *r = arr_to_js_new(charts, (FTO)chartsEntry_to_js_new);

  arr_free(charts);
  arr_free(cos);

  return (char *)r;
}
