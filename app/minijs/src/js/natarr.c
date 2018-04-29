// Copyright 23-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/natarr.h"
#include "lexer/Txpos.h"
#include "DEFS.h"

Obj *natarr_static(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Array.%s' not found", id _TH
  return NULL;
}

Obj *natarr_instance(Final_class *c, Pos *pos, char *id) {
  if (!strcmp(id, "size")) {
    return obj_new(id, type_new_int(), pos, true, false, true);
  }
  TH3(c, pos_new(0,0)) "Elelment '%s' is not found in Array instance", id _TH
  return NULL;
}
