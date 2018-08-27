// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class entry record.
/// <p>
/// Fields:
///   type: Entry type
///   js: js text transpiled
///   dependencies: Identifiers of JsEntry dependencies
#ifndef TYPES_JSENTRY_H
  # define TYPES_JSENTRY_H

#include "Type.h"
#include "Deps.h"

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct jsEntry_JsEntry JsEntry;

///
JsEntry *jsEntry_new(Type *type, char *js, Deps *deps);

///
Type *jsEntry_type(JsEntry *this);

///
char *jsEntry_js(JsEntry *this);

///
Deps *jsEntry_deps(JsEntry *this);

///
Json *jsEntry_to_json(JsEntry *this);

///
JsEntry *jsEntry_from_json(Json *s);

/*.-.*/

#endif

