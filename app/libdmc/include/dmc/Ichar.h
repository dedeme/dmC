// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator of char *

#ifndef DM_ICHAR_H
  #define DM_ICHAR_H

///
typedef struct ichar_Ichar Ichar;

typedef struct ochar_Ochar Ochar;

#define TY char
#define IT Ichar
#define OP Ochar
#define FUN(id) ichar ## _ ## id
#include "dmc/tpl/tit.h"
#undef TY
#undef IT
#undef OP
#undef FUN

#endif
