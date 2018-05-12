// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class to encapsulate a position in a file

#ifndef POS_H
  #define POS_H

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
Arr/*Json*/ *pos_serialize(Pos *this);

///
Pos *pos_restore(Arr/*Json*/ *s);

/*.-.*/

#endif
