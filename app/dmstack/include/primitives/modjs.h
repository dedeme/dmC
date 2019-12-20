// Copyright 17-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// JSON module.

#ifndef PRIMITIVES_MODJS_H
  #define PRIMITIVES_MODJS_H

#include "dmc/async.h"
#include "Pmodule.h"

///
Pmodule *modjs_mk (void);

///
void modjs_from_list (Machine *m);

///
void modjs_to_list (Machine *m);

///
void modjs_from_map (Machine *m);

///
void modjs_to_map (Machine *m);

#endif
