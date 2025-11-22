// Copyright 17-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "doc.h"
#include "fns.h"
#include "char.h"
#include "kut/js.h"
#include "kut/path.h"
#include "data/docE.h"
#include "data/rdCtx.h"
#include "data/stat.h"
#include "data/tp3.h"
#include "data/typedSym.h"
#include "data/imp.h"
#include "readers/stRd.h"
#include "readers/moduleRd.h"

char *doc_make (char *fpath, char *id, char *tx) {
  char *fail (int ln, char *msg) {
    // Arr<char>
    Arr *r = arr_new();
    arr_push(r, docE_to_js(docE_new_error(ln, msg)));
    return js_wa(r);
  }

  if (!*str_trim(tx))
    return fail(0, str_f("%s: File not found or empty file", fpath));

  // Arr<char>
  Arr *atx = str_csplit(tx, '\n');

  char *getDoc2 (int ln) {
    --ln;
    // Arr<char>
    Arr *r = arr_new();
    while (ln > 0) {
      --ln;
      char *l = str_trim(arr_get(atx, ln));
      if (str_starts(l, "/// ")) arr_push(r, str_right(l, 4));
      else if (str_starts(l, "///")) arr_push(r, str_right(l, 3));
      else break;
    }
    arr_reverse(r);
    return arr_cjoin(r, '\n');
  }

  char *getDoc1 (int ln) {
    --ln;
    // Arr<char>
    Arr *r = arr_new();
    while (ln > 0) {
      --ln;
      char *l = str_trim(arr_get(atx, ln));
      if (str_starts(l, "//:")) {
        arr_push(r, l);
        break;
      }
      else if (str_starts(l, "//")) arr_push(r, l);
      else break;
    }
    if (arr_size(r) && !str_starts(arr_peek(r), "//:")) return "";
    arr_reverse(r);

    char *doc = getDoc2(ln + 1);
    return *doc
      ? str_f("%s\n%s", doc, arr_cjoin(r, '\n'))
      : str_f("%s%s", doc, arr_cjoin(r, '\n'))
    ;
  }

  // id is in form 'name = \' or '' for enumerations
  // pars is Arr<char>
  char *getFnCode (char *id, Arr *pars) {
    int len = 80;
    char *r = id;
    EACH(pars, char, p) {
      if (strlen(r) > len) {
        r = str_f("%s\n  ", r);
        len = strlen(r) + 80;
      }
      r = str_f("%s%s", r, p);
      if (_i + 1 < arr_size(pars)) r = str_f("%s, ", r);
    }_EACH
    return r;
  }

  // Arr<docE>
  Arr *es = arr_new();
  // Map<Imp>
  Map *imports = map_new();
  // Arr<TypedSym>
  Arr *top_syms = arr_new();
  // Arr<Tp<Type, Type>>
  Map *type_defs = map_new();
  int enum_ix = 1;

  // Summary documentation
  // Arr<char>
  Arr *sm = arr_new();
  EACH(atx, char, line) {
    char *l = str_trim(line);
    if (str_starts(l, "import")) break;

    if (arr_size(sm)) {
      if (!str_starts(l, "///")) break;
    } else {
      if (!str_starts(l, "///")) continue;
    }
    arr_push(sm, str_right(l, str_starts(l, "/// ") ? 4 : 3));
  }_EACH
  if (arr_size(sm))
    arr_push(es, docE_new_summary(arr_cjoin(sm, '\n')));

  StRd *strd = stRd_new(rdCtx_new(fpath, id), tx);
  int with_block = FALSE;
  for (;;) {
    int public = TRUE;
    Token *tk = stRd_peek_token(strd);
    if (with_block && tk->tp != token_eof)
      return fail(tk->ln, str_f(
          "Expected: end of file after main block.\n"
          "  Actual: %s", token_to_str(tk)
      ));

    if (tk->tp == token_eof) break;
    if (tk->tp == token_operator) {
      if (*tk->value == '(') {
        Tp *tp1 = stRd_read_def_type(strd, TRUE);
        Type *left = tp1->e1;
        if (!left) {
          Exp *err = tp1->e2;
          return fail(err->ln, err->value);
        }

        Tp *tp2 = stRd_read_def_type(strd, FALSE);
        Type *right = tp2->e1;
        if (!right) {
          Exp *err = tp2->e2;
          return fail(err->ln, err->value);
        }

        char *err = type_check_def(type_get_generics(left), right);
        if (*err) return fail(tk->ln, err);

        char *id = type_did(left);
        int anyf (char *key) { return !strcmp(key, id); }
        if (arr_any(map_keys(type_defs), (FPRED)anyf))
          return fail(tk->ln, str_f("Type '%s' is duplicated", id));

        map_put(type_defs, id, tp_mk(left, right));

        arr_push(es, docE_new_type(
          id,
          str_f("%s %s", type_to_str(left), type_to_str(right)),
          tk->ln,
          getDoc2(tk->ln)
        ));
        continue;
      }

      if (*tk->value == '_') {
        stRd_next_token(strd);
        public = FALSE;
      }
    }

    Stat *st = stRd_next(strd);
    int ln = st->ln;
    StatT stp = st->tp;
    if (stp == stat_error) {
      if (!strcmp(st->value, stRd_end_of_file)) break;
      return fail(ln, st->value);
    } else if (stp == stat_close_block) {
      return fail(ln, "'}' out block");
    } else if (stp == stat_assign) {
      Tp *tp = st->value;
      Exp *symSq = tp->e1;
      if (symSq->tp != exp_symbol)
        return fail(ln, str_f(
          "Expected: symbol assignation \n  Actual: %s", stat_to_str(st)
        ));
      char *sym = symSq->value;
      int ffind (TypedSym *s) { return str_eq(s->id, sym); }
      TypedSym *old_sym = opt_get(arr_find(top_syms, (FPRED)ffind));
      if (old_sym)
        return fail(ln, str_f(
          "Symbol '%s' is duplicated (first assigned in line %d)",
          sym, old_sym->ln
        ));
      Imp *imp = opt_get(map_get(imports, sym));
      if (imp)
        return fail(ln, str_f(
          "Symbol '%s' is duplicated (first assigned in line %d)",
          sym, imp->ln
        ));

      Exp *e = tp->e2;
      switch (e->tp) {
        case exp_boolean:
        case exp_integer:
        case exp_float:
        case exp_string: {
          Type *t = NULL;
          switch (e->tp) {
            case exp_boolean:
              t = type_bool();
              break;
            case exp_integer:
              t = type_int();
              break;
            case exp_float:
              t = type_float();
              break;
            default:
              t = type_string();
          };
          char *cvalue = t == type_string()
            ? js_ws(str_replace(str_replace(e->value, "&", "&amp;"), "<", "&lt;"))
            : e->value
          ;
          TypedSym *ts = typedSym_new(ln, sym, t, opt_some(cvalue), TRUE);
          arr_push(top_syms, ts);

          if (public)
            arr_push(es, docE_new_value(
              sym,
              str_f("%s = %s", sym, fns_cut_msg(cvalue, 80)),
              st->ln,
              getDoc2(st->ln)
            ));
          break;
        }
        case exp_array0: {
          Type *t = opt_get(e->value);
          if (!t) return fail (e->ln, "Missing type in empty array");
          TypedSym *ts = typedSym_new(
            ln, sym,
            type_new("a", arr_new_from(t, NULL)),
            opt_none(), TRUE
          );
          arr_push(top_syms, ts);

          if (public)
            arr_push(es, docE_new_value(
              sym,
              str_f("%s = []", sym),
              st->ln,
              getDoc1(st->ln)
            ));

          break;
        }
        case exp_function: {
          Tp3 *tp = e->value;
          TypedSym *ts = typedSym_new(ln, sym, tp->e1, opt_none(), TRUE);
          arr_push(top_syms, ts);

          if (public)
            arr_push(es, docE_new_function(
              sym,
              str_f("%s ->", getFnCode(str_f("%s = \\", sym), tp->e2)),
              st->ln,
              getDoc1(st->ln)
            ));

          break;
        }
        default:
          return fail(ln, str_f(
            "Expected: empty array, function or constant\n  Actual: %s",
            exp_to_str(e)
          ));
      }
    } else if (stp == stat_indexed) {
      Tp3 *tuple = st->value;
      Type *rt = tuple->e1;
      char *fn = tuple->e2;
      // Arr<char>
      Arr *pars = tuple->e3;

      char ch = rt->id[0];

      if (ch == 'i') { // enumeration - int
        EACH(pars, char, p) {
          if (!*p) continue;

          int ffind (TypedSym *s) { return str_eq(s->id, p); }
          TypedSym *old_sym = opt_get(arr_find(top_syms, (FPRED)ffind));
          if (old_sym)
            return fail(ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              p, old_sym->ln
            ));
          Imp *imp = opt_get(map_get(imports, p));
          if (imp)
            return fail(ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              p, imp->ln
            ));

          char *itos = str_f("%d", _i);
          TypedSym *ts = typedSym_new(ln, p, type_int(), opt_some(itos), TRUE);
          arr_push(top_syms, ts);

        }_EACH

        if (public)
          arr_push(es, docE_new_enum(
            str_f("(%d)", enum_ix++),
            str_f("%s =", getFnCode("", pars)),
            st->ln,
            getDoc2(st->ln)
          ));
      } else if (ch == 's') { // enumeration - string
        EACH(pars, char, p) {
          if (!*p) return fail(ln, "String enumerations do not allow blanks");

          int ffind (TypedSym *s) { return str_eq(s->id, p); }
          TypedSym *old_sym = opt_get(arr_find(top_syms, (FPRED)ffind));
          if (old_sym)
            return fail(ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              p, old_sym->ln
            ));
          Imp *imp = opt_get(map_get(imports, p));
          if (imp)
            return fail(ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              p, imp->ln
            ));

          TypedSym *ts = typedSym_new(
            ln, p, type_string(), opt_some(js_ws(p)), TRUE
          );
          arr_push(top_syms, ts);

        }_EACH

        if (public)
          arr_push(es, docE_new_enum(
            str_f("(%d)", enum_ix++),
            str_f("%s =:", getFnCode("", pars)),
            st->ln,
            getDoc2(st->ln)
          ));
      } else { // Indexed
        Type *ft = rt;
        if (ch == '(') {
          Tp *types = opt_get(map_get(type_defs, str_sub(rt->id, 1, -1)));
          if (!types)
            return fail(ln, str_f(
              "Type '%s' not found in this module", type_to_str(rt)
            ));
          Type *left = types->e1;
          Type *right = types->e2;
          ft = type_replace_def_generics(rt, left, right);
        }

        if (ft->id[0] != 't')
          return fail(ln, str_f(
            "Indexed expected a tuple type but '%s' was found", type_to_str(rt)
          ));
        if (arr_size(pars) != arr_size(ft->subtypes))
          return fail(ln, "Parameters number of type and indexed does not match");

        // Arr<Type>
        Arr *fn_subs = arr_copy(ft->subtypes);
        arr_push(fn_subs, rt);
        Type *fntp = type_new("1", fn_subs);

        // Arr<char>
        Arr *all_syms = arr_copy(pars);
        arr_push(all_syms, fn);
        EACH(all_syms, char, sym) {
          int ffind (TypedSym *s) { return str_eq(s->id, sym); }
          TypedSym *old_sym = opt_get(arr_find(top_syms, (FPRED)ffind));
          if (old_sym)
            return fail(ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              sym, old_sym->ln
            ));
          Imp *imp = opt_get(map_get(imports, sym));
          if (imp)
            return fail(ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              sym, imp->ln
            ));
        }_EACH

        EACH(pars, char, p) {
          char *itos = str_f("%d", _i);
          TypedSym *ts = typedSym_new(ln, p, type_int(), opt_some(itos), TRUE);
          arr_push(top_syms, ts);
        }_EACH

        TypedSym *ts = typedSym_new(ln, fn, fntp, opt_none(), TRUE);
        arr_push(top_syms, ts);

        if (public)
          arr_push(es, docE_new_indexed(
            fn,
            getFnCode(str_f("%s : ", fn), pars),
            st->ln,
            getDoc1(st->ln)
          ));

      }
    } else if (stp == stat_import) {
      if (!public)
        return fail(ln, str_f(
          "Expected: assignation, indexed or enumeration\n  Actual: %s",
          stat_to_str(st)
        ));

      Tp *tuple = st->value;
      char *mpath = tuple->e1;
      if (*mpath == '/')
        return fail(ln , str_f("(%s) Absolute paths are not allowed.", mpath));

      char *local_md_id = opt_get(tuple->e2);
      if (!local_md_id) local_md_id = path_base(mpath);
      if (!char_is_id(local_md_id))
        return fail(ln, str_f("'%s' is not a valid symbol.", local_md_id));
      int ffind (TypedSym *s) { return str_eq(s->id, local_md_id);}
      TypedSym *ts = opt_get(arr_find(top_syms, (FPRED)ffind));
      if (ts)
        return fail(ln , str_f(
          "Symbol '%s' is duplicated (first assigned in line %d)",
          local_md_id, ts->ln
        ));

      EACH(imports, Kv, kv) {
        char *id = kv_key(kv);
        Imp *imp = kv_value(kv);
        if (str_eq(id, local_md_id))
          return fail(ln , str_f(
            "Symbol '%s' is duplicated (first assigned in line %d)",
            id, imp->ln
          ));
      }_EACH

      map_put(imports, local_md_id, imp_new(ln, mpath, local_md_id));

      if (public)
        arr_push(es, docE_new_import(
          local_md_id,
          str_f("import %s%s",
            mpath, opt_get(tuple->e2) ? str_f(" : %s", local_md_id) : ""
          ),
          st->ln,
          getDoc2(st->ln)
        ));

    } else {
      if (!public || st->tp != stat_block)
        return fail(ln, str_f(
          "Expected: import, indexed, enumeration, assignation or block in top code\n"
          "  Actual: %s", stat_to_str(st)
        ));
      with_block = TRUE;
    }
  }

  return arr_to_js(es, (FTO)docE_to_js);
}
