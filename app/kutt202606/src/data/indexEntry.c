// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/indexEntry.h"
#include "kut/js.h"

IndexEntry *indexEntry_new (char *path, char *id) {
  IndexEntry *this = MALLOC(IndexEntry);
  this->path = path;
  this->id = id;
  return this;
}

///
char *indexEntry_to_js (IndexEntry *this) {
  return js_wa(arr_new_from(
    js_ws(this->path),
    js_ws(this->id),
    NULL
  ));
}

///
IndexEntry *indexEntry_from_js (char *js) {
  void **a = arr_begin(js_ra(js));
  return indexEntry_new(
    js_rs(a[0]),
    js_rs(a[1])
  );
}
