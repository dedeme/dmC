// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Run code data.

#ifndef RUN_RN_CODE_H
  #define RUN_RN_CODE_H

#include "kut/arr.h"

///
typedef struct run_rn_code RnCode;

/// Constructor.
///   name: Module name. (name + ".tkut" is the module file).
///   modules: Modules it dependends on.
///   stats: Statements.
RnCode *rn_code_new (char *name, Arr *modules, Arr *stats);

/// Constructor of the main RnCode.
RnCode *rn_code_mk(char * name);

/// Solves types and prepare symbols. Modifies 'this' and returns it.
RnCode *rn_code_solve_types(RnCode *this);

/// Run code of 'this'
void rn_code_run(RnCode *this);

///
char *rn_code_to_js (RnCode *this);

///
RnCode *rn_code_from_js (char *js);

#endif
