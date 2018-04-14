// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Write js class

#ifndef JS_WCLASS_H
  # define JS_WCLASS_H

#include <dmc/all.h>
#include "ast/Class.h"

///
void wclass_start(Class *class);

///
void wclass_end(Class *class);

///
void wclass_object(Class *class, Map/*char*/ *imports, Cvalue *cv);

#endif
