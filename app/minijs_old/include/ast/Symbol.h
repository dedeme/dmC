// Copyright 27-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Symbol type

#ifndef AST_SYMBOL_H
  # define AST_SYMBOL_H

#include <dmc/all.h>
#include "ast/Type.h"

/*.-.*/

///
typedef struct symbol_Symbol Symbol;

///
Symbol *symbol_new(char *id, Type *type, bool is_val);

///
char *symbol_id(Symbol *this);

///
Type *symbol_type(Symbol *this);

///
bool symbol_is_val(Symbol *this);

/*.-.*/

#endif

