// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Container made of layers of dictionaries (Maps).

#ifndef DATA_DIC_LAYER_H
  #define DATA_DIC_LAYER_H

#include "kut/map.h"

/// Structure.
/// Arr<Map>
typedef Arr DicLayers;

/// Constructor
/// The new DicLayer already contains a Map.
DicLayers *dicLayers_new (void);

/// Adds a new layer on top of 'this'.
void dicLayers_add_layer (DicLayers *this);

/// Returns the firs element with 'key', searching in Maps from top to bottom.
/// NOTE: empty key (== "") returns "opt_none".
Opt *dicLayers_get_element (DicLayers *this, char *key);

/// Returns the top layer of 'this'.
Map *dicLayers_peek_layer (DicLayers *this);

/// Adds a new element in the top Map. If 'key' is duplicated, its value
/// is overwrite.
void dicLayers_put_element (DicLayers *this, char *key, void *value);

/// Removes the top layer of 'this'.
void dicLayers_remove_layer (DicLayers *this);

#endif
