// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/index.h"
#include "data/indexEntry.h"
#include "kut/js.h"

char *index_to_js (Index *this) {
  return arr_to_js(this, (FTO)indexEntry_to_js);
}

///
Index *index_from_js (char *js) {
  return arr_from_js(js, (FFROM)indexEntry_from_js);
}
