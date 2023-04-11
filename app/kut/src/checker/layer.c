// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "checker/layer.h"
#include "kut/DEFS.h"

struct layer_Layer {
  int user_control;
  // <Cksym>
  Arr *syms;
};

// syms is Arr<Cksym>
Layer *layer_new (int user_control, Arr *syms) {
  Layer *this = MALLOC(Layer);
  this->user_control = user_control;
  this->syms = syms;
  return this;
}

void layer_add (Layer *this, Cksym *sym) {
  arr_push(this->syms, sym);
}

Arr *layer_get_syms (Layer *this) {
  return this->syms;
}

int layer_with_user_control (Layer *this) {
  return this->user_control;
}
