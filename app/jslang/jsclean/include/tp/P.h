// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TP_P_H
  #define TP_P_H

/// Utility for file paths.

#include "dmc/std.h"

/*.-.*/

///
typedef struct p_P P;

///
P *p_new(char *absolute, char *relative);

///
char *p_absolute(P *this);

///
char *p_relative(P *this);

/*.-.*/

#define TY P                    // Element type
#define FN p                    // Function prefix
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

#endif


