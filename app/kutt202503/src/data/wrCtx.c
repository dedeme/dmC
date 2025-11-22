// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "data/wrCtx.h"
#include "kut/rs.h"
#include "data/typedSym.h"
#include "data/imp.h"
#include "data/module.h"
#include "modules.h"
#include "cts.h"

static WrCtx *new (
  char *md_id, char *md_path, Map *type_defs,
  Map *imports, Arr *top_syms, DicLayers *sym_types
) {
  WrCtx *this = MALLOC(WrCtx);
  this->md_id = md_id;
  this->md_path = md_path;
  this->type_defs = type_defs;
  this->imports = imports;
  this->top_syms = top_syms;
  this->sym_types = sym_types;
  return this;
}

WrCtx *wrCtx_new (Module *md) {
  return new (
    md->id, md->path, md->type_defs, md->imports,
    md->top_syms, dicLayers_new()
  );
}

int wrCtx_is_inner_symbol (WrCtx *this, Exp *e) {
  if (e->tp != exp_symbol) return FALSE;
  return opt_get(dicLayers_get_element(this->sym_types, e->value))
    ? TRUE : FALSE;
}

Opt *wrCtx_get_symbol (WrCtx *this, char *sym) {
  TypedSym *tps = opt_get(dicLayers_get_element(this->sym_types, sym));
  if (!tps) {
    int ffind (TypedSym *s) { return str_eq(s->id, sym); }
    tps = opt_get(arr_find(this->top_syms, (FPRED)ffind));
    if (!tps) return opt_none();
    return opt_some(tp_mk(tps, cts_true()));
  }
  return opt_some(tp_mk(tps, cts_false()));
}

/// Returns a Rs<char> with the canonical name of a definition type (It is in
/// the form 'realModuleName.sym').
///   ctx: Writer context.
///   mod: Module code name. It can be "" if only the symbol was indicated.
///   sym: Definition symbol.
Rs *wrCtx_canonical_def (WrCtx *this, char *mod, char *sym) {
  // \ -> Rs<char>
  Rs *error (void) {
    return rs_fail(str_f("Definition of type '%s%s' not found",
      !*mod ? "" : str_f("%s.", mod), sym
    ));
  }

  if (*mod) {
    Imp *im = opt_get(map_get(this->imports, mod));
    if (!im) return error();
    Module *md = modules_get(im->id);
    TypedSym *def = opt_get(map_get(md->type_defs, sym));
    if (!def) return error();
    return rs_ok(str_f("%s.%s", im->id, sym)); // Ok response
  }

  TypedSym *def = opt_get(map_get(this->type_defs, sym));
  if (!def) {
    char *imp_name = NULL;
    char *mod2 = NULL;
    EACH(this->imports, Kv, kv) {
      char *iname = kv_key(kv);
      Imp *im = kv_value(kv);
      Module *md = modules_get(im->id);
      TypedSym *def = opt_get(map_get(md->type_defs, sym));
      if (!def) continue;
      if (!mod2) {
        imp_name = iname;
        mod2 = im->id;
        continue;
      }
      return rs_fail(str_f(
        "Duplicate definition of type '%s': '%s.%s' and '%s.%s'",
        sym, imp_name, sym, iname, sym
      ));
    }_EACH
    if (!mod2) return error();
    return rs_ok(str_f("%s.%s", mod2, sym)); // Ok response
  }
  return rs_ok(str_f("%s.%s", this->md_id, sym));  // Ok response
}

Rs *wrCtx_expand (WrCtx *this, Type *t) {
  Rs *error (void) {
    return rs_fail(str_f("Type '%s' not found", type_to_str(t)));
  }

  if (t->id[0] != '(') return rs_ok(t);

  char *mod_id = type_dmd(t);
  char *sym = type_did(t);
  int ngs = arr_size(t->subtypes);
  if (*mod_id) {
    Imp *im = opt_get(map_get(this->imports, mod_id));
    if (!im) return error();
    Module *md = modules_get(im->id);
    Tp *def = opt_get(map_get(md->type_defs, sym));
    if (!def) return error();
    Type *left = def->e1;
    Type *right = def->e2;
    if (arr_size(left->subtypes) != ngs)
      return rs_fail(str_f(
        "Number of generics of %s and %s does not match",
        type_to_str(t), type_to_str(left)
      ));
    return rs_ok(type_replace_def_generics(t, left, right));
  }
  Tp *def = opt_get(map_get(this->type_defs, sym));
  if (!def) {
    Type *expand = NULL;
    EACH(this->imports, Kv, kv) {
      Imp *im = kv_value(kv);
      Module *md = modules_get(im->id);
      Tp *def = opt_get(map_get(md->type_defs, sym));
      if (!def) continue;
      if (!expand) {
        Type *left = def->e1;
        Type *right = def->e2;
        if (arr_size(left->subtypes) != ngs) continue;
        expand = type_replace_def_generics(t, left, right);
        continue;
      }
      Type *def_left = def->e1;
      return rs_fail(str_f(
          "Fund duplicate definition of type '%s': '%s' and '%s.%s'",
          sym, type_to_str(def_left), im->id, sym
        ));
    }_EACH
    if (!expand) return error();
    return rs_ok(expand);
  }
  Type *left = def->e1;
  Type *right = def->e2;
  if (arr_size(left->subtypes) != ngs)
    return rs_fail(str_f(
      "Number of generics of %s and %s does not match",
      type_to_str(t), type_to_str(left)
    ));
  return rs_ok(type_replace_def_generics(t, left, right));
}

