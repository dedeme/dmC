// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Array reader.<p>
///
/// <b>Template</b>
///   [value,*]
/// <b>Examples</b>
///   []
///   [34]
///   [a, b]

#ifndef LEXER_RARR_H
  #define LEXER_RARR_H

typedef struct value_Value Value;
typedef struct tx_Tx Tx;

/// rarr read an array<p>
/// Throws tx_excepton.<p>
Tx *rarr(Value **v, Tx *tx);

#endif
