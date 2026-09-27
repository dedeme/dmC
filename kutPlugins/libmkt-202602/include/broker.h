// Copyright 09-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Interface c-kut.

#ifndef BROKER_H
  #define BROKER_H

/// Returns total fees of a buy operation.
/// amount: Operation amount.
double broker_buy_fees (double amount);

/// Returns net cost of operation (cost + fees).
///   stocks Stocks number.
///   price Stock price.
double broker_buy (int stocks, double price);

/// Returns total fees of a sale operation.
///   amount: Operation amount.
double broker_sell_fees (double amount);

/// Returns net cost of operation (cost - fees).
///   stocks Stocks number.
///   price Stock price.
double broker_sell (int stocks, double price);

#endif
