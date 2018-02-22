// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Calculator for FollowMa

#ifndef MACALC2_H
  # define MACALC2_H

#include <stdlib.h>
#include <stdbool.h>

///
typedef struct macalc2_MaCalc2 MaCalc2;

/// Params:
///   short_avg_len Short average length
///   long_avg_len Long average length
///   quotes_len Data length
MaCalc2 *macalc2_new(
  size_t short_avg_len,
  size_t long_avg_len,
  size_t quotes_len
);

///
double macalc2_short_sum(MaCalc2 *this);

///
double macalc2_long_sum(MaCalc2 *this);

///
double *macalc2_short_values(MaCalc2 *this);

///
double *macalc2_long_values(MaCalc2 *this);

/// Adds quote to averages and returns
///   true : if comparison is feasible.
///   false: if comparison is not feasible.
/// If retun is true, 'inc' returns increment of (mshort - mlong) / mlong<br>
/// Used by FollowMa
bool macalc2_add(double *inc, MaCalc2 *this, double quote);


#endif

