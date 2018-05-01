// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Sym type

#ifndef AST_TYPE_H
  # define AST_TYPE_H

#include <dmc/all.h>

typedef Arr Atype;

///
enum Type_t {
  DATA,
  ANY,
  VOID,
  FN,
  UNKNOWN
};

/*.-.*/

///
typedef struct type_Type Type;

///
enum Type_t type_type(Type *this);

///
char *type_id(Type *this);

///
Atype *type_params(Type *this);

///
Json *type_serialize(Type *this);

///
Type *type_restore(Json *s);

/*.-.*/

///
Type *type_new_unknown(void);

///
Type *type_new_bool(void);

///
Type *type_new_byte(void);

///
Type *type_new_int(void);

///
Type *type_new_float(void);

///
Type *type_new_char(void);

///
Type *type_new_str(void);

///
Type *type_new_any(void);

///
Type *type_new_void(void);

///
Type *type_new_arr(Type *t);

///
Type *type_new_map(Type *t);

///
Type *type_new_fn(Atype *ts);

///
Type *type_new_data(char *id, Atype *generics);

///
bool type_eq(Type *this, Type *other);

/// If type is a not standard DATA must use its complete path_id
bool type_child(Type *this, Type *child);

///
bool type_is_unknown(Type *this);

///
char *type_to_str(Type *this);

#endif

