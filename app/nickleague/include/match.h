// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// League runner

#ifndef MATCH_H
  #define MATCH_H

#include "dmc/std.h"

/// Play a round and returns its results
///   odd: If the real number of players is odd.
///   nplayers: Convenience number of players. If odd == 1 the player number
///     'len(players) - 1' is a dummy player (is opponet rest that round)
///   round: Round to play.
///   quotes: Quotes to calculate results.
///   return: char[] of length 'nplayers'. Each char can be '1', 'x', '2' or
///     '-', corresponding with first player win, draw, second player win
///     or 'real' player rests.
char *match_round_results(
  int odd,
  int nplayers,
  int round,
  Arr *quotes
);

#endif
