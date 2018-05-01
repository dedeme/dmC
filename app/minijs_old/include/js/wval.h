// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Write js value

#ifndef JS_WVAL_H
  # define JS_WVAL_H

#include <dmc/all.h>
#include "ast/Class.h"

///
void wval_writebf(Buf *bf, Map/*char*/ *imports, Value *v);

///
void wval_write(LckFile *lck, Map/*char*/ *imports, Value *v);

#endif
