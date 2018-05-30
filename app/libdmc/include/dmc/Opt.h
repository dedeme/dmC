// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt is a structure for use nullable values.

#ifndef DM_OPT_H
  #define DM_OPT_H

#include <stdbool.h>

///
typedef struct opt_Opt Opt;

///
Opt *opt_null(void);

/// opt_new throws exception if 'v' is null.
/// Throws exception
Opt *opt_new(void *v);

///
bool opt_is_null(Opt *this);

/// opt_value trhows exception if 'this' value is null.
/// Throws exception.
void *opt_value(Opt *this);

#endif
