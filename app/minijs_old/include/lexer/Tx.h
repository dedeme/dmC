// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Class to encapsulate a position in a text


#ifndef LEXER_TX_H
  #define LEXER_TX_H

#include <stddef.h>
#include <stdbool.h>

typedef struct pos_Pos Pos;

/*.-.*/

///
typedef struct tx_Tx Tx;

///
Tx *tx_new(
  char *path,
  char *text,
  char *start,
  size_t nline,
  size_t nchar
);

///
char *tx_path(Tx *this);

///
char *tx_text(Tx *this);

///
char *tx_start(Tx *this);

///
size_t tx_nline(Tx *this);

///
size_t tx_nchar(Tx *this);

/*.-.*/
/// tx_move returns a new Tx changing the position of 'this'.
Tx *tx_move(Tx *this, char *pos, size_t nline, size_t nchar);

/// Print msg
void tx_printf(Tx *this, char *format, ...);

/// Return true if txpos_start(this) == txpos_end(this)
bool tx_at_end(Tx *this);

/// Return true if this == other, where other cames from this with txpos_move.
bool tx_eq(Tx *this, Tx *other);

/// Return true if this != other, where other cames from this with txpos_move.
bool tx_neq(Tx *this, Tx *other);

///
Pos *tx_pos(Tx *this);

#endif
