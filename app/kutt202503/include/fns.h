// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global functions.

#ifndef FNS_H
  #define FNS_H

#include "kut/arr.h"

/// Returns a new unique identifier type "'mod'_xxx" where xxx is an integer.
/// \s -> s
char *fns_next_id (char *mod);

/// Generate a function head.
///   with_return: TRUE if function retuns something.
///   pars       : (Arr<char>) Param names.
///   id         : Function name.
char *fns_mk_fn_head (int with_return, Arr *pars, char *id);

/// Returns C code of a file position.
char *fns_mk_pos (char *module_id, int line);

/// Returns the C code in 'fns' joined with '\n' or an empty string if every
/// value of 'fns' is also an empty string.
char *fns_fun_code (Arr *fns);

/// Returns 'msg' cut to 'len' runes.
char *fns_cut_msg (char *msg, int len);

#endif
