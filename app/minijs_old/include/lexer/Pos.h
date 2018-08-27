// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Class to encapsulate a position in a file

#ifndef LEXER_POS_H
  #define LEXER_POS_H

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct pos_Pos Pos;

///
Pos *pos_new(size_t nline, size_t nchar);

///
size_t pos_nline(Pos *this);

///
size_t pos_nchar(Pos *this);

///
Ajson *pos_to_json(Pos *this);

///
Pos *pos_from_json(Ajson *s);

/*.-.*/

#endif
