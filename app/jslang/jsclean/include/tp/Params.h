// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TP_PARAMS_H
  #define TP_PARAMS_H

#include "dmc/std.h"

/*.-.*/

///
typedef struct params_Params Params;

///
Params *params_new(Achar *roots, char *target);

///
Achar *params_roots(Params *this);

///
char *params_target(Params *this);

/*.-.*/

#define TY Params                    // Element type
#define FN params                    // Function prefix
#include "dmc/tpl/topt.h"
#undef TY
#undef FN


#endif
