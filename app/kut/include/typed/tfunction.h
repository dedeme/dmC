// Copyright 11-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kut typed function.

#ifndef TYPED_TFUNCTION_H
  #define TYPED_TFUNCTION_H

#include "iarr.h"
#include "stat.h"
#include "tval.h"
#include "ttype.h"

typedef struct tfunction_Tfunction Tfunction;

/// Constructor.
/// This function does not generate the typed code. It is done in
/// 'tfunction_run'.
///   id   : C code identifier.
///   tpars: Parameters type.
///   tret : Return type.
///   pars : Symbols array.
///   stat : Statement.
Tfunction *tfunction_new (
  Iarr *tpars, Ttype tret, Iarr *pars, StatCode *stat
);

/// Sets tfunction identifier. (Set in assignation)
void tfunction_set_id (Tfunction *this, char *id);

/// Returns tfunction identifier.
char *tfunction_get_id (Tfunction *this);

/// Returns tfunction C identifier.
char *tfunction_get_cid (Tfunction *this);

/// Returns tfunction paraemters types.
Iarr *tfunction_get_tpars (Tfunction *this);

/// Returns tfunction return type.
Ttype tfunction_get_tret (Tfunction *this);

/// Returns tfunction parameters.
Iarr *tfunction_get_pars (Tfunction *this);

/// Returns the statement.
StatCode *tfunction_get_st_cd (Tfunction *this);

/// Runs 'this' form normal code and returns a value that can be an
/// empty expression if the Tfunction 'this' return nothing.
/// If called for first time, the typed code is generated and 'st_cd' is
/// set to NULL.
///   this: Function to run.
///   pars: Arr<Exp> Solved parameter values.
Exp *tfunction_run (Tfunction *this, Arr *pars);

///
char *tfunction_to_str (Tfunction *this);

#endif
