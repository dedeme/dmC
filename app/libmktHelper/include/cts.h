// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Gobal constants.

#ifndef CTS_H
  #define CTS_H

/// Number of expected quotes in 'NICK'.tb files.
#define cts_historic_quotes 610

/// First real reference date index (from before to after) = 610 - 350.
/// After this index it remains the last 350 dates.
#define cts_real_reference_date_ix 260

/// Simulation initial capital.
#define cts_initial_capital 250000.0

/// Minimum cash to can bet.
#define cts_min_to_bet 13000.0

/// Bet amount.
#define cts_bet 12000.0

/// Quarantine days (5 days per week -> 9 weeks).
#define cts_days_loss 45

/// Quarantine limit ('sell >= buy * no_loss_multiplicator' is ok).
#define cts_no_loss_multiplicator 1.02

/// Limit to withdrawal cash (cts_initialCapital + cts_bet + cts_bet).
#define cts_withdrawal_limit 274000.0

/// Order type 'buy'.
#define cts_order_buy 0

/// Order type 'sell'.
#define cts_order_sell 1

/// Returns constants JSONized in an object with the following fields:
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
char *cts_to_js(void);

#endif
