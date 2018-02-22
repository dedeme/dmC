// Copyright 17-Feb-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Calculator for Follow

#ifndef UDCALC2_H
  # define UDCALC2_H

#include <stdlib.h>
#include <stdbool.h>

///
typedef struct udcalc2_UdCalc2 UdCalc2;

/// Params:
///   ud_len Up-Down length
///   quotes_len Data length
UdCalc2 *udcalc2_new(size_t ud_len, size_t quotes_len);

///
void udcalc2_free(UdCalc2 *this);

///
double *udcalc2_values(UdCalc2 *this);

/// Adds quote to averages and returns
///   true : if comparison is feasible.
///   false: if comparison is not feasible.
/// If retun is true, 'inc' returns increment of (udshort - udlong) / udlong<br>
/// Used by Follow
bool udcalc2_add(double *inc, UdCalc2 *this, double quote);


#endif

