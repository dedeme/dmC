// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for values

#ifndef LEXER_VALUES_H
  #define LEXER_VALUES_H

#include <dmc/all.h>
#include "lexer/Txpos.h"
#include "ast/Value.h"

/// values_read reads a Value. If fails throws a tx_exception.
Txpos *values_read(Value **val, Txpos *tx, bool is_group);

#endif
