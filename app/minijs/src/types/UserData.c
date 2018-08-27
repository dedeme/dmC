// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/UserData.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: UserData
  roots: Lchar *
  source: Cpath *
  target: char *
*/

/*.-.*/
struct userData_UserData {
  Lchar *roots;
  Cpath *source;
  char *target;
};

UserData *userData_new(Lchar *roots, Cpath *source, char *target) {
  UserData *this = MALLOC(UserData);
  XNULL(roots)
  this->roots = roots;
  XNULL(source)
  this->source = source;
  XNULL(target)
  this->target = target;
  return this;
}

Lchar *userData_roots(UserData *this) {
  XNULL(this)
  return this->roots;
}

Cpath *userData_source(UserData *this) {
  XNULL(this)
  return this->source;
}

char *userData_target(UserData *this) {
  XNULL(this)
  return this->target;
}
/*.-.*/
