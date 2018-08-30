// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Skeleton entry record.
/// <p>
/// Fields:
///   type: Entry type
#ifndef TYPES_SKENTRY_H
  # define TYPES_SKENTRY_H

#include "Type.h"

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct skEntry_SkEntry SkEntry;

///
SkEntry *skEntry_new(Type *type);

///
Type *skEntry_type(SkEntry *this);

///
Json *skEntry_to_json(SkEntry *this);

///
SkEntry *skEntry_from_json(Json *s);

/*.-.*/

#endif

