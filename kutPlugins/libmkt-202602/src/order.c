// Copyright 09-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "order.h"
#include "kut/DEFS.h"

Order *order_new (
  char *date, char *nick, OrderT type, int stocks, double price
) {
  Order *this = MALLOC(Order);
  this->date = date;
  this->nick = nick;
  this->type = type;
  this->stocks = stocks;
  this->price = price;
  return this;
}

/// Returns an Exp<<order>>.
Exp *order_to_exp (Order *this) {
  return exp_array(arr_new_from(
    exp_string(this->date),
    exp_string(this->nick),
    exp_int(this->type),
    exp_int(this->stocks),
    exp_float(this->price)
  ));
};
