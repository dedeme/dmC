// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List[Symbol]

#ifndef AST_LSYMBOL_H
  # define AST_LSYMBOL_H

#include "Symbol.h"
#include "Sym.h"

///
typedef List Lsymbol;

///
Lsymbol *lsymbol_new(void);

///
Lsymbol *lsymbol_cons(Lsymbol *this, Symbol *s);

///
Sym *lsymbol__get(Lsymbol *this, char *id);

#endif

