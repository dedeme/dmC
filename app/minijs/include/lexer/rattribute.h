// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class attribute reader.<p>
///
/// <b>Template</b>
///   enum (id,*)
///   type [val | var] id [; | = value]
/// <b>Examples</b>
///   enum ()
///   enum (FIRST)
///   enum (FIRST, SECOND, THIRD)
///   var v 23;
///   : Str val v2 = z
///   : (Int:) val f = n -> { }

#ifndef LEXER_RATTIBUTE_H
  #define LEXER_RATTIBUTE_H

#include "dmc/all.h"
#include "ast/Class.h"
#include "Tx.h"

/// rattribute reads one attribute.<br>
/// Throws tx_excepton
Tx *rattribute(Tx *tx, Class *c, bool is_public);

#endif
