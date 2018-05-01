// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for values

#ifndef LEXER_VALUES_H
  #define LEXER_VALUES_H

#include <dmc/all.h>
#include "lexer/Txpos.h"
#include "ast/Cvalue.h"

/// values_read reads a Value. If fails throws a tx_exception.
Txpos *values_read(Value **val, Txpos *tx, bool is_group);

/// values_read0 is equals to values_read(val, tx, false).
Txpos *values_read0(Value **val, Txpos *tx);

/// values_new_read reads the function 'new'. If fails trows a tx_exception
Txpos *values_new_read(
  Arr/*Cvalue*/ **values, Txpos *tx, Dvalue *dvalue, bool is_public
);

#endif
