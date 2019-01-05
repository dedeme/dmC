// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Var.h"
#include <ctype.h>

struct var_Var {
  enum var_Type gstype;
  char *id;
  char *type;
  char *value;
  char *ffree;
  char *doc_null;
};

Var *var_new(
  enum var_Type gstype,
  const char *id,
  const char *type,
  const char *value,
  const char *ffree,
  const char *doc_null
) {
  Var *this = malloc(sizeof(Var));
  this->gstype = gstype;
  this->id = str_new(id);
  this->type = str_new(type);
  this->value = str_new(value);
  this->ffree = str_new(ffree);
  this->doc_null = doc_null ? str_new(doc_null) : NULL;
  return this;
}

Var *var_copy_new(Var *this) {
  return var_new(
    this->gstype, this->id, this->type, this->value, this->ffree, this->doc_null
  );
}

void var_from_str(char **error_new_null, Var **var_new_null, const char *s) {
  *error_new_null = NULL;
  *var_new_null = NULL;

  // Arr[char]
  Arr *parts = str_csplit_trim_new(s, ':');
  int size = arr_size(parts);
  if (size < 3) {
    *error_new_null = str_f_new(
      "'%s': Too few elements in variable definition", s
    );
    arr_free(parts);
    return;
  }

  char *id = str_new(arr_get(parts, 0));
  enum var_Type gstype = var_GETTER;
  if (*id == '-') {
    gstype = var_NONE;
    str_right(&id, 1);
    str_ltrim(&id);
  }

  char *type = str_new(arr_get(parts, 1));

  char *value = str_new(arr_get(parts, 2));

  char *ffree = "";
  if (size > 3) {
    ffree = arr_get(parts, 3);
  }

  if (!*ffree) {
    if (
      str_eq(type, "int") ||
      str_eq(type, "double") ||
      str_eq(type, "time_t") ||
      str_eq(type, "size_t") ||
      str_starts(type, "enum ")
    ) {
      ffree = str_new("_i");
    } else if (str_ends(type, "*")) {
      if (str_starts(type, "char ")) {
        ffree = str_new("_s");
      } else if (str_starts(type, "Arr ")) {
        ffree = str_new("_a");
      } else if (str_starts(type, "Map ")) {
        ffree = str_new("_m");
      } else {
        ffree = str_new(type);
        str_left(&ffree, strlen(ffree) - 1);
        str_rtrim(&ffree);
        *ffree = tolower(*ffree);
      }
    } else {
      *error_new_null = str_f_new("'%s': Destructor is missing", s);
      free(id);
      free(type);
      arr_free(parts);
      return;
    }
  } else if (*ffree == '_') {
    if (
      str_eq(ffree, "_i") ||
      str_eq(ffree, "_s") ||
      str_eq(ffree, "_a") ||
      str_eq(ffree, "_m")
    ) {
      ffree = str_new(ffree);
    } else {
      *error_new_null = str_f_new(
        "'%s': Destructor code is not valid", s
      );
      free(id);
      free(type);
      arr_free(parts);
      return;
    }
  } else {
    ffree = str_new(ffree);
  }

  *var_new_null = var_new(gstype, id, type, value, ffree, NULL);
  free(id);
  free(type);
  free(value);
  free(ffree);
  arr_free(parts);
}

void var_free(Var *this) {
  free(this->id);
  free(this->type);
  free(this->value);
  free(this->ffree);
  free(this->doc_null);
  free(this);
}

enum var_Type var_gstype(Var *this) {
  return this->gstype;
}

char *var_id(Var *this) {
  return this->id;
}

char *var_type(Var *this) {
  return this->type;
}

char *var_value(Var *this) {
  return this->value;
}

char *var_ffree(Var *this) {
  return this->ffree;
}

char *var_doc_null(Var *this) {
  return this->doc_null;
}

void var_set_doc(Var *this, char *doc) {
  this->doc_null = str_new(doc);
}

void var_w_struct(Var *this, Buf *bf) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
   s = str_f_new("  %s%s;\n", this->type, this->id);
  } else {
   s = str_f_new("  %s %s;\n", this->type, this->id);
  }
  buf_add(bf, s);
  free(s);
}

void var_w_constructor(Var *this, Buf *bf) {
  char *s = str_f_new("  this->%s = %s;\n", this->id, this->value);
  buf_add(bf, s);
  free(s);
}

void var_w_destructor(Var *this, Buf *bf) {
  char *s;
  char *ffree = this->ffree;
  if (str_eq(ffree, "_i")) {
    return;
  }
  if (str_eq(ffree, "_s")) {
    s = str_f_new("  free(this->%s);\n", this->id);
  } else if (str_eq(ffree, "_a")) {
    s = str_f_new("  arr_free(this->%s);\n", this->id);
  } else if (str_eq(ffree, "_m")) {
    s = str_f_new("  map_free(this->%s);\n", this->id);
  } else if (*ffree == '!') {
    s = str_f_new("  %s(this->%s);\n", ffree + 1, this->id);
  } else {
    s = str_f_new("  %s_free(this->%s);\n", this->ffree, this->id);
  }
  buf_add(bf, s);
  free(s);
}

void var_w_get(Var *this, Buf *bf, StName *name) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
    s = str_f_new(
      "%s%s_%s(%s *this) {\n  return this->%s;\n}\n",
      this->type, stName_prefix(name), this->id, stName_id(name), this->id
    );
  } else {
    s = str_f_new(
      "%s %s_%s(%s *this) {\n  return this->%s;\n}\n",
      this->type, stName_prefix(name), this->id, stName_id(name), this->id
    );
  }
  buf_add(bf, s);
  free(s);
}

/// Returns 'VarType name_var(Name *this);\n'
void var_wh_get(Var *this, Buf *bf, StName *name) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
    s = str_f_new(
      "%s%s_%s(%s *this);\n",
      this->type, stName_prefix(name), this->id, stName_id(name)
    );
  } else {
    s = str_f_new(
      "%s %s_%s(%s *this);\n",
      this->type, stName_prefix(name), this->id, stName_id(name)
    );
  }
  buf_add(bf, s);
  free(s);
}
