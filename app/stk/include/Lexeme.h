// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef LEXEME_H
  #define LEXEME_H

#include "dmc/std.h"
#include "TkReader.h"
#include "Type.h"

///
typedef struct lexeme_Lexeme Lexeme;

///
Type *lexeme_type (Lexeme *this);

///
void lexeme_free (Lexeme *this);

///
Lexeme *lexeme_next (TkReader *reader);

#endif
