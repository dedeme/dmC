// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// lunary opertors reader.<p>
///
/// <b>Template</b>
///   op value
/// where
///   op :: - | ! | ~ | ++ | --
/// <b>Examples</b>
///   -12
///   - 16.34
///   !true
///   ++x
/// <b>Operators type</b>
///   - (A:A)
///   ~ (A:A)
///   ! (A:A)
///   ++ (A:A)
///   -- (A:A)

#ifndef LEXER_RLUNARY_H
  #define LEXER_RLUNARY_H

typedef struct value_Value Value;
typedef struct tx_Tx Tx;

/// rlunary read a lunary operator. It reads its next value.<p>
/// Throws tx_excepton.<p>
Tx *rlunary(Value **v, char *op, Tx *tx);

#endif
