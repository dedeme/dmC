// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map&lt;SkEntry>

#ifndef TYPES_MSKENTRY_H
  # define TYPES_MSKENTRY_H

#include "types/SkEntry.h"

typedef struct mskEntry_MskEntry MskEntry;

#define TY SkEntry                    // Element type
#define FN skEntry                    // Function prefix
#include "dmc/tpl/tmap.h"
#undef TY
#undef FN

#endif
