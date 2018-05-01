// Copyright 09-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types/Dtype.h"

/*.
-struct:@Dtype
  type: enum Type_t
  id: char *
  generics: Arr *
  params: Arr *
  super: Dtype *
  implements: Arr *
  objects: Map *
*/

/*.-.*/
struct dtype_Dtype {
  enum Type_t type;
  char *id;
  Arr *generics;
  Arr *params;
  Dtype *super;
  Arr *implements;
  Map *objects;
};

Dtype *_dtype_new(
  enum Type_t type,
  char *id,
  Arr *generics,
  Arr *params,
  Dtype *super,
  Arr *implements,
  Map *objects
) {
  Dtype *this = MALLOC(Dtype);
  this->type = type;
  this->id = id;
  this->generics = generics;
  this->params = params;
  this->super = super;
  this->implements = implements;
  this->objects = objects;
  return this;
}

inline
enum Type_t dtype_type(Dtype *this) {
  return this->type;
}

inline
char *dtype_id(Dtype *this) {
  return this->id;
}

inline
Arr *dtype_generics(Dtype *this) {
  return this->generics;
}

inline
Arr *dtype_params(Dtype *this) {
  return this->params;
}

inline
Dtype *dtype_super(Dtype *this) {
  return this->super;
}

inline
Arr *dtype_implements(Dtype *this) {
  return this->implements;
}

inline
Map *dtype_objects(Dtype *this) {
  return this->objects;
}
/*.-.*/

static Dtype *new(enum Type_t type, char *id) {
  return _dtype_new(type, id, arr_new(), arr_new(), NULL, arr_new(), map_new());
}

Dtype *dtype_new_data(
  char *id,
  Arr *generics,
  Arr *params,
  Dtype *super,
  Arr *implements
) {
  Dtype *this = new(DATA, id);
  this->generics = generics;
  this->params = params;
  this->super = super;
  this->implements = implements;
  return this;
}
