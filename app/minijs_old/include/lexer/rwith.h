// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// with operator reader.<p>
/// This operator translates conditions to a chain of ternary operators.
///
/// <b>Template</b>
///   value condition+ default
/// where
///   condition :: \ value ,+ : value
///   default :: \ : value
/// <b>Examples</b>
///   with x
///   \ 0, y + 2 : "a"
///   \ 1 : "b"
///   \ : "c"
/// is translated to
///   (x == 0 || x == y + 2) ? "a"
///   : x == 1 ? "b"
///   : "c"

#ifndef LEXER_RWITH_H
  #define LEXER_RWITH_H

typedef struct value_Value Value;
typedef struct tx_Tx Tx;

/// rwith read a with operator<p>
/// Throws tx_excepton.<p>
Tx *rwith(Value **v, Tx *tx);

#endif
