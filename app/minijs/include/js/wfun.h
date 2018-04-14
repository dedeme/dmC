// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Write js function

#ifndef JS_WFUN_H
  # define JS_WFUN_H

#include <dmc/all.h>
#include "ast/Class.h"

///
void wfun_writebf(Buf *bf, Map/*char*/ *imports, Value *v);

///
void wfun_write(LckFile *lck, Map/*char*/ *imports, Value *v);


#endif
