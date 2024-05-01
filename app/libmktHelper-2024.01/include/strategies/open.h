// Copyright 28-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// 'Open' strategy.
///
/// Use buy prices with open values to make calculations.


#ifndef STRATEGIES_OPEN_H
  #define STRATEGIES_OPEN_H

#include "kut/arr.h"

/// Returns the the folowing map 'JSONized'.
///   - Refs ([[n.].]):
///     References matrix of Dates(rows)xCos(columns).
///   - Orders ([[s, s, n, n, n].]):
///     Orders array. Each order has the following fields:
///       * date (s)  : In format YYYYMMDD.
///       * nick (s)  : Company nick.
///       * type (n)  : 0 -> buy, 1 -> sale.
///       * stocks (n): Number of stocks. Value came from integer.
///       * price (n) : Stock unitary price.
///   - Hassets ([n.]):
///     Assets historic. One value for each date.
///   - Hwithdrawals ([n.]):
///     Withdrawals historic. One value for each date.
///   - cash (n):
///     Final cash + withdrawals.
///   - Buys ([[s.].]):
///     Buy dates. One array for each company (from before to after).
///   - Sales ([[s.].]):
///     Sales dates. One array for each company (from before to after).
///   - Profits ([n.]):
///     Profits ratios. One for each company.
///
///   dates: Arr<char> in format YYYYMMDD.
///   cos  : Arr<char> company nicks.
///   opens: Arr<Vec>.
///   closes: Arr<Vec>.
///   refs: Arr<Vec>.
char *open_process (Arr *dates, Arr *cos, Arr *opens, Arr *closes, Arr *refs);

#endif
