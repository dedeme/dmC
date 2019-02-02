// Copyright 26-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global typedef's and defines

#ifndef DEFS_H
  #define DEFS_H

#include "dmc/std.h"

/// Path to Quotes/data
#define QUOTES_DATA_PATH "/dm/wwwcgi/dmcgi/Quotes/data"

/// Number of cycle to finish a process
#define CYCLES 20

/// Number of fleas per days group
#define FLEAS_PER_GROUP 2000

/// Days waiting before operating
#define DAYS_STAND_BY 125

///
#define MAX_DAYS 120

///
#define MIN_DAYS 20

///
#define MAX_STRIP 0.2

///
#define DAYS_FIRST_GROUP 600

///
#define DAYS_NEXT_GROUP 100

/// Fleas initial capital for each cycle
#define INITIAL_CAPITAL 150000.0

/// Bet
#define BET 15000.0

// Minimum operations to survive (divisor: (days - MAX_DAYS) / MIN_SELLS)
#define MIN_SELLS 9

//Maximun operations to survive (divisor: (days - MAX_DAYS) / MAX_SELLS)
#define MAX_SELLS 6

/// Range of mutation
#define MUTATION_MULTIPLIER 0.3

#endif
