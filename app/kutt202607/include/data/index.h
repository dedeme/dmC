// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module index.

#ifndef DATA_INDEX_H
  #define DATA_INDEX_H

#include "kut/arr.h"

///
typedef Arr Index;

///
char *index_to_js (Index *this);

///
Index *index_from_js (char *js);

#endif
