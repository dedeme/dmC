// Copyright 22-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expressions solver.

#ifndef RUNNER_SOLVER_H
  #define RUNNER_SOLVER_H

#include "kut/map.h"
#include "heap0.h"
#include "stat.h"

/// Solve an expression.
/// The expression returned is not empty nor break nor continue.
///   imports: Map<int>. Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Arr<Heap>. Module heaps of solved symbols.
///   exp: Expression to solve.
Exp *solver_solve(Map *imports, Heap0 *hp0, Arr *heaps, Exp *exp);

/// Solve an expression without symbols.
/// The expression returned is not empty nor break nor continue.
///   exp: Expression to solve.
Exp *solver_solve_isolate(Exp *exp);


#endif
