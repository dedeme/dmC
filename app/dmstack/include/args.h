// Copyright 23-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program arguments management.

#ifndef ARGS_H
  #define ARGS_H

#include "dmc/std.h"

/// If there is any error, show a message and returns '1'.
int args_init (int argc, char *args[]);

/// Returns Opt<char>. Arguments of dmstack.
/// Params are:
///   dms: Dms file.
Opt *args_param (char *key);

/// Returns Arr<char>. Argumentes of dms file.
Arr *args_dms_params (void);

/// Returns 0 if dmstack was called with the option '-d'
int args_is_production (void);

#endif
