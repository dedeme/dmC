// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/natint.h"
#include "lexer/Txpos.h"
#include "DEFS.h"

Obj *natint_static(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Int.%s' not found", id _TH
  return NULL;
}

Obj *natint_instance(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Elelment '%s' is not found in Int instance", id _TH
  return NULL;
}
