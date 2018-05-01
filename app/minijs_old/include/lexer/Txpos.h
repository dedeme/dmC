// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class to encapsulate a position in a text

#ifndef LEXER_TXPOS_H
  #define LEXER_TXPOS_H

#include "dmc/all.h"
#include "Cpath.h"
#include "lexer/Pos.h"

/*.-.*/

///
typedef struct txpos_Txpos Txpos;

///
Cpath *txpos_cpath(Txpos *this);

///
char *txpos_text(Txpos *this);

///
char *txpos_start(Txpos *this);

///
char *txpos_end(Txpos *this);

///
size_t txpos_nline(Txpos *this);

///
size_t txpos_nchar(Txpos *this);

///
Txpos *txpos_previous(Txpos *this);

/*.-.*/

/// txpos_new creates a new Txpos. The value of 'txpos_previous()' is set to
/// NULL.
Txpos *txpos_new(
  Cpath *cpath,
  char *text,
  char *start,
  char *end,
  size_t nline,
  size_t nchar
);

/// txpos_move change 'this' position. It last positio is saved in
/// 'txpos_previous()'
Txpos *txpos_move(Txpos *this, char *pos, size_t nline, size_t nchar);

/// Print msg
void txpos_printf(Txpos *this, char *format, ...);

/// Return true if txpos_start(this) == txpos_end(this)
bool txpos_at_end(Txpos *this);

/// Return true if this == other, where other cames from this with txpos_move.
bool txpos_eq(Txpos *this, Txpos *other);

/// Return true if this != other, where other cames from this with txpos_move.
bool txpos_neq(Txpos *this, Txpos *other);

///
Pos *txpos_pos(Txpos *this);

#endif
