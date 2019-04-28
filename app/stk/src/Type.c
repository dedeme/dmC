// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Type.h"

union type_Subtype {
  char noSubtype;
};

static union type_Subtype *subtype_new () {
  return malloc(sizeof(union type_Subtype));
}

struct type_Type {
  enum Kind kind;
  union type_Subtype *sub;
};

Type *type_new(enum Kind kind, union type_Subtype *sub) {
  Type *this = malloc(sizeof(Type));
  this->kind = kind;
  this->sub = sub;
  return this;
}

void type_free(Type *this) {
  if (this) {
    switch (this->kind) {
      case BOOL:
        free(this->sub);
    }
    free(this);
  }
};

enum Kind type_kind(Type *this) {
  return this->kind;
}

Type *type_bool_new () {
  union type_Subtype *sub = subtype_new();
  sub->noSubtype = 1;
  return type_new(BOOL, sub);
}
