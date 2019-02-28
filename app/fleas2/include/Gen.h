// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef GEN_H
  #define GEN_H

#include "dmc/std.h"
#include "dmc/Darr.h"

typedef Darr Gen;

/// Creates a new gen with n values equals to 0.5.
Gen *gen_new(int n);

///
void gen_free(Gen *this);

/// Number of elements of 'this'
int gen_n(Gen *this);

/// Pointer to values of 'this'
double *gen_values(Gen *this);

/// Returns 1 if g1 == g2 with a precision of 0.0000001
int gen_eq(Gen *g1, Gen *g2);

/// Returns a new gen mutation of 'this'
Gen *gen_mutate_new(Gen *this);

///
Gen *gen_copy_new(Gen *this);

///
Js *gen_to_js_new(Gen *this);

///
Gen *gen_from_js_new(Js *js);

#endif
