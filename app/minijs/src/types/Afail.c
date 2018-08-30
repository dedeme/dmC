// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types/Afail.h"

#define TY Fail                    // Element type
#define FN fail                    // Function prefix
#include "dmc/tpl/tarr.c"
#undef TY
#undef FN

void afail_add_if(Afail *this, Fail *e) {
  if (afail_size(this) < 10) {
    afail_add(this, e);
  }
}

void afail_add_arr_if(Afail *this, Afail *es) {
  EACH(es, Fail, e) {
    afail_add_if(this, e);
  }_EACH
}
