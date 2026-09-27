// Copyright 02-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Plugin for Kut.
/// NOTE: It is not a good idea check the number of variables here. It is better
///       to do it in the correponding Kut interface.

#ifndef LIBPLUG_H
  #define LIBPLUG_H

#include "types.h"

/// Constant function
Val libplug_const (Val vs);

/// Procedure test
Val libplug_echo (Val vs);

/// Function test.
Val libplug_sum (Val vs);

#endif
