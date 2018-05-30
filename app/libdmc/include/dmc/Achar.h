// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of char *

#ifndef DM_ACHAR_H
  #define DM_ACHAR_H

///
typedef struct achar_Achar Achar;

typedef struct ichar_Ichar Ichar;

#define TY char
#define CT Achar
#define IT Ichar
#define FUN(id) achar ## _ ## id
#include "dmc/tpl/tarr.h"
#undef TY
#undef CT
#undef IT
#undef FUN

/// Ascending natural sort
void achar_simple_sort (Achar *this);

/// Ascending local sort
void achar_local_sort (Achar *this);


#endif
