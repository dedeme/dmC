// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Calculator for moving average

#ifndef MACALC_H
  # define MACALC_H

#include <stdlib.h>
#include <stdbool.h>

enum macalc_Result {
  MACALC_NOT_VALID, // Few quotes or value < 0
  MACALC_BUY, // Buy position
  MACALC_SELL, // Sell position
  MACALC_NOP // Position between buy and sell
};

///
typedef struct macalc_MaCalc MaCalc;

/// Params:
///   avg_len Average length
///   mbuy Buy strip multiplier
///   msell Sell strip multiplier
///   quotes_len Data length
MaCalc *macalc_new(
  size_t avg_len,
  double mbuy,
  double msell,
  size_t quotes_len
);

///
double macalc_sum(MaCalc *this);

///
double *macalc_values(MaCalc *this);

/// Adds quote to this and returns
///   -2: If data is not prepared to use
///   0 : If quote is not valid (< 0)
///   1 : If
/// Used by MovingAverage
enum macalc_Result macalc_add(MaCalc *this, double quote);

#endif

