// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Modules to compile.
/// There are modules with or without code.

#ifndef MODULES_H
  #define MODULES_H

#include "data/module.h"

/// Initializes the array of modules.
void modules_init (void);

/// Returns the module with identifier 'id'.
/// If a no-existent module is reclamed, an exception is raised.
/// \s -> <module>
Module *modules_get (char *id);

/// Execute 'fn' with each element of 'modules'.
void modules_each (void (*fn)(Module *md));

/// Add to 'modules' a new module and all its imports.
/// If there is an error, returns it. Otherwise returns "".
///   is_main: TRUE if the application main module is going to be added.
///   path   : Canonical path.
///   id     : Identifier.
char *modules_add(int is_main, char *path, char *id);

#endif
