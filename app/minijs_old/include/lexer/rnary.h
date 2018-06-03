// Copyright 6-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Binary and ternary operators reader.<p>
///
/// <b>Template</b>
///   binary |
///   ternary
/// where
///   binary :: bin_op value
///   ternary :: ? value : value
/// <b>Examples</b>
///   3 + 2 * 4
///   a == b ? x : y
/// <b>bin_op</b>
///   == (A,A:Bool)
///   != (A,A:Bool)
///   > (A,A:Bool)
///   < (A,A:Bool)
///   >= (A,A:Bool)
///   <= (A,A:Bool)
///   + (A,A:A)
///   - (A,A:A)
///   * (A,A:A)
///   / (A,A:A)
///   % (A,A:A)
///   ^ (A,A:A)
///   & (A,A:A)
///   | (A,A:A)
///   ^^ (A,A:A)
///   >> (A,A:A)
///   << (A,A:A)
///   ?: (A,A:A)
///   >>> (A,A:A)

#ifndef LEXER_RNARY_H
  #define LEXER_RNARY_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rnary read a binary or ternary operator<p>
/// Throws tx_excepton.<p>
Tx *rnary(Value **v, Tx *tx, Value *v1);

#endif
