// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Block reader.<p>
///
/// A block is an array of statements wrapped in an Astat.<p>
///
/// <b>Template</b>
///   singleBlock | multipleBlock
/// where
///   singleBlock :: Statement ;
///   multipleBlock :: { Statement* }
/// <b>Examples</b>
///   {}
///   {"a" : z > y}
///   { "a" : 1, "b" : 2}

#ifndef LEXER_RBLOCK_H
  #define LEXER_RBLOCK_H

#include "dmc/all.h"
#include "ast/Astat.h"
#include "Tx.h"

/// rblock read a block either single or multiple<p>
/// Throws tx_excepton.
Tx *rblock(Astat **block, Tx *tx);

/// rsblock read a single block<p>
/// Throws tx_excepton.
Tx *rsblock(Astat **block, Tx *tx);

/// rmblock read a multiple block<p>
/// Throws tx_excepton.
Tx *rmblock(Astat **block, Tx *tx);

#endif
