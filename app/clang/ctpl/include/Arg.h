// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constructor arguments model.

#ifndef ARG_H
  #define ARG_H

#include "dmc/std.h"
#include "StName.h"

///
enum arg_Type {
  arg_NONE,
  arg_GETTER,
  arg_SETTER
};

///
typedef struct arg_Arg Arg;

///
Arg *arg_new(
  enum arg_Type gstype,
  const char *id,
  const char *type,
  const char *ffree,
  const char *fserial,
  const char *doc_null
);

///
Arg *arg_copy_new(Arg *this);

/// If error_new_null is null, arg_new_null is not null and vice versa.
void arg_from_str(char **error_new_null, Arg **arg_new_null, const char *s);

///
void arg_free(Arg *this);

///
enum arg_Type arg_gstype(Arg *this);

///
char *arg_id(Arg *this);

///
char *arg_type(Arg *this);

///
char *arg_ffree(Arg *this);

///
char *arg_fserial(Arg *this);

///
char *arg_doc_null(Arg *this);

///
void arg_set_doc(Arg *this, char *doc);

/// Returns '  ArgType arg;\n'
void arg_w_struct(Arg *this, Buf *bf);

/// Returns 'ArType arg"
void arg_w_constructor_head(Arg *this, Buf *bf);

/// Returns '  this->arg = arg;\n'
void arg_w_constructor_body(Arg *this, Buf *bf);

/// Returns '  FFree(arg);\n'
void arg_w_destructor(Arg *this, Buf *bf);

/// Returns 'ArgType name_arg(Name *this) {\n  return this->arg;\n}\n'
void arg_w_get(Arg *this, Buf *bf, StName *name);

/// Returns 'void name_set_arg(Name *this, ArgType v) {\n  this->arg = v;\n}\n'
void arg_w_set(Arg *this, Buf *bf, StName *name);

/// Returns '  arr_push(a, ToJs this->arg);\n'
void arg_w_to(Arg *this, Buf *bf);

/// Returns '  this->arg = FromJs arr_get(a, i++);\n'
void arg_w_from(Arg *this, Buf *bf);

/// Returns 'ArgType name_arg(Name *this);\n'
void arg_wh_get(Arg *this, Buf *bf, StName *name);

/// Returns 'void name_set_arg(Name *this, ArgType v);\n'
void arg_wh_set(Arg *this, Buf *bf, StName *name);





#endif
