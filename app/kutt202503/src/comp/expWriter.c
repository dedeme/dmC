// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/expWriter.h"
#include "kut/math.h"
#include "kut/js.h"
#include "data/typedSym.h"
#include "data/imp.h"
#include "data/tp3.h"
#include "data/stat.h"
#include "comp/built.h"
#include "comp/blockWriter.h"
#include "fns.h"
#include "modules.h"

//: \<wrCtx>, <exp>, [<exp>.] -> <wrERs>
WrERs *fmt (WrCtx *ctx, Exp *s, Exp *vs) {
  // Arr<char>
  Arr *fns = arr_new();
  // Arr<char>
  Arr *cvars = arr_new();

  WrERs *rs = expWriter_run(ctx, s);
  if (rs->is_error) return rs;
  Type *t = rs->tp;
  if (t != type_string()) return wrERs_failT(ctx, s->ln, type_string(), s, t);
  if (*rs->fun_code) arr_push(fns, rs->fun_code);

  if (vs->tp != exp_tuple)
    return wrERs_failE(ctx, vs->ln, "literal tuple", exp_to_str(vs));

  // Arr<Exp>
  Arr *vvs = vs->value;
  EACH(vvs, Exp, v) {
    WrERs *rs = expWriter_run(ctx, v);
    if (rs->is_error) return rs;
    Type *t = rs->tp;
    if (*rs->fun_code) arr_push(fns, rs->fun_code);
    arr_push(cvars, str_f(
      "___built_to_str(%s,%s)",
      type_to_c(t, ctx), rs->code
    ));
  }_EACH

  return wrERs_mk(type_string(), fns_fun_code(fns), str_f(
    "__str_fmt(%s,%s,"
    "__arr_new_from_carr((Val)%d, (Val[]){%s}))",
    fns_mk_pos(ctx->md_id, s->ln), rs->code,
    arr_size(vvs), arr_cjoin(cvars, ',')
  ));
};

WrERs *expWriter_run(WrCtx *ctx, Exp *ex) {
  int ln = ex->ln;
  ExpT tp = ex->tp;
  void *v = ex->value;

  switch (tp) {
    case exp_error: return wrERs_fail(ctx, ln, v);
    case exp_boolean: return wrERs_mk(
      type_bool(), "", str_f("((Val)%s)", v)
    );
    case exp_integer: return wrERs_mk(
      type_int(), "", str_f("((Val)%s)", v)
    );
    case exp_float: return wrERs_mk(
      type_float(), "", str_f("((Val)%s)", v)
    );
    case exp_string: return wrERs_mk(
      type_string(), "", str_f("((Val)%s)", js_ws(v))
    );
    case exp_array0: {
      Type *t = opt_get(v);
      if (!t) return wrERs_fail(ctx, ln, "Array type is missing in []");
      return wrERs_mk(type_new("a", arr_new_from(t, NULL)), "", "__arr_new()");
    }
    case exp_array: {
      // Arr<Exp>
      Arr *es = v;
      WrERs *ers = expWriter_run(ctx, *arr_begin(es));
      if (ers->is_error) return ers;
      Type *atp = ers->tp;

      // Arr<char>
      Arr *fns = arr_new();
      // Arr<char>
      Arr *vals = arr_new();

      EACH(es, Exp, e) {
        if (
          ( (e->tp == exp_array0 && atp->id[0] == 'a') ||
            (e->tp == exp_dictionary0 && atp->id[0] == 'd')
          ) && !opt_get((Opt *)(e->value))
        )
          e->value = opt_some(*arr_begin(atp->subtypes));

        WrERs *ers = expWriter_run(ctx, e);
        if (ers->is_error) return ers;
        Type *etp = ers->tp;

        Rs *rs = type_allow(ctx, map_new(), atp, etp);
        int *ok = rs_get(rs);
        if (!ok) return wrERs_fail(ctx, e->ln, rs_error(rs));
        if(!*ok) return wrERs_failT(ctx, e->ln, atp, e, etp);
        if (
          (etp->id[0] == '0' || etp->id[0] == '1') &&
          wrCtx_is_inner_symbol(ctx, e)
        )
          return wrERs_fail(ctx, ln, str_f(
            "%s: Inner functions can not be added to array", exp_to_str(e)
          ));

        arr_push(fns, ers->fun_code);
        arr_push(vals, ers->code);
      }_EACH

      return wrERs_mk(
        type_new("a", arr_new_from(atp, NULL)),
        fns_fun_code(fns),
        str_f(
          "__arr_new_from_carr((Val)%d, (Val[]){%s})",
          arr_size(es), arr_cjoin(vals, ',')
        )
      );
    }
    case exp_dictionary0: {
      Type *t = opt_get(v);
      if (!t) return wrERs_fail(ctx, ln, "Dictionary type is missing in {}");
      return wrERs_mk(type_new("d", arr_new_from(t, NULL)), "", "__arr_new()");
    }
    case exp_dictionary: {
      // Arr<Tp<Exp, Exp>>
      Arr *es = v;
      Tp *tp0 = *arr_begin(es);
      WrERs *ers = expWriter_run(ctx, tp0->e2);
      if (ers->is_error) return ers;
      Type *dtp = ers->tp;
      // Arr<char>
      Arr *fns = arr_new();
      // Arr<char>
      Arr *vals = arr_new();

      EACH(es, Tp, e) {
        Exp *key = e->e1;
        Exp *val = e->e2;
        if (
          ( (val->tp == exp_array0 && dtp->id[0] == 'a') ||
            (val->tp == exp_dictionary0 && dtp->id[0] == 'd')
          ) && !opt_get((Opt *)(val->value))
        )
          val->value = opt_some(*arr_begin(dtp->subtypes));


        if (key->tp != exp_string)
          return wrERs_failE(
            ctx, key->ln, "literal string or symbol", exp_to_str(key)
          );

        WrERs *ers = expWriter_run(ctx, val);
        if (ers->is_error) return ers;
        Type *etp = ers->tp;
        Rs *rs = type_allow(ctx, map_new(), dtp, etp);
        int *ok = rs_get(rs);
        if (!ok) return wrERs_fail(ctx, val->ln, rs_error(rs));
        if(!*ok) return wrERs_failT(ctx, val->ln, dtp, val, etp);
        if (
          (etp->id[0] == '0' || etp->id[0] == '1') &&
          wrCtx_is_inner_symbol(ctx, val)
        )
          return wrERs_fail(ctx, ln, str_f(
            "%s: Inner functions can not be values of dictionary",
            exp_to_str(val)
          ));

        arr_push(fns, ers->fun_code);
        arr_push(vals, str_f(
          "__arr_new_from_carr((Val)2, (Val[]){(Val)%s, %s})",
          js_ws(key->value), ers->code
        ));
      }_EACH
      return wrERs_mk(
        type_new("d", arr_new_from(dtp, NULL)),
        fns_fun_code(fns),
        str_f(
          "__dic_new_from_carr((Val)%d, (Val[]){%s})"
          , arr_size(es), arr_cjoin(vals, ',')
        )
      );
    }
    case exp_tuple: {
      // Arr<char>
      Arr *fns = arr_new();
      // Arr<char>
      Arr *vals = arr_new();
      // Arr<Type>
      Arr *tps = arr_new();
      // Arr<Exp>
      Arr *es = v;

      EACH(es, Exp, e) {
        WrERs *ers = expWriter_run(ctx, e);
        if (ers->is_error) return ers;
        Type *etp = ers->tp;

        if (
          (etp->id[0] == '0' || etp->id[0] == '1') &&
          wrCtx_is_inner_symbol(ctx, e)
        )
          return wrERs_fail(ctx, ln, str_f(
            "%s: Inner functions can not be in tuple", exp_to_str(e)
          ));


        arr_push(fns, ers->fun_code);
        arr_push(vals, ers->code);
        arr_push(tps, etp);
      }_EACH

      return wrERs_mk(
        type_new("t", tps),
        fns_fun_code(fns),
        str_f("__arr_new_from_carr((Val)%d, (Val[]){%s})",
          arr_size(es), arr_cjoin(vals, ',')
        )
      );
    }
    case exp_range: {
      // Arr<char>
      Arr *vs = arr_new();
      // Arr<char>
      Arr *fns = arr_new();
      EACH(v, Exp, e) {
        WrERs *rs = expWriter_run(ctx, e);
        if (rs->is_error) return rs;
        Type *t = rs->tp;
        if (t != type_int()) return wrERs_failT(ctx, e->ln, type_int(), e, t);

        arr_push(fns, rs->fun_code);
        arr_push(vs, rs->code);
      }_EACH

      return wrERs_mk(
        type_new("r", arr_new_from(type_int(), NULL)),
        fns_fun_code(fns),
        str_f(
          "__iter_range(%s,"
          "__arr_new_from_carr((Val)%d, (Val[]){%s})"
          ")",
          fns_mk_pos(ctx->md_id, ln), arr_size(v), arr_cjoin(vs, ',')
        )
      );
    }
    case exp_function: {
      Tp3 *tuple = v;
      Type *ft = tuple->e1;

      // Arr<char>
      Arr *pars = tuple->e2;

      Stat *st = tuple->e3; // st is type stat.block
      Rs *rs = wrCtx_expand(ctx, ft);
      Type *tp = rs_get(rs);
      if (!tp) return wrERs_fail(ctx, ln, rs_error(rs));

      if (tp->id[0] == '1' && !arr_size(tp->subtypes))
        return wrERs_fail(ctx, ln, "Function type is missing");

      if (type_fnpars(tp) != arr_size(pars)) {
        if (type_fnpars(tp) == -1)
          return wrERs_fail(
            ctx, ln, str_f("Wrong function type (%s)", type_to_str(ft))
          );
        return wrERs_fail(ctx, ln, str_f(
          "Parameters number of type (%d) and function (%d) does not match",
          type_fnpars(tp), arr_size(pars)
        ));
      }

      DicLayers *sym_types = ctx->sym_types;
      dicLayers_add_layer(sym_types);
      EACH(pars, char, p) {
        dicLayers_put_element(
          sym_types, p,
          typedSym_new(ln, p, type_fptype(tp, _i), opt_none(), TRUE)
        );
      }_EACH

      // Map<Type>
      Map *generics = map_new();
      // Arr<Type>
      Arr *rtype = arr_new();
      Type *frtype = opt_get(type_frtype(tp));
      if (frtype) {
        arr_push(rtype, frtype);

        Type **subtypes = (Type **)arr_begin(tp->subtypes);
        for (int i = 0; i < type_fnpars(tp); ++i) {
          char *gs = type_get_generics(*subtypes++);
          for (int j = 0; j < strlen(gs); ++j) {
            char *g = str_new_c(gs[j]);
            map_put(generics, g, type_new(g, arr_new()));
          }
        }
      }

      WrSRs *strs = blockWriter_run(
        rtype, generics, blockWriter_function, FALSE, FALSE,
        ctx, st->ln, st->value
      );
      dicLayers_remove_layer(sym_types);
      if (strs->is_error) return wrERs_fromWrSRsFail(strs);

      char *fnsym = fns_next_id(ctx->md_id);
      char *fndef = str_f(
        "%s%s\nVal %s=(Val)(void*)%s_f;",
        fns_mk_fn_head(
          arr_size(rtype) ? TRUE : FALSE, pars, str_f("%s_f", fnsym)
        ),
        strs->code, fnsym, fnsym
      );

      Type *rtp = tp;
      if (arr_size(rtype)) {
        // Arr<Type>
        Arr *sub_types = arr_copy(tp->subtypes);
        arr_pop(sub_types);
        arr_push(sub_types, *arr_begin(rtype));
        rtp = type_new("1", sub_types);
      }
      return wrERs_mk(rtp, fndef, fnsym);
    }
    case exp_symbol: {
      Tp *tp = opt_get(wrCtx_get_symbol(ctx, v));
      if (!tp) {
        Imp *imp = opt_get(map_get(ctx->imports, v));
        if (imp)
          return wrERs_fail(ctx, ln, str_f("Import '%s' used as local symbol", v));
        return wrERs_fail(ctx, ln, str_f("Symbol '%s' not found", v));
      }
      TypedSym *tps = tp->e1;
      int *is_top = tp->e2;
      tps->used = TRUE;
      return wrERs_mk(tps->tp, "",
        *is_top && (tps->tp)->id[0] == 'a'
          ? str_f(
              "((Val (*)(void))(%s_%s).o)()",
              ctx->md_id, v
            )
          : str_f("%s%s", *is_top ? str_f("%s_", ctx->md_id): "", v)
      );
    }
    case exp_point: {
      Tp *tp = v;
      char *md = tp->e1;
      char *sym = tp->e2;

      Tp *local_sym = opt_get(wrCtx_get_symbol(ctx, md));
      if (local_sym) {
        TypedSym *ts = local_sym->e1;
        Rs *rs = wrCtx_expand(ctx, ts->tp);
        Type *s = rs_get(rs);
        if (!s) return wrERs_fail(ctx, ln, rs_error(rs));
        if (s->id[0] == 't')
          return expWriter_run(ctx, exp_new_square(
            ln, exp_new_symbol(ln, md), exp_new_symbol(ln, sym)
          ));

        return wrERs_fail(ctx, ln, str_f(
          "Local symbol '%s' "
          "(defined in line %d) "
          "does not admit operator '.'",
          md, ts->ln
        ));
      }

      Imp *i = opt_get(map_get(ctx->imports, md));
      if (!i) return built_get(ctx, ln, md, sym);

      char *md_id = i->id;
      Module *m = modules_get(md_id);
      int ffind (TypedSym *ts) { return str_eq(ts->id, sym); }
      TypedSym *ts = opt_get(arr_find(m->exports, (FPRED)ffind));
      if (!ts)
        return wrERs_fail(ctx, ln, str_f("'%s.%s': Symbol not found", md, sym));
      Type *t = ts->tp;

      return wrERs_mk(ts->tp, "",
        t->id[0] == 'a'
          ? str_f("((Val (*)(void))(%s_%s).o)()", md_id, sym)
          : !opt_get(ts->cval)
            ? str_f("%s_%s", md_id, sym)
            : str_f("((Val)%s)", opt_get(ts->cval))
      );
    }
    case exp_square: {
      Tp *tuple = v;
      Exp *e1 = tuple->e1;
      Exp *e2 = tuple->e2;
      WrERs *rs1 = expWriter_run(ctx, e1);
      if (rs1->is_error) return rs1;
      Rs *trs1 = wrCtx_expand(ctx, rs1->tp);
      Type *t1 = rs_get(trs1);
      if (!t1) return wrERs_fail(ctx, e1->ln, rs_error(trs1));
      WrERs *rs2 = expWriter_run(ctx, e2);
      if (rs2->is_error) return rs2;
      Type *t2 = rs2->tp;
      char *fun_code =
        fns_fun_code(arr_new_from(rs1->fun_code, rs2->fun_code, NULL));
      switch (t1->id[0]) {
        case 's': {
          if (t2->id[0] != 'i')
            return wrERs_failT(ctx, e1->ln, type_int(), e2, t2);
          return wrERs_mk(type_string(), fun_code, str_f(
            "__str_get(%s,%s,%s)",
            fns_mk_pos(ctx->md_id, ln), rs1->code, rs2->code
          ));
        }
        case 'a': {
          if (t2->id[0] != 'i')
            return wrERs_failT(ctx, e1->ln, type_int(), e2, t2);
          return wrERs_mk(*arr_begin(t1->subtypes), fun_code, str_f(
            "__arr_get2(%s,%s,%s)",
            fns_mk_pos(ctx->md_id, ln), rs1->code, rs2->code
          ));
        }
        case 't': {
          if (t2->id[0] != 'i')
            return wrERs_failT(ctx, e1->ln, type_int(), e2, t2);
          char *ix = "";
          switch (e2->tp) {
            case exp_integer: {
              ix = e2->value;
              break;
            }
            case exp_symbol: {
              Tp *tp = opt_get(wrCtx_get_symbol(ctx, e2->value));
              if (tp) {
                TypedSym *ts = tp->e1;
                if (opt_get(ts->cval)) ix = opt_get(ts->cval);
              }
              break;
            }
            case exp_point: {
              WrERs *rs = expWriter_run(ctx, e2);
              if (rs->is_error) return rs;
              char *code = rs->code; // type ((Val)xxx)
              ix = str_sub(code, str_cindex(code, ')') + 1, -1);
              break;
            }
            default: {}
          }
          if (!*ix) return wrERs_failE(ctx, ln, "constant integer", exp_to_str(e2));

          int i = math_stoi(ix);
          if (i >= arr_size(t1->subtypes))
            return wrERs_fail(ctx, ln,
              str_f("index %d out of [0,%d]", i, arr_size(t1->subtypes) - 1)
            );

          return wrERs_mk(arr_begin(t1->subtypes)[i], fun_code, str_f(
            "__arr_get(%s,%s)",
            rs1->code, rs2->code
          ));
        }
        case 'd': {
          if (t2->id[0] != 's')
            return wrERs_failT(ctx, e1->ln, type_string(), e2, t2);
          return wrERs_mk(*arr_begin(t1->subtypes), fun_code, str_f(
            "__dic_get2(%s, %s,%s)",
            fns_mk_pos(ctx->md_id, ln), rs1->code, rs2->code
          ));
        }
        default:
          return wrERs_failE(ctx, e1->ln,
            "Expression of type string, array, tuple or dictionary",
            str_f("%s (of type %s)", exp_to_str(e1), type_to_str(rs1->tp)
          ));
      }
    }
    case exp_slice: {
      // Arr<char>
      Arr *fns = arr_new();
      char *code_l = "";
      char *code_r = "";
      Tp3 *tuple = v;
      Exp *ct = tuple->e1;
      Exp *e = opt_get(tuple->e2);
      if (e) {
        WrERs *rs = expWriter_run(ctx, e);
        if (rs->is_error) return rs;
        Type *t = rs->tp;
        if (t->id[0] != 'i')
          return wrERs_failT(ctx, e->ln, type_int(), e, t);
        arr_push(fns, rs->fun_code);
        code_l = rs->code;
      }
      code_l = *code_l ? code_l : "(Val)0";

      e = opt_get(tuple->e3);
      if (e) {
        WrERs *rs = expWriter_run(ctx, e);
        if (rs->is_error) return rs;
        Type *t = rs->tp;
        if (t->id[0] != 'i')
          return wrERs_failT(ctx, e->ln, type_int(), e, t);
        arr_push(fns, rs->fun_code);
        code_r = rs->code;
      }

      WrERs *rs_ct = expWriter_run(ctx, ct);
      if (rs_ct->is_error) return rs_ct;
      Type *t_ct = rs_ct->tp;
      arr_push(fns, rs_ct->fun_code);
      switch (t_ct->id[0]) {
        case 's': {
          return wrERs_mk(type_string(), fns_fun_code(fns), *code_r
            ? str_f("__str_sub_slice(%s,%s,%s)", rs_ct->code, code_l, code_r)
            : str_f("__str_right_slice(%s,%s)", rs_ct->code, code_l)
          );
        }
        case 'a': {
          return wrERs_mk(t_ct, fns_fun_code(fns), *code_r
            ? str_f("__arr_sub(%s,%s,%s)", rs_ct->code, code_l, code_r)
            : str_f("__arr_sub0(%s,%s)", rs_ct->code, code_l)
          );
        }
        default:
          return wrERs_failE(ctx, ct->ln,
            "Expression of type string or array",
            str_f("%s (of type %s)", exp_to_str(ct), type_to_str(t_ct)
          ));
      }
    }
    case exp_parenthesis: {
      return expWriter_parenthesis(ctx, ex, FALSE);
    }
    case exp_group: {
      WrERs *ers = expWriter_run(ctx, v);
      if (ers->is_error) return ers;
      return wrERs_mk(ers->tp, ers->fun_code, str_f("(%s)", ers->code));
    }
    case exp_unary: {
      Tp *tuple = v;
      char *sym = tuple->e1;
      Exp *e = tuple->e2;
      WrERs *ers = expWriter_run(ctx, e);
      if (ers->is_error) return ers;
      Type *tp = ers->tp;
      if (sym[0] == '!') {
        switch (tp->id[0]) {
          case 'a': return wrERs_mk(type_bool(), ers->fun_code,
              str_f("((Val)!__arr_size(%s).i)", ers->code));
          case 'b': return wrERs_mk(type_bool(), ers->fun_code,
              str_f("((Val)!(%s).b)", ers->code));
          default:
          return wrERs_failE(ctx, e->ln,
            "Expression of type boolean or array",
            str_f("%s (of type %s)", exp_to_str(e), type_to_str(tp)
          ));
        }
      }
      // opertor -
      switch (tp->id[0]) {
        case 'i':
        case 'f': return wrERs_mk(tp, ers->fun_code,
            str_f("((Val)-(%s).%s)", ers->code, tp->id));
        default:
          return wrERs_failE(ctx, e->ln,
            "Expression of type integer or float",
            str_f("%s (of type %s)", exp_to_str(e), type_to_str(tp)
          ));
      }
    }
    case exp_binary: {
      Tp3 *tuple = v;
      char *op = tuple->e1;
      Exp *e1 = tuple->e2;
      WrERs *ers1 = expWriter_run(ctx, e1);
      if (ers1->is_error) return ers1;
      Type *tp1 = ers1->tp;
      Exp *e2 = tuple->e3;
      WrERs *ers2 = expWriter_run(ctx, e2);
      if (ers2->is_error) return ers2;
      Type *tp2 = ers2->tp;

      char *fun_code = fns_fun_code(arr_new_from(
        ers1->fun_code, ers2->fun_code, NULL
      ));

      WrERs *check (Type *tp, char *code) {
        if (strcmp(tp1->id, tp2->id)) return wrERs_failT(ctx, e2->ln, tp1, e2, tp2);
        return wrERs_mk(tp, fun_code, code);
      }

      switch (op[0]) {
        case '+': {
          switch (tp1->id[0]) {
            case 's': return check(tp1, str_f(
                "((Val)str_f(\"%%s%%s\",(%s).s,(%s).s))",
                ers1->code, ers2->code
              ));
            case 'i':
            case 'f': return check(tp1, str_f(
                "((Val)((%s).%s %s (%s).%s))",
                ers1->code, tp1->id, op, ers2->code, tp1->id
              ));
            case 'a': {
              Rs *rs = type_eq(ctx, tp1, tp2);
              int *ok = rs_get(rs);
              if (!ok) return wrERs_fail(ctx, e2->ln, rs_error(rs));
              if (!*ok)
                return wrERs_failT(ctx, e2->ln, tp1, e2, tp2);
              return wrERs_mk(tp1, fun_code, str_f(
                "__arr_cat2(%s,%s)",
                ers1->code, ers2->code
              ));
            }
            default:
              return wrERs_failE(ctx, e1->ln,
                "Expression of type integer, float, string or array",
                str_f("%s (of type %s)", exp_to_str(e1), type_to_str(tp1))
              );
          }
        }
        case '-':
        case '*': {
          switch (tp1->id[0]) {
            case 'i':
            case 'f': return check(tp1, str_f(
                "((Val)((%s).%s %s (%s).%s))",
                ers1->code, tp1->id, op, ers2->code, tp1->id
              ));
            default:
              return wrERs_failE(ctx, e1->ln,
                "Expression of type integer or float",
                str_f("%s (of type %s)", exp_to_str(e1), type_to_str(tp1))
              );
          }
        }
        case '/': {
          switch (tp1->id[0]) {
            case 'i': return check(tp1, str_f(
                "___built_divi(%s,%s,%s)",
                fns_mk_pos(ctx->md_id, ln), ers1->code, ers2->code
              ));
            case 'f': return check(tp1, str_f(
                "___built_divf(%s,%s,%s)",
                fns_mk_pos(ctx->md_id, ln), ers1->code, ers2->code
              ));
            default:
              return wrERs_failE(ctx, e1->ln,
                "Expression of type integer or float",
                str_f("%s (of type %s)", exp_to_str(e1), type_to_str(tp1))
              );
          }
        }
        case '%': {
          switch (tp1->id[0]) {
            case 'i': return check(tp1, str_f(
                "((Val)((%s).i % (%s).i))", ers1->code, ers2->code
              ));
            default:
              return wrERs_failT(ctx, e1->ln, type_int(), e1, tp1);
          }
        }
        case '=':
        case '!': { // "==", "!="
          Rs *rs = type_eq(ctx, tp1, tp2);
          int *ok = rs_get(rs);
          if (!ok) return wrERs_fail(ctx, e2->ln, rs_error(rs));
          if (!*ok) return wrERs_failT(ctx, e2->ln, tp1, e2, tp2);

          Rs *trs = wrCtx_expand(ctx, tp1);
          Type *texp = rs_get(trs);
          if (!texp) return wrERs_fail(ctx, e1->ln, rs_error(trs));

          return wrERs_mk(type_bool(), fun_code, str_f(
            "%s___built_eq(%s,%s,%s)%s",
            str_eq(op, "!=") ? "((Val)!(": "",
            type_to_c(texp, ctx), ers1->code, ers2->code,
            str_eq(op, "!=") ? ").b)": ""
          ));
        }
        case '<':
        case '>': { //">", ">=", "<", "<="
          switch (tp1->id[0]) {
            case 's': return check(type_bool(), str_f(
                "((Val)(strcmp((%s).s,(%s).s) %s 0))",
                ers1->code, ers2->code, op
              ));
            case 'i':
            case 'f': return check(type_bool(), str_f(
                "((Val)((%s).%s %s (%s).%s))",
                ers1->code, tp1->id, op, ers2->code, tp2->id
              ));
            default:
              return wrERs_failE(ctx, e1->ln,
                "Expression of type integer, float or string",
                str_f("%s (of type %s)", exp_to_str(e1), type_to_str(tp1))
              );
          }
        }
        default: { // & |
          switch (tp1->id[0]) {
            case 'b': return wrERs_mk(tp1, fun_code, str_f(
                "((Val)((%s).b %s%s (%s).b))",
                ers1->code, op, op, ers2->code
              ));
            default:
              return wrERs_failT(ctx, e1->ln, type_bool(), e1, tp1);
          }
        }
      }
    }
    case exp_ternary: {
      Tp3 *tuple = v;
      Exp *e1 = tuple->e1;
      WrERs *ers1 = expWriter_run(ctx, e1);
      if (ers1->is_error) return ers1;
      Type *tp1 = ers1->tp;
      if (tp1->id[0] != 'b')
        return wrERs_failT(ctx, e1->ln, type_bool(), e1, tp1);

      Exp *e2 = tuple->e2;
      WrERs *ers2 = expWriter_run(ctx, e2);
      if (ers2->is_error) return ers2;
      Type *tp2 = ers2->tp;
      Exp *e3 = tuple->e3;
      WrERs *ers3 = expWriter_run(ctx, e3);
      if (ers3->is_error) return ers3;
      Type *tp3 = ers3->tp;

      Rs *okrs = type_eq(ctx, tp2, tp3);
      int *ok = rs_get(okrs);
      if (!ok) return wrERs_fail(ctx, e3->ln, rs_error(okrs));
      if (!*ok)
        return wrERs_failT(ctx, e3->ln, tp2, e3, tp3);

      char *fun_code = fns_fun_code(arr_new_from(
        ers1->fun_code, ers2->fun_code, ers3->fun_code, NULL
      ));

      return wrERs_mk(tp2, fun_code, str_f(
        "((%s).b\n?(%s)\n:(%s))",
        ers1->code, ers2->code, ers3->code
      ));
    }
    case exp_switch: {
      Tp *tp = v;
      Exp *var = tp->e1;
      // Arr<Tp<Arr<Exp>, Exp>>
      Arr *entries = arr_copy(tp->e2);

      WrERs *rs = expWriter_run(ctx, var);
      if (rs->is_error) return rs;
      Type *tc = rs->tp;
      char *tmp = fns_next_id(ctx->md_id);

      // Tp<Arr<Exp>, Exp>
      Tp *lastEntry = arr_pop(entries);
      Exp *laste = lastEntry->e2;
      WrERs *laste_rs = expWriter_run(ctx, laste);
      if (laste_rs->is_error) return laste_rs;
      Type *tlast = laste_rs->tp;

      Arr *funs = arr_new_from(rs->fun_code, NULL);
      arr_push(funs, str_f("Val %s = %s;", tmp, rs->code));
      // Arr<char>
      Arr *code = arr_new();

      if (!arr_size(entries)) { // only default entry
        arr_push(funs, laste_rs->fun_code);
        return wrERs_mk(tlast, fns_fun_code(funs), laste_rs->code);
      }

      EACH(entries, Tp, entry) { // Tp<Arr<Exp>, Exp>
        // Arr<Exp>
        Arr *cs = entry->e1;
        // Arr<char>
        Arr *cs_code = arr_new();
        EACH(cs, Exp, c) {
          WrERs *crs = expWriter_run(ctx, c);
          if (crs->is_error) return crs;
          arr_push(funs, crs->fun_code);
          Rs *okrs = type_eq(ctx, tc, crs->tp);
          int *ok = rs_get(okrs);
          if (!ok) return wrERs_fail(ctx, c->ln, rs_error(okrs));
          if (!*ok) return wrERs_failT(ctx, c->ln, tc, c, crs->tp);

          arr_push(cs_code, str_f(
            "___built_eq(%s,%s,%s).b", type_to_c(tc, ctx), tmp, crs->code
          ));
        }_EACH

        Exp *ei = entry->e2;
        WrERs *ers = expWriter_run(ctx, ei);
        if (ers->is_error) return ers;
        Rs *okrs = type_eq(ctx, tlast, ers->tp);
        int *ok = rs_get(okrs);
        if (!ok) return wrERs_fail(ctx, ei->ln, rs_error(okrs));
        if (!*ok) return wrERs_failT(ctx, ei->ln, tlast, ei, ers->tp);
        arr_push(funs, ers->fun_code);

        arr_push(code, str_f("(%s)\n?(%s)", arr_join(cs_code, "||"), ers->code));
      }_EACH

      arr_push(funs, laste_rs->fun_code);
      return wrERs_mk(tlast, fns_fun_code(funs),
        str_f("(%s\n:(%s))", arr_join(code, "\n:"), laste_rs->code)
      );
    }
    default: {
      EXC_KUTT(str_f("Expression type not found: %s", exp_to_str(ex)));
      return NULL; // Unreachable
    }
  }
}

WrERs *expWriter_parenthesis (WrCtx *ctx, Exp *ex, int is_stat) {
  Tp *tp = ex->value;
  Exp *fn = tp->e1;
  // Arr<Exp>
  Arr *pars = tp->e2;

  if (fn->tp == exp_point) { // sys, arr special functions.
    Tp *tuple = fn->value;
    char *mod = tuple->e1;
    char *sym = tuple->e2;
    if (
      str_eq(mod, "sys") &&
      !opt_get(wrCtx_get_symbol(ctx, "sys")) &&
      !opt_get(map_get(ctx->imports, "sys"))
    ) {
      if (
        !is_stat &&
        ( str_eq(sym, "print") ||
          str_eq(sym, "println") ||
          str_eq(sym, "test")
        )
      )
        return wrERs_failE(ctx, fn->ln, "function with return", exp_to_str(fn));

      if (str_eq(sym, "print") || str_eq(sym, "println")) {
        if (!arr_size(pars)) {
          if (str_eq(sym, "println"))
            return wrERs_mk(type_bool(), "",  "puts(\"\");");
          return wrERs_mk(type_bool(), "",  "");
        }
        // Arr<char>
        Arr *fns = arr_new();
        // Arr<char>
        Arr *template_bf = arr_new();
        // Arr<char>
        Arr *values_bf = arr_new();
        EACH(pars, Exp, p) {
          WrERs *rs = expWriter_run(ctx, p);
          if (rs->is_error) return rs;
          Type *t = rs->tp;

          arr_push(fns, rs->fun_code);
          arr_push(template_bf, "%s");
          arr_push(values_bf, str_f("___built_to_str(%s,%s).s",
            type_to_c(t, ctx), rs->code
          ));
        }_EACH
        char *end_line = str_eq(sym, "print") ? "" : "\\n";
        return wrERs_mk(type_bool(), fns_fun_code(fns), str_f("printf(\"%s%s\",%s);",
          arr_cjoin(template_bf, ' '), end_line, arr_cjoin(values_bf,  ',')
        ));
      }
      if (str_eq(sym, "test")) {
        if (arr_size(pars) != 2)
          return wrERs_failE(
            ctx, fn->ln, "2 parameters", math_itos(arr_size(pars))
          );
        Exp *pars0 = *arr_begin(pars);
        Exp *pars1 = arr_begin(pars)[1];
        WrERs *rs1 = expWriter_run(ctx, pars0);
        if (rs1->is_error) return rs1;
        Type *t1 = rs1->tp;
        WrERs *rs2 = expWriter_run(ctx, pars1);
        if (rs2->is_error) return rs2;
        Type *t2 = rs2->tp;
        Rs *rs = type_eq(ctx, t1, t2);
        int *ok = rs_get(rs);
        if (!ok) return wrERs_fail(ctx, fn->ln, rs_error(rs));
        if (!*ok)
          return wrERs_fail(ctx, fn->ln, str_f(
            "Test failed:\n"
            "Expected: Expression of type %s\n"
            "  Actual: %s (of type %s)",
            type_to_str(t2), exp_to_str(pars0), type_to_str(t1)
          ));

        rs = wrCtx_expand(ctx, t1);
        Type *texp = rs_get(rs);
        if (!texp) return wrERs_fail(ctx, pars0->ln, rs_error(rs));

        char *tmp1 = fns_next_id(ctx->md_id);
        char *tmp2 = fns_next_id(ctx->md_id);
        return wrERs_mk(type_bool(),
          fns_fun_code(arr_new_from(rs1->fun_code, rs2->fun_code, NULL)),
          str_f(
            "Val %s = %s;"
            "Val %s = %s;"
            "if (!___built_eq("
            "%s, %s, %s).b) {\n"
            "printf("
            "\"%%s:\\n  Test failed:\\nExpected: %%s\\n  Actual: %%s\\n\","
            "___built_mpos(%s)"
            ",___built_to_str(%s, %s).s"
            ",___built_to_str(%s, %s).s"
            ");\n  exit(0);\n}",
            tmp1, rs1->code,
            tmp2, rs2->code,
            type_to_c(texp, ctx), tmp1, tmp2,
            fns_mk_pos(ctx->md_id, fn->ln),
            type_to_c(t2, ctx), tmp2,
            type_to_c(t1, ctx), tmp1
        ));
      }
      if (str_eq(sym, "toStr")) {
        if (arr_size(pars) != 1)
          return wrERs_failE(
            ctx, fn->ln, "1 parameter", math_itos(arr_size(pars))
          );
        Exp *pars0 = *arr_begin(pars);
        WrERs *rs = expWriter_run(ctx, pars0);
        if (rs->is_error) return rs;
        Type *t = rs->tp;
        return wrERs_mk(type_string(), rs->fun_code, str_f(
          "___built_to_str(%s,%s)",
          type_to_c(t, ctx), rs->code
        ));
      }
      if (str_eq(sym, "type")) {
        if (arr_size(pars) != 1)
          return wrERs_failE(
            ctx, fn->ln, "1 parameter", math_itos(arr_size(pars))
          );
        Exp *pars0 = *arr_begin(pars);
        WrERs *rs = expWriter_run(ctx, pars0);
        if (rs->is_error) return rs;
        Type *t = rs->tp;
        return wrERs_mk(type_string(), rs->fun_code, str_f(
            "((Val)%s)", js_ws(type_to_str(t))
        ));
      }
      if (str_eq(sym, "typeExpand")) {
        if (arr_size(pars) != 1)
          return wrERs_failE(
            ctx, fn->ln, "1 parameter", math_itos(arr_size(pars))
          );
        Exp *pars0 = *arr_begin(pars);
        WrERs *rs = expWriter_run(ctx, pars0);
        if (rs->is_error) return rs;

        Rs *ers = wrCtx_expand(ctx, rs->tp);
        Type *texp = rs_get(ers);
        if (!texp) return wrERs_fail(ctx, pars0->ln, rs_error(ers));

        return wrERs_mk(type_string(), rs->fun_code, str_f(
          "((Val)%s)", js_ws(type_to_str(texp))
        ));
      }
      if (str_eq(sym, "used")) {
        if (arr_size(pars) != 1)
          return wrERs_failE(
            ctx, fn->ln, "1 parameter", math_itos(arr_size(pars))
          );
        Exp *e = *arr_begin(pars);
        if (e->tp != exp_symbol)
          return wrERs_failE(ctx, fn->ln, "symbol", exp_to_str(e));
        char *v = e->value;
        Tp *tp = opt_get(wrCtx_get_symbol(ctx, v));
        if (!tp) return wrERs_fail(ctx, fn->ln, str_f("Symbol '%s' not found", v));
        TypedSym *tps = tp->e1;
        tps->used = TRUE;
        return wrERs_mk(type_bool(), "", "");
      }
    }
    if (
      str_eq(mod, "arr") &&
      !opt_get(wrCtx_get_symbol(ctx, "arr")) &&
      !opt_get(map_get(ctx->imports, "arr"))
    ) {
      if (str_eq(sym, "push") | str_eq(sym, "unshiflt")) {
        if (arr_size(pars) != 2)
          return wrERs_failE(
            ctx, fn->ln, "2 parameters", math_itos(arr_size(pars))
          );
        Exp *e = arr_begin(pars)[1];
        WrERs *rs = expWriter_run(ctx, e);
        if (rs->is_error) return rs;
        Type *etp = rs->tp;
        if ((etp->id[0] == '0' || etp->id[0] == '1') && (
          e->tp != exp_symbol || wrCtx_is_inner_symbol(ctx, e)
        ))
          return wrERs_fail(ctx, e->ln, str_f(
            "%s: Inner functions can not be in container", exp_to_str(e)
          ));
        // Continue as normal function.
      } else if (str_eq(sym, "insert")) {
        if (arr_size(pars) != 3)
          return wrERs_failE(
            ctx, fn->ln, "3 parameters", math_itos(arr_size(pars))
          );
        Exp *e = arr_begin(pars)[2];
        WrERs *rs = expWriter_run(ctx, e);
        if (rs->is_error) return rs;
        Type *etp = rs->tp;
        if ((etp->id[0] == '0' || etp->id[0] == '1') && (
          e->tp != exp_symbol || wrCtx_is_inner_symbol(ctx, e)
        ))
          return wrERs_fail(ctx, e->ln, str_f(
            "%s: Inner functions can not be in container", exp_to_str(e)
          ));
        // Continue as normal function.
      }
    }
    if (
      str_eq(mod, "dic") &&
      !opt_get(wrCtx_get_symbol(ctx, "dic")) &&
      !opt_get(map_get(ctx->imports, "dic"))
    ) {
      if (!is_stat && str_eq(sym, "put"))
        return wrERs_failE(ctx, fn->ln, "function with return", exp_to_str(fn));

      if (str_eq(sym, "put")) {
        if (arr_size(pars) != 3)
          return wrERs_failE(
            ctx, fn->ln, "3 parameters", math_itos(arr_size(pars))
          );
        Exp *e = arr_begin(pars)[2];
        WrERs *rs = expWriter_run(ctx, e);
        if (rs->is_error) return rs;
        Type *etp = rs->tp;
        if ((etp->id[0] == '0' || etp->id[0] == '1') && (
          e->tp != exp_symbol || wrCtx_is_inner_symbol(ctx, e)
        ))
          return wrERs_fail(ctx, e->ln, str_f(
            "%s: Inner functions can not be in container", exp_to_str(e)
          ));
        // Continue as normal function.
      }
    }
    if (
      str_eq(mod, "str") &&
      !opt_get(wrCtx_get_symbol(ctx, "str")) &&
      !opt_get(map_get(ctx->imports, "str"))
    ) {
      if (str_eq(sym, "fmt")) {
        if (arr_size(pars) != 2)
          return wrERs_failE(
            ctx, fn->ln, "2 parameters", math_itos(arr_size(pars))
          );

        return fmt(ctx, *arr_begin(pars), arr_begin(pars)[1]);
      }
    }
  } else if (fn->tp == exp_symbol) {  // built-in functions
    char *sym = fn->value;
    if (str_eq(sym, "throw")) {
      if (arr_size(pars) != 2)
        return wrERs_failE(ctx, fn->ln, "2 parameters",  math_itos(arr_size(pars)));
      Exp *e0 = *arr_begin(pars);
      if (e0->tp != exp_array0)
        return wrERs_failE(ctx, e0->ln, "empty array",  exp_to_str(e0));
      Type *rtp = opt_get((Opt *)e0->value);
      if (!rtp)
        return wrERs_fail(ctx, e0->ln, "Type is missing in '[]'");

      Exp *e = arr_begin(pars)[1];
      WrERs *rs = expWriter_run(ctx, e);
      if (rs->is_error) return rs;
      if (rs->tp != type_string())
        return wrERs_failT(ctx, e->ln, type_string(), e, rs->tp);
      char *tmp = fns_next_id(ctx->md_id);
      // Arr<char>
      Arr *fns = arr_new();
      arr_push(fns, rs->fun_code);
      arr_push(fns, str_f(
        "Val %s(void) {\n"
        "  ___built_throw(%s,%s);\n"
        "  return (Val)0; // fake return \n"
        "}",
        tmp, fns_mk_pos(ctx->md_id, fn->ln), rs->code
      ));
      return wrERs_mk(rtp, fns_fun_code(fns), str_f("%s()", tmp));
    }
  }
  // End of sys special functions.

  WrERs *rs = expWriter_run(ctx, fn);
  if (rs->is_error) return rs;

  Rs *trs = wrCtx_expand(ctx, rs->tp);
  Type *t = rs_get(trs);
  if (!t) return wrERs_fail(ctx, fn->ln, rs_error(trs));

  if (t->id[0] != '1' && t->id[0] != '0')
    return wrERs_failE(ctx, fn->ln, "function", exp_to_str(fn));
  if (t->id[0] == '0' && !is_stat)
    return  wrERs_failE(ctx, fn->ln, "function with return", exp_to_str(fn));
  if (type_fnpars(t) != arr_size(pars))
    return wrERs_failE(
      ctx, ex->ln, str_f("%d paremeters", type_fnpars(t)), exp_to_str(ex)
    );

  // Map<Type>
  Map *generics = map_new();
  // Arr<char>
  Arr *fns = arr_new();
  // Arr<char>
  Arr *cpars = arr_new();
  if (*rs->fun_code) arr_push(fns, rs->fun_code);

  EACH(pars, Exp, p) {
    Type *texpected = type_fptype(t, _i);

    ExpT ptp = p->tp;
    if (
      (ptp == exp_array0 && !opt_get(p->value)) ||
      (ptp == exp_dictionary0 && !opt_get(p->value)) ||
      ptp == exp_function
    ) {
      arr_push(cpars, "");
    } else {
      WrERs *rs = expWriter_run(ctx, p);
      if (rs->is_error) return rs;
      Type *tactual = rs->tp;
      Rs *ors = type_allow(ctx, generics, texpected, tactual);
      int *aok = rs_get(ors);
      if (!aok) return wrERs_fail(ctx, p->ln, rs_error(ors));
      if (!*aok)
        return wrERs_failT(ctx, p->ln,
          type_replace_generics(texpected, generics),
          p, tactual
        );

      arr_push(fns, rs->fun_code);
      arr_push(cpars, rs->code);
    }
  }_EACH

  EACH(pars, Exp, p) {
    Type *texpected = type_fptype(t, _i);

    ExpT ptp = p->tp;
    if (ptp == exp_array0 && !opt_get(p->value)) {
      if (texpected->id[0] != 'a')
        return wrERs_fail(ctx, p->ln, "Unexpected empty array");
      Type *t2 = type_replace_generics(texpected, generics);
      Exp *p2 = exp_new_array0(p->ln, *arr_begin(t2->subtypes));
      WrERs *rs = expWriter_run(ctx, p2);
      if (rs->is_error) return rs;
      arr_set(cpars, _i, rs->code);
    } else if (ptp == exp_dictionary0 && !opt_get(p->value)) {
      if (texpected->id[0] != 'd')
        return wrERs_fail(ctx, p->ln, "Unexpected empty dictionary");
      Type *t2 = type_replace_generics(texpected, generics);
      Exp *p2 = exp_new_dictionary0(p->ln, *arr_begin(t2->subtypes));
      WrERs *rs = expWriter_run(ctx, p2);
      if (rs->is_error) return rs;
      arr_set(cpars, _i, rs->code);
    } else if (ptp == exp_function) {
      Tp3 *tp = p->value;
      Exp *p2 = exp_new_function(p->ln,
        type_replace_generics(texpected, generics), tp->e2, tp->e3
      );

      WrERs *rs = expWriter_run(ctx, p2);
      if (rs->is_error) return rs;
      Type *tactual = rs->tp;
      Rs *ors = type_allow(ctx, generics, texpected, tactual);
      int *aok = rs_get(ors);
      if (!aok) return wrERs_fail(ctx, p->ln, rs_error(ors));
      if (!aok)
        return wrERs_failE(ctx, p->ln,
          str_f("Expression of type %s", type_to_str(texpected)),
          str_f("%s (of type %s)", exp_to_str(p), exp_type_to_str(p))
        );

      arr_push(fns, rs->fun_code);
      arr_set(cpars, _i, rs->code);
    }
  }_EACH

  // Opt<Type>
  Opt *rtype = type_frtype(t);

  return wrERs_mk(
    is_stat ? type_bool() : type_replace_generics(opt_get(rtype), generics),
    fns_fun_code(fns),
    str_f("((%s)(%s).o)(%s)", type_fctype(t), rs->code, arr_cjoin(cpars, ','))
  );
}
