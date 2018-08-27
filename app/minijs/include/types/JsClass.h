// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// JavaScript Class management
/// <p>
/// Fields:
///   entries: Hashtable with top entries.
#ifndef TYPES_JSCLASS_H
  # define TYPES_JSCLASS_H

#include "types/MjsEntry.h"

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct jsClass_JsClass JsClass;

///
JsClass *jsClass_new(MjsEntry *entries);

///
MjsEntry *jsClass_entries(JsClass *this);

///
Json *jsClass_to_json(JsClass *this);

///
JsClass *jsClass_from_json(Json *s);

/*.-.*/

#endif
