// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Record for manage dependencies.
/// <p>
/// Fields:
///   class: Path of class.
///   entry: Identifier of entry.
#ifndef TYPES_DEP_H
  # define TYPES_DEP_H

#include "Cpath.h"

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct dep_Dep Dep;

///
Dep *dep_new(Cpath *class, char *entry);

///
Cpath *dep_class(Dep *this);

///
char *dep_entry(Dep *this);

/// Equality
bool dep_eq(Dep *this, Dep *other);

///
Ajson *dep_to_json(Dep *this);

///
Dep *dep_from_json(Ajson *s);

/*.-.*/

#endif

