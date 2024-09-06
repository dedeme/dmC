// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "function.h"
#include "DEFS.h"
#include "heap.h"

struct function_Function {
  Imports *imports;
  Heap0 *heap0;
  Heaps *heaps;
  // <char>
  Arr *pars;
  StatCode *stat;
};

// pars is Arr<char>
Function *function_new (Arr *pars, StatCode *stat) {
  Function *this = MALLOC(Function);
  this->imports = imports_new();
  this->heap0 = heap0_new();
  this->heaps = heaps_new();
  this->pars = pars;
  this->stat = stat;
  return this;
}

Imports *function_get_imports (Function *this) {
  return this->imports;
}

Heap0 *function_get_heap0 (Function *this) {
  return this->heap0;
}

Heaps *function_get_heaps (Function *this) {
  return this->heaps;
}

// <char>
Arr *function_get_pars (Function *this) {
  return this->pars;
}

StatCode *function_get_stat (Function *this) {
  return this->stat;
}

char *function_to_str (Function *this) {
  return str_f(
    "(\\%s -> %s)",
    arr_join(this->pars, ", "),
    stat_to_str(stat_code_stat(this->stat))
  );
}
