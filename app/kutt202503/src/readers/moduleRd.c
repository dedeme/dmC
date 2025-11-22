// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "readers/moduleRd.h"
#include "kut/map.h"
#include "kut/js.h"
#include "kut/path.h"
#include "kut/file.h"
#include "data/tp.h"
#include "data/tp3.h"
#include "readers/stRd.h"
#include "data/stat.h"
#include "data/rdCtx.h"
#include "data/typedSym.h"
#include "data/imp.h"
#include "data/indexEntry.h"
#include "data/module.h"
#include "db.h"
#include "char.h"
#include "modules.h"

// Rs<Module>
Rs *moduleRd_read (int is_main, char *fpath, char *id, char *tx) {
  // Rs<Module>
  Rs *fail (int ln, char *msg) {
    return rs_fail(str_f("%s\n  %s:%d:", msg, fpath, ln));
  }

  // Map<Imp>
  Map *imports = map_new();
  // Arr<TypedSym>
  Arr *exports = arr_new();
  // Arr<TypedSym>
  Arr *top_syms = arr_new();
  // Arr<Tp<Type, Type>>
  Map *type_defs = map_new();
  // Arr<Stat>
  Arr *code = arr_new();

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
      if (str_eq(sym, "main") && is_main)
        return fail(ln, "Main module can not define a 'main' top symbol");
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
          char *cvalue = t == type_string() ? js_ws(e->value) : e->value;
          TypedSym *ts = typedSym_new(ln, sym, t, opt_some(cvalue), TRUE);
          arr_push(top_syms, ts);
          if (public) arr_push(exports, ts);
          arr_push(code, st);
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
          if (public) arr_push(exports, ts);
          arr_push(code, st);
          break;
        }
        case exp_function: {
          Tp3 *tp = e->value;
          TypedSym *ts = typedSym_new(ln, sym, tp->e1, opt_none(), TRUE);
          arr_push(top_syms, ts);
          if (public) arr_push(exports, ts);
          arr_push(code, st);
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
          if (public) arr_push(exports, ts);
          arr_push(code, stat_new_assign(
            ln, exp_new_symbol(ln, p), exp_new_integer(ln, itos)
          ));
        }_EACH
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
          if (public) arr_push(exports, ts);
          arr_push(code, stat_new_assign(
            ln, exp_new_symbol(ln, p), exp_new_string(ln, p)
          ));
        }_EACH
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
          if (public) arr_push(exports, ts);
          arr_push(code, stat_new_assign(
            ln, exp_new_symbol(ln, p), exp_new_integer(ln, itos)
          ));
        }_EACH

        TypedSym *ts = typedSym_new(ln, fn, fntp, opt_none(), TRUE);
        arr_push(top_syms, ts);
        if (public) arr_push(exports, ts);
        Exp *par_to_exp (char *p) { return exp_new_symbol(ln, p); }

        arr_push(code, stat_new_assign(
          ln, exp_new_symbol(ln, fn), exp_new_function(
            ln, fntp, pars, stat_new_block(ln, arr_new_from(
              stat_new_return(ln, opt_some(exp_new_tuple(ln, arr_map(
                pars, (FMAP)par_to_exp
              )))),
              NULL
            ))
          )
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

      char *rpath = str_f("%s.ktt", mpath);
      char *path = path_cat(path_parent(fpath), rpath, NULL);
      if (!file_exists(path)) path = path_cat(db_main_root(), rpath, NULL);
      IndexEntry *ixe = db_get_path_id(path);
      path = ixe->path;
      char *comp_md_id = ixe->id;
      if (!*path) return fail(ln, str_f("Module '%s' not found.", mpath));

      EACH(imports, Kv, kv) {
        char *id = kv_key(kv);
        Imp *imp = kv_value(kv);
        if (str_eq(id, local_md_id))
          return fail(ln , str_f(
            "Symbol '%s' is duplicated (first assigned in line %d)",
            id, imp->ln
          ));
        if (str_eq(imp->id, comp_md_id))
          return fail(ln , str_f(
            "Symbol '%s' is duplicated (first assigned in line %d)",
            comp_md_id, imp->ln
          ));
      }_EACH

      map_put(imports, local_md_id, imp_new(ln, mpath, comp_md_id));

      char *err = modules_add(FALSE, path, comp_md_id);
      if (*err) return fail(ln, err);
    } else {
      if (!public || !is_main || st->tp != stat_block)
        return fail(ln, str_f(
          "Expected: import, indexed, enumeration, assignation or block in top code\n"
          "  Actual: %s", stat_to_str(st)
        ));
      with_block = TRUE;
      arr_push(code, st);
    }
  }

  if (is_main && !with_block) {
    return fail(1, "Block code is missing in main module");
  }

  return rs_ok(
    module_new(is_main, fpath, id, imports, exports, top_syms, type_defs, code)
  );
}
