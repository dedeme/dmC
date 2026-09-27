// Copyright 09-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Market order.

#ifndef MODEL_ORDER_H
  #define MODEL_ORDER_H

#include "exp.h"

typedef enum {
  ORDER_BUY, ORDER_SELL
} OrderT;

///
typedef struct {
  // Order date.
  char *date;
  // Company nick.
  char *nick;
  // ORDER_BUY or ORDER_SELL.
  OrderT type;
  // Stocks number.
  int stocks;
  // Price of each stock.
  double price;
} Order;

/// Constructor.
///   date  : Order date.
///   nick  : Company nick.
///   type  : order.buy or order.sell.
///   stocks: Stocks number.
///   price : Price of each stock.
Order *order_new (
  char *date, char *nick, OrderT type, int stocks, double price
);

/// Returns an Exp<<order>>.
Exp *order_to_exp (Order *this);


#endif
