// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Layer of Cksym.

#ifndef CHECKER_LAYER_H
  #define CHECKER_LAYER_H

#include "kut/arr.h"
#include "checker/cksym.h"

///
typedef struct layer_Layer Layer;

/// Constructor.
///   used_control: if it is FALSE if layers_not_used returns ever an empty
///                 array.
///   syms        : Arr<Cksym>. Initial symbols of layer.
Layer *layer_new (int used_control, Arr *syms);

/// Adds a Cksym.
void layer_add (Layer *this, Cksym *sym);

/// Returns Arr<Cksym>
Arr *layer_get_syms (Layer *this);

///
int layer_with_used_control (Layer *this);

#endif
