// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "League.h"
#include "match.h"

/* .
-League: to
  # Arr[char]
  nicks: Arr - char *
  # Arr[char] Each char can be '1', 'x', '2' or '-', corresponding with
  # first player win, draw, second player win or 'real' player rests.
  results: Arr - char *
*/
/*--*/

struct League_League {
  Arr *nicks;
  Arr *results;
};

static League *_league_new (Arr *nicks, Arr *results) {
  League *this = MALLOC(League);
  this->nicks = nicks;
  this->results = results;
  return this;
}

Arr *league_nicks (League *this) {
  return this->nicks;
}

Arr *league_results (League *this) {
  return this->results;
}

Js *league_to_js (League *this) {
  // Arr[Js]
  Arr *js = arr_new();
  arr_push(js, arr_to_js(this->nicks, (FTO)js_ws));
  arr_push(js, arr_to_js(this->results, (FTO)js_ws));
  return js_wa(js);
}

/*--*/

// Nicks is Arr[char] and quotes is Arr[Darr]
League *league_new(Arr *nicks, Arr *quotes) {
  // Arr[char]
  Arr *results = arr_new();

  int odd = 0;
  int nplayers = arr_size(nicks);
  if (nplayers % 2 == 1) {
    odd = 1;
    ++nplayers;
  }

  RANGE0(round, nplayers - 1)
    arr_push(results, match_round_results(odd, nplayers, round, quotes));
  _RANGE

  return _league_new(nicks, results);
}
