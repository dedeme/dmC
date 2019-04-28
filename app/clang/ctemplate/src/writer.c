// Copyright 25-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writer.h"
#include <ctype.h>
#include "Log.h"
#include "Tpl.h"

// comment is Opt[char]
static void add_comment (Buf *bf, Opt *comment) {
  if (opt_is_empty(comment)) {
    buf_add(bf, "///\n");
  } else {
    buf_add(bf, str_f(
      "///%s\n",
      str_join(str_csplit(opt_get(comment), '\n'), "\n///")
    ));
  }
}

static char *mk_type (char *type) {
  if (str_eq(type, "bool")) return "int";
  if (str_eq(type, "char")) return type;
  if (str_eq(type, "int")) return type;
  if (str_eq(type, "long")) return type;
  if (str_eq(type, "size_t")) return type;
  if (str_eq(type, "time_t")) return type;
  if (str_eq(type, "float")) return type;
  if (str_eq(type, "double")) return type;
  if (str_eq(type, "char*")) return "char *";
  if (str_eq(type, "Iarr")) return "Iarr *";
  if (str_eq(type, "Darr")) return "Darr *";
  if (str_starts(type, "Arr-")) return "Arr *";
  if (str_starts(type, "List-")) return "List *";
  if (str_starts(type, "Map-")) return "Map *";
  if (str_starts(type, "Hash-")) return "Hash *";
  return str_f("%s *", type);
}

static char *mk_type_name (char *type, char *name) {
  char *t = mk_type(type);
  return str_f(
    "%s%s%s",
    t, str_ends(t, "*") ? "" : " ", name
  );
}

static char *mk_to_fpointer (char *type) {
  if (str_eq(type, "char*")) {
    return "js_ws";
  }
  char *ltype = str_new(type);
  *ltype = tolower(*ltype);
  return str_f("%s_to_js", ltype);
}

static char *mk_to_single (char *type, char *name) {
  if (str_eq(type, "bool")) {
    return str_f("js_wb(%s)", name);
  }
  if (str_eq(type, "char") || str_eq(type, "int")) {
    return str_f("js_wi((int)%s)", name);
  }
  if (
    str_eq(type, "long") ||
    str_eq(type, "size_t") ||
    str_eq(type, "time_t")
  ) {
    return str_f("js_wl((long)%s)", name);
  }
  if (str_eq(type, "float")) {
    return str_f("js_wd((double)%s)", name);
  }
  if (str_eq(type, "double")) {
    return str_f("js_wd(%s)", name);
  }
  return str_f("%s(%s)", mk_to_fpointer(type), name);
}

static char *mk_to(char *type, char *name) {
  char *prefix = "  arr_push(js, ";
  char *postfix = ");\n";
  name = str_f("this->%s", name);
  char *s;
  // Arr[char]
  Arr *parts = str_csplit(type, '-');
  if (arr_size(parts) == 2) {
    char *coll = arr_get(parts, 0);
    char *single = arr_get(parts, 1);
    if (str_eq(coll, "Opt")) {
      s = str_f (
        "opt_is_empty(%s)\n    ? js_wn()\n    : %s\n  ",
        name, mk_to_single(single, name)
      );
    } else {
      char *lcoll = str_new(coll);
      *lcoll = tolower(*lcoll);
      s = str_f(
        "%s_to_js(%s, (FTO)%s)",
        lcoll, name, mk_to_fpointer(single)
      );
    }
  } else {
    s = mk_to_single(type, name);
  }
  return str_f("%s%s%s", prefix, s, postfix);
}

static char *mk_from_fpointer (char *type) {
  if (str_eq(type, "char*")) {
    return "js_rs";
  }
  char *ltype = str_new(type);
  *ltype = tolower(*ltype);
  return str_f("%s_from_js", ltype);
}

static char *mk_from_single(char *type) {
  if (str_eq(type, "char")) {
    return "(char)js_ri(*p++)";
  }
  if (str_eq(type, "bool") || str_eq(type, "int")) {
    return "(char)js_ri(*p++)";
  }
  if (
    str_eq(type, "long") ||
    str_eq(type, "size_t") ||
    str_eq(type, "time_t")
  ) {
    return str_f("(%s)js_rl(*p++)", type);
  }
  if (str_eq(type, "float") ) {
    return "(float)js_rd(*p++)";
  }
  if (str_eq(type, "double")) {
    return "js_rd(*p++)";
  }
  return str_f("%s(*p++)", mk_from_fpointer(type));
}

static char *mk_from(char *type, char *name) {
  char *prefix = str_f("  this->%s = ", name);
  char *postfix = ";\n";
  char *s;
  // Arr[char]
  Arr *parts = str_csplit(type, '-');
  if (arr_size(parts) == 2) {
    char *coll = arr_get(parts, 0);
    char *single = arr_get(parts, 1);
    if (str_eq(coll, "Opt")) {
      s = str_f (
        "js_is_null(*p)\n"
        "      ? p++? opt_empty(): NULL\n"
        "      : opt_new(%s)\n  ",
        mk_from_single(single)
      );
    } else {
      char *lcoll = str_new(coll);
      *lcoll = tolower(*lcoll);
      s = str_f(
        "%s_from_js(*p++, (FFROM)%s)",
        lcoll, mk_to_fpointer(single)
      );
    }
  } else {
    s = mk_from_single(type);
  }
  return str_f("%s%s%s", prefix, s, postfix);
}

// -------------------------------------------------------------------- ▲▲▲▲▲▲▲▲
// -------------------------------------------------------------------- h-c file
// -------------------------------------------------------------------- ▼▼▼▼▼▼▼▼

// args is Arr[TplArgument]
static void add_h_constructor (
  Buf *bf, int is_private, char *stname, char *stlname, Arr *args
) {
  // ------------------------------------------------------------------------ //
  void *mk_type_name_arg (TplArgument *a) {                                   //
    return mk_type_name(tplArgument_type(a), tplArgument_name(a));            //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  // Arr[char]
  Arr *args_str = arr_from_it(
    it_map(arr_to_it(args), (FCOPY)mk_type_name_arg)
  );
  char *as;
  if (arr_size(args) < 4) {
    as = str_join(args_str, ", ");
  } else {
    as = str_f("\n  %s\n", str_join(args_str, ",\n  "));
  }

  buf_add(bf, str_f(
    "%s *%s%s_new(%s)", stname, is_private ? "_" : "", stlname, as
  ));
}

static void add_h_getter(
  Buf *bf, char *stname, char *stlname, TplArgument *a
) {
  buf_add(bf, str_f(
    "%s_%s(%s *this)",
    mk_type_name(tplArgument_type(a), stlname), tplArgument_name(a), stname
  ));
}

static void add_h_setter (
  Buf *bf, char *stname, char *stlname, TplArgument *a
) {
  buf_add(bf, str_f(
    "void %s_set_%s(%s *this, %s)",
    stlname, tplArgument_name(a), stname,
    mk_type_name(tplArgument_type(a), "value")
  ));
}

static void add_h_fgetter(
  Buf *bf, char *stname, char *stlname, TplVariable *f
) {
  buf_add(bf, str_replace(tplVariable_type(f), "#",
    str_f("%s_%s(%s *this)", stlname, tplVariable_name(f), stname)
  ));
}

static void add_h_fsetter (
  Buf *bf, char *stname, char *stlname, TplVariable *f
) {
  buf_add(bf, str_f(
    "void %s_set_%s(%s *this, %s)",
    stlname, tplVariable_name(f), stname,
    str_replace(tplVariable_type(f), "#", "value")
  ));
}

static void add_h_to (Buf *bf, char *stname, char *stlname) {
  buf_add(bf, str_f("Js *%s_to_js(%s *this)", stlname, stname));
}

static void add_h_from (Buf *bf, char *stname, char *stlname) {
  buf_add(bf, str_f("%s *%s_from_js(Js *js)", stname, stlname));
}

// --------------------------------------------------------------------- ▲▲▲▲▲▲▲
// --------------------------------------------------------------------- c file
// --------------------------------------------------------------------- ▼▼▼▼▼▼▼

static void add_c_structure (Buf *bf, char *fname, char *stname, Tpl *t) {
  buf_add(bf, str_f("struct %s_%s{\n", fname, stname));
  EACH(tpl_arguments(t), TplArgument, a)
    buf_add(bf, str_f("  %s;\n", mk_type_name(
      tplArgument_type(a), tplArgument_name(a)
    )));
  _EACH
  EACH(tpl_variables(t), TplVariable, v)
    buf_add(bf, str_f("  %s;\n", mk_type_name(
      tplVariable_type(v), tplVariable_name(v)
    )));
  _EACH
  EACH(tpl_functions(t), TplVariable, f)
    buf_add(bf, str_f("  %s;\n", str_replace(
      tplVariable_type(f), "#", tplVariable_name(f)
    )));
  _EACH
  buf_add(bf, str_f("};\n\n", fname, stname));
}

static void add_c_constructor (Buf *bf, char *stname, Tpl *t) {
  buf_add(bf, str_f("  %s *this = MALLOC(%s);\n", stname, stname));
  EACH(tpl_arguments(t), TplArgument, a)
    buf_add(bf, str_f(
      "  this->%s = %s;\n", tplArgument_name(a), tplArgument_name(a)
    ));
  _EACH
  EACH(tpl_variables(t), TplVariable, v)
    buf_add(bf, str_f(
      "  this->%s = %s;\n", tplVariable_name(v), tplVariable_value(v)
    ));
  _EACH
  EACH(tpl_functions(t), TplVariable, f)
    buf_add(bf, str_f(
      "  this->%s = %s;\n", tplVariable_name(f), tplVariable_value(f)
    ));
  _EACH
  buf_add(bf, "  return this;\n");
}

static void add_c_getter(Buf *bf, TplArgument *a) {
  buf_add(bf, str_f("  return this->%s;\n", tplArgument_name(a)));
}

static void add_c_setter(Buf *bf, TplArgument *a) {
  buf_add(bf, str_f("  this->%s = value;\n", tplArgument_name(a)));
}

static void add_c_to (Buf *bf, Tpl *t) {
  buf_add(bf, "  // Arr[Js]\n");
  buf_add(bf, "  Arr *js = arr_new();\n");
  EACH(tpl_arguments(t), TplArgument, a)
    buf_add(bf, mk_to( tplArgument_type(a), tplArgument_name(a)));
  _EACH
  EACH(tpl_variables(t), TplVariable, v)
    TplArgument *a = tplVariable_to_tplArgument(v);
    buf_add(bf, mk_to( tplArgument_type(a), tplArgument_name(a)));
  _EACH
  buf_add(bf, "  return js_wa(js);\n");
}

static void add_c_from (Buf *bf, char *stname, Tpl *t) {
  buf_add(bf, "  // Arr[Js]\n");
  buf_add(bf, "  Arr *a = js_ra(js);\n");
  buf_add(bf, "  Js **p = (Js **)arr_start(a);\n");
  buf_add(bf, str_f("  %s *this = MALLOC(%s);\n", stname, stname));
  EACH(tpl_arguments(t), TplArgument, a)
    buf_add(bf, mk_from( tplArgument_type(a), tplArgument_name(a)));
  _EACH
  EACH(tpl_variables(t), TplVariable, v)
    TplArgument *a = tplVariable_to_tplArgument(v);
    buf_add(bf, mk_from( tplArgument_type(a), tplArgument_name(a)));
  _EACH
  EACH(tpl_functions(t), TplVariable, f)
    buf_add(bf, str_f(
      "  this->%s = %s;\n", tplVariable_name(f), tplVariable_value(f))
    );
  _EACH
  buf_add(bf, "  return this;\n");
}

// --------------------------------------------------------------------- ▲▲▲▲▲▲▲
// --------------------------------------------------------------------- write_h
// --------------------------------------------------------------------- ▼▼▼▼▼▼▼

static int write_h (char *finclude, Arr *tps) {
  Log *log = log_new(finclude);
  if (!file_exists(finclude)) {
    log_msg(log, "file", str_f("File '%s' not found"));
    return 0;
  }

  char *fname = path_only_name(finclude);

  FileLck *source = file_ropen(finclude);
  FileLck *target = file_wopen(path_cat(sys_home(), "tmp.h", NULL));

  int r = 0;
  int ok = 0;
  char *l;
  while (*(l = file_read_line(source))) {
    file_write_text(target, l);
    if (str_eq(str_trim(l), "/*--*/")) {
      file_write_text(target, "\n");
      ok = 1;
      break;
    }
  }

  if (ok) {
    EACH(tps, Tpl, t)
      if (tpl_struct_type(t) == HIDDEN) {
        continue;
      }

      char *stname = tpl_struct_name(t);
      char *stlname = str_new(stname);
      *stlname = tolower(*stlname);

      Buf *bf = buf_new();

      // ----------------------------------------------------------- Constructor

      add_comment (bf, tpl_struct_comment(t));
      buf_add(bf, str_f(
        "typedef struct %s_%s %s;\n\n", fname, stname, stname
      ));

      if (tpl_struct_type(t) == PUBLIC) {
        add_comment(bf, opt_empty());
        add_h_constructor(bf, 0, stname, stlname, tpl_arguments(t));
        buf_add(bf, ";\n\n");
      }

      // ------------------------------------------------------------- Arguments

      EACH(tpl_arguments(t), TplArgument, a)
        if (
          tplArgument_access(a) == GETTER ||
          tplArgument_access(a) == GETTER_SETTER
        ) {
          add_comment(bf, tplArgument_comment(a));
          add_h_getter(bf, stname, stlname, a);
          buf_add(bf, ";\n\n");
        }

        if (tplArgument_access(a) == GETTER_SETTER) {
          add_comment(bf, opt_empty());
          add_h_setter(bf, stname, stlname, a);
          buf_add(bf, ";\n\n");
        }
      _EACH

      // ------------------------------------------------------------- Variables

      EACH(tpl_variables(t), TplVariable, v)
        TplArgument *a = tplVariable_to_tplArgument(v);
        if (
          tplArgument_access(a) == GETTER ||
          tplArgument_access(a) == GETTER_SETTER
        ) {
          add_comment(bf, tplArgument_comment(a));
          add_h_getter(bf, stname, stlname, a);
          buf_add(bf, ";\n\n");
        }

        if (tplArgument_access(a) == GETTER_SETTER) {
          add_comment(bf, opt_empty());
          add_h_setter(bf, stname, stlname, a);
          buf_add(bf, ";\n\n");
        }
      _EACH

      // ------------------------------------------------------------- Functions

      EACH(tpl_functions(t), TplVariable, f)
        if (
          tplVariable_access(f) == GETTER ||
          tplVariable_access(f) == GETTER_SETTER
        ) {
          add_comment(bf, tplVariable_comment(f));
          add_h_fgetter(bf, stname, stlname, f);
          buf_add(bf, ";\n\n");
        }

        if (tplVariable_access(f) == GETTER_SETTER) {
          add_comment(bf, opt_empty());
          add_h_fsetter(bf, stname, stlname, f);
          buf_add(bf, ";\n\n");
        }
      _EACH

      // ------------------------------------------------------------------ Json

      if (tpl_serial_type(t) == TO || tpl_serial_type(t) == SERIAL) {
        add_comment(bf, opt_empty());
        add_h_to(bf, stname, stlname);
        buf_add(bf, ";\n\n");
      }

      if (tpl_serial_type(t) == FROM || tpl_serial_type(t) == SERIAL) {
        add_comment(bf, opt_empty());
        add_h_from(bf, stname, stlname);
        buf_add(bf, ";\n\n");
      }

      file_write_text(target, buf_str(bf));
    _EACH

    ok = 0;
    while (*(l = file_read_line(source))) {
      if (str_eq(str_trim(l), "/*--*/")) {
        file_write_text(target, l);
        ok = 1;
        break;
      }
    }

    if (ok) {
      while (*(l = file_read_line(source))) {
        file_write_text(target, l);
      }
      r = 1;
    } else {
      log_msg (log, "file", "End symbol '/*--*/' not found");
    }
  } else {
    log_msg (log, "file", "Start symbol '/*--*/' not found");
  }

  file_close(source);
  file_close(target);
  return r;
}

// --------------------------------------------------------------------- ▲▲▲▲▲▲▲
// --------------------------------------------------------------------- write_c
// --------------------------------------------------------------------- ▼▼▼▼▼▼▼

static int write_c (char *fsrc, Arr *tps) {
  Log *log = log_new(fsrc);
  if (!file_exists(fsrc)) {
    log_msg(log, "file", str_f("File '%s' not found"));
    return 0;
  }

  char *fname = path_only_name(fsrc);

  FileLck *source = file_ropen(fsrc);
  FileLck *target = file_wopen(path_cat(sys_home(), "tmp.c", NULL));

  int r = 0;
  int ok = 0;
  char *l;
  while (*(l = file_read_line(source))) {
    if (str_eq(str_trim(l), "/*.")) {
      file_write_text(target, "/* .\n");
      continue;
    }
    file_write_text(target, l);
    if (str_eq(str_trim(l), "/*--*/")) {
      file_write_text(target, "\n");
      ok = 1;
      break;
    }
  }

  if (ok) {
    EACH(tps, Tpl, t)
      char *stname = tpl_struct_name(t);
      char *stlname = str_new(stname);
      *stlname = tolower(*stlname);

      Buf *bf = buf_new();

      // ----------------------------------------------------------- Constructor

      if (tpl_struct_type(t) == HIDDEN) {
        buf_add(bf, str_f(
          "typedef struct %s_%s %s;\n\n", fname, stname, stname
        ));
      }

      add_c_structure(bf, fname, stname, t);

      buf_add(bf, tpl_struct_type(t) == PUBLIC ? "" : "static ");
      add_h_constructor(
        bf, tpl_struct_type(t) == PRIVATE, stname, stlname, tpl_arguments(t)
      );
      buf_add(bf, " {\n");
      add_c_constructor(bf, stname, t);
      buf_add(bf, "}\n\n");

      // ------------------------------------------------------------- Arguments

      EACH(tpl_arguments(t), TplArgument, a)
        if (
          tplArgument_access(a) == GETTER ||
          tplArgument_access(a) == GETTER_SETTER
        ) {
          add_h_getter(bf, stname, stlname, a);
          buf_add(bf, " {\n");
          add_c_getter(bf, a);
          buf_add(bf, "}\n\n");
        }

        if (tplArgument_access(a) == GETTER_SETTER) {
          add_h_setter(bf, stname, stlname, a);
          buf_add(bf, " {\n");
          add_c_setter(bf, a);
          buf_add(bf, "}\n\n");
        }
      _EACH

      // ------------------------------------------------------------- Variables

      EACH(tpl_variables(t), TplVariable, v)
        TplArgument *a = tplVariable_to_tplArgument(v);
        if (
          tplArgument_access(a) == GETTER ||
          tplArgument_access(a) == GETTER_SETTER
        ) {
          add_h_getter(bf, stname, stlname, a);
          buf_add(bf, " {\n");
          add_c_getter(bf, a);
          buf_add(bf, "}\n\n");
        }

        if (tplArgument_access(a) == GETTER_SETTER) {
          add_h_setter(bf, stname, stlname, a);
          buf_add(bf, " {\n");
          add_c_setter(bf, a);
          buf_add(bf, "}\n\n");
        }
      _EACH

      // ------------------------------------------------------------- Functions

      EACH(tpl_functions(t), TplVariable, f)
        if (
          tplVariable_access(f) == GETTER ||
          tplVariable_access(f) == GETTER_SETTER
        ) {
          add_h_fgetter(bf, stname, stlname, f);
          buf_add(bf, " {\n");
          add_c_getter(bf, tplVariable_to_tplArgument(f));
          buf_add(bf, "}\n\n");
        }

        if (tplVariable_access(f) == GETTER_SETTER) {
          add_h_fsetter(bf, stname, stlname, f);
          buf_add(bf, " {\n");
          add_c_setter(bf, tplVariable_to_tplArgument(f));
          buf_add(bf, "}\n\n");
        }
      _EACH

      // ------------------------------------------------------------------ Json

      if (tpl_serial_type(t) == TO || tpl_serial_type(t) == SERIAL) {
        add_h_to(bf, stname, stlname);
        buf_add(bf, " {\n");
        add_c_to(bf, t);
        buf_add(bf, "}\n\n");
      }

      if (tpl_serial_type(t) == FROM || tpl_serial_type(t) == SERIAL) {
        add_h_from(bf, stname, stlname);
        buf_add(bf, " {\n");
        add_c_from(bf, stname, t);
        buf_add(bf, "}\n\n");
      }

      file_write_text(target, buf_str(bf));
    _EACH

    ok = 0;
    while (*(l = file_read_line(source))) {
      if (str_eq(str_trim(l), "/*--*/")) {
        file_write_text(target, l);
        ok = 1;
        break;
      }
    }

    if (ok) {
      while (*(l = file_read_line(source))) {
        file_write_text(target, l);
      }
      r = 1;
    } else {
      log_msg (log, "file", "End symbol '/*--*/' not found");
    }
  } else {
    log_msg (log, "file", "Start symbol '/*--*/' not found");
  }

  file_close(source);
  file_close(target);
  return r;
}

// tps is Arr[Tpl]
int writer_mk_tmp (char *finclude, char *fsrc, Arr *tps) {
  if (!write_h(finclude, tps)) {
    return 0;
  }

  if (!write_c(fsrc, tps)) {
    return 0;
  }

  return 1;
}

void writer_copy_tmp (char *finclude, char *fsrc) {
  file_copy(path_cat(sys_home(), "tmp.h", NULL), finclude);
  file_copy(path_cat(sys_home(), "tmp.c", NULL), fsrc);
}
