// Copyright 21-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Arg.h"
#include <ctype.h>

struct arg_Arg {
  enum arg_Type gstype;
  char *id;
  char *type;
  char *ffree;
  char *fserial;
  char *doc_null;
};

///
Arg *arg_new(
  enum arg_Type gstype,
  const char *id,
  const char *type,
  const char *ffree,
  const char *fserial,
  const char *doc_null
) {
  Arg *this = malloc(sizeof(Arg));
  this->gstype = gstype;
  this->id = str_new(id);
  this->type = str_new(type);
  this->ffree = str_new(ffree);
  this->fserial = str_new(fserial);
  this->doc_null = doc_null ? str_new(doc_null) : NULL;
  return this;
}

Arg *arg_copy_new(Arg *this) {
  return arg_new(
    this->gstype, this->id, this->type, this->ffree, this->fserial,
    this->doc_null
  );
}

void arg_from_str(char **error_new_null, Arg **arg_new_null, const char *s) {
  *error_new_null = NULL;
  *arg_new_null = NULL;
  char *tmp = NULL;

  // Arr[char]
  Arr *parts = str_csplit_trim_new(s, ':');
  int size = arr_size(parts);
  if (size < 2) {
    *error_new_null = str_f_new(
      "'%s': Too few elements in constructor arguments", s
    );
    arr_free(parts);
    return;
  }

  char *id = str_new(arr_get(parts, 0));
  enum arg_Type gstype = arg_GETTER;
  if (*id == '-') {
    gstype = arg_NONE;
    tmp = id;
    id = str_right_new(tmp, 1);
    free(tmp);
    tmp = id;
    id = str_ltrim_new(tmp);
    free(tmp);
  } else if (*id == '@') {
    gstype = arg_SETTER;
    tmp = id;
    id = str_right_new(tmp, 1);
    free(tmp);
    tmp = id;
    id = str_ltrim_new(tmp);
    free(tmp);
  }

  char *type = str_new(arr_get(parts, 1));

  char *ffree = "";
  if (size > 2) {
    ffree = arr_get(parts, 2);
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
        tmp = ffree;
        ffree = str_left_new(tmp, strlen(ffree) - 1);
        free(tmp);
        tmp = ffree;
        ffree = str_rtrim_new(tmp);
        free(tmp);
        *ffree = tolower(*ffree);
      }
    } else {
      *error_new_null = str_f_new(
        "'%s': Destructor is missing", s
      );
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

  char *fserial = "";
  if (size > 3) {
    fserial = arr_get(parts, 3);
  }

  if (!*fserial) {
    if (str_eq(type, "int") || str_starts(type, "enum ")) {
      fserial = str_new("_i");
    } else if (str_ends(type, "*")) {
      if (str_starts(type, "char ")) {
        fserial = str_new("_s");
      } else if (str_starts(type, "Arr ") || str_starts(type, "Map ")) {
        fserial = str_new("");
      } else {
        fserial = str_new(type);
        tmp = fserial;
        fserial = str_left_new(tmp, strlen(fserial) - 1);
        free(tmp);
        tmp = fserial;
        fserial = str_rtrim_new(tmp);
        free(tmp);
        *fserial = tolower(*fserial);
      }
    } else {
      fserial = str_new("");
    }
  } else {
    char *prefix = "";
    if (str_starts(type, "Arr ")) {
      prefix = "_a ";
    }
    if (str_starts(type, "Map ")) {
      prefix = "_m ";
    }

    if (*fserial == '_') {
      if (
        str_eq(fserial, "_i") ||
        str_eq(fserial, "_b") ||
        str_eq(fserial, "_s") ||
        ( str_starts(fserial, "_d") &&
          *(fserial + 2) >= '0' &&
          *(fserial + 2) <= '9' &&
          !*(fserial + 3)
        )
      ) {
        fserial = str_cat_new(prefix, fserial, NULL);
      } else {
        *error_new_null = str_f_new(
          "'%s': Serializer code is not valid", s
        );
        free(id);
        free(type);
        free(ffree);
        arr_free(parts);
        return;
      }
    } else {
      fserial = str_cat_new(prefix, fserial, NULL);
    }
  }

  *arg_new_null = arg_new(gstype, id, type, ffree, fserial, NULL);
  free(id);
  free(type);
  free(ffree);
  free(fserial);
  arr_free(parts);
}

void arg_free(Arg *this) {
  free(this->id);
  free(this->type);
  free(this->ffree);
  free(this->fserial);
  free(this->doc_null);
  free(this);
}

enum arg_Type arg_gstype(Arg *this) {
  return this->gstype;
}

char *arg_id(Arg *this) {
  return this->id;
}

char *arg_type(Arg *this) {
  return this->type;
}

char *arg_ffree(Arg *this) {
  return this->ffree;
}

char *arg_fserial(Arg *this) {
  return this->fserial;
}

char *arg_doc_null(Arg *this) {
  return this->doc_null;
}

void arg_set_doc(Arg *this, char *doc) {
  this->doc_null = str_new(doc);
}

void arg_w_struct(Arg *this, Buf *bf) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
    s = str_f_new("  %s%s;\n", this->type, this->id);
  } else {
    s = str_f_new("  %s %s;\n", this->type, this->id);
  }
  buf_add(bf, s);
  free(s);
}

void arg_w_constructor_head(Arg *this, Buf *bf) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
    s = str_f_new("%s%s", this->type, this->id);
  } else {
    s = str_f_new("%s %s", this->type, this->id);
  }
  buf_add(bf, s);
  free(s);
}

void arg_w_constructor_body(Arg *this, Buf *bf) {
  char *s;
  s = str_f_new("  this->%s = %s;\n", this->id, this->id);
  buf_add(bf, s);
  free(s);
}

void arg_w_destructor(Arg *this, Buf *bf) {
  char *s;
  char *ffree = this->ffree;
  if (str_eq(ffree, "_i")) {
    return;
  }
  if (str_eq(ffree, "_s")) {
    s = str_f_new("    free(this->%s);\n", this->id);
  } else if (str_eq(ffree, "_a")) {
    s = str_f_new("    arr_free(this->%s);\n", this->id);
  } else if (str_eq(ffree, "_m")) {
    s = str_f_new("    map_free(this->%s);\n", this->id);
  } else if (*ffree == '!') {
    s = str_f_new("    %s(this->%s);\n", ffree + 1, this->id);
  } else {
    s = str_f_new("    %s_free(this->%s);\n", this->ffree, this->id);
  }
  buf_add(bf, s);
  free(s);
}

void arg_w_get(Arg *this, Buf *bf, StName *name) {
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

void arg_w_set(Arg *this, Buf *bf, StName *name) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
    s = str_f_new(
      "void %s_set_%s(%s *this, %sv) {\n  this->%s = v;\n}\n",
      stName_prefix(name), this->id, stName_id(name), this->type, this->id
    );
  } else {
    s = str_f_new(
      "void %s_set_%s(%s *this, %s v) {\n  this->%s = v;\n}\n",
      stName_prefix(name), this->id, stName_id(name), this->type, this->id
    );
  }
  buf_add(bf, s);
  free(s);
}

/// Returns '  arr_push(a, ToJs this->arg);\n'
void arg_w_to(Arg *this, Buf *bf) {
  char *fserial = this->fserial;
  if (!*fserial) {
    return;
  }
  char *s;
  if (str_eq(fserial, "_i")) {
    s = str_f_new("  arr_push(a, js_wi_new(this->%s));\n", this->id);
  } else if (str_eq(fserial, "_b")) {
    s = str_f_new("  arr_push(a, js_wb_new(this->%s));\n", this->id);
  } else if (str_eq(fserial, "_s")) {
    s = str_f_new("  arr_push(a, js_ws_new(this->%s));\n", this->id);
  } else if (str_starts(fserial, "_d")) {
    s = str_f_new(
      "  arr_push(a, js_wd_new(this->%s, %s));\n", this->id, fserial + 2
    );
  } else if (str_starts(fserial, "_a ")) {
    char *tmp = str_new(fserial);
    char *tmp2 = tmp;
    tmp = str_right_new(tmp2, 3);
    free(tmp2);
    tmp2 = tmp;
    tmp = str_ltrim_new(tmp2);
    free(tmp2);
    if (str_eq(tmp, "_s")) {
      s = str_f_new(
        "  arr_push(a, arr_to_js_new(this->%s, (FTO)js_ws_new));\n",
        this->id
      );
    } else {
      s = str_f_new(
        "  arr_push(a, arr_to_js_new(this->%s, (FTO)%s_to_js_new));\n",
        this->id, tmp
      );
    }
    free(tmp);
  } else if (str_starts(fserial, "_m ")) {
    char *tmp = str_new(fserial);
    char *tmp2 = tmp;
    tmp = str_right_new(tmp2, 3);
    free(tmp2);
    tmp2 = tmp;
    tmp = str_ltrim_new(tmp2);
    free(tmp2);
    if (str_eq(tmp, "_s")) {
      s = str_f_new(
        "  arr_push(a, map_to_js_new(this->%s, (FTO)js_ws_new));\n",
        this->id
      );
    } else {
      s = str_f_new(
        "  arr_push(a, map_to_js_new(this->%s, (FTO)%s_to_js_new));\n",
        this->id, tmp
      );
    }
    free(tmp);
  } else {
    s = str_f_new(
      "  arr_push(a, %s_to_js_new(this->%s));\n", fserial, this->id
    );
  }

  buf_add(bf, s);
  free(s);
}

/// Returns '  this->arg = FromJs arr_get(a, i++);\n'
void arg_w_from(Arg *this, Buf *bf) {
  char *fserial = this->fserial;
  if (!*fserial) {
    return;
  }
  char *s;
  if (str_eq(fserial, "_i")) {
    s = str_f_new("  int %s = js_ri(arr_get(a, i++));\n", this->id);
  } else if (str_eq(fserial, "_b")) {
    s = str_f_new("  int %s = js_rb(arr_get(a, i++));\n", this->id);
  } else if (str_eq(fserial, "_s")) {
    s = str_f_new("  char *%s = js_rs_new(arr_get(a, i++));\n", this->id);
  } else if (str_starts(fserial, "_d")) {
    s = str_f_new("  double %s = js_rd(arr_get(a, i++));\n", this->id);
  } else if (str_starts(fserial, "_a ")) {
    char *tmp = str_new(fserial);
    char *tmp2 = tmp;
    tmp = str_right_new(tmp2, 3);
    free(tmp2);
    tmp2 = tmp;
    tmp = str_ltrim_new(tmp2);
    free(tmp2);
    if (str_eq(tmp, "_s")) {
      s = str_f_new(
        "  Arr *%s = arr_from_js_new(arr_get(a, i++), "
        "(FFROM)js_rs_new, free);\n",
        this->id
      );
    } else {
      s = str_f_new(
        "  Arr *%s = arr_from_js_new(arr_get(a, i++), "
        "(FFROM)%s_from_js_new, (FPROC)%s_free);\n",
        this->id, tmp, tmp
      );
    }
    free(tmp);
  } else if (str_starts(fserial, "_m ")) {
    char *tmp = str_new(fserial);
    char *tmp2 = tmp;
    tmp = str_right_new(tmp2, 3);
    free(tmp2);
    tmp2 = tmp;
    tmp = str_ltrim_new(tmp2);
    free(tmp2);
    if (str_eq(tmp, "_s")) {
      s = str_f_new(
        "  Map *%s = map_from_js_new(arr_get(a, i++), "
        "(FFROM)js_rs_new, free);\n",
        this->id
      );
    } else {
      s = str_f_new(
        "  Map *%s = map_from_js_new(arr_get(a, i++), "
        "(FFROM)%s_from_js_new, (FPROC)%s_free);\n",
        this->id, tmp, tmp
      );
    }
    free(tmp);
  } else {
    s = str_f_new(
      "  %s%s = %s_from_js_new(arr_get(a, i++));\n",
      this->type, this->id, fserial
    );
  }

  buf_add(bf, s);
  free(s);
}

/// Returns 'ArgType name_arg(Name *this);\n'
void arg_wh_get(Arg *this, Buf *bf, StName *name) {
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

/// Returns 'void name_set_arg(Name *this, ArgType v);\n'
void arg_wh_set(Arg *this, Buf *bf, StName *name) {
  char *s;
  if (this->type[strlen(this->type) - 1] == '*') {
    s = str_f_new(
      "void %s_set_%s(%s *this, %sv);\n",
      stName_prefix(name), this->id, stName_id(name), this->type
    );
  } else {
    s = str_f_new(
      "void %s_set_%s(%s *this, %s v);\n",
      stName_prefix(name), this->id, stName_id(name), this->type
    );
  }
  buf_add(bf, s);
  free(s);
}
