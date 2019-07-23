// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Data set of a league

#ifndef LEAGUE_H
  #define LEAGUE_H

#include "dmc/std.h"

/*--*/

///
///   Arguments:
///     nicks: Arr-char*
///     results: Arr-char*
typedef struct League_League League;

/// Arr[char]
Arr *league_nicks (League *this);

/// Arr[char] Each char can be '1', 'x', '2' or '-', corresponding with
/// first player win, draw, second player win or 'real' player rests.
Arr *league_results (League *this);

///
Js *league_to_js (League *this);

/*--*/

/// nicks is Arr[char] and quotes is Arr[Darr]
League *league_new(Arr *nicks, Arr *quotes);

#endif
