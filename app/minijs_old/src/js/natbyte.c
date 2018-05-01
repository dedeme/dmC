// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/natbyte.h"
#include "lexer/Txpos.h"
#include "DEFS.h"

Obj *natbyte_static(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Byte.%s' not found", id _TH
  return NULL;
}

Obj *natbyte_instance(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Elelment '%s' is not found in Byte instance", id _TH
  return NULL;
}
