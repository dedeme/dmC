// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Wrapper of an Arr&lt;Dep>.
#ifndef TYPES_DEPS_H
  # define TYPES_DEPS_H

#include "types/Dep.h"
#include "dmc/Json.h"
#include "dmc/It.h"

///
typedef struct deps_Deps Deps;

///
Deps *deps_new(void);

/// Add 'dep' if it does not exist
void deps_add(Deps *this, Dep *dep);

///
size_t deps_size(Deps *this);

///
It/*Dep*/ *deps_to_it(Deps *this);

///
Json *deps_to_json(Deps *this);

///
Deps *deps_from_json(Json *js);


#endif
