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
  Arr *heaps;
  // <char>
  Arr *vars;
  StatCode *stat;
};

Function *function_new (Arr *vars, StatCode *stat) {
  Function *this = MALLOC(Function);
  this->imports = map_new();
  this->heap0 = heap0_new();
  this->heaps = arr_new();
  this->vars = vars;
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

// <Heap>
Arr *function_get_heaps (Function *this) {
  return this->heaps;
}

// <char>
Arr *function_get_vars (Function *this) {
  return this->vars;
}

StatCode *function_get_stat (Function *this) {
  return this->stat;
}

//  imports is Map<int>
//  heaps is Arr<Heap>
Function *function_set_context (
  Function *old, Map *imports, Heap0 *heap0, Arr *heaps
) {
  Function *this = MALLOC(Function);
  this->imports = imports;
  this->heap0 = heap0;
  this->heaps = heaps;
  this->vars = old->vars;
  this->stat = old->stat;
  return this;
}

Exp *function_run (Function *this, Arr *pars) {
  CHECK_PARS("<function>", arr_size(this->vars), pars);

  Heap *hp = heap_new();
  EACH(this->vars, char, vname) {
    heap_add(hp, vname, arr_get(pars, _i));
  }_EACH

  Exp* r = runner_run_stat(
    arr_new(),
    this->imports, this->heap0, heap_add_to_arr(hp, this->heaps),
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
    arr_join(this->vars, ", "),
    stat_to_str(stat_code_stat(this->stat))
  );
}
