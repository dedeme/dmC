// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "StName.h"
#include <ctype.h>

struct stName_StName {
  char *id;
  char *prefix;
  int public;
  enum stName_serialType stype;
  char *doc_null;
};

StName *stName_new(
  const char *id,
  int public,
  enum stName_serialType stype,
  const char *doc_null
) {
  StName *this = malloc(sizeof(StName));
  this->id = str_new(id);
  this->prefix = str_new(id);
  *this->prefix = tolower(*id);
  this->public = public;
  this->stype = stype;
  this->doc_null = doc_null ? str_new(doc_null) : NULL;
  return this;
}

StName *stName_copy_new(StName *this) {
  return stName_new(this->id, this->public, this->stype, this->doc_null);
}

void stName_from_str(
  char **error_new_null,
  StName **name_new_null,
  const char *s
) {
  *error_new_null = NULL;
  *name_new_null = NULL;

  // Arr[char]
  Arr *parts = str_csplit_trim_new(s, ':');
  char *id = str_new(arr_get(parts, 0));
  int public = 1;
  if (*id == '-') {
    public = 0;
    str_right(&id, 1);
    str_ltrim(&id);
  }

  int size = arr_size(parts);
  enum stName_serialType stype = stName_NONE;
  if (size > 1) {
    char *t = arr_get(parts, 1);
    if (str_eq(t, "serial")) {
      stype = stName_SERIAL;
    } else if (str_eq(t, "to")) {
      stype = stName_TO;
    } else if (str_eq(t, "from")) {
      stype = stName_FROM;
    } else {
      *error_new_null = str_f_new(
        "'%s': Serialization type is not valid", s
      );
      arr_free(parts);
      free(id);
      return;
    }
  }

  *name_new_null = stName_new(id, public, stype, NULL);
  arr_free(parts);
  free(id);
}

void stName_free(StName *this) {
  free(this->id);
  free(this->prefix);
  free(this->doc_null);
  free(this);
}

char *stName_id(StName *this) {
  return this->id;
}

char *stName_prefix(StName *this) {
  return this->prefix;
}

int stName_public(StName *this) {
  return this->public;
}

enum stName_serialType stName_stype(StName *this) {
  return this->stype;
}

char *stName_doc_null(StName *this) {
  return this->doc_null;
}

void stName_set_doc(StName *this, const char *doc) {
  this->doc_null = str_new(doc);
}

void stName_w_prefix(StName *this, Buf *bf) {
  char *s = str_f_new("%s_", this->prefix);
  buf_add(bf, s);
  free(s);
}

void stName_w_struct(StName *this, Buf *bf) {
  char *s = str_f_new("struct %s_%s {\n", this->prefix, this->id);
  buf_add(bf, s);
  free(s);
}

void stName_w_constructor_header(StName *this, Buf *bf) {
  char *s;
  if (this->public) {
    s = str_f_new("%s *%s_new(", this->id, this->prefix);
  } else {
    s = str_f_new("%s *_%s_new(", this->id, this->prefix);
  }
  buf_add(bf, s);
  free(s);
}

void stName_w_constructor_body(StName *this, Buf *bf) {
  char *s = str_f_new(
    "  %s *this = malloc(sizeof(%s));\n", this->id, this->id
  );
  buf_add(bf, s);
  free(s);
}

/// Writes 'void name_free(Name *this) {\n'
void stName_w_destructor(StName *this, Buf *bf) {
  char *s = str_f_new("void %s_free(%s *this) {\n", this->prefix, this->id);
  buf_add(bf, s);
  free(s);
}

void stName_w_to_header(StName *this, Buf *bf) {
  char *s = str_f_new(
    "Js *%s_to_js_new(%s *this) {\n", this->prefix, this->id
  );
  buf_add(bf, s);
  free(s);
}

void stName_w_from_header(StName *this, Buf *bf) {
  char *s = str_f_new(
    "%s *%s_from_js_new(Js *js) {\n", this->id, this->prefix
  );
  buf_add(bf, s);
  free(s);
}

void stName_w_from_body(StName *this, Buf *bf) {
  char *s;
  if (this->public) {
    s = str_f_new("  return %s_new(", this->prefix);
  } else {
    s = str_f_new("  return _%s_new(", this->prefix);
  }
  buf_add(bf, s);
  free(s);
}

void stName_wh_struct(StName *this, Buf *bf) {
  char *s = str_f_new(
    "typedef struct %s_%s %s;\n", this->prefix, this->id, this->id
  );
  buf_add(bf, s);
  free(s);
}

void stName_wh_constructor(StName *this, Buf *bf) {
  char *s = str_f_new("%s *%s_new(", this->id, this->prefix);
  buf_add(bf, s);
  free(s);
}

/// Writes 'void name_free(Name *this);\n'
void stName_wh_destructor(StName *this, Buf *bf) {
  char *s = str_f_new("void %s_free(%s *this);\n", this->prefix, this->id);
  buf_add(bf, s);
  free(s);
}

void stName_wh_to(StName *this, Buf *bf) {
  char *s = str_f_new(
    "Js *%s_to_js_new(%s *this);\n", this->prefix, this->id
  );
  buf_add(bf, s);
  free(s);
}

void stName_wh_from(StName *this, Buf *bf) {
  char *s = str_f_new(
    "%s *%s_from_js_new(Js *js);\n", this->id, this->prefix
  );
  buf_add(bf, s);
  free(s);
}
