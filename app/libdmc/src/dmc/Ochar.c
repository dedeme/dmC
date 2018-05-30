// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Ochar.h"

#define TY char
#define CT Ochar
#define FUN(id) ochar ## _ ## id
#include "dmc/tpl/topt.c"
#undef TY
#undef CT
#undef FUN
