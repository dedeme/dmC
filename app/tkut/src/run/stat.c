// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/js.h"
#include "run/stat.h"
#include "run/ex.h"

Stat *stat_new (int nline, int nchar, StatT type, Arr *exprs) {
  Stat *this = MALLOC(Stat);
  this->nline = nline;
  this->nchar = nchar;
  this->type = type;
  this->exprs = exprs;
  return this;
}

/// Run 'this'
void stat_run(RnCode *code, Stat *this) {
}

///
char *stat_to_js (Stat *this) {
  return js_wa(arr_new_from(
    js_wi(this->nline),
    js_wi(this->nchar),
    js_wi(this->type),
    arr_to_js(this->exprs, (FTO) ex_to_js),
    NULL
  ));
}

///
Stat *stat_from_js (char *js) {
  /// <char>
  char **a = (char **)arr_begin(js_ra(js));
  return stat_new (
    js_ri(a[0]),
    js_ri(a[1]),
    js_ri(a[2]),
    arr_from_js(a[3], (FFROM) ex_from_js)
  );
}
