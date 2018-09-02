// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tp/Params.h"

/*.
-struct: Params
  roots: Achar *
  target: char *
*/
/*.-.*/
#include "dmc/ct/Ajson.h"

struct params_Params {
  Achar *roots;
  char *target;
};

Params *params_new(Achar *roots, char *target) {
  Params *this = MALLOC(Params);
  XNULL(roots)
  this->roots = roots;
  XNULL(target)
  this->target = target;
  return this;
}

Achar *params_roots(Params *this) {
  XNULL(this)
  return this->roots;
}

char *params_target(Params *this) {
  XNULL(this)
  return this->target;
}
/*.-.*/

#define TY Params                    // Element type
#define FN params                    // Function prefix
#include "dmc/tpl/topt.c"
#undef TY
#undef FN
