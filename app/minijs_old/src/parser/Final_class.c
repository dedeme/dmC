// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/Final_class.h"
#include "ast/Class.h"
#include "lexer/lexer.h"
#include "lexer/Txpos.h"
#include "global.h"
#include "parser/Lobj.h"
#include "parser/parser.h"

#include "js/wclass.h"

/*.
-struct:@Final_class
  cpath: Cpath *
  public: bool
  -
  -imports: Map * = map_new()
  -objects: Map * = map_new()
*/

/*.-.*/
struct final_class_Final_class {
  Cpath *cpath;
  bool public;

  Map *imports;
  Map *objects;
};

Final_class *_final_class_new(Cpath *cpath, bool public) {
  Final_class *this = MALLOC(Final_class);
  this->cpath = cpath;
  this->public = public;
  this->imports = map_new();
  this->objects = map_new();
  return this;
}

inline
Cpath *final_class_cpath(Final_class *this) {
  return this->cpath;
}

inline
bool final_class_public(Final_class *this) {
  return this->public;
}
/*.-.*/

Final_class *final_class_new(Cpath *path) {
  Class *c = NULL;
  EACH(global_classes(), Class, cl) {
    if (cpath_eq(class_cpath(cl), path)) {
      c = cl;
      break;
    }
  }_EACH

  if (!c) {
    c = lexer_compile(path);
    arr_add(global_classes(), c);
  }

  wclass_start(c);


  Final_class *this = _final_class_new(
    path,
    class_public(c)
  );

// IMPORTS

  return this;
}

Obj *final_class_object(Final_class *this, char *object) {
  EACH(this->objects, Kv, kv) {
    if (!strcmp(kv->key, object)) {
      return kv->value;
    }
  }_EACH

  Cpath *path = this->cpath;
  Class *c = NULL;
  EACH(global_classes(), Class, cl) {
    if (cpath_eq(class_cpath(cl), path)) {
      c = cl;
      break;
    }
  }_EACH

  if (!c)
    THROW "Class '%s' is missing", cpath_id(path) _THROW

  EACH(class_cvalues(c), Cvalue, v) {
    if (!strcmp(cvalue_id(v), object)) {
      Dvalue *dv = cvalue_dvalue(v);
      Type *tp = dvalue_type(dv);
      if (type_is_unknown(tp))
        THROW exc_illegal_state("type of 'tp' is unknown") _THROW

      Type *tpv = parser_value_type(this, lobjs_new(), tp, dvalue_value(dv));

      if (!tpv)
        TH3(this, cvalue_pos(v)) "Value type is 'void'" _TH

      bool is_static = cvalue_is_static(v);
      Obj *o = obj_new(
        is_static ? str_printf("%s.%s", cpath_id(path), object) : object,
        tp,
        cvalue_pos(v),
        cvalue_is_public(v),
        is_static,
        cvalue_type(v) == VAL
      );
      map_put(this->objects, object, o);

      wclass_object(c, this->imports, v);
      return o;
    }
  }_EACH

  // test 'supers' unimplemented

  TH2(c, pos_new(0,0)) "'%s.%s' not found", cpath_id(path), object _TH

  return NULL;
}
