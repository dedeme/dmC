// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Definition model.

#ifndef DEF_H
  #define DEF_H

#include "dmc/std.h"
#include "StName.h"

///
typedef struct def_Def Def;

/// Arrays:
///   args : Arr[Arg]
///   vars : Arr[Var]
///   funs : Arr[Fun]
Def *def_new(
  StName *name,
  Arr *args,
  Arr *vars,
  Arr *funs
);

///
void def_free(Def *this);

///
StName *def_name(Def *this);

/// Arr[Arg] - Read only
Arr *def_args(Def *this);

/// Arr[Var] - Read only
Arr *def_vars(Def *this);

/// Arr[Fun] - Read only
Arr *def_funs(Def *this);

#endif
