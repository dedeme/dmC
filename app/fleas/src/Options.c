// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Options.h"
#include "dmc/all.h"

/*
-struct: @Options
  best:bool
  sel:bool
*/

/*.-.*/
struct options_Options {
  bool best;
  bool sel;
};

Options *_options_new(bool best, bool sel) {
  Options *this = MALLOC(Options);
  this->best = best;
  this->sel = sel;
  return this;
}

inline
bool options_best(Options *this) {
  return this->best;
}

inline
bool options_sel(Options *this) {
  return this->sel;
}
/*.-.*/

static Options *options = NULL;

void options_init(bool best, bool ibex) {
  if (options)
    THROW "Variable 'options' was already initialized" _THROW
  options = _options_new(best, ibex);
}

Options *options_get() {
  if (!options)
    THROW "Variable 'options' without initialization" _THROW
  return options;
}
