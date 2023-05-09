// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "checker/layer.h"

struct layer_Layer {
  int used_control;
  // <Cksym>
  Arr *syms;
};

// syms is Arr<Cksym>
Layer *layer_new (int used_control, Arr *syms) {
  Layer *this = MALLOC(Layer);
  this->used_control = used_control;
  this->syms = syms;
  return this;
}

void layer_add (Layer *this, Cksym *sym) {
  arr_push(this->syms, sym);
}

Arr *layer_get_syms (Layer *this) {
  return this->syms;
}

int layer_with_used_control (Layer *this) {
  return this->used_control;
}
