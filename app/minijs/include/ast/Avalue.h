// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Value]

#ifndef AST_AVALUE_H
  # define AST_AVALUE_H

#include "Value.h"

///
typedef Arr Avalue;

///
Avalue *avalue_new(void);

///
void avalue_add(Avalue *this, Value *t);

///
Value *avalue_get(Avalue *this, int ix);

///
Arr/*Json*/ *avalue_serialize(Avalue *this);

///
Avalue *avalue_restore(Arr/*Json*/ *j);


#endif

