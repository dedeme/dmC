// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Struct name model.

#ifndef STNAME_H
  #define STNAME_H

#include "dmc/std.h"

///
enum stName_serialType {
  stName_SERIAL,
  stName_TO,
  stName_FROM,
  stName_NONE
};

///
typedef struct stName_StName StName;

///
StName *stName_new(
  const char *id,
  int public,
  enum stName_serialType stype,
  const char *doc_null
);

///
StName *stName_copy_new(StName *this);

/// If error_new_null is null, name_new_null is not null and vice versa.
void stName_from_str(
  char **error_new_null,
  StName **name_new_null,
  const char *s
);

///
void stName_free(StName *this);

///
char *stName_id(StName *this);

///
char *stName_prefix(StName *this);

///
int stName_public(StName *this);

///
enum stName_serialType stName_stype(StName *this);

///
char *stName_doc_null(StName *this);

///
void stName_set_doc(StName *this, const char *doc);

/// Writes 'name_'
void stName_w_prefix(StName *this, Buf *bf);

/// Writes 'struct name_Name {\n'
void stName_w_struct(StName *this, Buf *bf);

/// Writes 'Name *name_new(' or 'Name *_name_new('
void stName_w_constructor_header(StName *this, Buf *bf);

/// Writes '  Name *this = malloc(sizeof(Name));\n'
void stName_w_constructor_body(StName *this, Buf *bf);

/// Writes 'void name_free(Name *this) {\n'
void stName_w_destructor(StName *this, Buf *bf);

/// Writes 'Js *name_to_js_new(Name *this) {\n'
void stName_w_to_header(StName *this, Buf *bf);

/// Writes 'Name *name_from_js_new(Js *js) {\n'
void stName_w_from_header(StName *this, Buf *bf);

/// Writes 'return name_new(' or 'return _name_new('
void stName_w_from_body(StName *this, Buf *bf);

/// Writes 'typedef struct name_Name Name;\n"
void stName_wh_struct(StName *this, Buf *bf);

/// Writes 'Name *name_new('
void stName_wh_constructor(StName *this, Buf *bf);

/// Writes 'void name_free(Name *this);\n'
void stName_wh_destructor(StName *this, Buf *bf);

/// Writes 'Js *name_to_js_new(Name *this);\n"
void stName_wh_to(StName *this, Buf *bf);

/// Writes 'Name *name_from_js_new(Js *js);\n"
void stName_wh_from(StName *this, Buf *bf);

#endif
