// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Value reader.<p>
///
/// <b>Template</b>
/// <b>Examples</b>

#ifndef LEXER_RVALUE_H
  #define LEXER_RVALUE_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rvalue reads one value in 'v'.<p>
/// If v is not valid value, it throws a tx_exception.
/// Throws tx_excepton.<p>
Tx *rvalue(Value **v, Tx *tx);

#endif
