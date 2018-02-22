// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Calculator for Up-Down

#ifndef UDCALC_H
  # define UDCALC_H

#include <stdlib.h>
#include <stdbool.h>

enum udcalc_Result {
  UDCALC_NOT_VALID, // value < 0
  UDCALC_BUY, // Buy position
  UDCALC_SELL, // Sell position
  UDCALC_NOP // Position between buy and sell
};

///
typedef struct udcalc_UdCalc UdCalc;

/// Params:
///   ud_len Average length
///   mbuy Buy strip multiplier
///   msell Sell strip multiplier
///   quotes_len Data length
UdCalc *udcalc_new(
  size_t ud_len,
  double mbuy,
  double msell,
  size_t quotes_len
);

///
void udcalc_free(UdCalc *this);

///
double *udcalc_values(UdCalc *this);

/// Adds quote to this and returns
///   -2: If data is not prepared to use
///   0 : If quote is not valid (< 0)
///   1 : If
/// Used by MovingAverage
enum udcalc_Result udcalc_add(UdCalc *this, double quote);

#endif

