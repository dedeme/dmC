// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "function.h"
#include "DEFS.h"
#include "heap.h"
#include "runner/runner.h"
#include "runner/fail.h"

struct function_Function {
  // <int>
  Map *imports;
  Heap0 *heap0;
  // <Heap>
  Heaps *heaps;
  // <char>
  Arr *pars;
  StatCode *stat;
};

// pars is Arr<char>
Function *function_new (Arr *pars, StatCode *stat) {
  Function *this = MALLOC(Function);
  this->imports = map_new();
  this->heap0 = heap0_new();
  this->heaps = heaps_new(heap_new());
  this->pars = pars;
  this->stat = stat;
  return this;
}

// <int>
Map *function_get_imports (Function *this) {
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

//  imports is Map<int>
//  heaps is Arr<Heap>
Function *function_set_context (
  Function *old, Map *imports, Heap0 *heap0, Heaps *heaps
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
  CHECK_PARS("<function>", arr_size(this->pars), pars);

  Heap *hp = heap_new();
  EACH(this->pars, char, p) {
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
  return str_f(
    "(\\%s -> %s)",
    arr_join(this->pars, ", "),
    stat_to_str(stat_code_stat(this->stat))
  );
}
