// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Dep.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
struct: Dep
  class: Cpath *: cpath
  entry: char *: _string
*/

/*.-.*/
struct dep_Dep {
  Cpath *class;
  char *entry;
};

Dep *dep_new(Cpath *class, char *entry) {
  Dep *this = MALLOC(Dep);
  XNULL(class)
  this->class = class;
  XNULL(entry)
  this->entry = entry;
  return this;
}

Cpath *dep_class(Dep *this) {
  XNULL(this)
  return this->class;
}

char *dep_entry(Dep *this) {
  XNULL(this)
  return this->entry;
}

bool dep_eq(Dep *this, Dep *other) {
  return cpath_eq(
    this->class, other->class) && str_eq(this->entry, other->entry
  );
}

Ajson *dep_to_json(Dep *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  ajson_add(serial, json_warray(cpath_to_json(this->class)));
  jarr_astring(serial, this->entry);
  return serial;
}

Dep *dep_from_json(Ajson *serial) {
  XNULL(serial)
  Dep *this = MALLOC(Dep);
  size_t i = 0;
  this->class = cpath_from_json(json_rarray(ajson_get(serial, i++)));
  this->entry = jarr_gstring(serial, i++);
  return this;
}
/*.-.*/

