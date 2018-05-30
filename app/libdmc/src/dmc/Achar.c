// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include "dmc/Achar.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

#define TY char
#define CT Achar
#define IT Ichar
#define FUN(id) achar ## _ ## id
#include "dmc/tpl/tarr.c"
#undef TY
#undef CT
#undef IT
#undef FUN

void arr_simple_sort (Achar *this) {
  XNULL(this)

  bool cmp(char *e1, char *e2) { return str_cmp(e1, e2) > 0; }
  return achar_sort(this, cmp);
}

void arr_local_sort (Achar *this) {
  XNULL(this)

  bool cmp(char *e1, char *e2) { return strcoll(e1, e2) > 0; }
  return achar_sort(this, cmp);
}
