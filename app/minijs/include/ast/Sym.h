// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Sym type

#ifndef AST_SYM_H
  # define AST_SYM_H

#include <dmc/all.h>
#include "ast/Type.h"

/*.-.*/

///
typedef struct sym_Sym Sym;

///
Sym *sym_new(Type *type, bool is_val);

///
Type *sym_type(Sym *this);

///
bool sym_is_val(Sym *this);

/*.-.*/

#endif

