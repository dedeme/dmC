// Copyright 03-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "read/module.h"
#include "kut/path.h"

// <Module>
Arr *stock = NULL;

struct read_module {
  char *id;
  char *name;
  // <Symbol>
  Arr *syms;
  // <Stat>
  Arr *stats;
};

Module *module_new (char *module_name) {
  Module *this = MALLOC(Module);
  this->id = str_eq(module_name, "<dynamic>")
    ? module_name
    : opt_eget(path_canonical(str_f("%s.tkut", module_name)))
  ;
  this->name = module_name;
  this->syms = arr_new();
  this->stats = arr_new();
  return this;
}

/// Returns the statements of 'this' (Arr<Stat>).
Arr *module_stats(Module *this) {
  return this->stats;
}

/// Set values of 'this' intially fixed to empty arrays.
///   this: Module to update.
///   syms: Main symbols of module.
///   stats: Statements array.
void module_update(Module *this, Arr *syms, Arr *stats) {
  this->syms = syms;
  this->stats = stats;
}

/// Initialize modules stock.
void module_clear_stock (void) {
  stock = arr_new();
}

/// Adds module to stock.
void module_add_to_stock (Module *md) {
  if (!stock) EXC_ILLEGAL_STATE("Stock has not been initialized");
  arr_push(stock, md);
}

/// Returns Arr<int> with number of symbols defined, an entry for each module.
Arr *module_stock_indexes (void) {
  if (!stock) EXC_ILLEGAL_STATE("Stock has not been initialized");
  // <int>
  Arr *r = arr_new();
  EACH(stock, Module, md) {
    int *n = ATOMIC(sizeof(int));
    *n = arr_size(md->syms);
    arr_push(r, n);
  }_EACH
  return r;
}
