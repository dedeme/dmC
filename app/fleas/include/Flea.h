// Copyright 27-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Flea structure

#ifndef FLEA_H
  #define FLEA_H

#include "dmc/std.h"
#include "Gen.h"
#include "market/Quote.h"

typedef struct fresults_Fresults Fresults;

/*--*/

///
typedef struct flea_Flea Flea;

///
void flea_free(Flea *this);

///
char *flea_id(Flea *this);

///
Gen *flea_gen(Flea *this);

///
int flea_ndays(Flea *this);

///
double flea_buy_strip(Flea *this);

///
double flea_sell_strip(Flea *this);

///
int flea_eq_gen(Flea *this, Flea *other);

///
Js *flea_to_js_new(Flea *this);

///
Flea *flea_from_js_new(Js *js);

/*--*/

/// Makes a Flea with an intial mutation
Flea *flea_new(char *id);

/// Makes a Flea with a 'gen'
Flea *flea_gen_new(char *id, Gen *gen);

/// Makes a new flea
Flea *flea_copy_new(Flea *this);

/// Makes a Flea mutating 'this'
Flea *flea_mutate_new(Flea *this, char *new_id);

/// Cycle initialization
void flea_init(Flea *this, int companies_number);

/// Cycle process
Fresults *flea_process_new(Flea *this, Quote **quotes, int q_ix, int q_end);

#endif
