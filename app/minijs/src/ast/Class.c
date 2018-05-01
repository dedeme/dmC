// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Class.h"

/*.
struct: @Class
  id: char *: _string
  +local: bool: _bool
  +super: char *: _string
  generics: Achar *: achar
  imports: Mchar *: mchar
  statics: Aatt *: aatt
  instance: Aatt *: aatt
*/

/*.-.*/
struct class_Class {
  char *id;
  bool local;
  char *super;
  Achar *generics;
  Mchar *imports;
  Aatt *statics;
  Aatt *instance;
};

Class *_class_new(
  char *id,
  bool local,
  char *super,
  Achar *generics,
  Mchar *imports,
  Aatt *statics,
  Aatt *instance
) {
  Class *this = MALLOC(Class);
  this->id = id;
  this->local = local;
  this->super = super;
  this->generics = generics;
  this->imports = imports;
  this->statics = statics;
  this->instance = instance;
  return this;
}

inline
char *class_id(Class *this) {
  return this->id;
}

inline
bool class_local(Class *this) {
  return this->local;
}

inline
void class_set_local(Class *this, bool value) {
  this->local = value;
}

inline
char *class_super(Class *this) {
  return this->super;
}

inline
void class_set_super(Class *this, char *value) {
  this->super = value;
}

inline
Achar *class_generics(Class *this) {
  return this->generics;
}

inline
Mchar *class_imports(Class *this) {
  return this->imports;
}

inline
Aatt *class_statics(Class *this) {
  return this->statics;
}

inline
Aatt *class_instance(Class *this) {
  return this->instance;
}

Json *class_serialize(Class *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_astring(serial, this->id);
  jarr_abool(serial, this->local);
  jarr_astring(serial, this->super);
  arr_add(serial, achar_serialize(this->generics));
  arr_add(serial, mchar_serialize(this->imports));
  arr_add(serial, aatt_serialize(this->statics));
  arr_add(serial, aatt_serialize(this->instance));
  return json_warray(serial);
}

Class *class_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Class *this = MALLOC(Class);
  size_t i = 0;
  this->id = jarr_gstring(serial, i++);
  this->local = jarr_gbool(serial, i++);
  this->super = jarr_gstring(serial, i++);
  this->generics = achar_restore(arr_get(serial, i++));
  this->imports = mchar_restore(arr_get(serial, i++));
  this->statics = aatt_restore(arr_get(serial, i++));
  this->instance = aatt_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

inline
Class *class_new(char *id) {
  return _class_new(
    id, false, "", achar_new(), mchar_new(), aatt_new(), aatt_new()
  );
}

Type *class__type(Class *this, Atype *generics) {
  if (arr_size(generics) != arr_size(this->generics)) {
    return NULL;
  }
  return type_new_data(this->id, generics);
}
