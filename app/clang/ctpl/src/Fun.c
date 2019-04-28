// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Fun.h"

struct fun_Fun {
  enum fun_Type gstype;
  char *id;
  char *template;
  char *value;
  char *doc_null;
};

Fun *fun_new(
  enum fun_Type gstype,
  const char *id,
  const char *template,
  const char *value,
  const char *doc_null
) {
  Fun *this = malloc(sizeof(Fun));
  this->gstype = gstype;
  this->id = str_new(id);
  this->template = str_new(template);
  this->value = str_new(value);
  this->doc_null = doc_null ? str_new(doc_null) : NULL;
  return this;
}

Fun *fun_copy_new(Fun *this) {
  return fun_new(
    this->gstype, this->id, this->template, this->value, this->doc_null
  );
}

void fun_from_str(char **error_new_null, Fun **fun_new_null, const char *s) {
  *error_new_null = NULL;
  *fun_new_null = NULL;
  char *tmp = NULL;

  // Arr[char]
  Arr *parts = str_csplit_trim_new(s, ':');
  int size = arr_size(parts);
  if (size != 2) {
    *error_new_null = str_f_new(
      "'%s': Bad elements number in function definition", s
    );
    arr_free(parts);
    return;
  }

  char *template = str_new(arr_get(parts, 0));
  enum fun_Type gstype = fun_GETTER;
  if (*template == '-') {
    gstype = fun_NONE;
    tmp = template;
    template = str_right_new(tmp, 1);
    free(tmp);
    tmp = template;
    template = str_ltrim_new(tmp);
    free(tmp);
  }

  int ix0 = str_cindex(template, '(');
  if (ix0 == -1) {
    *error_new_null = str_f_new("'%s': Bad function definition (1)", s);
    arr_free(parts);
    free(template);
    return;
  }
  ix0 = str_cindex_from(template, '*', ix0);
  if (ix0 == -1) {
    *error_new_null = str_f_new("'%s': Bad function definition (2)", s);
    arr_free(parts);
    free(template);
    return;
  }
  int ix1 = str_cindex_from(template, ')', ix0);
  if (ix1 == -1) {
    *error_new_null = str_f_new("'%s': Bad function definition (3)", s);
    arr_free(parts);
    free(template);
    return;
  }

  char *id = str_new(template);
  tmp = id;
  id = str_sub_new(tmp, ix0 + 1, ix1);
  free(tmp);
  tmp = id;
  id = str_trim_new(tmp);
  free(tmp);

  char *tp = str_new(template);
  tmp = tp;
  tp = str_left_new(tmp, ix0 + 1);
  free(tmp);
  tmp = template;
  template = str_right_new(tmp, ix1);
  free(tmp);
  tmp = tp;
  tp = str_cat_new(tmp, "$", template, NULL);
  free(tmp);

  char *value = str_new(arr_get(parts, 1));

  *fun_new_null = fun_new(gstype, id, tp, value, NULL);
  free(template);
  free(id);
  free(tp);
  free(value);
  arr_free(parts);
}

void fun_free(Fun *this) {
  free(this->id);
  free(this->template);
  free(this->value);
  free(this->doc_null);
  free(this);
}

enum fun_Type fun_gstype(Fun *this) {
  return this->gstype;
}

char *fun_id(Fun *this) {
  return this->id;
}

char *fun_template(Fun *this) {
  return this->template;
}

char *fun_value(Fun *this) {
  return this->value;
}

char *fun_doc_null(Fun *this) {
  return this->doc_null;
}

void fun_set_doc(Fun *this, char *doc) {
  this->doc_null = str_new(doc);
}

void fun_w_struct(Fun *this, Buf *bf) {
  char *tmp = str_f_new("  %s;\n", this->template);
  char *s = str_replace_new(tmp, "$", this->id);
  buf_add(bf, s);
  free(tmp);
  free(s);
}

void fun_w_constructor(Fun *this, Buf *bf) {
  char *rp = str_f_new("this->%s", this->id);
  char *tmp = str_new(this->template);
  char *tp = str_replace_new(tmp, "$", rp);
  char *s = str_f_new("  %s = %s;\n", tp, this->value);
  buf_add(bf, s);
  free(rp);
  free(tp);
  free(tmp);
  free(s);
}

/// Returns '(name_fun(Name *this))Template {\n  return this->fun;\n}\n'
void fun_w_get(Fun *this, Buf *bf, StName *name) {
  char *rp = str_f_new(
    "%s_%s(%s *this)", stName_prefix(name), this->id, stName_id(name)
  );
  char *tmp = str_new(this->template);
  char *tp = str_replace_new(tmp, "$", rp);
  char *s = str_f_new(
    "%s {\n  return this->%s;\n}\n", tp, this->id);
  buf_add(bf, s);
  free(rp);
  free(tp);
  free(tmp);
  free(s);
}

/// Returns '(name_fun)Template;\n'
void fun_wh_get(Fun *this, Buf *bf, StName *name) {
  char *rp = str_f_new(
    "%s_%s(%s *this)", stName_prefix(name), this->id, stName_id(name)
  );
  char *tmp = str_new(this->template);
  char *tp = str_replace_new(tmp, "$", rp);
  char *s = str_f_new("%s;\n", tp);
  buf_add(bf, s);
  free(rp);
  free(tp);
  free(tmp);
  free(s);
}
