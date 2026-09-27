// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "module.h"

struct module_Module {
  Imports *imports;
  Exports *exports;
  Heap0 *heap0;
  Heap *heap;
  // <StatCode>
  Arr *code;
};

Module *module_new (Imports *imports, Exports *exports, Heap0 *hp0, Arr *code) {
  Module *this = MALLOC(Module);
  this->imports = imports;
  this->exports = exports;
  this->heap0 = hp0;
  this->heap = heap_new();
  this->code = code;
  return this;
}

// <int>
Imports *module_get_imports(Module *this) {
  return this->imports;
}

Exports *module_get_exports(Module *this) {
  return this->exports;
}

Heap0 *module_get_heap0(Module *this) {
  return this->heap0;
}

Heap *module_get_heap(Module *this) {
  return this->heap;
}

// <StatCode>
Arr *module_get_code(Module *this) {
  return this->code;
}
