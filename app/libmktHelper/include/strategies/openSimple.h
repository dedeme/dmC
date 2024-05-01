// Copyright 28-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// 'OpenSimple' strategy.
///
/// Use buy prices with open values to make calculations.


#ifndef STRATEGIES_OPEN_SIMPLE_H
  #define STRATEGIES_OPEN_SIMPLE_H

#include "kut/arr.h"

/// Returns the the folowing values.
///   - rt[0]: sales -> Number of sales.
///   - rt[1]: assets -> cash + withdrawals + portfolio evaluated with closes.
///   - rt[2]: accs -> cash + withdrawals + portfolio evaluated with prices.
///   - rt[3]; refAssets -> cash + withdrawals + portfolio evaluated with references.
///
///   opens: Arr<Vec>.
///   closes: Arr<Vec>.
///   refs: Arr<Vec>.
double *openSimple_calc (Arr *opens, Arr *closes, Arr *refs);

/// Returns the the folowing map 'JSONized'.
///   - sales (n)    :  Number of sales.
///   - assets (n)   : cash + withdrawals + portfolio evaluated with closes.
///   - accs (n)     : cash + withdrawals + portfolio evaluated with prices.
///   - refAssets (n): cash + withdrawals + portfolio evaluated with references.
///
///   opens: Arr<Vec>.
///   closes: Arr<Vec>.
///   refs: Arr<Vec>.
char *openSimple_process (Arr *opens, Arr *closes, Arr *refs);

#endif
