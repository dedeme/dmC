// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "function.h"
#include "heap.h"
#include "symix.h"
#include "runner/runner.h"
#include "runner/fail.h"

struct function_Function {
  Imports *imports;
  Heap0 *heap0;
  Heaps *heaps;
  Iarr *pars;
  StatCode *stat;
};

Function *function_new (Iarr *pars, StatCode *stat) {
  Function *this = MALLOC(Function);
  this->imports = imports_new();
  this->heap0 = heap0_new();
  this->heaps = heaps_new(heap_new());
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

Iarr *function_get_pars (Function *this) {
  return this->pars;
}

StatCode *function_get_stat (Function *this) {
  return this->stat;
}

//  imports is Map<int>
//  heaps is Arr<Heap>
Function *function_set_context (
  Function *old, Imports *imports, Heap0 *heap0, Heaps *heaps
) {
  Function *this = MALLOC(Function);
  this->imports = imports;
  this->heap0 = heap0;
  this->heaps = heaps;
  this->pars = old->pars;
  this->stat = old->stat;
  return this;
}

Exp *function_run (Function *this, Arr *pars) {
  CHECK_PARS("<function>", iarr_size(this->pars), pars);

  Heap *hp = heap_new();
  EACHI(this->pars, p) {
    heap_add(hp, p, arr_get(pars, _i));
  }_EACH

  Exp *r = runner_run_stat(
    stack_new(),
    this->imports, this->heap0, heaps_add(this->heaps, hp),
    this->stat
  );
  if (exp_is_break(r))
    EXC_KUT(fail_add_stack(
      str_f("In '%s'\n'break' without 'while' or 'for'", exp_to_str(r)),
      exp_get_break(r)
    ));
  if (exp_is_continue(r))
    EXC_KUT(fail_add_stack(
      str_f("In '%s'\n'continue' without 'while' or 'for'", exp_to_str(r)),
      exp_get_continue(r)
    ));
  if (exp_is_empty_return(r)) r = exp_empty();
  return r;
}

// params is Arr<Exp>
Exp *function_to_exp (Function *this, Arr *params) {
  return exp_pr(exp_function(this), params);
}

char *function_to_str (Function *this) {
  // Arr<char>
  Arr *ps = arr_new();
  EACHI(this->pars, i) {
    arr_push(ps, symix_get(i));
  }_EACH
  return str_f(
    "(\\%s -> %s)",
    arr_join(ps, ", "),
    stat_to_str(stat_code_stat(this->stat))
  );
}
