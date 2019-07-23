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
  Gc *gc;
};

TplArgument *tplArgument_new (Tpl *tpl) {
  Gc *gc = tpl_gc(tpl);
  TplArgument *this = GC_MALLOC(TplArgument);
  this->comment = opt_empty();
  this->access = NO_ACCESS;
  this->name = "";
  this->type = "";
  this->gc = gc;
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

static char *test_single_type(Gc *gc, char *t) {
  GCL_NEW

  if (str_index(coll, str_f(gcl, " %s ", t)) != -1) {
    return GCL_CLEAN(str_f(gc, "'%s' used as single type", t));
  }

  if (str_ends(t, "*") && !str_eq(t, "char*")) {
    return GC_CLEAN("Only 'char *' ends at '*'");
  }

  return GCL_CLEAN("");
}

static char *test_collection_type(char *t) {
  GC_NEW

  if (str_index(coll, str_f(gc, " %s ", t)) != -1) {
    return GC_CLEAN("");
  }
  return GC_CLEAN(
    "Only 'Opt', 'Arr', 'List', 'Map' or 'Hash' can be collection type"
  );
}

static char *test_type (Gc *gc, char *type) {
  if (!*type) {
    return "Argument type is empty";
  }

  GCL_NEW

  // Arr[char]
  Arr *parts = str_csplit_trim(gcl, type, '-');
  int sz = arr_size(parts);
  char *err;
  if (sz == 1) {
    err = test_single_type(gc, arr_get(parts, 0));
    if (*err) {
      return GCL_CLEAN(err);
    }
  } else if (sz == 2) {
    err = test_collection_type(arr_get(parts, 0));
    if (*err) {
      return GCL_CLEAN(err);
    }
    err = test_single_type(gc, arr_get(parts, 1));
    if (*err) {
      return GCL_CLEAN(err);
    }
  } else {
    return GCL_CLEAN("Too much '-'");
  }

  return GCL_CLEAN("");
}

static char *intrim (Gc *gc, char *s){
  GCL_NEW

  Buf *bf = buf_new(gcl);
  int i;
  for (i = 0; i < strlen(s); ++i) {
    char ch = s[i];
    if (ch > ' ') {
      buf_cadd(bf, ch);
    }
  }

  return GCL_CLEAN(buf_to_str(gc, bf));
}

char *tplArgument_set_name_type (
  Gc *gc, TplArgument *this, char *name, char *type
) {
  if (!*name) {
    return "Argument name is empty";
  }

  type = intrim(this->gc, type);

  char *err = test_type(gc, type);

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
  Gc *gc;
};

TplVariable *tplVariable_new (Tpl *tpl) {
  Gc *gc = tpl_gc(tpl);
  TplVariable *this = GC_MALLOC(TplVariable);
  this->comment = opt_empty();
  this->access = NO_ACCESS;
  this->name = "";
  this->type = "";
  this->value = "";
  this->gc = gc;
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
  Gc *gc, TplVariable *this, char *name, char *type, char *value
) {
  if (!*name) {
    return "Argument name is empty";
  }

  type = intrim(this->gc, type);

  char *err = test_type(gc, type);

  if (*err) {
    return err;
  }

  this->name = name;
  this->type = type;
  this->value = value;
  return err;
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

  GC_NEW

  char *pre = str_ltrim(gc, str_left(gc, def, begin));
  if (strlen(pre) < 2) {
    return GC_CLEAN("Bad function definition: Expected return type");
  }

  char *post = str_rtrim(gc, str_right(gc, def, end));
  if (strlen(post) < 2) {
    return GC_CLEAN("Bad function definition: Expected arguments");
  }

  char *name = str_trim(gc, str_sub(gc, def, begin, end));
  if (*name != '*') {
    return GC_CLEAN("Bad function definition: Expected asterisc");
  }
  name = str_ltrim(this->gc, str_right(gc, name, 1));
  if (!strlen(name)) {
    return GC_CLEAN("Bad function definition: Name is missing");
  }

  this->name = name;
  this->type = str_f(this->gc, "%s*#%s", pre, post);
  this->value = value;

  return GC_CLEAN("");
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
  enum ConstructorType constructor_type;
  enum SerialType serial_type;
  char *struct_name;
  // Arr[TplArgument]
  Arr *arguments;
  // Arr[TplVariable]
  Arr *variables;
  // Arr[TplVariable]
  Arr *functions;
  Gc *gc;
};


Tpl *tpl_new (Gc *gc) {
  Tpl *this = GC_MALLOC(Tpl);
  this->struct_comment = opt_empty();
  this->struct_type = PUBLIC;
  this->constructor_type = NORMAL;
  this->serial_type = NONE;
  this->struct_name = "";
  this->arguments = arr_new(gc);
  this->variables = arr_new(gc);
  this->functions = arr_new(gc);
  this->gc = gc;
  return this;
}

Gc *tpl_gc (Tpl *this) {
  return this->gc;
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

enum ConstructorType tpl_constructor_type (Tpl *this) {
  return this->constructor_type;
}

void tpl_set_constructor_type (Tpl *this, enum ConstructorType type) {
  this->constructor_type = type;
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

Gc *tplArgument_gc(TplArgument *this) {
  return this->gc;
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

char *tpl_to_str (Gc *gc, Tpl *this) {
  GCL_NEW

  Buf *args = buf_new(gcl);
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
    buf_add(args, str_f(gcl, "%s: %s\n", a->name, a->type));
  _EACH

  Buf *vars = buf_new(gcl);
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
    buf_add(vars, str_f(gcl, "%s: %s: %s\n", v->name, v->type, v->value));
  _EACH

  Buf *funs = buf_new(gcl);
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
    buf_add(funs, str_f(gcl,
      "%s: %s\n",
      str_replace(gcl, v->type, "#", v->name),
      v->value
    ));
  _EACH

  return GCL_CLEAN(str_cat(
    gc,
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
    *buf_str(vars) ? str_f(gcl, "---\n%s", buf_str(vars)) : "",
    *buf_str(funs)
      ? *buf_str(vars)
        ? str_f(gcl, "---\n%s", buf_str(funs))
        : str_f(gcl, "---\n---\n%s", buf_str(funs))
      : "",
    NULL
  ));
}
