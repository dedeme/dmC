// Copyright 19-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ast/Type.h"
#include "ast/Csym.h"

/*.
-struct: Csym
  is_public: bool
  is_static: bool
  class_id: char *
  id: char *
  type: Type *
*/

/*.-.*/
struct csym_Csym {
  bool is_public;
  bool is_static;
  char *class_id;
  char *id;
  Type *type;
};

Csym *csym_new(
  bool is_public,
  bool is_static,
  char *class_id,
  char *id,
  Type *type
) {
  Csym *this = MALLOC(Csym);
  this->is_public = is_public;
  this->is_static = is_static;
  XNULL(class_id)
  this->class_id = class_id;
  XNULL(id)
  this->id = id;
  XNULL(type)
  this->type = type;
  return this;
}

bool csym_is_public(Csym *this) {
  XNULL(this)
  return this->is_public;
}

bool csym_is_static(Csym *this) {
  XNULL(this)
  return this->is_static;
}

char *csym_class_id(Csym *this) {
  XNULL(this)
  return this->class_id;
}

char *csym_id(Csym *this) {
  XNULL(this)
  return this->id;
}

Type *csym_type(Csym *this) {
  XNULL(this)
  return this->type;
}
/*.-.*/
