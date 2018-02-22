// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Mutable data.<p>
/// A Gen has two values of type uint:
///   actual : It is the value of Gen.
///   max : Maximun value for 'actual', exclusive.
/// The constructor 'gen_new' assigns randomly an actual value to the new Gen.

#ifndef GEN_H
  # define GEN_H

#include <stdlib.h>
#include <Json.h>

///
typedef struct gen_Gen Gen;

/// gen_new creates a new Gen with a randomly actual value.
Gen *gen_new(uint maxOptions);

///
Gen *gen_copy(Gen *this);

/// gen_mutate returns a new Gen mutated from 'g'.
Gen *gen_mutate(Gen *this);

///
uint gen_max(Gen *this);

///
uint gen_actual(Gen *this);

///
Json *gen_serialize(Gen *this);

///
Gen *gen_restore(Json *serial);

#endif

