// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Order.h"

/* .
-BuyData
  stocks: int
  ponderation: double

===

-Order
  -type: int
  -data_nullable: BuyData *
*/

/*--*/

struct buyData_BuyData {
  int stocks;
  double ponderation;
};

BuyData *_buyData_new(int stocks, double ponderation) {
  BuyData *this = malloc(sizeof(BuyData));
  this->stocks = stocks;
  this->ponderation = ponderation;
  return this;
}

void buyData_free(BuyData *this) {
  free(this);
};

int buyData_stocks(BuyData *this) {
  return this->stocks;
}

double buyData_ponderation(BuyData *this) {
  return this->ponderation;
}

struct order_Order {
  int type;
  BuyData *data_nullable;
};

Order *_order_new(int type, BuyData *data_nullable) {
  Order *this = malloc(sizeof(Order));
  this->type = type;
  this->data_nullable = data_nullable;
  return this;
}

void order_free(Order *this) {
  buyData_free(this->data_nullable);
  free(this);
};

/*--*/

Order *order_none_new(void) {
  return _order_new(0, NULL);
}

Order *order_buy_new(int stocks, double ponderation) {
  return _order_new(1, _buyData_new(stocks, ponderation));
}

Order *order_sell_new(void) {
  return _order_new(2, NULL);
}

int order_is_none(Order *this) {
  return !this->type;
}

int order_is_buy(Order *this) {
  return this->type == 1;
}

int order_is_sell(Order *this) {
  return this->type == 2;
}

BuyData *order_buy_data(Order *this) {
  if (this->type == 1) {
    return this->data_nullable;
  }
  FAIL("Order is not Buy")
  return NULL;
}
