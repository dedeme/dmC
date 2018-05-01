// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/natfloat.h"
#include "lexer/Txpos.h"
#include "DEFS.h"

Obj *natfloat_static(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Float.%s' not found", id _TH
  return NULL;
}

Obj *natfloat_instance(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Elelment '%s' is not found in Float instance", id _TH
  return NULL;
}
