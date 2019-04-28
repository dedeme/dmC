// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Tpl.h"
#include "dmc/Dec.h"

struct tpl_TplArgument {
  // Opt[char]
  Opt *comment;
  enum AttType access;
  char *name;
  char *type;
};

TplArgument *tplArgument_new (void) {
  TplArgument *this = MALLOC(TplArgument);
  this->comment = opt_empty();
  this->access = NO_ACCESS;
  this->name = "";
  this->type = "";
  return this;
}

// Opt[char]
Opt *tplArgument_comment (TplArgument *this) {
  return this->comment;
}

void tplArgument_set_comment (TplArgument *this, char *doc) {
  this->comment = opt_new(doc);
}

enum AttType tplArgument_access (TplArgument *this) {
  return this->access;
}

void tplArgument_set_access (TplArgument *this, enum AttType type) {
  this->access = type;
}

char *tplArgument_name (TplArgument *this) {
  return this->name;
}

char *tplArgument_type (TplArgument *this) {
  return this->type;
}

static char *coll = " Opt Arr List Map Hash ";

static char *test_single_type(char *t) {
  if (str_index(coll, str_f(" %s ", t)) != -1) {
    return str_f("'%s' used as single type", t);
  }

  if (str_ends(t, "*") && !str_eq(t, "char*")) {
    return "Only 'char *' ends at '*'";
  }

  return "";
}

static char *test_collection_type(char *t) {
  if (str_index(coll, str_f(" %s ", t)) != -1) {
    return "";
  }
  return "Only 'Opt', 'Arr', 'List', 'Map' or 'Hash' can be collection type";
}

static char *test_type (char *type) {
  if (!*type) {
    return "Argument type is empty";
  }

  // Arr[char]
  Arr *parts = str_csplit_trim(type, '-');
  int sz = arr_size(parts);
  char *err;
  if (sz == 1) {
    err = test_single_type(arr_get(parts, 0));
    if (*err) {
      return err;
    }
  } else if (sz == 2) {
    err = test_collection_type(arr_get(parts, 0));
    if (*err) {
      return (err);
    }
    err = test_single_type(arr_get(parts, 1));
    if (*err) {
      return err;
    }
  } else {
    return "Too much '-'";
  }

  return "";
}

static char *intrim (char *s){
  Buf *bf = buf_new();
  int i;
  for (i = 0; i < strlen(s); ++i) {
    char ch = s[i];
    if (ch > ' ') {
      buf_cadd(bf, ch);
    }
  }
  return buf_to_str(bf);
}

char *tplArgument_set_name_type (TplArgument *this, char *name, char *type) {
  if (!*name) {
    return "Argument name is empty";
  }

  type = intrim(type);

  char *err = test_type(type);

  if (*err) {
    return err;
  }

  this->name = name;
  this->type = type;
  return "";
}

// ------------------- //
// ------------------- //

struct tpl_TplVariable {
  // Opt[char]
  Opt *comment;
  enum AttType access;
  char *name;
  char *type;
  char *value;
};

TplVariable *tplVariable_new (void) {
  TplVariable *this = MALLOC(TplVariable);
  this->comment = opt_empty();
  this->access = NO_ACCESS;
  this->name = "";
  this->type = "";
  this->value = "";
  return this;
}

// Opt[char]
Opt *tplVariable_comment (TplVariable *this) {
  return this->comment;
}

void tplVariable_set_comment (TplVariable *this, char *doc) {
  this->comment = opt_new(doc);
}

enum AttType tplVariable_access (TplVariable *this) {
  return this->access;
}

void tplVariable_set_access (TplVariable *this, enum AttType type) {
  this->access = type;
}

char *tplVariable_name (TplVariable *this) {
  return this->name;
}

char *tplVariable_type (TplVariable *this) {
  return this->type;
}

char *tplVariable_value (TplVariable *this) {
  return this->value;
}

char *tplVariable_set_name_type_value (
  TplVariable *this, char *name, char *type, char *value
) {
  if (!*name) {
    return "Argument name is empty";
  }

  type = intrim(type);

  char *err = test_type(type);

  if (*err) {
    return err;
  }

  this->name = name;
  this->type = type;
  this->value = value;
  return "";
}

char *tplVariable_set_function (TplVariable *this, char *def, char *value) {
  int begin = str_cindex(def, '(');
  if (begin == -1) {
    return "Bad function definition: Expected return type";
  }
  ++begin;
  int end = str_cindex_from(def, ')', begin);
  if (end == -1) {
    return "Bad function definition: Expected arguments";
  }

  char *pre = str_ltrim(str_left(def, begin));
  if (strlen(pre) < 2) {
    return "Bad function definition: Expected return type";
  }

  char *post = str_rtrim(str_right(def, end));
  if (strlen(post) < 2) {
    return "Bad function definition: Expected arguments";
  }

  char *name = str_trim(str_sub(def, begin, end));
  if (*name != '*') {
    return "Bad function definition: Expected asterisc";
  }
  name = str_ltrim(str_right(name, 1));
  if (!strlen(name)) {
    return "Bad function definition: Name is missing";
  }

  this->name = name;
  this->type = str_f("%s*#%s", pre, post);
  this->value = value;
  return "";
}

TplArgument *tplVariable_to_tplArgument (TplVariable *this) {
  return (TplArgument *)this;
}

// ------------------- //
// ------------------- //

struct tpl_Tpl {
  // Opt[char]
  Opt *struct_comment;
  enum StructType struct_type;
  enum SerialType serial_type;
  char *struct_name;
  // Arr[TplArgument]
  Arr *arguments;
  // Arr[TplVariable]
  Arr *variables;
  // Arr[TplVariable]
  Arr *functions;
};


Tpl *tpl_new (void) {
  Tpl *this = MALLOC(Tpl);
  this->struct_comment = opt_empty();
  this->struct_type = PUBLIC;
  this->serial_type = NONE;
  this->struct_name = "";
  this->arguments = arr_new();
  this->variables = arr_new();
  this->functions = arr_new();
  return this;
}

// Opt[char]
Opt *tpl_struct_comment (Tpl *this) {
  return this->struct_comment;
}

void tpl_set_struct_comment (Tpl *this, char *doc) {
  this->struct_comment = opt_new(doc);
}

enum StructType tpl_struct_type (Tpl *this) {
  return this->struct_type;
}

void tpl_set_struct_type (Tpl *this, enum StructType type) {
  this->struct_type = type;
}

enum SerialType tpl_serial_type (Tpl *this) {
  return this->serial_type;
}

void tpl_set_serial_type (Tpl *this, enum SerialType type) {
  this->serial_type = type;
}

char *tpl_struct_name (Tpl *this) {
  return this->struct_name;
}

void tpl_set_struct_name (Tpl *this, char *name) {
  this->struct_name = name;
}

Arr *tpl_arguments (Tpl *this) {
  return this->arguments;
}

int tpl_arguments_size (Tpl *this) {
  return arr_size(this->arguments);
}

void tpl_add_argument (Tpl *this, TplArgument *ar) {
  arr_push(this->arguments, ar);
}

Arr *tpl_variables (Tpl *this) {
  return this->variables;
}

void tpl_add_variable (Tpl *this, TplVariable *var) {
  arr_push(this->variables, var);
}

Arr *tpl_functions (Tpl *this) {
  return this->functions;
}

void tpl_add_function (Tpl *this, TplVariable *fun) {
  arr_push(this->functions, fun);
}

char *tpl_to_str (Tpl *this) {
  Buf *args = buf_new();
  EACH(this->arguments, TplArgument, a)
    if (opt_is_empty(a->comment)) {
      buf_add(args, "[No doc]");
    } else {
      buf_add(args, opt_get(a->comment));
    }
    buf_cadd(args, '\n');
    buf_add(
      args,
      a->access == NO_ACCESS ? "-" : a->access == GETTER ? "" : "@"
    );
    buf_add(args, str_f("%s: %s\n", a->name, a->type));
  _EACH

  Buf *vars = buf_new();
  EACH(this->variables, TplVariable, v)
    if (opt_is_empty(v->comment)) {
      buf_add(vars, "[No doc]");
    } else {
      buf_add(vars, opt_get(v->comment));
    }
    buf_cadd(vars, '\n');
    buf_add(
      vars,
      v->access == NO_ACCESS ? "-" : v->access == GETTER ? "" : "@"
    );
    buf_add(vars, str_f("%s: %s: %s\n", v->name, v->type, v->value));
  _EACH

  Buf *funs = buf_new();
  EACH(this->functions, TplVariable, v)
    if (opt_is_empty(v->comment)) {
      buf_add(funs, "[No doc]");
    } else {
      buf_add(funs, opt_get(v->comment));
    }
    buf_cadd(funs, '\n');
    buf_add(
      funs,
      v->access == NO_ACCESS ? "-" : v->access == GETTER ? "" : "@"
    );
    buf_add(funs, str_f(
      "%s: %s\n",
      str_replace(v->type, "#", v->name),
      v->value
    ));
  _EACH

  return str_cat(
    opt_is_empty(this->struct_comment)
      ? "[No doc]"
      : opt_get(this->struct_comment),
    "\n",
    this->struct_type == PRIVATE
      ? "-"
      : this->struct_type == HIDDEN ? "=" : "",
    this->struct_name,
    this->serial_type == TO
      ? ": to\n"
      : this->serial_type == FROM
        ? ": from\n"
        : this->serial_type == SERIAL ? ": serial\n" : "\n",
    buf_str(args),
    *buf_str(vars) ? str_f("---\n%s", buf_str(vars)) : "",
    *buf_str(funs)
      ? *buf_str(vars)
        ? str_f("---\n%s", buf_str(funs))
        : str_f("---\n---\n%s", buf_str(funs))
      : "",
    NULL
  );
}
