// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/Final_class.h"
#include "ast/Class.h"
#include "lexer/lexer.h"
#include "lexer/Txpos.h"
#include "global.h"

#include "js/wclass.h"

/*.+.
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

Final_class *final_class_new(bool local, Cpath *path) {
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

Obj *final_class_object_type(Final_class *this, char *object) {
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
      Type *tp = dvalue_type(cvalue_dvalue(v));
      if (type_is_unknown(tp))
        THROW exc_illegal_state("type of 'tp' is unknown") _THROW

      wclass_object(c, this->imports, v);

      Obj *o = obj_new(
        tp,
        cvalue_pos(v),
        cvalue_is_public(v),
        cvalue_is_static(v),
        cvalue_type(v) == VAL
      );
      map_put(this->objects, object, o);
      return o;
    }
  }_EACH

  THROW "test 'supers' unimplemented" _THROW
  return NULL;
}
