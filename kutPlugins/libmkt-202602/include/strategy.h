// Copyright 09-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Market strategy.

#ifndef MODEL_STRATEGY_H
  #define MODEL_STRATEGY_H

#include "exp.h"
#include "cquotes.h"

/// Returns the result of a simulation:
///   (Exp<[ <order>.],  [f.], [f.], [f.], [f.],  f, [[s.].], [[s.].], [f. ]]>)
///     which is (Exp<<stRs>>)
///   params: Arr<<cquotes>, [[f.].]>
///             cqts      : Quotes to perform the simulation.
///             References: Companies references (days x cos).
Exp *libmkt_strategy_open (Arr *params);

/// Returns the result of a simulation. The result is:
///   [0] sales (f)   : Number of sales.
///   [1] assets (f)  : cash + withdrawals + portfolio evaluated with closes.
///   [2] accs (f)    : cash + withdrawals + portfolio evaluated with prices.
///   [3] rfAssets (f): cash + withdrawals + portfolio evaluated with references.
///
///   qts       : Company quotes.
///   references: Company references (days x cos).
double *libmkt_strategy_open_simple_c (Quotes *qts, double **references);

/// Returns the result of a simulation.
/// Returns the the following Exp<{f.}>:
///   - sales (f)   : Number of sales.
///   - assets (f)  : cash + withdrawals + portfolio evaluated with closes.
///   - accs (f)    : cash + withdrawals + portfolio evaluated with prices.
///   - rfAssets (f): cash + withdrawals + portfolio evaluated with references.
///   params: Arr<<cquotes>, [[f.].]>
///             cqts      : Quotes to perform the simulation.
///             References: Companies references (days x cos).
Exp *libmkt_strategy_open_simple (Arr *params);

/// Returns the result of a simulation. The result is:
///   [0] sales (f)   : Number of sales.
///   [1] assets (f)  : cash + withdrawals + portfolio evaluated with closes.
///   [2] accs (f)    : cash + withdrawals + portfolio evaluated with prices.
///   [3] rfAssets (f): cash + withdrawals + portfolio evaluated with references.
///   [4] Profits ([n.])  : Profits ratio average. Portfolio evaluated with Closes.
///                         One entry for each Parameter in the same order.
///   [5] RfProfits ([n.]): Profits ratio average. Portfolio evaluated with references.
///                         One entry for each Parameter in the same order.
///
///   qts       : Company quotes.
///   references: Company references (days x cos).
double *libmkt_strategy_open_simple2_c (Quotes *qts, double **references);

/// Returns the result of a simulation.
/// Returns the the following Exp<{f.}>:
///   - sales (f)   : Number of sales.
///   - assets (f)  : cash + withdrawals + portfolio evaluated with closes.
///   - accs (f)    : cash + withdrawals + portfolio evaluated with prices.
///   - rfAssets (f): cash + withdrawals + portfolio evaluated with references.
///   - Profits ([n.])  : Profits ratio average. Portfolio evaluated with Closes.
///                       One entry for each Parameter in the same order.
///   - RfProfits ([n.]): Profits ratio average. Portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
///   params: Arr<<cquotes>, [[f.].]>
///             cqts      : Quotes to perform the simulation.
///             References: Companies references (days x cos).
Exp *libmkt_strategy_open_simple2 (Arr *params);

/// Returns the result of a simulation. The result is:
/// Returns the the following 'dic'.
///   [1] Sales ([f.])   : Number of sales.
///                      One entry for each Parameter in the same order.
///   [2] Assets ([f.])  : cash + withdrawals + portfolio evaluated with closes.
///                      One entry for each Parameter in the same order.
///   [3] Accs ([f.])     : cash + withdrawals + portfolio evaluated with prices.
///                      One entry for each Parameter in the same order.
///   [4] RfAssets ([f.]): cash + withdrawals + portfolio evaluated with references.
///                      One entry for each Parameter in the same order.
///
/// Params:
///   mdId: Module identifier.
///   qts       : Company quotes.
///   params: Module parameters (Arr<double>).
double **libmkt_strategy_group_c (char *mdId, Quotes *qts, Arr *params);

/// Returns the result of a simulation.
/// Returns the the following 'dic'.
///   - Sales ([f.])   : Number of sales.
///                      One entry for each Parameter in the same order.
///   - Assets ([f.])  : cash + withdrawals + portfolio evaluated with closes.
///                      One entry for each Parameter in the same order.
///   - Accs ([f.])     : cash + withdrawals + portfolio evaluated with prices.
///                      One entry for each Parameter in the same order.
///   - RfAssets ([f.]): cash + withdrawals + portfolio evaluated with references.
///                      One entry for each Parameter in the same order.
///   params: Arr<s, <cquotes>, [[f.].]>
///             md      : Model.
///             cqts    : Quotes to perform the simulation.
///             Params  : Array of model parameters.
Exp *libmkt_strategy_group (Arr *params);

/// Returns the result of a simulation. The result is:
/// Returns the the following 'dic'.
///   [1] Sales ([f.])   : Number of sales.
///                      One entry for each Parameter in the same order.
///   [2] Assets ([f.])  : cash + withdrawals + portfolio evaluated with closes.
///                      One entry for each Parameter in the same order.
///   [3] Accs ([f.])     : cash + withdrawals + portfolio evaluated with prices.
///                      One entry for each Parameter in the same order.
///   [4] RfAssets ([f.]): cash + withdrawals + portfolio evaluated with references.
///                      One entry for each Parameter in the same order.
///   [5] Profits ([f.])  : Profits ratio average. Portfolio evaluated with Closes.
///                       One entry for each Parameter in the same order.
///   [6] RfProfits ([f.]): Profits ratio average. Portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
///
/// Params:
///   mdId: Module identifier.
///   qts       : Company quotes.
///   params: Module parameters (Arr<double>).
double **libmkt_strategy_group2_c (char *mdId, Quotes *qts, Arr *params);


/// Returns the result of a simulation.
/// Returns the the following 'dic'.
///   - Sales ([f.])    : Number of sales.
///                       One entry for each Parameter in the same order.
///   - Assets ([f.])   : cash + withdrawals + portfolio evaluated with closes.
///                       One entry for each Parameter in the same order.
///   - Accs ([f.])     : cash + withdrawals + portfolio evaluated with prices.
///                      One entry for each Parameter in the same order.
///   - RfAssets ([f.]) : cash + withdrawals + portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
///   - Profits ([f.])  : Profits ratio average. Portfolio evaluated with Closes.
///                       One entry for each Parameter in the same order.
///   - RfProfits ([f.]): Profits ratio average. Portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
///   params: Arr<s, <cquotes>, [[f.].]>
///             md      : Model.
///             cqts    : Quotes to perform the simulation.
///             Params  : Array of model parameters.
Exp *libmkt_strategy_group2 (Arr *params);


#endif
