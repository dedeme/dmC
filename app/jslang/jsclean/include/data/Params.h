// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DATA_PARAMS_H
  #define DATA_PARAMS_H

#include "dmc/std.h"

/*--*/

/// Main parameters
///   Arguments:
///     roots: Arr-char
///     target: char*
typedef struct Params_Params Params;

///
Params *params_new (Arr *roots, char *target);

/// Arr[Char]
Arr *params_roots (Params *this);

///
char *params_target (Params *this);

/*--*/

/// Kv[Params] Process 'main()' parameters. If there is some error on command
/// line, returns it in 'kv_key'. Otherwise 'kv_key' is an empty string and
/// Params are in 'kv_value;
Kv *params_read (int argc, char *argv[]);

#endif
