// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class to encapsulate a position in a file

#ifndef LEXER_POS_H
  #define LEXER_POS_H

#include "dmc/all.h"

/*.-.*/

///
typedef struct pos_Pos Pos;

///
Pos *pos_new(size_t nline, size_t nchar);

///
size_t pos_nline(Pos *this);

///
size_t pos_nchar(Pos *this);

///
Json *pos_serialize(Pos *this);

///
Pos *pos_restore(Json *s);

/*.-.*/

#endif
