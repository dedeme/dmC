// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "module.h"
#include "kut/DEFS.h"

struct module_Module {
  Imports *imports;
  // char
  Arr *exports;
  Heap0 *heap0;
  // <StatCode>
  Arr *code;
  char *js;
};

Module *module_new (
  Imports *imports, Arr *exports, Heap0 *hp0, Arr *code, char *js
) {
  Module *this = MALLOC(Module);
  this->imports = imports;
  this->exports = exports;
  this->heap0 = hp0;
  this->code = code;
  this->js = js;
  return this;
}

Imports *module_get_imports(Module *this) {
  return this->imports;
}

Heap0 *module_get_heap0(Module *this) {
  return this->heap0;
}

// <StatCode>
Arr *module_get_code(Module *this) {
  return this->code;
}

char *module_get_js(Module *this) {
  return this->js;
}

int module_contains_export (Module *this, char *exp) {
  EACH(this->exports, char, e) {
    if (!strcmp(e, exp)) return TRUE;
  }_EACH
  return FALSE;
}
