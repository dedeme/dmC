// Copyright 13-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constructor (function new) reader.<p>
///
/// <b>Template</b>
///   new_id,* -> block
/// where
///   new_id :: mark? id
///   mark :: + | @ | @+ | >
/// <b>Examples</b>
///   -> { ... }
///   a, b -> { ... }
///   +a -> { ... }
///   > a, @ c, @+ d, e, +f-> { ... }

#ifndef LEXER_RNEW_H
  #define LEXER_RNEW_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "ast/Class.h"
#include "Tx.h"

/// rnew read a class constructor.<p>
/// Throws tx_excepton.
Tx *rnew(Value **value, Tx *tx, Class *c, Type *tp, bool is_public);

#endif
