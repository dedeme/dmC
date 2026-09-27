// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "checker/layers.h"
#include "fileix.h"
#include "bmodule.h"
#include "symix.h"
#include "checker/cksym.h"

struct layers_Layers {
  Layer *layer;
  Layers *layers;
};

static Layers root = { .layer = NULL, .layers = NULL };

Layers *layers_new (Layer *layer) {
  return layers_add(&root, layer);
}

Layers *layers_add (Layers *this, Layer *layer) {
  Layers *r = MALLOC(Layers);
  r->layer = layer;
  r->layers = this;
  return r;
}

void layers_add_symbol(Layers *this, int fix, int nline, int symbol) {
  if (this->layers == &root) return;
  // <Cksym>
  Arr *syms = layer_get_syms(this->layer);
  EACH(syms, Cksym, sym) {
    if (cksym_get_id(sym) == symbol) {
      printf(
        "%s:%d: Symbol '%s' already declared in line %d\n",
        fileix_to_str(fix), nline, symix_get(symbol), cksym_get_nline(sym)
      );
      return;
    }
  }_EACH
  arr_push(syms, cksym_new(symbol, fix, nline));
}

int layers_err_if_not_found(Layers *this, Imports *imports, Cksym *sym) {
  int id = cksym_get_id(sym);
  while (this != &root) {
    EACH(layer_get_syms(this->layer), Cksym, sym2) {
      if (cksym_get_id(sym2) == id) {
        cksym_set_used(sym2);
        return -1;
      }
    }_EACH
    this = this->layers;
  }

  int fix = imports_get_fix(imports, id);
  if (fix != -1) return fix;

  if (bmodule_exists(id)) return -1;

  printf(
    "%s:%d: Symbol not declared (%s)\n",
    fileix_to_str(cksym_get_fix(sym)), cksym_get_nline(sym), symix_get(id)
  );
  return -1;
}

// Arr<Cksym>
Arr *layers_not_used(Layers *this) {
  // Cksym
  Arr *r = arr_new();
  if (layer_with_user_control(this->layer)) {
    EACH(layer_get_syms(this->layer), Cksym, sym){
      if (!cksym_is_used(sym)) arr_push(r, sym);
    }_EACH
  }
  return r;
}
