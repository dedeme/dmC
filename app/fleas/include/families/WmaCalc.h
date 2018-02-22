// Copyright 23-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Calculator for weighted moving average

#ifndef WMACALC_H
  # define WMACALC_H

#include <stdlib.h>
#include <stdbool.h>

enum wmacalc_Result {
  WMACALC_NOT_VALID, // Few quotes or value < 0
  WMACALC_BUY, // Buy position
  WMACALC_SELL, // Sell position
  WMACALC_NOP // Position between buy and sell
};

///
typedef struct wmacalc_WmaCalc WmaCalc;

/// Params:
///   avg_len Average length
///   mbuy Buy strip multiplier
///   msell Sell strip multiplier
WmaCalc *wmacalc_new(
  size_t avg_len,
  double mbuy,
  double msell
);

///
double wmacalc_sum(WmaCalc *this);

/// Adds quote to this and returns
///   -2: If data is not prepared to use
///   0 : If quote is not valid (< 0)
///   1 : If
/// Used by MovingAverage
enum wmacalc_Result wmacalc_add(WmaCalc *this, double quote);

#endif

