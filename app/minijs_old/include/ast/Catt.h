// Copyright 05-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Class Attribute (Methods and Vals).<p>
/// There are following types of Catt:
///   new: Constructor.
///   val: They are invariant. It can not be null.
///   var: Their value can change. It can be null.
///   nat: They are invariant and it only one value is null. Used when the
///        the attribute exists in javascript.

#ifndef AST_CATT_H
  # define AST_CATT_H

typedef struct type_Type Type;
typedef struct value_Value Value;

///
enum Catt_t {
  catt_VAR,
  catt_VAL,
  catt_NAT,
  catt_NEW
};

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct catt_Catt Catt;

///
Catt *catt_new(
  bool is_public,
  bool is_static,
  enum Catt_t t,
  char *id,
  Type *type,
  Value *value
);

///
bool catt_is_public(Catt *this);

///
bool catt_is_static(Catt *this);

///
enum Catt_t catt_t(Catt *this);

///
char *catt_id(Catt *this);

///
Type *catt_type(Catt *this);

///
Value *catt_value(Catt *this);

///
Ajson *catt_to_json(Catt *this);

///
Catt *catt_from_json(Ajson *s);

/*.-.*/

#endif
