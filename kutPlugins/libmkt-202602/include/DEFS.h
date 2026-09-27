// Copyright 05-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DEFS_H
  #define DEFS_H

/// Initial capital.
#define INITIAL_CAPITAL 300000.0

/// Number of quotes in historic tables.
#define libmkt_HISTORIC_QUOTES 610

/// Bet
#define BET 15000.0

/// Minumum to bet
#define MIN_TO_BET 16000.0

/// Maximum purchased companies (= initialCapital / bet).
#define MAX_COS 20

/// Delay days after a sale with losses (in 5 days per week).
#define DAYS_LOSS 45

/// Multiplicator to calculate if should be applied 'daysLoss'.
/// f
#define NO_LOSS_MULTIPLICATOR 1.02

/// Cash limit to do a withdrawal (= initialCapital + bet + bet).
#define WITHDRAWAL_LIMIT 330000.0

#endif
