// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "obj.h"
#include "exp.h"

#define module "<module>"
#define bmodule "<bmodule>"
#define bfunction "<bfunction>"
#define iter "<iter>"

Exp *obj_module (Module *value) {
  return exp_object(module, value);
}

Module *obj_get_module (Exp *this) {
  return exp_get_object(module, this);
}

int obj_is_module (Exp *this) {
  return exp_is_object(module, this);
}

Exp *obj_bmodule (char *value) {
  return exp_object(bmodule, value);
}

char *obj_get_bmodule (Exp *this) {
  return exp_get_object(bmodule, this);
}

int obj_is_bmodule (Exp *this) {
  return exp_is_object(bmodule, this);
}

Exp *obj_bfunction (Bfunction value) {
  return exp_object(bfunction, value);
}

Bfunction obj_get_bfunction (Exp *this) {
  return exp_get_object(bfunction, this);
}

int obj_is_bfunction (Exp *this) {
  return exp_is_object(bfunction, this);
}

Exp *obj_iter (It *value) {
  return exp_object(iter, value);
}

It *obj_get_iter (Exp *this) {
  return exp_get_object(iter, this);
}

int obj_is_iter (Exp *this) {
  return exp_is_object(iter, this);
}

