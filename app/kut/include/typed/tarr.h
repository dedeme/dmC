// Copyright 20-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Typed arrays.

#ifndef TYPED_TARR_H
  #define TYPED_TARR_H

#include "kut/arr.h"
#include "kut/map.h"
#include "tval.h"

/// Typed array of ints.
typedef struct tarr_Arri Arri;
/// Typed array of ints.
typedef struct tarr_Arrf Arrf;
/// Typed array of ints.
typedef struct tarr_Arrs Arrs;

/// Constructor from Arr with ints.
/// 'a' is Arr<Exp>
Arri *tarr_fromi (Arr *a);

/// Constructor from Arr with floats.
/// 'a' is Arr<Exp>
Arrf *tarr_fromf (Arr *a);

/// Constructor from Arr with strings.
/// 'a' is Arr<Exp>
Arrs *tarr_froms (Arr *a);

/// Constructor from Map<Exp> of strings.
Arrs *tarr_fromd (Map *m);

/// Returns Arr<Exp> of ints.
Arr *tarr_to_arri (Arri *this);

/// Returns Arr<Exp> of floats.
Arr *tarr_to_arrf (Arrf *this);

/// Returns Arr<Exp> of strings.
Arr *tarr_to_arrs (Arrs *this);

/// Returns Map<Exp> of Strings.
Map *tarr_to_arrd (Arrs *this);


#endif
