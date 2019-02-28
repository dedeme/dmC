// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef FLEA_H
  #define FLEA_H

#include "dmc/std.h"
#include "Gen.h"

/*--*/

///
typedef struct flea_Flea Flea;

///
void flea_free(Flea *this);

///
char *flea_date(Flea *this);

///
char *flea_cycle(Flea *this);

///
char *flea_idn(Flea *this);

///
Gen *flea_gen(Flea *this);

///
Js *flea_to_js_new(Flea *this);

///
Flea *flea_from_js_new(Js *js);

/*--*/

/// Creates a new flea.
///   date: Date of creation.
///   cycle: Cycle of creation.
///   idn: Identifier number in cycle.
///    n: Number of elements of its gen.
Flea *flea_new(
  char *date,
  char *cycle,
  char *idn,
  int n
);

/// Returns a new Flea mutation of 'this', with a new identifier 'id'.
///   this: Flea
///   date: Date of creation.
///   cycle: Cycle of creation.
///   idn: Identifier number in cycle.
Flea *flea_mutate_new(
  Flea *this,
  char *date,
  char *cycle,
  char *idn
);

Flea *flea_copy_new(Flea *f);

#endif
