// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Command line options.

#ifndef OPTIONS_H
  # define OPTIONS_H

#include "stdbool.h"

/*.-.*/

///
typedef struct options_Options Options;

///
bool options_best(Options *this);

///
bool options_ibex(Options *this);

/*.-.*/

/// options_init initialize the global variable 'options'. This function only
/// can be called once.
void options_init(bool best, bool ibex);

/// options_get returns the global variable 'options' intialized with
/// 'options_init'
Options *options_get();

#endif

