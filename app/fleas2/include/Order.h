// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef ORDER_H
  #define ORDER_H

#include "dmc/std.h"

/*--*/

///
typedef struct buyData_BuyData BuyData;

///
void buyData_free(BuyData *this);

///
int buyData_stocks(BuyData *this);

///
double buyData_ponderation(BuyData *this);

///
typedef struct order_Order Order;

///
void order_free(Order *this);

/*--*/

/// Creates a no order.
Order *order_none_new(void);

/// Creates a buy order
Order *order_buy_new(int stocks, double ponderation);

/// Creates a sell order
Order *order_sell_new(void);

///
int order_is_none(Order *this);

///
int order_is_buy(Order *this);

///
int order_is_sell(Order *this);

///
BuyData *order_buy_data(Order *this);

#endif
