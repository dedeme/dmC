// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "writer.h"
#include "Def.h"
#include "StName.h"
#include "Arg.h"
#include "Var.h"
#include "Fun.h"

// 'defs' is Arr[Def]
void writer_c(Arr *defs, FileLck *lck) {
  EACH(defs, Def, d)
    StName *stName = def_name(d);
    // Arr[Arg]
    Arr *args = def_args(d);
    // Arr[Var]
    Arr *vars = def_vars(d);
    // Arr[Fun]
    Arr *funs = def_funs(d);

    Buf *bf = buf_new();

    // Write struct --------------------------------------------------

    stName_w_struct(stName, bf);
    EACH(args, Arg, a) arg_w_struct(a, bf); _EACH
    EACH(vars, Var, v) var_w_struct(v, bf); _EACH
    EACH(funs, Fun, f) fun_w_struct(f, bf); _EACH
    buf_add(bf, "};\n\n");

    // Write constructor ---------------------------------------------

    stName_w_constructor_header(stName, bf);
    int first = 1;
    if (arr_size(args) > 3) {
      buf_add(bf, "\n  ");
      EACH(args, Arg, a)
        if (first) first = 0; else buf_add(bf, ",\n  ");
        arg_w_constructor_head(a, bf);
      _EACH
      buf_add(bf, "\n) {\n");
    } else {
      EACH(args, Arg, a)
        if (first) first = 0; else buf_add(bf, ", ");
        arg_w_constructor_head(a, bf);
      _EACH
      buf_add(bf, ") {\n");
    }
    stName_w_constructor_body(stName, bf);
    EACH(args, Arg, a) arg_w_constructor_body(a, bf); _EACH
    EACH(vars, Var, v) var_w_constructor(v, bf); _EACH
    EACH(funs, Fun, f) fun_w_constructor(f, bf); _EACH
    buf_add(bf, "  return this;\n}\n\n");

    // Write destructor ----------------------------------------------

    stName_w_destructor(stName, bf);
    buf_add(bf, "  if (this) {\n");
    EACH(args, Arg, a) arg_w_destructor(a, bf); _EACH
    EACH(vars, Var, v) var_w_destructor(v, bf); _EACH
    buf_add(bf, "    free(this);\n  }\n};\n\n");

    // Write get -----------------------------------------------------

    EACH(args, Arg, a)
      if (arg_gstype(a) != arg_NONE) {
        arg_w_get(a, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    EACH(vars, Var, v)
      if (var_gstype(v) != var_NONE) {
        var_w_get(v, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    EACH(funs, Fun, f)
      if (fun_gstype(f) != fun_NONE) {
        fun_w_get(f, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    // Write set -----------------------------------------------------

    EACH(args, Arg, a)
      if (arg_gstype(a) == arg_SETTER) {
        arg_w_set(a, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    // Write to ------------------------------------------------------

    if (
      stName_stype(stName) == stName_SERIAL ||
      stName_stype(stName) == stName_TO
    ) {
      stName_w_to_header(stName, bf);
      buf_add(bf, "  // Arr[Js]\n  Arr *a = arr_new(free);\n");
      EACH(args, Arg, a) arg_w_to(a, bf); _EACH
      buf_add(
        bf, "  Js *r = js_wa_new(a);\n  arr_free(a);\n  return r;\n}\n\n"
      );
    }

    // Write from ----------------------------------------------------

    if (
      stName_stype(stName) == stName_SERIAL ||
      stName_stype(stName) == stName_FROM
    ) {
      stName_w_from_header(stName, bf);
      buf_add(bf, "  // Arr[Js]\n  Arr *a = js_ra_new(js);\n  int i = 0;\n");
      EACH(args, Arg, a) arg_w_from(a, bf); _EACH
      buf_add(bf, "  arr_free(a);\n");
      stName_w_from_body(stName, bf);
      first = 1;
      EACH(args, Arg, a)
        if (first) first = 0; else buf_add(bf, ", ");
        buf_add(bf, arg_id(a));
      _EACH
      buf_add(bf, ");\n}\n\n");
    }

    file_write_text(lck, buf_str(bf));
    buf_free(bf);
  _EACH
}

void writer_h(Arr *defs, FileLck *lck) {
  EACH(defs, Def, d)
    StName *stName = def_name(d);
    // Arr[Arg]
    Arr *args = def_args(d);
    // Arr[Var]
    Arr *vars = def_vars(d);
    // Arr[Fun]
    Arr *funs = def_funs(d);

    Buf *bf = buf_new();

    // Write struct --------------------------------------------------

    if (stName_doc_null(stName)) {
      buf_add(bf, stName_doc_null(stName));
    } else {
      buf_add(bf, "///\n");
    }
    stName_wh_struct(stName, bf);
    buf_cadd(bf, '\n');

    // Write constructor ---------------------------------------------

    if (stName_public(stName)) {
      buf_add(bf, "///\n");
      stName_wh_constructor(stName, bf);
      int first = 1;
      if (arr_size(args) > 3) {
        buf_add(bf, "\n  ");
        EACH(args, Arg, a)
          if (first) first = 0; else buf_add(bf, ",\n  ");
          arg_w_constructor_head(a, bf);
        _EACH
        buf_add(bf, "\n);\n\n");
      } else {
        EACH(args, Arg, a)
          if (first) first = 0; else buf_add(bf, ", ");
          arg_w_constructor_head(a, bf);
        _EACH
        buf_add(bf, ");\n\n");
      }
    }

    // Write destructor ----------------------------------------------

    buf_add(bf, "///\n");
    stName_wh_destructor(stName, bf);
    buf_cadd(bf, '\n');

    // Write get -----------------------------------------------------

    EACH(args, Arg, a)
      if (arg_gstype(a) != arg_NONE) {
        if (arg_doc_null(a)) {
          buf_add(bf, arg_doc_null(a));
        } else {
          buf_add(bf, "///\n");
        }
        arg_wh_get(a, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    EACH(vars, Var, v)
      if (var_gstype(v) != var_NONE) {
        if (var_doc_null(v)) {
          buf_add(bf, var_doc_null(v));
        } else {
          buf_add(bf, "///\n");
        }
        var_wh_get(v, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    EACH(funs, Fun, f)
      if (fun_gstype(f) != fun_NONE) {
        if (fun_doc_null(f)) {
          buf_add(bf, fun_doc_null(f));
        } else {
          buf_add(bf, "///\n");
        }
        fun_wh_get(f, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    // Write set -----------------------------------------------------

    EACH(args, Arg, a)
      if (arg_gstype(a) == arg_SETTER) {
        if (arg_doc_null(a)) {
          buf_add(bf, arg_doc_null(a));
        } else {
          buf_add(bf, "///\n");
        }
        arg_wh_set(a, bf, stName);
        buf_cadd(bf, '\n');
      }
    _EACH

    // Write to ------------------------------------------------------

    if (
      stName_stype(stName) == stName_SERIAL ||
      stName_stype(stName) == stName_TO
    ) {
      buf_add(bf, "///\n");
      stName_wh_to(stName, bf);
      buf_cadd(bf, '\n');
    }

    // Write from ----------------------------------------------------

    if (
      stName_stype(stName) == stName_SERIAL ||
      stName_stype(stName) == stName_FROM
    ) {
      buf_add(bf, "///\n");
      stName_wh_from(stName, bf);
      buf_cadd(bf, '\n');
    }

    file_write_text(lck, buf_str(bf));
    buf_free(bf);
  _EACH
}
