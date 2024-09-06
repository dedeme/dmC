// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strategies hub.


#ifndef STRATEGIES_HUB_H
  #define STRATEGIES_HUB_H

#include "kut/map.h"

/// Returns complete result of a model with single parameters.
/// Input:
///   - command (s)     : "strategy"
///   - type (s)        : One of "open", "openSimple" or "openSimple2".
///   - modelId (s)     : Model identifier.
///   - Dates ([s.])    : Dates in format YYYYMMDD.
///   - Cos ([s.])      : Company nicks.
///   - Opens ([[n.].]) : Opens matrix of Dates(rows)xCos(columns).
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Maxs ([[n.].])  : Maxima matrix of Dates(rows)xCos(columns). Optional.
///   - Mins ([[n.].])  : Minima matrix of Dates(rows)xCos(columns). Optional.
///   - Params ([n.])   : Model parameters.
///
/// Output normal:
///   - Refs ([[n.].])  :
///     References matrix of Dates(rows)xCos(columns).
///   - Orders ([[s, s, n, n, n].]):
///     Orders array. Each order has the following fields:
///       * date (s)  : In format YYYYMMDD.
///       * nick (s)  : Company nick.
///       * type (n)  : 0 -> buy, 1 -> sale.
///       * stocks (n): Number of stocks. Value came from integer.
///       * price (n) : Stock unitary price.
///   - Hreals ([n.]):
///     Real assets historic (calculated with closes). One value for each date.
///   - Haccs ([n.]):
///     Accouning assets historic (calculated with buy prices). One value for each date.
///   - Hrefs ([n.]):
///     Reference assets historic (calculated with references). One value for each date.
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
/// Output openSimple:
///   - sales (n)    :  Number of sales.
///   - assets (n)   : cash + withdrawals + portfolio evaluated with closes.
///   - accs (n)     : cash + withdrawals + portfolio evaluated with prices.
///   - refAssets (n): cash + withdrawals + portfolio evaluated with references.
///
/// Output openSimple2:
///   - sales (n)    :  Number of sales.
///   - assets (n)   : cash + withdrawals + portfolio evaluated with closes.
///   - accs (n)     : cash + withdrawals + portfolio evaluated with prices.
///   - refAssets (n): cash + withdrawals + portfolio evaluated with references.
///   - profits (n)  : Profits ratio average. Portfolio evaluated with Closes.
///   - rfProfits (n): Profits ratio average. Portfolio evaluated with references.
///
/// NOTE:
///   Maxs and Mins are not necessary for "open", "openSimple" nor "openSimple2".
char *strategiesHub_run(Map *rq);

#endif
