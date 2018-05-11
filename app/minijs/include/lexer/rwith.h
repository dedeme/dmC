// Copyright 9-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// with operator reader.<p>
/// This operator translates if conditions to a chain of ternary operators.
///
/// <b>Template</b>
///   value condition* default
///   ternary
/// where
///   condition :: \ value ,* : value
///   default :: \ : value
/// <b>Examples</b>
///   with x
///     \ 0, y + 2 : "a"
///     \ 1 : "b"
///     \ : "c"
/// is translated to
///   (x == 0 || x == y + 2) ? "a"
///   : x == 1 ? "b"
///   : "c"
/// NOTE: with x \ : 8 is directly translated to 8.

#ifndef LEXER_RWITH_H
  #define LEXER_RWITH_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rnary read a with operator<p>
/// Throws tx_excepton.<p>
Tx *rwith(Value **v, Tx *tx);

#endif
