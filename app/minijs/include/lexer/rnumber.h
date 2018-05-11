// Copyright 5-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Number reader.<p>
///
/// <b>Template</b>
///   int_bool
///   float
/// where
///   int_bool :: int | int t
///   int :: dec | hexa | oct | bin

#ifndef LEXER_RNUMBER_H
  #define LEXER_RNUMBER_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rnumber read a number in 'v'.<p>
/// Throws tx_excepton.<p>
Tx *rnumber(Value **v, Tx *tx);

#endif
