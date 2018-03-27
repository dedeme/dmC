// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for big text chuncks

#ifndef LEXER_BLOCK_H
  #define LEXER_BLOCK_H

#include <dmc/all.h>
#include "lexer/Txpos.h"
#include "ast/Class.h"
#include "ast/Dvalue.h"
#include "types/Type.h"

/// Creates and reads a top class. If fails it throws a tx_exception.
Class *block_top_class(Txpos *tx, Cpath *cpath);

/// Creates and reads a type. If fails it throws a tx_exception.
Txpos *block_type(Type **type, Txpos *tx);

/// Creates and reads a type. If fails it throws a tx_exception.
Txpos *block_declaration(Dvalue **d, Txpos *tx, Type *tp);

#endif
