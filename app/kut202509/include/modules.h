// Copyright 11-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Management of an array of Opt<Opt<<Module>>, where each index match the
/// same file index of the corresponding module.
/// NOTE 1:
///   - A module has an index (ix) in the list.
///   - A module is defined in a file 'f'.
///   - 'f' has a file index 'fix'.
///   - 'ix' == 'fix'
/// NOTE 2:
///   An array entry index can point to:
///     - option_none() [Module not registered]. There is no module with such
///       index.
///     - option_some(option_none()) [Module pending to be read]. There is
///       a module with such index, but it is not defined yet.
///     - option_some(option_some(module)). [Module read] There is a defined
///       module with such index.

#ifndef MODULES_H
  #define MODULES_H

#include "kut/arr.h" //   TO REMOVE.
#include "module.h"

/// Creates the modules array.
void modules_init (void);

/// Adds a module pending to be read.
/// NOTE1: Every 'index < fix' id added as 'Module not registred'.
/// NOTE2: Adding an already registred 'ix' is ok, but dones not make any change.
///   fix: Index of module file.
void modules_add (int fix);

/// Set the value of module 'ix' as 'Module read'.
///   ix : Index of module (and module file)
///   mod: Module
void modules_set (int ix, Module *mod); // CHANGE char by module

/// Returns Opt<Opt<Module>> with the module 'ix':
///   - Module alredy read: opt_some(opt_some(module))
///   - Module pending to be read: opt_some(opt_none())
///   - Module not registered: opt_none()
///   ix: Index of mudule (and module file)
Opt *modules_get(int ix);

/// Returns Opt<Module> with the module 'ix' or raise an EXC_IO if module is
/// not registered
/// The module returned can be:
///   - Module alredy read: opt_some(module)
///   - Module pending to be read: opt_none()
///   ix: Index of mudule (and module file)
Opt *modules_get_ok(int ix);

/// Returns Arr<Opt<Opt<Module>>> with a shallow copy of the module array.
Arr *modules_get_array(void);

#endif
