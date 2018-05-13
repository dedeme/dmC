// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map reader.<p>
///
/// <b>Template</b>
///   {keyValue,*}
/// where
///   keyValue :: "key" : value
/// <b>Examples</b>
///   {}
///   {"a" : z > y}
///   { "a" : 1, "b" : 2}

#ifndef LEXER_RMAP_H
  #define LEXER_RMAP_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rmap read a map<p>
/// Throws tx_excepton.<p>
Tx *rmap(Value **v, Tx *tx);

#endif
