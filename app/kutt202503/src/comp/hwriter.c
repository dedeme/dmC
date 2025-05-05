// Copyright 22-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/hwriter.h"
#include "data/typedSym.h"

char *hwriter_write (Module *md) {
  // Arr<char>
  Arr *bf = arr_new();
  EACH(md->top_syms, TypedSym, tsym) {
    arr_push(bf, str_f("extern Val %s_%s;", md->id, tsym->id));
  }_EACH
  return arr_cjoin(bf, '\n');
}
