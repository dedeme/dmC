// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable list of layer.
/// Layer = Arr<Cksym>

#ifndef CHECKER_LAYERS_H
  #define CHECKER_LAYERS_H

#include "kut/arr.h"
#include "kut/map.h"
#include "checker/cksym.h"
#include "checker/layer.h"

///
typedef struct layers_Layers Layers;

/// Creates Layers with a layer.
Layers *layers_new (Layer *layer);

/// Adds 'layer' on front of 'this'.
Layers *layers_add (Layers *this, Layer *layer);

/// Adds a symbol in the top layer of 'this'.
/// If 'symbol' is duplicated, it prints a error message.
///   this  :  Layers object.
///   fix   : Module-file index.
///   nline : Line number where symbol apears.
///   symbol: Symbol to add.
void layers_add_symbol(Layers *this, int fix, int nline, char *symbol);

/// Searchs 'sym' in this. If sym is found in 'imports', returns the index
/// 'fix' of it. Otherwise returns -1 and, if 'sym' is not found in 'this',
/// prints an error message.
/// 'imports' is a Map<int> whit module imports.
int layers_err_if_not_found(Layers *this, Map *imports, Cksym *sym);

/// Searchs 'sym' in the last layer of 'this', printing an error message if such
/// symbol is found.
void layers_err_if_found(Layers *this, Cksym *sym);

/// Returns an Arr<Cksym> of symbols not used in the last layer.
Arr *layers_not_used(Layers *this);

#endif
