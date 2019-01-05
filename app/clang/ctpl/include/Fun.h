// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Struct function model.

#ifndef FUN_H
  #define FUN_H

#include "dmc/std.h"
#include "StName.h"

///
enum fun_Type {
  fun_NONE,
  fun_GETTER
};

///
typedef struct fun_Fun Fun;

///
Fun *fun_new(
  enum fun_Type gstype,
  const char *id,
  const char *template,
  const char *value,
  const char *doc_null
);

///
Fun *fun_copy_new(Fun *this);

/// If error_new_null is null, fun_new_null is not null and vice versa.
void fun_from_str(char **error_new_null, Fun **fun_new_null, const char *s);

///
void fun_free(Fun *this);

///
enum fun_Type fun_gstype(Fun *this);

///
char *fun_id(Fun *this);

///
char *fun_template(Fun *this);

///
char *fun_value(Fun *this);

///
char *fun_doc_null(Fun *this);

///
void fun_set_doc(Fun *this, char *doc);

/// Returns '  (fun)Template;\n'
void fun_w_struct(Fun *this, Buf *bf);

/// Returns '  (this->fun)Template = value;\n'
void fun_w_constructor(Fun *this, Buf *bf);

/// Returns '(name_fun(Name *this))Template {\n  return this->fun;\n}\n'
void fun_w_get(Fun *this, Buf *bf, StName *name);

/// Returns '(name_fun(Name *this))Template;\n'
void fun_wh_get(Fun *this, Buf *bf, StName *name);

#endif
