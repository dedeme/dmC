// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Reads a value

#ifndef LEXER_RVALUE_H
  #define LEXER_RVALUE_H

typedef struct value_Value Value;
typedef struct tx_Tx Tx;

///
Tx *rvalue(Value **v, Tx *tx);

#endif
