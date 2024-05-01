// Copyright 30-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hub of strategy groups calculators.


#ifndef STGROUPS_HUB_H
  #define STGROUPS_HUB_H

#include "kut/map.h"
#include "vec.h"

/// Returns strategy results of a model for several parameters.
/// Input:
///   - command (s)     : "group"
///   - type (s)        : One of "open" or "open2".
///   - modelId (s)     : Model identifier.
///   - Opens ([[n.].]) : Opens matrix of Dates(rows)xCos(columns).
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Maxs ([[n.].])  : Maxima matrix of Dates(rows)xCos(columns).
///                       No used with "open" nor "open2".
///   - Mins ([[n.].])  : Minima matrix of Dates(rows)xCos(columns).
///                       No used with "open" nor "open2".
///   - Params ([[n.].]): Array of model parameters.
/// Output normal:
/// Returns the the folowing map 'JSONized'.
///   - Sales ([n.])    : Number of sales.
///                       One entry for each Parameter in the same order.
///   - Assets ([n.])   : cash + withdrawals + portfolio evaluated with closes.
///                       One entry for each Parameter in the same order.
///   - Accs ([n.])     : cash + withdrawals + portfolio evaluated with prices.
///                       One entry for each Parameter in the same order.
///   - RefAssets ([n.]): cash + withdrawals + portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
/// Output 2:
/// Returns the the folowing map 'JSONized'.
///   - Sales ([n.])    : Number of sales.
///                       One entry for each Parameter in the same order.
///   - Assets ([n.])   : cash + withdrawals + portfolio evaluated with closes.
///                       One entry for each Parameter in the same order.
///   - Accs ([n.])     : cash + withdrawals + portfolio evaluated with prices.
///                       One entry for each Parameter in the same order.
///   - RfAssets ([n.]) : cash + withdrawals + portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
///   - Profits ([n.])  : Profits ratio average. Portfolio evaluated with Closes.
///                       One entry for each Parameter in the same order.
///   - RfProfits ([n.]): Profits ratio average. Portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
char *stGroupsHub_run(Map *rq);

#endif
