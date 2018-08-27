// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/JsEntry.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
struct: JsEntry
  type: Type *: type
  js: char *: _string
  deps: Deps *: deps
*/

/*.-.*/
struct jsEntry_JsEntry {
  Type *type;
  char *js;
  Deps *deps;
};

JsEntry *jsEntry_new(Type *type, char *js, Deps *deps) {
  JsEntry *this = MALLOC(JsEntry);
  XNULL(type)
  this->type = type;
  XNULL(js)
  this->js = js;
  XNULL(deps)
  this->deps = deps;
  return this;
}

Type *jsEntry_type(JsEntry *this) {
  XNULL(this)
  return this->type;
}

char *jsEntry_js(JsEntry *this) {
  XNULL(this)
  return this->js;
}

Deps *jsEntry_deps(JsEntry *this) {
  XNULL(this)
  return this->deps;
}

Json *jsEntry_to_json(JsEntry *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  ajson_add(serial, type_to_json(this->type));
  jarr_astring(serial, this->js);
  ajson_add(serial, deps_to_json(this->deps));
  return json_warray(serial);
}

JsEntry *jsEntry_from_json(Json *js) {
  XNULL(js)
  Ajson *serial = json_rarray(js);
  JsEntry *this = MALLOC(JsEntry);
  size_t i = 0;
  this->type = type_from_json(ajson_get(serial, i++));
  this->js = jarr_gstring(serial, i++);
  this->deps = deps_from_json(ajson_get(serial, i++));
  return this;
}
/*.-.*/
