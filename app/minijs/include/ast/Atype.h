// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Type]

#ifndef AST_ATYPE_H
  # define AST_ATYPE_H

#include "Type.h"

///
typedef Arr Atype;

///
Atype *atype_new(void);

///
void atype_add(Atype *this, Type *t);

///
Type *atype_get(Atype *this, int ix);

///
Json *atype_serialize(Atype *this);

///
Atype *atype_restore(Json *j);


#endif

