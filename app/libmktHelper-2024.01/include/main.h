// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program entry.
///
/// OVERVIEW ---------------------------
///
/// kmarketSpeed returns its results by 'stdout'.
/// Warnings are returned by the field "Errors" inside the normal response.
/// Exceptions are returned by 'stderr'.
///
/// CONSTANTS --------------------------
///
/// Returns constants used by 'kmarketSpeed'.
/// Input:
///   - command (s): "constants"
/// Output:
///   - historicQuotes (i): Number of expected quotes in 'NICK'.tb files.
///   - initialCapital (f): Simulation initial capital.
///   - minToBet (f): Minimum cash to can bet.
///   - bet (f): Bet amount.
///   - daysLoss (f): Quarantine days (5 days per week -> 9 weeks).
///   - noLossMultiplicator: Quarantine limit
///                          ('sell >= buy * no_loss_multiplicator' is ok).
///   - withdrawalLimit (f): Limit to withdrawal cash (initialCapital + bet + bet).
///   - orderBuy (i): Order type 'buy'.
///   - orderSell (i): Order type 'sell'.
///
/// READING QUOTES ---------------------
///
/// Returns the quotes requested normalized (removing -1).
/// Input:
///   - command (s)     : "readQuotes"
///   - path (s)        : Directory of quotes.
///   - nquotes (i)     : 2 -> read open and closes.
///                       4 -> read open, close, maxs and mins.
///   - Cos ([s.])      : Companies to read quotes.
/// Output:
///   - Dates ([s.])    : Dates of quotes.
///   - Cos ([s.])      : Companies read.
///   - Opens ([[n.].]) : Opens matrix of Dates(rows)xCos(columns).
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Maxs ([[n.].])  : Maxima matrix of Dates(rows)xCos(columns). (For nquotes == 4)
///   - Mins ([[n.].])  : Minima matrix of Dates(rows)xCos(columns). (For nquotes == 4)
///   - Errors ([s.])   : Detected errors.
/// 'Cos' requested and returnd can not match. In such case one o more error
/// messages are registered in 'Errors'.
///
/// READING VOLUMES --------------------
///
/// Returns the averages of volume for each company in function of indicated
/// samples.
/// Input:
///   - command (s): "volumes"
///   - path (s)   : Directory of quotes.
///   - samples (i): Number of samples to read.
///   - Cos ([s.]) : Companies to read volumes.
/// Output:
///   - Vols ([f.]): Average values for each company in 'Cos'. If a company can
///                  be read, its volume is 0.
///
/// MAKING REFERENCES ------------------
///
/// Returns references for model, parameters and closes.
/// Input:
///   - command (s)     : "mkRefs"
///   - modelId (s)     : Model identifier.
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Params ([n.])   : Model parameters.
/// Output:
///   - Refs [[n.].]: References matrix of Dates(rows)xCos(columns).
///
/// STRATEGY ---------------------------
///
/// Returns complete result of a model with single parameters.
/// Input:
///   - command (s)     : "strategy"
///   - type (s)        : One of "open", "openSimple" or "openSimple2".
///   - modelId (s)     : Model identifier.
///   - Dates ([s.])    : Dates in format YYYYMMDD.
///   - Cos ([s.])      : Company nicks.
///   - Opens ([[n.].]) : Opens matrix of Dates(rows)xCos(columns).
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Maxs ([[n.].])  : Maxima matrix of Dates(rows)xCos(columns).
///                       No used with "open" nor "openSimple".
///   - Mins ([[n.].])  : Minima matrix of Dates(rows)xCos(columns).
///                       No used with "open" nor "openSimple".
///   - Params ([n.])   : Model parameters.
/// Output normal:
///   - Refs ([[n.].])  : References matrix of Dates(rows)xCos(columns).
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
/// Output simple:
/// Returns the the folowing map 'JSONized'.
///   - sales (n)    : Number of sales.
///   - assets (n)   : cash + withdrawals + portfolio evaluated with closes.
///   - refAssets (n): cash + withdrawals + portfolio evaluated with references.
/// Output simple2:
/// Returns the the folowing map 'JSONized'.
///   - sales (n)    : Number of sales.
///   - assets (n)   : cash + withdrawals + portfolio evaluated with closes.
///   - refAssets (n): cash + withdrawals + portfolio evaluated with references.
///   - profits (n)  : Profits ratio average. Portfolio evaluated with Closes.
///   - rfProfits (n): Profits ratio average. Portfolio evaluated with references.
///
/// NOTE:
///   Maxs and Mins are not necessary for "open" nor "openSimple".
///
/// GROUPS -----------------------------
///
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
///   - RefAssets ([n.]): cash + withdrawals + portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
/// Output 2:
/// Returns the the folowing map 'JSONized'.
///   - Sales ([n.])    : Number of sales.
///                       One entry for each Parameter in the same order.
///   - Assets ([n.])   : cash + withdrawals + portfolio evaluated with closes.
///                       One entry for each Parameter in the same order.
///   - RfAssets ([n.]) : cash + withdrawals + portfolio evaluated with references.
///                       One entry for each Parameter in the same order.
///   - Profits ([n.])  : Profits ratio average. Portfolio evaluated with Closes.
///                       One entry for each Parameter in the same order.
///   - RfProfits ([n.]): Profits ratio average. Portfolio evaluated with references.
///                       One entry for each Parameter in the same order.

#ifndef MAIN_H
  #define MAIN_H

///
int main(int argc, char *argv[]);

#endif
