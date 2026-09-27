// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef READERS_TOKEN_RD_H
  #define READERS_TOKEN_RD_H

#include "readers/charRd.h"
#include "data/token.h"

/// Record structure.
typedef struct {
  CharRd *chr; // Character reader.
  Token *tk; // Current token.
} TokenRd;

/// Constructor.
///   tx: Text to read.
TokenRd *tokenRd_new (char *tx);

/// Returns the current token without avancing the character reader.
/// \<tokenRd> -> <token>
Token *tokenRd_peek (TokenRd *this);

/// Returns the current token and advances the character reader.
Token *tokenRd_next(TokenRd *this);

#endif
