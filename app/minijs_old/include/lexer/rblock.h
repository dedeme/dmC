// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Reads a block

#ifndef LEXER_RBLOCK_H
  #define LEXER_RBLOCK_H

typedef struct astat_Astat Astat;
typedef struct tx_Tx Tx;

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
