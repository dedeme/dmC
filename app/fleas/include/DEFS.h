// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DEFS_H
  #define DEFS_H

/// Path to Quotes/data
#define QUOTES_DATA_PATH "/dm/wwwcgi/dmcgi/Quotes/data"

/// Number of cycle after insertion to finish a process for parameter
#define CYCLES 5

/// Number of cycle to insert historic results
#define INSERTION_CYCLE 10

/// Number of fleas per days group
#define FLEAS_PER_GROUP 2000

/// Fleas initial capital for each cycle
#define INITIAL_CAPITAL 150000

/// Bet
#define BET 15000

/// Minimun cash to bet
#define MIN_TO_BET 20000

/// Minimum operations to survive (divisor: days / minSells)
#define MIN_SELLS 15

/// Maximun operations to survive (divisor: days / maxSells)
#define MAX_SELLS 6

/// Percentage of max profits to be selectable
#define CUT_PROFITS 0.8

/// Range of mutation
#define MUTATION_MULTIPLIER 0.3

/// Maximun number of fleas in "_best"
#define MAXIMUM_HISTORIC_BESTS 252

#endif
