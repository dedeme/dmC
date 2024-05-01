// Copyright 28-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Broker management.

#ifndef BROKER_H
  #define BROKER_H

/// Buy fees.
///   amount: Operation amount.
double broker_buy_fees(double amount);

/// Buy net cost.
///   stocks: Stocks to buy.
///   price : Unitary price.
double broker_buy (int stocks, double price);

/// Buy fees.
///   amount: Operation amount.
double broker_sell_fees(double amount);

/// Sell net incomes.
///   stocks: Stocks to sell.
///   price : Unitary price.
double broker_sell (int stocks, double price);

#endif
