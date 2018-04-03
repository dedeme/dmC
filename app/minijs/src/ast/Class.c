// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Class.h"

/*.
struct: @Class
  cpath: Cpath *: cpath
  +public: bool: _bool
  +generics: Arr *: _sarray
  +imports: Arr *: _array import
  -extends: Implement *: implement
  +implements: Arr *: _array implement
  -cvalues: Arr *: _array cvalue


*/

/*.-.*/
struct class_Class {
  Cpath *cpath;
  bool public;
  Arr *generics;
  Arr *imports;
  Implement *extends;
  Arr *implements;
  Arr *cvalues;
};

Class *_class_new(
  Cpath *cpath,
  bool public,
  Arr *generics,
  Arr *imports,
  Implement *extends,
  Arr *implements,
  Arr *cvalues
) {
  Class *this = MALLOC(Class);
  this->cpath = cpath;
  this->public = public;
  this->generics = generics;
  this->imports = imports;
  this->extends = extends;
  this->implements = implements;
  this->cvalues = cvalues;
  return this;
}

inline
Cpath *class_cpath(Class *this) {
  return this->cpath;
}

inline
bool class_public(Class *this) {
  return this->public;
}

inline
void class_set_public(Class *this, bool value) {
  this->public = value;
}

inline
Arr *class_generics(Class *this) {
  return this->generics;
}

inline
void class_set_generics(Class *this, Arr *value) {
  this->generics = value;
}

inline
Arr *class_imports(Class *this) {
  return this->imports;
}

inline
void class_set_imports(Class *this, Arr *value) {
  this->imports = value;
}

inline
Arr *class_implements(Class *this) {
  return this->implements;
}

inline
void class_set_implements(Class *this, Arr *value) {
  this->implements = value;
}

Json *class_serialize(Class *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, cpath_serialize(this->cpath));
  jarr_abool(serial, this->public);
  jarr_aarray(serial, this->generics, (Json*(*)(void*))json_wstring);
  jarr_aarray(serial, this->imports, (Json*(*)(void*)) import_serialize);
  arr_add(serial, implement_serialize(this->extends));
  jarr_aarray(serial, this->implements, (Json*(*)(void*)) implement_serialize);
  jarr_aarray(serial, this->cvalues, (Json*(*)(void*)) cvalue_serialize);
  return json_warray(serial);
}

Class *class_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Class *this = MALLOC(Class);
  size_t i = 0;
  this->cpath = cpath_restore(arr_get(serial, i++));
  this->public = jarr_gbool(serial, i++);
  this->generics = jarr_garray(serial, i++, (void*(*)(Json*))json_rstring);
  this->imports = jarr_garray(serial, i++, (void*(*)(Json*)) import_restore);
  this->extends = implement_restore(arr_get(serial, i++));
  this->implements = jarr_garray(serial, i++, (void*(*)(Json*)) implement_restore);
  this->cvalues = jarr_garray(serial, i++, (void*(*)(Json*)) cvalue_restore);
  return this;
}
/*.-.*/

Class *class_mk(Cpath *cpath) {
  Class *r = _class_new(
    cpath, true, arr_new(), arr_new(), NULL, arr_new(), arr_new()
  );

  return r;
}

inline
Implement *class_extends(Class *this) {
  return this->extends;
}

inline
void class_set_extends(Class *this, Implement *value) {
  this->extends = value;
}

inline
Arr/*Cvalue*/ *class_cvalues(Class *this) {
  return this->cvalues;
}

void class_add_cvalue(Class *this, Cvalue *cval) {
  if (type_type(dvalue_type(cvalue_dvalue(cval))) == UNKNOWN)
    THROW "Type of %s is unknown", dvalue_id(cvalue_dvalue(cval)) _THROW

  Cvalue *old_val = NULL;
  EACH(this->cvalues, Cvalue, val) {
    if (!strcmp(
      dvalue_id(cvalue_dvalue(val)),
      dvalue_id(cvalue_dvalue(cval))
    )) {
      old_val = val;
      break;
    }
  }_EACH
  if (old_val)
    THROW
      "Identifier '%s' is duplicated. First was defined in %s:%zu[%zu]",
      dvalue_id(cvalue_dvalue(cval)),
      cpath_id(class_cpath(this)),
      pos_nline(dvalue_pos(cvalue_dvalue(old_val))),
      pos_nchar(dvalue_pos(cvalue_dvalue(old_val)))
    _THROW

  arr_add(this->cvalues, cval);
}

