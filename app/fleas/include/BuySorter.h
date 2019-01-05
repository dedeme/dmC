// Copyright 28-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Sorter of buys

#ifndef BUYSORTER_H
  #define BUYSORTER_H

#include "dmc/std.h"
#include "market/Buy.h"

///
typedef struct buySorter_BuySorter BuySorter;

///
BuySorter *buySorter_new(int companies_number);

///
void buySorter_add(BuySorter *this, Buy *buy, int ponderation);

/// Returns an Arr[Buy] sorted from greatest to lowest and free resources.<p>
/// The array returned must be freed with arr_free.
Arr *buySorter_free_new(BuySorter *this);

#endif
