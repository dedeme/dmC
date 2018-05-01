// Copyright 23-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "js/natmap.h"
#include "lexer/Txpos.h"
#include "DEFS.h"

Obj *natmap_static(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Map.%s' not found", id _TH
  return NULL;
}

Obj *natmap_instance(Final_class *c, Pos *pos, char *id) {
  TH3(c, pos_new(0,0)) "Elelment '%s' is not found in Map instance", id _TH
  return NULL;
}
