// Copyright 26-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Gen structure

#ifndef GEN_H
  #define GEN_H

#include "dmc/std.h"

/// Gen fields.
enum gen_Ix { gen_DAYS, gen_BUY, gen_SELL };

///
typedef struct gen_Gen Gen;

///
Gen *gen_new(void);

///
Gen *gen_values_new(double days, double buy_strip, double sell_strip);

///
Gen *gen_copy_new(Gen *this);

///
double gen_get(Gen *this, enum gen_Ix ix);

///
void gen_set(Gen *this, enum gen_Ix ix, double value);

///
Gen *gen_mutate_new(Gen *this);

///
double gen_proximity(Gen *this, Gen *other);

///
Js *gen_to_js_new(Gen *this);

///
Gen *gen_from_js_new(Js *js);

#endif
