// Copyright 17-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Maker of ordered groups of object compilation names and their groups of
/// modules.

#ifndef COMP_OGROUPING_H
  #define COMP_OGROUPING_H

#include "kut/arr.h"

/// Returns a sorted Arr<Tp<char, Arr<char>>> of
/// [objectFileName, [compilatedModulesIn]].
///   deps: Arr<Tp<char, Arr<char>>> of [module, [importedModules]].
///         The first element must be the main module data.
Arr *ogrouping_run (Arr *deps);

/// g is Arr<Tp<char, Arr<char>>>
char *ogrouping_to_str (Arr *g);

#endif
