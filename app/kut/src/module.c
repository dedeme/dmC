// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "module.h"
#include "kut/DEFS.h"

struct module_Module {
  // <int>
  Map *imports;
  Heap0 *heap0;
  Heap *heap;
  // <StatCode>
  Arr *code;
};

Module *module_new (Map *imports, Heap0 *hp0, Arr *code) {
  Module *this = MALLOC(Module);
  this->imports = imports;
  this->heap0 = hp0;
  this->heap = heap_new();
  this->code = code;
  return this;
}

// <int>
Map *module_get_imports(Module *this) {
  return this->imports;
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
