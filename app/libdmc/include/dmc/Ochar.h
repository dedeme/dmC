// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt of char *

#ifndef DM_OCHAR_H
  #define DM_OCHAR_H

///
typedef struct achar_Ochar Ochar;

#define TY char
#define CT Ochar
#define FUN(id) ochar ## _ ## id
#include "dmc/tpl/topt.h"
#undef TY
#undef CT
#undef FUN

#endif
