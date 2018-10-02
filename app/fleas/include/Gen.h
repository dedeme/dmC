// Copyright 27-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Unit of mutation

#ifndef GEN_H
  #define GEN_H

#include "dmc/std.h"
#include "dmc/Json.h"

///
typedef struct gen_Gen Gen;

/// 'gens' is a double[size]
Gen *gen_new(int size, double *gens);

/// Mutation is +- 10%
Gen *gen_mutate(Gen *this);

///
Json *gen_to_json(Gen *this);

///
Gen *gen_from_json(Json *s);

/// There are not range test.
double gen_get(Gen *this, int ix);

/// Mesure of proximity of two gens
double gen_proximity(Gen *this, Gen *other);
#endif
