// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map&lt;JsEntry>

#ifndef TYPES_MJSENTRY_H
  # define TYPES_MJSENTRY_H

#include "types/JsEntry.h"

typedef struct mjsEntry_MjsEntry MjsEntry;

#define TY JsEntry                    // Element type
#define FN jsEntry                    // Function prefix
#include "dmc/tpl/tmap.h"
#undef TY
#undef FN

#endif
