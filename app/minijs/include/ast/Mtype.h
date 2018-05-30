// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map[Type]

#ifndef AST_MTYPE_H
  # define AST_MTYPE_H

#include "Type.h"

///
typedef Map Mtype;

///
Mtype *mtype_new(void);

///
void mtype_put(Mtype *this, char *id, Type *t);

///
Type *mtype__get(Atype *this, char *id);

#endif

