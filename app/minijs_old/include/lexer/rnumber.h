// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

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

typedef struct value_Value Value;
typedef struct tx_Tx Tx;

/// rnumber read a number in 'v'.<p>
/// Throws tx_excepton.<p>
Tx *rnumber(Value **v, Tx *tx);

#endif
