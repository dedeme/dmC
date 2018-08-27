// Copyright 05-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Type

#ifndef AST_TYPE_H
  # define AST_TYPE_H

typedef struct ltype_Ltype Ltype;
typedef struct asym_Asym Asym;

///
enum Type_t {
  type_ANY,
  type_VOID,
  type_UNKNOWN,
  type_FN,
  type_DATA
};

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct type_Type Type;

///
enum Type_t type_t(Type *this);

///
char *type_id(Type *this);

///
Ltype *type_params(Type *this);

///
Ajson *type_to_json(Type *this);

///
Type *type_from_json(Ajson *s);

/*.-.*/

///
Type *type_new_any(void);

///
Type *type_new_void(void);

///
Type *type_new_unknown(void);

///
Type *type_new_fn(Ltype *params);

///
Type *type_new_data(char *id, Ltype *generics);

///
bool type_eq(Type *t1, Type *t2);

/// type_test_symbols returns "" if all symbols are known or the identifier
/// which does not mach.
char *type_test_symbols(Type *this, Asym *symbols);

///
char *type_to_str(Type *this);

#endif
