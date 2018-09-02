// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tp/P.h"

/*.
-struct: P
  absolute: char *
  relative: char *
*/
/*.-.*/
#include "dmc/ct/Ajson.h"

struct p_P {
  char *absolute;
  char *relative;
};

P *p_new(char *absolute, char *relative) {
  P *this = MALLOC(P);
  XNULL(absolute)
  this->absolute = absolute;
  XNULL(relative)
  this->relative = relative;
  return this;
}

char *p_absolute(P *this) {
  XNULL(this)
  return this->absolute;
}

char *p_relative(P *this) {
  XNULL(this)
  return this->relative;
}
/*.-.*/

#define TY P                    // Element type
#define FN p                    // Function prefix
#include "dmc/tpl/tarr.c"
#undef TY
#undef FN
