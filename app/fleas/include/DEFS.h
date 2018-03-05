// Copyright 15-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants

#ifndef DEFS_H
  # define DEFS_H

///
#define DATA_VERSION "fleas\nData version: 201712\n"

///
#define QUOTES_NUMBER 550

///
#define NICKS_NUMBER 70

/// Speed to weight moving average of incomes, buys and sells
#define STAT_INTERVAL 10

/// Minimal cycles to be in bests
#define TEST_INTERVAL 10

/// Mininal buy operations to survive
#define MIN_BUYS 45

///
#define INITIAL_CASH 125000

///
#define FLEAS_NUMBER 5000

///
enum Company_property {
  C_STATUS,
  C_SELECTED,
  C_IBEX,
  C_INVERTIA,
  C_INFOMERCADOS
};

///
enum Family_option {
  FAMILIES_ALL,
  FAMILY_BEST,
  FAMILY_SEL_BEST
};

///
enum Family_type {
  BUY_AND_HOLD,
  UP_DOWN,
  MOVING_AVERAGE,
  WMOVING_AVERAGE,
  RSI,
  FOLLOW,
  FOLLOW_MA,
  FOLLOW_WMA,
  FAMILIES_END
};

///
#define BESTS_NUMBER 100

///
#define BESTS_FAMILIES_NUMBER 10

/// WORSTS_NUMBER must be greater or equals as BESTS_NUMBER
#define WORSTS_NUMBER (200 + FAMILIES_END * BESTS_FAMILIES_NUMBER)

#endif
