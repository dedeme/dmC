// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include "dmc/Ichar.h"

#define TY char
#define IT Ichar
#define OP Ochar
#define FUN(id) ichar ## _ ## id
#include "dmc/tpl/tit.c"
#undef TY
#undef IT
#undef OP
#undef FUN
