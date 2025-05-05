// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/module.h"

Module *module_new(
  int is_main, char *path, char *id, Map *imports, Arr *exports,
  Arr *top_syms, Map *type_defs, Arr *code
) {
  Module *this = MALLOC(Module);
  this->is_main = is_main;
  this->path = path;
  this->id = id;
  this->imports = imports;
  this->exports = exports;
  this->top_syms = top_syms;
  this->type_defs = type_defs;
  this->code = code;
  return this;
}
