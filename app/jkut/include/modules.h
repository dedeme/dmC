// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of an array of Opt<Module>, where each index match the
/// same file index of the corresponding module.
/// NOTE 1:
///   - A module has an index (ix) in the list.
///   - A module is defined in a file 'f'.
///   - 'f' has a file index 'fix'.
///   - 'ix' == 'fix'

#ifndef MODULES_H
  #define MODULES_H

#include "module.h"

/// Creates the modules array.
void modules_init (void);

/// Ads the value of module 'ix'.
///   ix : Index of module (and module file)
///   mod: Module
void modules_add (int ix, Module *mod);

/// Returns Opt<Module> with the module 'ix' or 'opt_none()' if such module
/// does not exist.
///   ix: Index of mudule (and module file)
Opt *modules_get(int ix);

/// Returns the modules array
/// Opt<Module>
Arr *modules_get_array();

#endif
