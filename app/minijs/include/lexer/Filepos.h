// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class to encapsulate a position in a file

#ifndef LEXER_FILEPOS_H
  #define LEXER_FILEPOS_H

#include "dmc/all.h"

/*.-.*/

///
typedef struct filepos_Filepos Filepos;

///
Filepos *filepos_new(char *path, size_t nline, size_t nchar);

///
char *filepos_path(Filepos *this);

///
size_t filepos_nline(Filepos *this);

///
size_t filepos_nchar(Filepos *this);

///
Json *filepos_serialize(Filepos *this);

///
Filepos *filepos_restore(Json *s);

/*.-.*/

#endif
