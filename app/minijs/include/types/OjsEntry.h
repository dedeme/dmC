// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Opt&lt;JsEntry>

#ifndef TYPES_OJSENTRY_H
  # define TYPES_OJSENTRY_H

#include "types/JsEntry.h"

typedef struct ojsEntry_OjsEntry OjsEntry;

#define TY JsEntry                    // Element type
#define FN jsEntry                    // Function prefix
#include "dmc/tpl/topt.h"
#undef TY
#undef FN

#endif
