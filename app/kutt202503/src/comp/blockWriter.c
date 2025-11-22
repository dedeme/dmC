// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/blockWriter.h"
#include "kut/buf.h"
#include "kut/js.h"
#include "kut/math.h"
#include "data/typedSym.h"
#include "data/stat.h"
#include "data/tp3.h"
#include "data/tp4.h"
#include "data/tp5.h"
#include "comp/expWriter.h"
#include "fns.h"

static void warn (WrCtx *ctx, int ln, char *msg) {
  char *fmap (char *l) { return str_f("  %s", l); }
  puts(str_f("%s:%d:\n%s",
    ctx->md_path, ln,
    arr_cjoin(arr_map(str_csplit(msg, '\n'), (FMAP)fmap), '\n')
  ));
}

// rtp is Arr<Type> with the return type or empty.
// generics is Map<Type> with de known genercic values
WrSRs *blockWriter_run(
  Arr *rtp, Map *generics, BlockT kind, int is_loop, int is_try,
  WrCtx *ctx, int ln, Arr *sts
) {
  WrSRsStopT stopped = wrSRs_no;
  // Arr<char>
  Arr *bf = arr_new();
  if (kind == blockWriter_inner) {
    dicLayers_add_layer(ctx->sym_types);
  } else if (kind != blockWriter_top) {
    DicLayers *symTypes = ctx->sym_types;
    Map *last_layer = dicLayers_peek_layer(symTypes);
    dicLayers_add_layer(symTypes);
    EACH(last_layer, Kv, kv) {
      dicLayers_put_element(symTypes, kv_key(kv), kv_value(kv));
    }_EACH
  }

  EACH (sts, Stat, st) {
    int ln = st->ln;
    StatT tp = st->tp;
    void *v = st->value;
    if (stopped != wrSRs_no) {
      warn(ctx, ln, "Statement after break-return-throw");
      stopped = wrSRs_no;
    }

    switch (tp) {
      case stat_error: return wrSRs_fail(ctx, ln, v);
      case stat_assign: {
        Tp *tp = v;
        Exp *sym_sq = tp->e1;
        Exp *e = tp->e2;

        if (kind == blockWriter_top) {
          WrERs *rs = expWriter_run(ctx, e);
          if (rs->is_error) return wrSRs_fromWrERsFail(rs);
          Type *t = rs->tp;
          if (sym_sq->tp != exp_symbol)
            return wrSRs_failE(ctx, sym_sq->ln, "symbol", exp_to_str(sym_sq));
          char *sym = sym_sq->value;
          switch (t->id[0]) {
            case 'b':
            case 'i':
            case 'f':
            case 's': {
              Tp *tp = opt_get(wrCtx_get_symbol(ctx, sym));
              if (!tp) EXC_KUTT("Option is none");
              TypedSym *ts = tp->e1;
              char *cval = opt_get(ts->cval);
              if (!cval) EXC_KUTT("Option is none");
              arr_push(bf, str_f("Val %s_%s=(Val)%s;",
                ctx->md_id, sym, cval
              ));
              break;
            }
            case 'a': {
              char *vid = fns_next_id(ctx->md_id);
              char *fid = fns_next_id(ctx->md_id);
              arr_push(bf, str_f("Varr *%s = NULL;", vid));
              arr_push(bf, str_f(
                "Val %s(void) {\n"
                "if (%s) return (Val)%s;\n"
                "%s = __arr_new().a;\n"
                "return (Val)%s;\n"
                "};",
                fid, vid, vid, vid, vid
              ));
              arr_push(bf, str_f("Val %s_%s=(Val)(void*)%s;",
                ctx->md_id, sym, fid
              ));
              break;
            }
            case '0':
            case '1':
            case '(': {
              char *id = rs->code;
              char *fun_code = rs->fun_code;
              int ix = str_index(fun_code, str_f("Val %s=", id));
              int ix2 = str_cindex_from(fun_code, '=', ix);
              char *code = str_f("%sVal %s_%s%s",
                str_left(fun_code, ix), ctx->md_id, sym,
                str_right(fun_code, ix2)
              );
              int ix3 = str_last_cindex(id, '_');
              arr_push(bf, str_replace(code, id, str_f("%s_%s%s",
                str_left(id, ix3), sym, str_right(id, ix3)
              )));
              break;
            }
            default:
              return wrSRs_failE(
                ctx, ln, "constant, empty array or function", exp_to_str(e)
              );
          }
        } else {
          switch (sym_sq->tp) {
            case exp_symbol: {
              char *sym = sym_sq->value;
              DicLayers *sym_types = ctx->sym_types;
              Map *layer = dicLayers_peek_layer(sym_types);
              TypedSym *tps = opt_get(map_get(layer, sym));
              if (tps) {
                return wrSRs_fail(ctx, sym_sq->ln, str_f(
                  "Symbol '%s' is duplicated (first assigned in line %d)",
                  sym, tps->ln
                ));
              }

              // Opt<char>
              Opt *lit;
              switch (e->tp) {
                case exp_boolean:
                case exp_integer:
                case exp_float:
                  lit = opt_some(e->value);
                  break;
                case exp_string:
                  lit = opt_some(js_ws(e->value));
                  break;
                default: lit = opt_none();
              };

              if (e->tp == exp_function) {
                Tp3 *tp = e->value;
                dicLayers_put_element(
                  sym_types,
                  sym,
                  typedSym_new(sym_sq->ln, sym, tp->e1, lit, FALSE)
                );
                WrERs *rs = expWriter_run(ctx, e);
                if (rs->is_error) return wrSRs_fromWrERsFail(rs);

                char *id = rs->code;
                char *fun_code = rs->fun_code;
                int ix = str_index(fun_code, id);
                int ix2 = str_index_from(fun_code, str_f("Val %s=", id), ix);
                char *code = str_f("%s%s%s",
                  str_left(fun_code, ix), sym,
                  str_sub(fun_code, ix + strlen(id), ix2)
                );
                arr_push(bf, str_f("Val %s;", sym));
                arr_push(bf, code);
                arr_push(bf, str_f("%s=(Val)(void*)%s_f;", sym, sym));
              } else {
                WrERs *rs = expWriter_run(ctx, e);
                if (rs->is_error) return wrSRs_fromWrERsFail(rs);
                Type *t = rs->tp;

                dicLayers_put_element(
                  sym_types,
                  sym,
                  typedSym_new(sym_sq->ln, sym, t, lit, FALSE)
                );

                if (*rs->fun_code) arr_push(bf, rs->fun_code);
                arr_push(bf, str_f("Val %s = %s;", sym, rs->code));
              }
              break;
            }
            case exp_square: {
              WrERs *rs = expWriter_run(ctx, e);
              if (rs->is_error) return wrSRs_fromWrERsFail(rs);
              Type *t = rs->tp;

              Tp *tuple = sym_sq->value;
              Exp *e1 = tuple->e1;
              Exp *e2 = tuple->e2;

              WrERs *rs1 =  expWriter_run(ctx, e1);
              if (rs1->is_error) return wrSRs_fromWrERsFail(rs1);
              Rs *trs1 = wrCtx_expand(ctx, rs1->tp);
              Type *t1 = rs_get(trs1);
              if (!t1) return wrSRs_fail(ctx, e1->ln, rs_error(trs1));

              WrERs *rs2 = expWriter_run(ctx, e2);
              if (rs2->is_error) return wrSRs_fromWrERsFail(rs2);
              Type *t2 = rs2->tp;
              if (*rs->fun_code) arr_push(bf, rs->fun_code);
              if (*rs1->fun_code) arr_push(bf, rs1->fun_code);
              if (*rs2->fun_code) arr_push(bf, rs2->fun_code);
              if ((t->id[0] == '0' || t->id[0] == '1') && (
                e->tp != exp_symbol || wrCtx_is_inner_symbol(ctx, e)
              ))
                return wrSRs_fail(ctx, ln, str_f(
                  "%s: Inner functions can not be in container",
                  exp_to_str(sym_sq)
                ));

              switch (t1->id[0]) {
                case 'a': {
                  if (t2->id[0] != 'i')
                    return wrSRs_failE(ctx, e2->ln, "integer", exp_to_str(e2));
                  Type *subt = *arr_begin(t1->subtypes);
                  Rs *okrs = type_allow(ctx, map_new(), subt, t);
                  int *ok = rs_get(okrs);
                  if (!ok) return wrSRs_fail(ctx, e->ln, rs_error(okrs));
                  if (!*ok) return wrSRs_failT(ctx, e->ln, subt, e, t);
                  arr_push(bf, str_f(
                    "__arr_set2(%s,%s,%s,%s);",
                    fns_mk_pos(ctx->md_id, ln), rs1->code, rs2->code, rs->code
                  ));
                  break;
                }
                case 't': {
                  if (t2->id[0] != 'i')
                    return wrSRs_failE(ctx, e2->ln, "integer", exp_to_str(e2));
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
                      if (rs->is_error) return wrSRs_fromWrERsFail(rs);
                      char *code = rs->code; // type ((Val)xxx)
                      ix = str_sub(code, str_cindex(code, ')') + 1, -1);
                      break;
                    }
                    default: {}
                  }
                  if (!*ix)
                    return wrSRs_failE(
                      ctx, ln, "Constant integer", exp_to_str(e1)
                    );

                  int i = math_stoi(ix);
                  if (i >= arr_size(t1->subtypes))
                    return wrSRs_fail(ctx, ln, str_f(
                      "index %d out of [0,%d]", ix, arr_size(t1->subtypes) - 1
                    ));
                  Type *subt = arr_begin(t1->subtypes)[i];
                  Rs *okrs = type_allow(ctx, map_new(), subt, t);
                  int *ok = rs_get(okrs);
                  if (!ok) return wrSRs_fail(ctx, e->ln, rs_error(okrs));
                  if (!*ok) return wrSRs_failT(ctx, e->ln, subt, e, t);
                  arr_push(bf, str_f(
                    "__arr_set(%s,%s,%s);",
                    rs1->code, rs2->code, rs->code
                  ));
                  break;
                }
              case 'd': {
                if (t2->id[0] != 's')
                  return wrSRs_failE(ctx, e2->ln, "string", exp_to_str(e2));
                Type *subt = *arr_begin(t1->subtypes);
                Rs *okrs = type_allow(ctx, map_new(), subt, t);
                int *ok = rs_get(okrs);
                if (!ok) return wrSRs_fail(ctx, e->ln, rs_error(okrs));
                if (!*ok) return wrSRs_failT(ctx, e->ln, subt, e, t);
               arr_push(bf, str_f(
                  "__dic_set(%s,%s,%s,%s);",
                  fns_mk_pos(ctx->md_id, e2->ln), rs1->code, rs2->code, rs->code
                ));
                break;
              }
              default:
                return wrSRs_failE(ctx, e1->ln,
                  "Expression of type Array, tuple or dictionary",
                  str_f("%s (of type %s)", exp_to_str(e1), type_to_str(rs1->tp))
                );
              }
              break;
            }
            default:
              return wrSRs_failE(ctx, sym_sq->ln, "symbol", exp_to_str(sym_sq));
          }
        }
        break;
      }
      case stat_multiassign: {
        Tp *tp = v;
        // Arr<char>
        Arr *syms = tp->e1;
        Exp *tuple = tp->e2;
        WrERs *rs = expWriter_run(ctx, tuple);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        Rs *trs = wrCtx_expand(ctx, rs->tp);
        Type *tt = rs_get(trs);
        if (!tt) return wrSRs_fail(ctx, tuple->ln, rs_error(trs));
        if (tt->id[0] != 't')
          return wrSRs_failE(ctx, tuple->ln,
            "Expression of type tuple",
             str_f("%s (of type %s)", exp_to_str(tuple), type_to_str(tt))
          );
        if (arr_size(tt->subtypes) < arr_size(syms))
          return wrSRs_fail(ctx, tuple->ln,
            str_f("Tuple with number of elements < %d", arr_size(syms))
          );

        if (*rs->fun_code) arr_push(bf, rs->fun_code);

        DicLayers *sym_types = ctx->sym_types;
        Map *layer = dicLayers_peek_layer(sym_types);

        EACH(syms, char, sym) {
          if (!*sym) continue;

          TypedSym *tps = opt_get(map_get(layer, sym));
          if (tps) {
            return wrSRs_fail(ctx, ln, str_f(
              "Symbol '%s' is duplicated (first assigned in line %d)",
              sym, tps->ln
            ));
          }
          arr_push(bf, str_f("Val %s;", sym));
        }_EACH

        arr_push(bf, "{");
        arr_push(bf, str_f("Val *_ptuple = ((%s).a)->begin;", rs->code));
        EACH(syms, char, sym) {
          if (!*sym) continue;

          Type *t = arr_begin(tt->subtypes)[_i];
          dicLayers_put_element(
            sym_types, sym, typedSym_new(ln, sym, t, opt_none(), FALSE)
          );
          arr_push(bf, str_f("%s = _ptuple[%d];", sym, _i));
        }_EACH
        arr_push(bf, "}");
        break;
      }
      case stat_addas:
      case stat_subas:
      case stat_mulas:
      case stat_divas:
      case stat_modas:
      case stat_andas:
      case stat_oras: {
        Tp3 *tp3;
        switch (tp) {
          case stat_addas:
            tp3 = tp3_mk("+", "ifsa", "integer, float, string or array");
            break;
          case stat_subas:
            tp3 = tp3_mk("-", "if", "integer or float");
            break;
          case stat_mulas:
            tp3 = tp3_mk("*", "if", "integer or float");
            break;
          case stat_divas:
            tp3 = tp3_mk("/", "if", "integer or float");
            break;
          case stat_modas:
            tp3 = tp3_mk("%", "i", "integer");
            break;
          case stat_andas:
            tp3 = tp3_mk("&", "b", "boolean");
            break;
          default:
            tp3 = tp3_mk("|", "b", "boolean");
        }
        char *sym = tp3->e1;
        char *tps = tp3->e2;
        char *msg = tp3->e3;

        Tp *tuple = v;
        Exp *e1 = tuple->e1;
        Exp *e2 = tuple->e2;

        WrERs *rs1 =  expWriter_run(ctx, e1);
        if (rs1->is_error) return wrSRs_fromWrERsFail(rs1);
        Rs *trs1 = wrCtx_expand(ctx, rs1->tp);
        Type *t1 = rs_get(trs1);
        if (!t1) return wrSRs_fail(ctx, e1->ln, rs_error(trs1));

        WrERs *rs2 =  expWriter_run(ctx, e2);
        if (rs2->is_error) return wrSRs_fromWrERsFail(rs2);
        Type *t2 = rs2->tp;

        if (str_cindex(tps, t1->id[0]) == -1)
          return wrSRs_failE(ctx, e1->ln,
            str_f("Expression of type %s", msg),
            str_f("%s (of type %s)", exp_to_str(e1), type_to_str(t1))
          );

        if (t1->id[0] != t2->id[0])
          return wrSRs_failT(ctx, e2->ln, t1, e2, t2);

        if (*rs1->fun_code) arr_push(bf, rs1->fun_code);
        if (*rs2->fun_code) arr_push(bf, rs2->fun_code);

        Tp *squareTp = e1->value;
        Exp *ct = squareTp->e1;
        WrERs *rs_ct =  expWriter_run(ctx, ct);
        Type *t_ct0 = rs_ct->tp;
        Rs *t_ct_rs = wrCtx_expand(ctx, t_ct0);
        Type *t_ct = rs_get(t_ct_rs);
        if (!t_ct) return wrSRs_fail(ctx, ct->ln, rs_error(t_ct_rs));

        Exp *ix = squareTp->e2;
        WrERs *rs_ix = expWriter_run(ctx, ix);
        Type *t_ix = rs_ix->tp;
        char *pos = fns_mk_pos(ctx->md_id, ln);

        switch (*t_ct->id) {
          case 'a':
          case 't':
          case 'd': {
            if (*t_ct->id == 'a' || *t_ct->id == 't') {
              if (*t_ix->id != 'i')
                return wrSRs_failT(ctx, ix->ln, type_int(), ix, t_ix);
            } else if (*t_ix->id != 's') {
              return wrSRs_failT(ctx, ix->ln, type_string(), ix, t_ix);
            }
            arr_push(bf, "{");
            arr_push(bf, str_f("Val _ct = %s;", rs_ct->code));
            arr_push(bf, str_f("Val _ix = %s;", rs_ix->code));
            char *old_value;
            if (*t_ct->id == 'a')
              old_value = str_f("__arr_get2(%s,_ct,_ix)", pos);
            else if (*t_ct->id == 't')
              old_value = str_f("__arr_get(_ct,_ix)");
            else
              old_value = str_f("__dic_get2(%s, _ct,_ix)", pos);
            char *new_value = str_f("(%s)", rs2->code);
            char *op;
            switch (t1->id[0]) {
              case 'i':
                op = str_f("(Val)(%s.i %s%s.i)", old_value, sym, new_value);
                break;
              case 'f':
                op = str_f("(Val)(%s.f %s%s.f)", old_value, sym, new_value);
                break;
              case 'b':
                op = str_f("(Val)(%s.b %s%s.b)", old_value, sym, new_value);
                break;
              case 's':
                op = str_f(
                  "(Val)str_f(\"%%s%%s\",%s.s,%s.s)", old_value, new_value
                );
                break;
              case 'a':
                op = str_f("__arr_cat2(%s,%s)", old_value, new_value);
                break;
              default: EXC_KUTT("Unexpected type");
            };
            if (*t_ct->id == 'a')
              arr_push(bf, str_f(
                "__arr_set2(%s,_ct,_ix,%s);", pos, op
              ));
            else if (*t_ct->id == 't')
              arr_push(bf, str_f(
                "__arr_set(_ct,_ix,%s);", op
              ));
            else
              arr_push(bf, str_f(
                "__dic_set(%s,_ct,_ix,%s);", pos, op
              ));

            arr_push(bf, "}");
            break;
          }
          default:
            return wrSRs_failE(ctx, ct->ln,
              "Expression of type array, tuple or dictionary",
              str_f("%s (of type %s)", exp_to_str(ct), type_to_str(t_ct))
            );
        }
        break;
      }
      case stat_function: {
        WrERs *rs = expWriter_parenthesis(ctx, v, TRUE);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        if (*rs->fun_code) arr_push(bf, rs->fun_code);
        arr_push(bf, str_f("%s;", rs->code));
        break;
      }
      case stat_block: {
        WrSRs *rs = blockWriter_run(
          rtp, generics, blockWriter_inner, is_loop, is_try, ctx, ln, v
        );
        if (rs->is_error) return rs;

        if (rs->stopped != wrSRs_no && _i < arr_size(sts) - 1)
          warn(ctx, ln, "There are spare statements after block-breaker");

        arr_push(bf, str_f("%s%s",
          kind == blockWriter_top
            ? str_f("void %s_main() ", ctx->md_id)
            : "",
          rs->code
        ));

        stopped = rs->stopped;
        break;
      }
      case stat_break:
      case stat_continue: {
        char *tx = tp == stat_break ? "break" : "continue";
        if (!is_loop)
          return wrSRs_fail(ctx, ln, str_f("'%s' out of loop", tx));

        if (is_try) arr_push(bf, "exc_remove();");
        arr_push(bf, str_f("%s;", tx));

        if (_i < arr_size(sts) - 1)
          warn(ctx, ln, str_f("There are spare statements after '%s'", tx));
        stopped = wrSRs_by_break;
        break;
      }
      case stat_return: {
        Exp *e = opt_get(v);
        if (!e) {
          if (arr_size(rtp)) {
            return wrSRs_failE(ctx, ln,
              str_f("return type %s", type_to_str(*arr_begin(rtp))),
              "no return"
            );
          }
          if (is_try) arr_push(bf, "exc_remove();");
          arr_push(bf, "return;");
        } else {
          if (kind == blockWriter_top || !arr_size(rtp))
            return wrSRs_failE(ctx, ln, "return nothing", exp_to_str(e));

          Type *t = *arr_begin(rtp);
          if (
            e->tp != exp_point &&
            (e->tp != exp_symbol || wrCtx_is_inner_symbol(ctx, e))
          ) {
            if (t->id[0] == '0' || t->id[0] == '1')
              return wrSRs_fail(ctx, ln, "Inner functions can not be returned");
          }

          switch (e->tp) {
            case exp_array0: {
              if (t->id[0] == 'a' && !opt_get((Opt *)e->value))
                e = exp_new_array0(e->ln, opt_some(*arr_begin(t->subtypes)));
            }
            case exp_dictionary0: {
              if (t->id[0] == 'd' && !opt_get((Opt *)e->value))
                e = exp_new_dictionary0(e->ln, opt_some(*arr_begin(t->subtypes)));
            }
            default: {}
          };

          WrERs *rs = expWriter_run(ctx, e);
          if (rs->is_error) return wrSRs_fromWrERsFail(rs);
          Rs *okrs = type_allow(ctx, generics, t, rs->tp);
          int *ok = rs_get(okrs);
          if (!ok) return wrSRs_fail(ctx, ln, rs_error(okrs));
          if (!*ok) return wrSRs_failT(ctx, ln, t, e, rs->tp);
          arr_set(rtp, 0, rs->tp);
          if (*rs->fun_code) arr_push(bf, rs->fun_code);
          if (is_try) arr_push(bf, "exc_remove();");
          arr_push(bf, str_f("return %s;", rs->code));
        }
        if (_i < arr_size(sts) - 1)
          warn(ctx, ln, "There are spare statements after 'return'");
        stopped = wrSRs_by_return;
        break;
      }
      case stat_throw: {
        Exp *e = v;
        WrERs *rs =  expWriter_run(ctx, e);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        Type *t = rs->tp;
        if (t->id[0] != 's')
          return wrSRs_failT(ctx, e->ln, type_string(), e, rs->tp);

        if (*rs->fun_code) arr_push(bf, rs->fun_code);
        arr_push(bf, str_f(
          "___built_throw(%s,%s);", fns_mk_pos(ctx->md_id, ln), rs->code
        ));
        stopped = wrSRs_by_return;
        break;
      }
      case stat_trace: {
        Tp *tp = v;
        WrERs *rs = expWriter_run(ctx, tp->e2);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        Type *t = rs->tp;
        if (*rs->fun_code) arr_push(bf, rs->fun_code);
        arr_push(bf, str_f(
          "___built_trace((Val)%s,%s,%s,%s);",
          *((int *)tp->e1) ? "1" : "0", fns_mk_pos(ctx->md_id, ln),
          type_to_c(t, ctx), rs->code
        ));
        break;
      }
      case stat_while: {
        Tp *tp = v;
        Exp *e = opt_get(tp->e1);
        Stat *st = tp->e2; // stat type block
        char *cond = "(Val)1";
        if (e) {
          WrERs *rs =  expWriter_run(ctx, e);
          if (rs->is_error) return wrSRs_fromWrERsFail(rs);
          Type *t = rs->tp;
          if (t->id[0] != 'b') return wrSRs_failT(ctx, e->ln, type_bool(), e, t);
          if (*rs->fun_code) arr_push(bf, rs->fun_code);
          cond = rs->code;
        }
        WrSRs *st_rs = blockWriter_run(
          rtp, generics, blockWriter_inner, TRUE, is_try, ctx, st->ln, st->value
        );
        if (st_rs->is_error) return st_rs;
        arr_push(bf, str_f("while ((%s).b)\n%s", cond, st_rs->code));
        break;
      }
      case stat_if: { // Statements are block ones.
        Tp3 *tp = v;
        Exp *e = tp->e1;
        Stat *st1 = tp->e2; // stat type block
        Stat *st2 = opt_get((Opt *)tp->e3); // stat type block
        WrERs *rs =  expWriter_run(ctx, e);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        Type *t = rs->tp;
        if (t->id[0] != 'b') return wrSRs_failT(ctx, e->ln, type_bool(), e, t);
        if (*rs->fun_code) arr_push(bf, rs->fun_code);

        WrSRs *st1rs = blockWriter_run(
          rtp, generics, blockWriter_inner, is_loop, is_try, ctx,
          st1->ln, st1->value
        );
        if (st1rs->is_error) return st1rs;
        char *code1 = str_f("if ((%s).b)\n%s", rs->code, st1rs->code);
        if (!st2) {
          arr_push(bf, code1);
        } else {
          WrSRs *st2rs = blockWriter_run(
            rtp, generics, blockWriter_inner, is_loop, is_try, ctx,
            st2->ln, st2->value
          );
          if (st2rs->is_error) return st2rs;
          char *code2 = str_f("%s\nelse %s", code1, st2rs->code);
          stopped = st1rs->stopped == wrSRs_no
            ? wrSRs_no
            : st1rs->stopped == wrSRs_by_break
              ? st2rs->stopped == wrSRs_no
                ? wrSRs_no
                : wrSRs_by_break
              : st2rs->stopped
          ;
          arr_push(bf, code2);
        }
        break;
      }
      case stat_for: {
        Tp4 *tuple = v;
        char *ix = opt_get(tuple->e1);
        char *e = tuple->e2;
        Exp *ct = tuple->e3;
        Stat *st = tuple->e4; // stat type block

        WrERs *rs =  expWriter_run(ctx, ct);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        Type *t_ct = rs->tp;
        switch (*t_ct->id) {
          case 'a':
          case 'd':
          case 'r':
            break; // Ok
          default: return wrSRs_failE(ctx, ct->ln,
            "Array, dictionary or iterator",
             str_f("%s (of type %s)", exp_to_str(ct), type_to_str(t_ct))
          );
        }

        Type *t = *arr_begin(t_ct->subtypes);
        DicLayers *sym_types = ctx->sym_types;
        dicLayers_add_layer(sym_types);
        if (ix)
          dicLayers_put_element(
            sym_types, ix, typedSym_new(ln, ix,
            *t_ct->id == 'd' ? type_string() : type_int(),
            opt_none(), TRUE
          ));
        dicLayers_put_element(sym_types, e, typedSym_new(ln, e, t, opt_none(), TRUE));
        WrSRs *st_rs = blockWriter_run(
          rtp, generics, blockWriter_param, TRUE, is_try, ctx, st->ln, st->value
        );
        if (st_rs->is_error) return st_rs;
        dicLayers_remove_layer(sym_types);

        if (*rs->fun_code) arr_push(bf, rs->fun_code);
        switch (*t_ct->id) {
          case 'a': {
            arr_push(bf, "{\nint64_t _i = 0;");
            arr_push(bf, str_f("Varr *_a = (%s).a;", rs->code));
            arr_push(bf, "Val *_p = _a->begin;");
            arr_push(bf, "Val *_end = _a->end;");
            arr_push(bf, "while (_p < _end) {");
            arr_push(bf, str_f("Val %s = *_p++;", e));
            if (ix) arr_push(bf, str_f("Val %s = (Val)_i++;", ix));
            arr_push(bf, st_rs->code);
            arr_push(bf, "}}");
            break;
          }
          case 'd': {
            arr_push(bf, "{\nint64_t _i = 0;");
            arr_push(bf, str_f("Varr *_a = (%s).a;", rs->code));
            arr_push(bf, "Val *_p = _a->begin;");
            arr_push(bf, "Val *_end = _a->end;");
            arr_push(bf, "while (_p < _end) {");
            arr_push(bf, "Varr *_kv = (*_p++).a;");
            arr_push(bf, str_f("Val %s = _kv->begin[1];", e));
            if (ix) arr_push(bf, str_f("Val %s = *_kv->begin;", ix));
            arr_push(bf, st_rs->code);
            arr_push(bf, "}}");
            break;
          }
          case 'r':
            arr_push(bf, "{\nint64_t _i = 0;");
            arr_push(bf, str_f("Val _it = (%s);", rs->code));
            arr_push(bf, "while (__iter_hasNext(_it).b) {");
            arr_push(bf, str_f("Val %s = __iter_next(_it);", e));
            if (ix) arr_push(bf, str_f("Val %s = (Val)_i++;", ix));
            arr_push(bf, st_rs->code);
            arr_push(bf, "}}");
            break;
        }

        break;
      }
      case stat_for_range: {
        Tp5 *tuple = v;
        char *var = tuple->e1;
        Exp *start = tuple->e2;
        Exp *end = tuple->e3;
        Exp *step = opt_get(tuple->e4);
        Stat *st =tuple->e5;

        WrERs *rs_start =  expWriter_run(ctx, start);
        if (rs_start->is_error) return wrSRs_fromWrERsFail(rs_start);
        if (rs_start->tp != type_int())
          return wrSRs_failT(ctx, start->ln, type_int(), start, rs_start->tp);
        if (*rs_start->fun_code) arr_push(bf, rs_start->fun_code);

        WrERs *rs_end =  expWriter_run(ctx, end);
        if (rs_end->is_error) return wrSRs_fromWrERsFail(rs_end);
        if (rs_end->tp != type_int())
          return wrSRs_failT(ctx, end->ln, type_int(), end, rs_end->tp);
        if (*rs_end->fun_code) arr_push(bf, rs_end->fun_code);

        WrERs *rs_step = NULL;
        if (step) {
          rs_step = expWriter_run(ctx, step);
          if (rs_step->is_error) return wrSRs_fromWrERsFail(rs_step);
          if (rs_step->tp != type_int())
            return wrSRs_failT(ctx, step->ln, type_int(), step, rs_step->tp);
          if (*rs_step->fun_code) arr_push(bf, rs_step->fun_code);
        }

        DicLayers *sym_types = ctx->sym_types;
        dicLayers_add_layer(sym_types);
        dicLayers_put_element(
          sym_types, var, typedSym_new(ln, var, type_int(), opt_none(), TRUE)
        );
        WrSRs *st_rs = blockWriter_run(
          rtp, generics, blockWriter_param, TRUE, is_try, ctx, st->ln, st->value
        );
        if (st_rs->is_error) return st_rs;
        dicLayers_remove_layer(sym_types);

        arr_push(bf, "{");
        arr_push(bf, str_f("int64_t _start = (%s).i;", rs_start->code));
        arr_push(bf, str_f("int64_t _end = (%s).i;", rs_end->code));
        if (!rs_step) {
          arr_push(bf, "for (int64_t _i = _start; _i < _end; ++_i){");
        } else {
          char *pos = fns_mk_pos(ctx->md_id, ln);
          arr_push(bf, str_f("int64_t _step = (%s).i;", rs_step->code));
          arr_push(bf, str_f(
            "if (_step == 0) ___built_throw(%s, (Val) \"'for' step is 0\");",
            pos
          ));
          arr_push(bf,
            "for (int64_t _i = _start; "
            "_step > 0 ? _i <= _end : _i >= _end"
            "; _i += _step){"
          );
        }

        arr_push(bf, str_f("Val %s = (Val)_i;", var));
        arr_push(bf, st_rs->code);
        arr_push(bf, "}}");
        break;
      }
      case stat_try: {
        Tp4 *tuple = v;
        Stat *st = tuple->e1;
        char *var = tuple->e2;
        Stat *st_catch = tuple->e3;

        WrSRs *st_rs = blockWriter_run(
          rtp, generics, blockWriter_inner, is_loop, TRUE, ctx, st->ln, st->value
        );
        if (st_rs->is_error) return st_rs;
        char *st_c = st_rs->code;

        DicLayers *sym_types = ctx->sym_types;
        dicLayers_add_layer(sym_types);
        dicLayers_put_element(
          sym_types, var,
          typedSym_new(st_catch->ln, var, type_string(), opt_none(), TRUE)
        );
        WrSRs *st_catch_rs = blockWriter_run(
          rtp, generics, blockWriter_param, is_loop, is_try, ctx,
          st_catch->ln, st_catch->value
        );
        dicLayers_remove_layer(sym_types);

        if (st_catch_rs->is_error) return st_catch_rs;
        char *st_catch_c = st_catch_rs->code;

        stopped =
          st_rs->stopped == wrSRs_no || st_catch_rs->stopped == wrSRs_no
          ? wrSRs_no
          : st_rs->stopped == wrSRs_by_break || st_catch_rs->stopped == wrSRs_by_break
            ? wrSRs_by_break
            : wrSRs_by_return
        ;

        arr_push(bf, "{");
        arr_push(bf,
            "jmp_buf *__TRY_buf = MALLOC(jmp_buf);"
            "exc_add(__TRY_buf);"
            "int __TRY_r = setjmp(*__TRY_buf);"
            "if (__TRY_r == 0) {"
        );
        arr_push(bf, st_c);
        arr_push(bf,
            "  exc_remove();"
            "} else {"
        );
        arr_push(bf, str_f("Val %s = (Val)exc_msg(exc_get());", var));
        arr_push(bf, st_catch_c);
        arr_push(bf, "}");
        arr_push(bf, "}");
        break;
      }
      case stat_switch: {
        Tp *tp = v;
        Exp *var = tp->e1;
        // Arr<Tp<Arr<Exp>, Stat>>
        Arr *entries = arr_copy(tp->e2);

        WrERs *rs =  expWriter_run(ctx, var);
        if (rs->is_error) return wrSRs_fromWrERsFail(rs);
        Type *tc = rs->tp;
        char *tmp = fns_next_id(ctx->md_id);

        int default_code = FALSE;
        // Tp<Arr<Exp>, Stat>
        Tp *last_entry = arr_peek(entries);
        Stat *lasts = last_entry->e2;

        // Arr<Exp>
        Arr *last_cs = last_entry->e1;
        if (arr_size(last_cs) == 1) {
          Exp *c = *arr_begin(last_cs);
          if (c->tp == exp_symbol && str_eq(c->value, "default")) {
            arr_pop(entries);
            default_code = TRUE;
          }
        }

        // Arr<char>
        Arr *funs = arr_new();
        if (*rs->fun_code) arr_push(funs, rs->fun_code);
        arr_push(funs, str_f("Val %s = %s;", tmp, rs->code));
        // Arr<char>
        Arr *code = arr_new();
        stopped = wrSRs_by_return;
        EACH(entries, Tp, entry) { // tp is Tp<Arr<Exp>, Stat>
          // Arr<Exp>
          Arr *cs = entry->e1;
          // Arr<char>
          Arr *cs_code = arr_new();
          EACH(cs, Exp, c) {
            WrERs *crs =  expWriter_run(ctx, c);
            if (crs->is_error) return wrSRs_fromWrERsFail(crs);
            if (*crs->fun_code) arr_push(funs, crs->fun_code);
            Rs *okrs = type_eq(ctx, tc, crs->tp);
            int *ok = rs_get(okrs);
            if (!ok) return wrSRs_fail(ctx, c->ln, rs_error(okrs));
            if (!*ok) return wrSRs_failT(ctx, c->ln, tc, c, crs->tp);

            arr_push(cs_code, str_f(
              "___built_eq(%s,%s,%s).b", type_to_c(tc, ctx), tmp, crs->code
            ));
          }_EACH
          Stat *st = entry->e2;
          WrSRs *srs = blockWriter_run(
            rtp, generics, blockWriter_inner, is_loop, is_try, ctx,
            st->ln, st->value);
          if (srs->is_error) return srs;

          stopped =
            stopped == wrSRs_no || srs->stopped == wrSRs_no
            ? wrSRs_no
            : stopped == wrSRs_by_break || srs->stopped == wrSRs_by_break
              ? wrSRs_by_break
              : wrSRs_by_return
          ;
          arr_push(code, str_f("if (%s) %s", arr_join(cs_code, "||"), srs->code));
        }_EACH

        if (default_code) {
          WrSRs *srs = blockWriter_run(
            rtp, generics, blockWriter_inner, is_loop, is_try, ctx,
            lasts->ln, lasts->value);
          if (srs->is_error) return srs;
          stopped =
            stopped == wrSRs_no || srs->stopped == wrSRs_no
            ? wrSRs_no
            : stopped == wrSRs_by_break || srs->stopped == wrSRs_by_break
              ? wrSRs_by_break
              : wrSRs_by_return
          ;
          arr_push(code, srs->code);
        } else {
          stopped = wrSRs_no;
        }
        arr_push(bf,str_f(
          "%s\n%s", fns_fun_code(funs), arr_join(code, "else\n")
        ));
        break;
      }
      default: EXC_KUTT(str_f("Unexpected statement:\n%s", stat_to_str(st)));
    }
  }_EACH

  if (kind != blockWriter_top) {
    EACH(dicLayers_peek_layer(ctx->sym_types), Kv, kv) {
      TypedSym *v = kv_value(kv);
      if (!v->used) warn(ctx, v->ln, str_f("Symbol '%s' not used", v->id));
    }_EACH

    dicLayers_remove_layer(ctx->sym_types);
    arr_insert(bf, 0, "{\n");
    arr_push(bf, "}\n");

    if (
      stopped != wrSRs_by_return &&
      kind == blockWriter_function &&
      arr_size(rtp)
    ) {
      return wrSRs_failE(ctx, ln,
        str_f("Return of type %s" , type_to_str(*arr_begin(rtp))),
        "Possible no return at end of block"
      );
    }

    return wrSRs_mk(stopped, arr_cjoin(bf, '\n'));
  }

  return wrSRs_mk(stopped, arr_cjoin(bf, '\n'));
}
