// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/parser.h"
#include "global.h"
#include "lexer/Txpos.h"
#include "ast/Class.h"
#include "parser/Final_class.h"

Obj *parser_type(bool local, Cpath *path, char *object) {
  Final_class *fc = NULL;
  EACH(global_final_classes(), Final_class, c) {
    if (cpath_eq(final_class_cpath(c), path)) {
      fc = c;
      break;
    }
  }_EACH

  if (!fc) {
    fc = final_class_new(local, path);
    arr_add(global_final_classes(), fc);
  }

  if (!final_class_public(fc) && !local)
    TH2(class_mk(final_class_cpath(fc)), pos_new(0, 0))
      "'%s' is private and only can be called from its own directory",
      cpath_id(final_class_cpath(fc))
    _TH

  return final_class_object_type(fc, object);
}
