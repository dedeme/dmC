// Copyright 28-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "broker.h"

double broker_buy_fees(double amount) {
  return (amount > 50000      // broker
      ? amount * 0.001
      : 9.75
    ) +
    amount * 0.00003 +       // market
    0.11 +                   // Execution fee
    amount * 0.004           // tobin + penalty
  ;
}

double broker_buy (int stocks, double price) {
  double amount = stocks * price;
  return amount + broker_buy_fees(amount);
}

double broker_sell_fees(double amount) {
  return (amount > 50000      // broker
      ? amount * 0.001
      : 9.75
    ) +
    amount * 0.00003 +       // market
    0.11 +                   // Execution fee
    amount * 0.002           // penalty
  ;
}

double broker_sell (int stocks, double price) {
  double amount = stocks * price;
  return amount - broker_sell_fees(amount);
}
