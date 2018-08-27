// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/JsClass.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
struct: JsClass
  entries: MjsEntry *: _object jsEntry
*/

/*.-.*/
struct jsClass_JsClass {
  MjsEntry *entries;
};

JsClass *jsClass_new(MjsEntry *entries) {
  JsClass *this = MALLOC(JsClass);
  XNULL(entries)
  this->entries = entries;
  return this;
}

MjsEntry *jsClass_entries(JsClass *this) {
  XNULL(this)
  return this->entries;
}

Json *jsClass_to_json(JsClass *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_aobject(serial, (Map *)this->entries, (Json*(*)(void*)) jsEntry_to_json);
  return json_warray(serial);
}

JsClass *jsClass_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  JsClass *this = MALLOC(JsClass);
  size_t i = 0;
  this->entries = (MjsEntry *)jarr_gobject(serial, i++, (void*(*)(Json*)) jsEntry_from_json);
  return this;
}
/*.-.*/
