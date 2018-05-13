// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

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

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rarr read an array<p>
/// Throws tx_excepton.<p>
Tx *rarr(Value **v, Tx *tx);

#endif
