// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Cpath.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/path.h"
#include "dmc/DEFS.h"

/*.
struct: Cpath
  root: char *: _string
  relative: char *: _string
*/

/*.-.*/
struct cpath_Cpath {
  char *root;
  char *relative;
};

Cpath *cpath_new(char *root, char *relative) {
  Cpath *this = MALLOC(Cpath);
  XNULL(root)
  this->root = root;
  XNULL(relative)
  this->relative = relative;
  return this;
}

char *cpath_root(Cpath *this) {
  XNULL(this)
  return this->root;
}

char *cpath_relative(Cpath *this) {
  XNULL(this)
  return this->relative;
}

Ajson *cpath_to_json(Cpath *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_astring(serial, this->root);
  jarr_astring(serial, this->relative);
  return serial;
}

Cpath *cpath_from_json(Ajson *serial) {
  XNULL(serial)
  Cpath *this = MALLOC(Cpath);
  size_t i = 0;
  this->root = jarr_gstring(serial, i++);
  this->relative = jarr_gstring(serial, i++);
  return this;
}
/*.-.*/

char *cpath_complete(Cpath *this) {
  XNULL(this)
  return str_printf("%s/%s", this->root, this->relative);
}

bool cpath_eq(Cpath *this, Cpath *other) {
  XNULL(this)
  XNULL(other)
  return str_eq(this->relative, other->relative);
}

char *cpath_name(Cpath *this) {
  XNULL(this);
  return path_only_name(this->relative);
}
