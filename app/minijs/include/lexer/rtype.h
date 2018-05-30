// Copyright 3-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Type reader.<p>
///
/// <b>Template</b>
///   :(type*:type?) |
///   :[type] |
///   :{type} |
///   :* |
///   :id[&lt;type+>]
/// <b>Examples</b>
///   :(:)
///   :(Str, Int:Char)
///   :(It&lt;Str>, Int:)
///   :[Str]
///   :{Dic&lt;Str, Str>}
///   :*
///   :Str
///   :It&lt;Str>
///   :Dic&lt;Str, Str>

#ifndef LEXER_RTYPE_H
  #define LEXER_RTYPE_H

#include "dmc/all.h"
#include "ast/Type.h"
#include "Tx.h"

/// rtype reads one type in 'tp'.<p>
/// If tx does not start with ':' set 'tp' to NULL and returns 'tx', but it
//  does not throw exception.
/// Throws tx_excepton.<p>
Tx *rtype(Type **tp, Tx *tx);

/// rtype2 reads one type in 'tp'.<p>
/// 'tx' does not start with ':'.
/// Throws tx_excepton.<p>
Tx *rtype2(Type **tp, Tx *tx);

#endif
