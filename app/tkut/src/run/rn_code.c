// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "kut/js.h"
#include "run/rn_code.h"
#include "run/stat.h"


///
struct run_rn_code {
  char *name;
  int was_run;
  // <RnCode>
  Arr *modules;
  // <Layers>
  Arr *heap;
  // <Stat>
  Arr *stats;
};

// modules is Arr<RnCode>
// stats is Arr<Stat>
RnCode *rn_code_new (char *name, Arr *modules, Arr *stats) {
  RnCode *this = MALLOC(RnCode);
  this->name = name;
  this->was_run = FALSE;
  this->modules = modules;
  this->heap = arr_new();
  this->stats = stats;
  return this;
}

RnCode *rn_code_mk(char * name) {
  return rn_code_new(name, arr_new(), arr_new());
}

RnCode *rn_code_solve_types(RnCode *this) {
  return this;
}

void rn_code_run(RnCode *this) {
  if (this->was_run) return;
  this->was_run = TRUE;
}

char *rn_code_to_js (RnCode *this) {
  return js_wa(arr_new_from(
    js_ws(this->name),
    arr_to_js(this->modules, (FTO) rn_code_to_js),
    arr_to_js(this->stats, (FTO) stat_to_js),
    NULL
  ));
}

///
RnCode *rn_code_from_js (char *js) {
  /// <char>
  char **a = (char **)arr_begin(js_ra(js));
  return rn_code_new (
    js_rs(a[0]),
    arr_from_js(a[1], (FFROM) rn_code_from_js),
    arr_from_js(a[2], (FFROM) stat_from_js)
  );
}
