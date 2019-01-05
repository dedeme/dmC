// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Def.h"
#include "Arg.h"
#include "Var.h"
#include "Fun.h"

struct def_Def {
  StName *name;
  // Arr[Arg]
  Arr *args;
  // Arr[Var]
  Arr *vars;
  // Arr[Fun]
  Arr *funs;
};

// Arrays:
//   args : Arr[Arg]
//   vars : Arr[Var]
//   funs : Arr[Fun]
Def *def_new(
  StName *name,
  Arr *args,
  Arr *vars,
  Arr *funs
) {
  // Arr[Arg]
  Arr *args2 = arr_new((FPROC)arg_free);
  arr_cat(args2, args, (FCOPY)arg_copy_new);
  // Arr[Var]
  Arr *vars2 = arr_new((FPROC)var_free);
  arr_cat(vars2, vars, (FCOPY)var_copy_new);
  // Arr[Fun]
  Arr *funs2 = arr_new((FPROC)fun_free);
  arr_cat(funs2, funs, (FCOPY)fun_copy_new);

  Def *this = malloc(sizeof(Def));
  this->name = stName_copy_new(name);
  this->args = args2;
  this->vars = vars2;
  this->funs = funs2;
  return this;
}

void def_free(Def *this) {
  stName_free(this->name);
  arr_free(this->args);
  arr_free(this->vars);
  arr_free(this->funs);
  free(this);
}

StName *def_name(Def *this) {
  return this->name;
}

// Arr[Arg]
Arr *def_args(Def *this) {
  return this->args;
}

// Arr[Var]
Arr *def_vars(Def *this) {
  return this->vars;
}

// Arr[Fun]
Arr *def_funs(Def *this) {
  return this->funs;
}
