// Copyright 03-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "read/sym_heap.h"
#include "read/module.h"

SymHeap *sym_heap_new (void) {
  return (SymHeap *)arr_new();
}
