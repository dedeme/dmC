// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of an array of Opt<Module>, where each index match the
/// same file index of the corresponding module.
/// NOTE 1:
///   - A module has an index (ix) in the list.
///   - A module is defined in a file 'f'.
///   - 'f' has a file index 'fix'.
///   - 'ix' == 'fix'
/// NOTE 2:
///   An array entry index can point to:
///     - option_none(). There is no module with such index.
///     - option_some(option_none()). There is a module with such index, but
///       it is not defined yet.
///     - option_some(option_some(module)). There is a defined module with such
///       index.

#ifndef MODULES_H
  #define MODULES_H

#include "module.h"

/// Creates the modules array.
void modules_init (void);

/// Add an 'opt_none()' as module.
/// NOTE: The module has an identifier but it is not read yet.
///   ix: Index of module file.
void modules_add (int ix);

/// Set the value of module 'ix' as 'opt_some(mod)'.
///   ix : Index of module (and module file)
///   mod: Module
void modules_set (int ix, Module *mod);

/// Returns Opt<Opt<Module>> with the module 'ix' (defined: opt_some(module), or
/// undefiend: opt_none) or 'opt_none()' if such module does not exist.
///   ix: Index of mudule (and module file)
Opt *modules_get(int ix);

/// Returns Opt<Module> with the module 'ix' or raise an EXC_IO if module does
/// not exist.
/// The module returned can be defined (opt_some(module)), or
/// undefiend (opt_none).
///   ix: Index of mudule (and module file)
Opt *modules_get_ok(int ix);

/// Returns Arr<Opt<Opt<Module>>> with a copy of the modules array.
Arr *modules_get_array(void);

#endif
