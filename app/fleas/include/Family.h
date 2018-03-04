// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Family data.

#ifndef FAMILY_H
  # define FAMILY_H

#include "dmc/Json.h"
#include "stdbool.h"
#include "Gen.h"
#include "DEFS.h"

/*.-.*/

///
typedef struct family_Family Family;

///
enum Family_option family_opt(Family *this);

///
Gen *family_gen(Family *this);

///
Json *family_serialize(Family *this);

///
Family *family_restore(Json *s);

/*.-.*/

///
Family *family_new(enum Family_option opt);

///
bool family_eq(Family *this, Family *other);

///
Family *family_copy(Family *this);

/// family_mutate returns a new Family mutated from 'this'.
Family *family_mutate(Family *this);

///
uint family_max(Family *this);

///
uint family_actual(Family *this);

#endif

