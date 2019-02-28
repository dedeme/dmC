// Copyright 25-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "calcAssets.h"
#include "dmc/date.h"
#include "Acc.h"
#include "io.h"
#include "Order.h"
#include "DEFS.h"

typedef struct orderResults_OrderResults OrderResults;

struct orderResults_OrderResults {
  int buys;
  int sells;
};

OrderResults *orderResults_new(int buys, int sells) {
  OrderResults *this = malloc(sizeof(OrderResults));
  this->buys = buys;
  this->sells = sells;
  return this;
}

void orderResults_free(OrderResults *this) {
  free(this);
};

typedef struct buyToDo_BuyToDo BuyToDo;

struct buyToDo_BuyToDo {
  int co;
  int stocks;
  double ponderation;
};

BuyToDo *buyToDo_new(int co, int stocks, double ponderation) {
  BuyToDo *this = malloc(sizeof(BuyToDo));
  this->co = co;
  this->stocks = stocks;
  this->ponderation = ponderation;
  return this;
}

void buyToDo_free(BuyToDo *this) {
  free(this);
};


// orders is Arr[Order]. One for each co.
static OrderResults *do_orders0_new(Acc *acc, int ixco, Arr *orders) {
  double *opens = io_opens();
  OrderResults *r = orderResults_new(0, 0);
  EACH_IX(orders, Order, o, co)
    if (order_is_sell(o)) {
      double price = opens[ixco + co];
      if (price <= 0) {
        price = io_closes()[ixco + co - io_qnicks()];
      }
      r->sells += acc_sell(acc, co, price);
    }
  _EACH
  return r;
}

// orders is Arr[Order]. One for each co.
static OrderResults *do_orders_new(Acc *acc, int ixco, Arr *orders, int nBuys) {
  double *opens = io_opens();
  OrderResults *r = orderResults_new(0, 0);
  // Arr[BuyToDo]
  Arr *buys = arr_new((FPROC)buyToDo_free);
  EACH_IX(orders, Order, o, co)
    if (order_is_sell(o)) {
      double price = opens[ixco + co];
      if (price <= 0) {
        price = io_closes()[ixco + co - io_qnicks()];
      }
      r->sells += acc_sell(acc, co, price);
    } else if (order_is_buy(o)) {
      BuyData *d = order_buy_data(o);
      arr_push(buys, buyToDo_new(
        co, buyData_stocks(d), buyData_ponderation(d)
      ));
    }
  _EACH

  int fsort(BuyToDo *b1, BuyToDo *b2) {
    return b1->ponderation < b2->ponderation;
  }
  arr_sort(buys, (FGREATER)fsort);
  int c = 0;
  EACH_IX(buys, BuyToDo, b, ix)
    if (ix >= nBuys) {
      break;
    }
    int co = b->co;
    double price = opens[ixco + co];
    if (price > 0) {
      acc_buy(acc, co, b->stocks, price);
      ++c;
    }
  _EACH
  r->buys = c;

  arr_free(buys);

  return r;
}

Rs *calcAssets_new(Model *fmodel, Flea *f) {
  int qnicks = io_qnicks();
  double *closes = io_closes();

  Darr *params = model_params_new(fmodel, f);
  Acc *acc = acc_new();
  Arr *cos = model_cos_new(fmodel, params, qnicks, closes);

  int ixco = 0;
  int buys = 0;
  int sells = 0;
  // Arr[Order]
  Arr *orders = arr_new((FPROC)order_free);
  REPEAT(qnicks)
    arr_push(orders, order_none_new());
  _REPEAT

  REPEAT(io_qdays())
    double cash = acc_cash(acc);
    OrderResults *ors = cash < MIN_TO_BET
      ? do_orders0_new(acc, ixco, orders)
      : do_orders_new(acc, ixco, orders, (cash - MIN_TO_BET) / BET + 1);
    buys += ors->buys;
    sells += ors->sells;
    orderResults_free(ors);

    RANGE0(i, qnicks)
      void *co = arr_get(cos, i);
      double close = closes[ixco + i];
      arr_set(orders, i, model_order_new(fmodel, params, co, close));
    _RANGE

    ixco += qnicks;
  _REPEAT

  Rs *r = rs_new(
    date_to_str_new(date_now()), flea_copy_new(f), darr_copy_new(params),
    acc_liquidation(acc), 0, 0, 0, buys, sells
  );

  darr_free(params);
  acc_free(acc);
  arr_free(cos);
  arr_free(orders);

  return r;
}
