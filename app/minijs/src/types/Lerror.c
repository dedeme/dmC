// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types/Lerror.h"

#define TY Error                    // Element type
#define FN error                    // Function prefix
#include "dmc/tpl/tlist.c"
#undef TY
#undef FN

Lerror *lerror_add_if(Lerror *this, Error *e) {
  if (lerror_count(this) < 10) {
    return lerror_cons(this, e);
  }
  return this;
}

Lerror *lerror_add_list(Lerror *this, Lerror *es) {
  es = lerror_reverse(es);
  EACHL(es, Error, e) {
    this = lerror_add_if(this, e);
  }_EACH
  return this;
}
