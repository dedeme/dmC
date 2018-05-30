// Copyright 17-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Achar]

#ifndef AST_AACHAR_H
  # define AST_AACHAR_H

#include "Achar.h"

///
typedef struct aachar_Aachar Aachar;

///
Aachar *aachar_new(void);

///
size_t aachar_size(Aachar *this);
///
void aachar_add(Aachar *this, Achar *a);

///
Achar *aachar_get(Aachar *this, int ix);

///
Arr/*Json*/ *aachar_serialize(Aachar *this);

///
Aachar *aachar_restore(Arr/*Json*/ *j);


#endif

