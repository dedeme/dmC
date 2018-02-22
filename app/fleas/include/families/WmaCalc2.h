// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Calculator for FollowWma

#ifndef WMACALC2_H
  # define WMACALC2_H

#include <stdlib.h>
#include <stdbool.h>

///
typedef struct wmacalc2_WmaCalc2 WmaCalc2;

/// Params:
///   short_avg_len Short average length
///   long_avg_len Long average length
///   quotes_len Data length
WmaCalc2 *wmacalc2_new(
  size_t short_avg_len,
  size_t long_avg_len,
  size_t quotes_len
);

///
void wmacalc2_free(WmaCalc2 *this);

///
double wmacalc2_short_sum(WmaCalc2 *this);

///
double wmacalc2_long_sum(WmaCalc2 *this);

/// Adds quote to averages and returns
///   true : if comparison is feasible.
///   false: if comparison is not feasible.
/// If retun is true, 'inc' returns increment of (mshort - mlong) / mlong<br>
/// Used by FollowWma
bool wmacalc2_add(double *inc, WmaCalc2 *this, double quote);


#endif

