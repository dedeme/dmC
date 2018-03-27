// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Class.h"
#include "ast/Cvalue.h"

/*.
struct: @Class
  cpath: Cpath *: cpath
  generics: Arr *: _sarray
  cvalues: Arr *: _array cvalue
*/

/*.-.*/
struct class_Class {
  Cpath *cpath;
  Arr *generics;
  Arr *cvalues;
};

Class *_class_new(Cpath *cpath, Arr *generics, Arr *cvalues) {
  Class *this = MALLOC(Class);
  this->cpath = cpath;
  this->generics = generics;
  this->cvalues = cvalues;
  return this;
}

inline
Cpath *class_cpath(Class *this) {
  return this->cpath;
}

inline
Arr *class_generics(Class *this) {
  return this->generics;
}

inline
Arr *class_cvalues(Class *this) {
  return this->cvalues;
}

Json *class_serialize(Class *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, cpath_serialize(this->cpath));
  jarr_aarray(serial, this->generics, (Json*(*)(void*))json_wstring);
  jarr_aarray(serial, this->cvalues, (Json*(*)(void*)) cvalue_serialize);
  return json_warray(serial);
}

Class *class_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Class *this = MALLOC(Class);
  size_t i = 0;
  this->cpath = cpath_restore(arr_get(serial, i++));
  this->generics = jarr_garray(serial, i++, (void*(*)(Json*))json_rstring);
  this->cvalues = jarr_garray(serial, i++, (void*(*)(Json*)) cvalue_restore);
  return this;
}
/*.-.*/

Class *class_mk(Cpath *cpath) {
  Class *r = _class_new(
    cpath, arr_new(), arr_new()
  );

  return r;
}
