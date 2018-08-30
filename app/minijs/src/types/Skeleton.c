// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Skeleton.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
struct: Skeleton
  entries: MskEntry *: _object skEntry
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct skeleton_Skeleton {
  MskEntry *entries;
};

Skeleton *skeleton_new(MskEntry *entries) {
  Skeleton *this = MALLOC(Skeleton);
  XNULL(entries)
  this->entries = entries;
  return this;
}

MskEntry *skeleton_entries(Skeleton *this) {
  XNULL(this)
  return this->entries;
}

Json *skeleton_to_json(Skeleton *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_aobject(serial, (Map *)this->entries, (Json*(*)(void*)) skEntry_to_json);
  return json_warray(serial);
}

Skeleton *skeleton_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  Skeleton *this = MALLOC(Skeleton);
  size_t i = 0;
  this->entries = (MskEntry *)jarr_gobject(serial, i++, (void*(*)(Json*)) skEntry_from_json);
  return this;
}
/*.-.*/
