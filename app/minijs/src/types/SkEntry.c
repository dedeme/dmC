// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/SkEntry.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
struct: SkEntry
  type: Type *: type
*/

/*.-.*/
struct skEntry_SkEntry {
  Type *type;
};

SkEntry *skEntry_new(Type *type) {
  SkEntry *this = MALLOC(SkEntry);
  XNULL(type)
  this->type = type;
  return this;
}

Type *skEntry_type(SkEntry *this) {
  XNULL(this)
  return this->type;
}

Json *skEntry_to_json(SkEntry *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  ajson_add(serial, type_to_json(this->type));
  return json_warray(serial);
}

SkEntry *skEntry_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  SkEntry *this = MALLOC(SkEntry);
  size_t i = 0;
  this->type = type_from_json(ajson_get(serial, i++));
  return this;
}
/*.-.*/
