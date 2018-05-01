// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/natstr.h"
#include "lexer/Txpos.h"
#include "DEFS.h"

Obj *natstr_static(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Array.%s' not found", id _TH
  return NULL;
}

Obj *natstr_instance(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Elelment '%s' is not found in Array instance", id _TH
  return NULL;
}
