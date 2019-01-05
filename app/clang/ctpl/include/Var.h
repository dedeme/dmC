// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Struct variables model

#ifndef VAR_H
  #define VAR_H

#include "dmc/std.h"
#include "StName.h"

///
enum var_Type {
  var_NONE,
  var_GETTER
};

///
typedef struct var_Var Var;

///
Var *var_new(
  enum var_Type gstype,
  const char *id,
  const char *type,
  const char *value,
  const char *ffree,
  const char *doc_null
);

///
Var *var_copy_new(Var *this);

/// If error_new_null is null, var_new_null is not null and vice versa.
void var_from_str(char **error_new_null, Var **var_new_null, const char *s);

///
void var_free(Var *this);

///
enum var_Type var_gstype(Var *this);

///
char *var_id(Var *this);

///
char *var_type(Var *this);

///
char *var_value(Var *this);

///
char *var_ffree(Var *this);

///
char *var_doc_null(Var *this);

///
void var_set_doc(Var *this, char *doc);

/// Returns '  VarType var;\n'
void var_w_struct(Var *this, Buf *bf);

/// Returns '  this->var = value;\n'
void var_w_constructor(Var *this, Buf *bf);

/// Returns '  FFree(var);\n'
void var_w_destructor(Var *this, Buf *bf);

/// Returns 'VarType name_var(Name *this) {\n  return this->var;\n}\n'
void var_w_get(Var *this, Buf *bf, StName *name);

/// Returns 'VarType name_var(Name *this);\n'
void var_wh_get(Var *this, Buf *bf, StName *name);

#endif
