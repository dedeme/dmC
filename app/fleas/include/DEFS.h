// Copyright 27-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Gloabal constants.

#ifndef DEFS_H
  #define DEFS_H

#include "dmc/std.h"

/// Path of directory with quotes data.
#define QUOTES_DATA "/dm/wwwcgi/dmcgi/Quotes/data"
/// Data version of quotes data.
#define DATA_VERSION "Quotes\nData version: 201809\n"

/// Number of fleas per family
#define FLEA_NUMBER_PER_FAMILY 5000
/// Multiplier to flea mutation
#define MUTATION_MULTIPLIER (.7 + rnd_d() * .6)
/// Fleas initial capital for each cycle
#define INITIAL_CAPITAL 120000
/// Number of fleas in ranking
#define RANKING_NUMBER 100
/// Mininal operations to survive
#define MIN_SELLS 45

#endif
