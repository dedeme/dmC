// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TKREADER_H
  #define TKREADER_H

#include "dmc/std.h"
#include "Token.h"

///
typedef char *(*tkReader_Source)(void *reader);

///
typedef Token *(*tkReader_Next_new)(void *reader);

/*--*/

///
typedef struct tkReader_TkReader TkReader;

///
TkReader *tkReader_new(void *reader, tkReader_Source fsource, tkReader_Next_new fnext);

///
void tkReader_free(TkReader *this);

/*--*/

/// Return a source description
char *tkReader_source (TkReader *this);

/// Return the following token. When are no more tokens, returns a token
/// with value = "".
Token *tkReader_next_new (TkReader *this);


#endif
