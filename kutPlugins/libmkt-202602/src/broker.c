// Copyright 09-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "broker.h"

double broker_buy_fees (double amount) {
  return (amount > 50000.0          // broker
      ? amount * 0.001
      : 9.75
    ) +
    1 +                      // market
    0.11 +                   // Execution fee
    amount * 0.004           // tobin + penalty
  ;
}

/// Returns net cost of operation (cost + fees).
///   stocks Stocks number.
///   price Stock price.
double broker_buy (int stocks, double price) {
  double amount = stocks * price;
  return amount + broker_buy_fees(amount);
}

/// Returns total fees of a sale operation.
///   amount: Operation amount.
double broker_sell_fees (double amount) {
  return (amount > 500000.0         // broker
      ? amount * 0.001
      : 9.75
    ) +
    1 +                      // market
    0.11 +                   // Execution fee
    amount * 0.002           // penalty
  ;
}

/// Returns net cost of operation (cost - fees).
///   stocks Stocks number.
///   price Stock price.
double broker_sell (int stocks, double price) {
  double amount = stocks * price;
  return amount - broker_sell_fees(amount);
}
