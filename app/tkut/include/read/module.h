// Copyright 03-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module and Modules stock data.

#ifndef READ_MODULE_H
  #define READ_MODULE_H

#include "kut/arr.h"

///
typedef struct read_module Module;

/// Constructor
///   module_name: Module name. It is spected that the corresponding file exists.
Module *module_new (char *module_name);

/// Returns the statements of 'this' (Arr<Stat>).
Arr *module_stats(Module *this);

/// Set values of 'this' intially fixed to empty arrays.
///   this: Module to update.
///   syms: Main symbols of module.
///   stats: Statements array.
void module_update(Module *this, Arr *syms, Arr *stats);

/// Initialize modules stock.
void module_clear_stock (void);

/// Adds module to stock.
void module_add_to_stock (Module *md);

/// Returns Arr<int> with number of symbols defined, an entry for each module.
Arr *module_stock_indexes (void);

#endif
