// Copyright 22-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "typed/treader.h"
#include "DEFS.h"
#include "kut/rs.h"
#include "kut/js.h"
#include "typed/tfail.h"
#include "typed/theap.h"
#include "typed/texRead.h"
#include "typed/tfunction.h"
#include "typed/tbmodules.h"
#include "runner/fail.h"
#include "module.h"
#include "modules.h"
#include "symix.h"
#include "fileix.h"

typedef enum stopType {
  BREAK, CONTINUE, WITHOUT_STOP
} StopType;

static int read_single_stat (
  Buf *bf, int is_loop, int n_try_rt, int n_try_loop,
  Ttype tret, Theap *th, StatCode *st_cd
);
static int read_block (
  Buf *bf, int is_loop, int n_try_rt, int n_try_loop,
  int without_header, Ttype tret, Theap *th, Arr *sts
);

static int read_assign_xxx (
  Buf *bf, Theap *th, StatCode *st_cd, Tp *left_right, char op
) {
  Exp *left = tp_e1(left_right);
  Exp *right = tp_e2(left_right);
  if (exp_is_sq(left)) {
    Tp *tp2 = exp_get_sq(left);
    Exp *ct = tp_e1(tp2);
    Exp *ix = tp_e2(tp2);
    Buf *bf2 = buf_new();
    Ttype tc = tex_read(bf2, th, st_cd, ct);
    if (op == '+') {
      tfail_check_etype(st_cd, ct, tc, 4, (Ttype[]) {
        ttype_AINT, ttype_AFLOAT, ttype_ASTRING, ttype_DIC
      });
    } else {
      tfail_check_etype(st_cd, ct, tc, 3, (Ttype[]) {
        ttype_AINT, ttype_AFLOAT, ttype_ASTRING
      });
    }
    if (tc == ttype_DIC) {
      Buf *bf3 = buf_new();
      Ttype tix = tex_read(bf3, th, st_cd, ix);
      tfail_check_etype1(st_cd, ix, tix, ttype_STRING);
      Buf *bf4 = buf_new();
      Ttype tright = tex_read(bf4, th, st_cd, right);
      tfail_check_etype1(st_cd, right, tright, ttype_STRING);

      buf_add(bf, str_f(
        "dic_set(%s, %s, str_f(\"%%s%%s\", dic_get_or_exc(%s, %s), %s));\n",
        buf_str(bf2), buf_str(bf3),
        buf_str(bf2), buf_str(bf3), buf_str(bf4)
      ));
    } else {
      Buf *bf3 = buf_new();
      Ttype tix = tex_read(bf3, th, st_cd, ix);
      tfail_check_etype1(st_cd, ix, tix, ttype_INT);
      Buf *bf4 = buf_new();
      Ttype tright = tex_read(bf4, th, st_cd, right);
      switch (tc) {
        case ttype_AINT:
          tfail_check_etype1(st_cd, right, tright, ttype_INT);
          break;
        case ttype_AFLOAT:
          tfail_check_etype1(st_cd, right, tright, ttype_FLOAT);
          break;
        default:
          tfail_check_etype1(st_cd, right, tright, ttype_STRING);
      }

      if (tc == ttype_ASTRING) {
        if (op != '+')
          tfail_throw(st_cd,
            str_f("Operator '%c' is not valid for string", op)
          );
        buf_add(bf, str_f(
          "tarr_setS(%s, %s, str_f(\"%%s%%s\", tarr_getS(%s, %s), %s));\n",
          buf_str(bf2), buf_str(bf3),
          buf_str(bf2), buf_str(bf3), buf_str(bf4)
        ));
      } else
        buf_add(bf, str_f(
          "tarr_set%s(%s, %s, tarr_get%s(%s, %s) %c %s);\n",
          ttype_to_str(tc), buf_str(bf2), buf_str(bf3),
          ttype_to_str(tc), buf_str(bf2), buf_str(bf3), op, buf_str(bf4)
        ));
    }
  } else if (exp_is_pt(left)) {
    Tp *tp2 = exp_get_pt(left);
    Exp *ct = tp_e1(tp2);
    Exp *ix = tp_e2(tp2);
    Buf *bf2 = buf_new();
    Ttype tc = tex_read(bf2, th, st_cd, ct);
    tfail_check_etype1(st_cd, ct, tc, ttype_DIC);
    if (op != '+')
      tfail_throw(st_cd,
        str_f("Operator '%c' is not valid for dictionary", op)
      );
    char *k = symix_get(exp_get_sym(ix));
    Buf *bf4 = buf_new();
    Ttype tright = tex_read(bf4, th, st_cd, right);
    tfail_check_etype1(st_cd, right, tright, ttype_STRING);

    buf_add(bf, str_f(
      "dic_set(%s, \"%s\", str_f(\"%%s%%s\", dic_get_or_exc(%s, \"%s\"), %s));\n",
      buf_str(bf2), k, buf_str(bf2), k, buf_str(bf4)
    ));
  } else {
    tfail_throw(st_cd, fail_type("array or dictionary", left));
  }
  return FALSE;
}

/// sts is Arr<StatCode>
static int read_block (
  Buf *bf, int is_loop, int n_try_rt, int n_try_loop,
  int without_header, Ttype tret, Theap *th, Arr *sts
) {
  if (without_header) {
    buf_add(bf, "{\n");
    theap_add_separator(th);
  }
  int with_return = FALSE;
  EACH(sts, StatCode, stat) {
    int r = read_single_stat(bf, is_loop, n_try_rt, n_try_loop, tret, th, stat);
    with_return = with_return || r;
  }_EACH
  TheapEntry *e = opt_get(theap_check_used(th));
  if (e)
    tfail_throw(e->st, str_f("Symbol not used '%s'", symix_get(e->sym)));

  theap_remove_block(th);
  buf_add(bf, "}\n");
  return with_return;
}

static int read_single_stat (
  Buf *bf, int is_loop, int n_try_rt, int n_try_loop,
  Ttype tret, Theap *th, StatCode *st_cd
) {
  Stat *st = stat_code_stat(st_cd);

  // --------------------------------------------------------------------- block
  if (stat_is_block(st)) {
    return read_block(
      bf, is_loop, n_try_rt, n_try_loop, TRUE, tret, th, stat_get_block(st)
    );
  // --------------------------------------------------------------------- break
  } else if (stat_is_break(st)) {
    if (!is_loop) tfail_throw(st_cd, "Break out of loop");
    else {
      REPEAT(n_try_loop) {
        buf_add(bf, "texc_remove();\n");
      }_REPEAT
      buf_add(bf,"break;\n");
    }
  // ------------------------------------------------------------------ continue
  } else if (stat_is_continue(st)) {
    if (!is_loop) tfail_throw(st_cd, "Continue out of loop");
    else {
      REPEAT(n_try_loop) {
        buf_add(bf, "texc_remove();\n");
      }_REPEAT
      buf_add(bf, "continue;\n");
    }
  // -------------------------------------------------------------------- return
  } else if (stat_is_return(st)) {
    Exp *v = stat_get_return(st);
    if (exp_is_empty(v)) {
      if (tret != ttype_NO_RETURN)
        tfail_throw(st_cd,
          "Expected not return value, buf it was found\n%s",
          exp_to_js(v)
        );
      REPEAT(n_try_rt) {
        buf_add(bf, "texc_remove();\n");
      }_REPEAT
      buf_add(bf, "return;\n");
    } else {
      Buf* bf2 = buf_new();
      Ttype t = tex_read(bf2, th, st_cd, v);
      tfail_check_etype1(st_cd, v, t, tret);
      char *gsym = symix_get(symix_generate());
      buf_add(bf, str_f("Tval %s_ret;\n", gsym));
      switch (tret) {
        case ttype_BOOL:
          buf_add(bf, str_f("%s_ret.b = %s;\n", gsym, buf_str(bf2)));
          break;
        case ttype_INT:
          buf_add(bf, str_f("%s_ret.i = %s;\n", gsym, buf_str(bf2)));
          break;
        case ttype_FLOAT:
          buf_add(bf, str_f("%s_ret.f = %s;\n", gsym, buf_str(bf2)));
          break;
        case ttype_STRING:
          buf_add(bf, str_f("%s_ret.s = %s;\n", gsym, buf_str(bf2)));
          break;
        case ttype_AINT:
          buf_add(bf, str_f("%s_ret.I = %s;\n", gsym, buf_str(bf2)));
          break;
        case ttype_AFLOAT:
          buf_add(bf, str_f("%s_ret.F = %s;\n", gsym, buf_str(bf2)));
          break;
        default:
          buf_add(bf, str_f("%s_ret.S = %s;\n", gsym, buf_str(bf2)));
          break;
      }
      REPEAT(n_try_rt) {
        buf_add(bf, "texc_remove();\n");
      }_REPEAT
      buf_add(bf, str_f("return %s_ret;\n", gsym));
    }
    return TRUE;
  // --------------------------------------------------------------------- trace
  } else if (stat_is_trace(st)) {
    // <int, Exp>
    Tp *tp = stat_get_trace(st);
    Buf* bf2 = buf_new();
    Ttype t = tex_read(bf2, th, st_cd, tp_e2(tp));
    char *to_str_code = str_f("built_to_str%s", ttype_to_str(t));
    buf_add(bf, str_f(
      "built_trace(%d, \"%s\", %s(%s));\n",
      *(int*)tp_e1(tp), tfail_position(st_cd), to_str_code, buf_str(bf2)
    ));
  // ---------------------------------------------------------------------- func
  } else if (stat_is_func(st)) {
    // <Exp, Arr<Exp>>
    Tp *tp = exp_get_pr(stat_get_func(st));
    Exp *fn = tp_e1(tp);
    // <Exp>
    Arr *pars = tp_e2(tp);
    int call_npars = arr_size(pars);
    Ttype *call_tpars = ATOMIC(sizeof(Ttype) * call_npars);
    // <char>
    Arr *apars_code = arr_new();
    Arr *apars_bcode = arr_new();
    Ttype *pcall_tpars = call_tpars;
    EACH(pars, Exp, e) {
      Buf *bf2 = buf_new();
      Ttype t = tex_read(bf2, th, st_cd, e);
      *pcall_tpars++ = t;
      char *v = str_new(buf_str(bf2));
      arr_push(apars_code, str_f("(Tval)%s", v));
      arr_push(apars_bcode, v);
    }_EACH
    char *pars_code = str_f("(Tval[]){%s}", arr_join(apars_code, ","));
    char *pars_bcode = arr_join(apars_bcode, ",");
    char *call_type = ttype_group_to_str(
      iarr_new(call_npars, (int *)call_tpars)
    );

    Module *md = opt_eget(modules_get_ok(stat_code_file_ix(st_cd)));
    if (exp_is_sym(fn)) {
      int fn_sym = exp_get_sym(fn);
      char *fn_str = symix_get(fn_sym);
      Heap0Entry *e = opt_get(heap0_get(module_get_heap0(md), fn_sym));
      if (e) {
        Exp *eexp = heap0_entry_exp(e);
        if (exp_is_tfunction(eexp)) {
          Tfunction *tfn = exp_get_tfunction(eexp);
          char *fn_type = ttype_group_to_str(tfunction_get_tpars(tfn));
          if (strcmp(call_type, fn_type))
            tfail_throw(st_cd,
              "Tfunction type '%s' called with arguments of type '%s'",
              fn_type, call_type
            );
          buf_add(bf, str_f("%s(%s);\n", tfunction_get_cid(tfn), pars_code));
        } else {
          tfail_throw(st_cd, "'%s' is not a typed function", fn_str);
        }
      } else {
        tfail_throw(st_cd, "Symbol '%s' not found", fn_str);
      }
    } else if (exp_is_pt(fn)) {
      // <Exp, Exp>
      Tp *tp2 = exp_get_pt(fn);
      if (exp_is_sym(tp_e1(tp2)) && exp_is_sym(tp_e2(tp2))) {
        int mod_sym = exp_get_sym(tp_e1(tp2));
        char *mod_id = symix_get(mod_sym);
        int fn_sym = exp_get_sym(tp_e2(tp2));
        char *fn_id = symix_get(fn_sym);
        char *fn_str = str_f("%s.%s", mod_id, fn_id);

        int fix2 = imports_get_fix(module_get_imports(md), mod_sym);
        if (fix2 != -1) {
          Module *md2 = opt_get(modules_get_ok(fix2));
          if (!md2)
            EXC_KUT(str_f("Internal error: Module %s not found", mod_id));
          Heap0Entry *e = opt_get(heap0_get(module_get_heap0(md2), fn_sym));
          if (e) {
            Exp *eexp = heap0_entry_exp(e);
            if (exp_is_tfunction(eexp)) {
              Tfunction *tfn = exp_get_tfunction(eexp);
              char *fn_type = ttype_group_to_str(tfunction_get_tpars(tfn));
              if (strcmp(call_type, fn_type))
                tfail_throw(st_cd,
                  "Tfunction type '%s' called with arguments of type '%s'",
                  fn_type, call_type
                );
              buf_add(bf, str_f("%s(%s);\n", tfunction_get_cid(tfn), pars_code));
            } else {
              tfail_throw(st_cd, "'%s' is not a typed function", fn_str);
            }
          } else {
            tfail_throw(st_cd, "Symbol '%s' not found", fn_str);
          }
        } else {
          TbmodRs *md_rs = tbmodules_get(
            mod_id, fn_id, FALSE, call_type, pars_bcode
          );
          if (md_rs->tret == ttype_ERROR) tfail_throw(st_cd, md_rs->code);
          char *code = md_rs->code;
          if (!strcmp(mod_id, "sys")) {
            if (
              !strcmp(fn_id, "test") || !strcmp(fn_id, "assert") ||
              !strcmp(fn_id, "throw")
            ) {
              code = str_replace(
                code, "@#@", str_f("\"%s\"", tfail_position(st_cd))
              );
            }
          }
          buf_add(bf, str_f("%s;\n", code));
        }
      } else {
        tfail_throw(st_cd,
          "Expected expression type fn(...) or module.fn(...), "
          "but it was found:\n  %s", exp_to_js(fn)
        );
      }
    } else {
      tfail_throw(st_cd,
        "Expected expression type fn(...) or module.fn(...), "
        "but it was found:\n  %s", exp_to_js(fn)
      );
    }
  // -------------------------------------------------------------------- assign
  } else if (stat_is_assign(st)) {
    // <Exp, Exp>
    Tp *tp = stat_get_assign(st);
    Buf* bf2 = buf_new();
    Exp *val = tp_e2(tp);
    Ttype t = tex_read(bf2, th, st_cd, val);
    Exp *left = tp_e1(tp);
    if (exp_is_sym(left)) {
      int sym = exp_get_sym(left);
      char *ssym = symix_get(sym);
      if (!theap_add(th, sym, t, st_cd))
        tfail_throw(st_cd, str_f("Reassignation of symbol '%s'", ssym));
      if (exp_is_sym(val) && exp_get_sym(val) == sym) {
        char *tmp = symix_get(symix_generate());
        buf_add(bf, str_f(
          "%s %s = %s;\n"
          "%s %s = %s;\n",
          ttype_to_c(t), tmp, buf_str(bf2),
          ttype_to_c(t), ssym, tmp
        ));
      } else {
        buf_add(bf, str_f("%s %s = %s;\n", ttype_to_c(t), ssym, buf_str(bf2)));
      }
      return FALSE;
    }
    if (exp_is_sq(left)) {
      // <Exp, Exp>
      Tp *v2 = exp_get_sq(left);
      Buf *bf3 = buf_new();
      Exp *ct = tp_e1(v2);
      Ttype ct_t = tex_read(bf3, th, st_cd, ct);
      tfail_check_etype(st_cd, ct, ct_t, 4, (Ttype[]){
        ttype_AINT, ttype_AFLOAT, ttype_ASTRING, ttype_DIC
      });
      if (ct_t == ttype_DIC) {
        Buf *bf4 = buf_new();
        Exp *k = tp_e2(v2);
        Ttype k_t = tex_read(bf4, th, st_cd, k);
        tfail_check_etype1(st_cd, k, k_t, ttype_STRING);
        tfail_check_etype1(st_cd, val, t, ttype_STRING);
        buf_add(bf, str_f("dic_set(%s, %s, %s);\n",
          buf_str(bf3), buf_str(bf4), buf_str(bf2)
        ));
      } else {
        Buf *bf4 = buf_new();
        Exp *ix = tp_e2(v2);
        Ttype ix_t = tex_read(bf4, th, st_cd, ix);
        tfail_check_etype1(st_cd, ix, ix_t, ttype_INT);
        if (ct_t == ttype_AINT) {
          tfail_check_etype1(st_cd, val, t, ttype_INT);
          buf_add(bf, str_f("tarr_setI(%s, %s, %s);\n",
            buf_str(bf3), buf_str(bf4), buf_str(bf2)
          ));
        } else if (ct_t == ttype_AFLOAT) {
          tfail_check_etype1(st_cd, val, t, ttype_FLOAT);
          buf_add(bf, str_f("tarr_setF(%s, %s, %s);\n",
            buf_str(bf3), buf_str(bf4), buf_str(bf2)
          ));
        } else {
          tfail_check_etype1(st_cd, val, t, ttype_STRING);
          buf_add(bf, str_f("tarr_setS(%s, %s, %s);\n",
            buf_str(bf3), buf_str(bf4), buf_str(bf2)
          ));
        }
      }
      return FALSE;
    }
    if (exp_is_pt(left)) {
      // <Exp, Exp>
      Tp *v2 = exp_get_pt(left);
      Buf *bf3 = buf_new();
      Exp *ct = tp_e1(v2);
      Ttype ct_t = tex_read(bf3, th, st_cd, ct);
      tfail_check_etype1(st_cd, ct, ct_t, ttype_DIC);
      tfail_check_etype1(st_cd, val, t, ttype_STRING);
      buf_add(bf, str_f("dic_set(%s, \"%s\", %s);\n",
        buf_str(bf3), symix_get(exp_get_sym(tp_e2(v2))), buf_str(bf2)
      ));
      return FALSE;
    }
    tfail_throw(st_cd, fail_type("symbol, array or dictionary", left));
  // ----------------------------------------------------------------  assignxxx
  } else if (stat_is_add_as(st)) {
    Tp *tp = stat_get_add_as(st);
    return read_assign_xxx(bf, th, st_cd, tp, '+');
  } else if (stat_is_sub_as(st)) {
    Tp *tp = stat_get_sub_as(st);
    return read_assign_xxx(bf, th, st_cd, tp, '-');
  } else if (stat_is_mul_as(st)) {
    Tp *tp = stat_get_mul_as(st);
    return read_assign_xxx(bf, th, st_cd, tp, '*');
  } else if (stat_is_div_as(st)) {
    Tp *tp = stat_get_div_as(st);
    return read_assign_xxx(bf, th, st_cd, tp, '/');
  } else if (stat_is_mod_as(st)) {
    Tp *tp = stat_get_mod_as(st);
    return read_assign_xxx(bf, th, st_cd, tp, '%');
  // ----------------------------------------------------------------------- try
  } else if (stat_is_try(st)) {
    // [<StatCode>, <int>, <StatCode>, <Opt<StatCode>>]
    Arr *ps = stat_get_try(st);
    StatCode *try_code = arr_get(ps, 0);
    int *catch_sym = arr_get(ps, 1);
    char *catch_var = symix_get(*catch_sym);
    StatCode *catch_code = arr_get(ps, 2);
    StatCode *finally_code = opt_get(arr_get(ps, 3));
    if (finally_code)
      tfail_throw(
        finally_code, "'finally' clause is not allowed in typed functions"
      );
    buf_add(bf,
      "{\n"
      "jmp_buf *__TRY_buf = MALLOC(jmp_buf);\n"
      "texc_add(__TRY_buf);\n"
      "if (!setjmp(*__TRY_buf)) {\n"
    );
    int rt1 = read_single_stat(
      bf, is_loop, n_try_rt + 1, n_try_loop + 1, tret, th, try_code
    );
    buf_add(bf, str_f(
      "texc_remove();\n"
      "} else {\n"
      "texc_remove();\n"
      "char *%s = texc_get();\n",
      catch_var
    ));
    theap_add_separator(th);
    theap_add(th, *catch_sym, ttype_STRING, catch_code);
    int rt2 = read_single_stat(
      bf, is_loop, n_try_rt, n_try_loop, tret, th, catch_code
    );
    buf_add(bf, "}}\n");
    theap_remove_block(th);
    return rt1 && rt2;
  // ------------------------------------------------------------------------ if
  } else if (stat_is_if(st)) {
    // [<Exp>, <StatCode>, Opt<StatCode>]
    Arr *ps = stat_get_if(st);
    Buf *bf1 = buf_new();
    Exp *cond = arr_get(ps, 0);
    Ttype tcond = tex_read(bf1, th, st_cd, cond);
    tfail_check_etype1(st_cd, cond, tcond, ttype_BOOL);
    Buf *bf2 = buf_new();
    int rt1 = read_single_stat(
      bf2, is_loop, n_try_rt, n_try_loop, tret, th, arr_get(ps, 1)
    );
    StatCode *st_cd2 = opt_get(arr_get(ps, 2));
    if (st_cd2) {
      Buf *bf3 = buf_new();
      int rt2 = read_single_stat(
        bf3, is_loop, n_try_rt, n_try_loop, tret, th, st_cd2
      );
      buf_add(bf, str_f("if(%s){\n%s\n}else{\n%s\n}\n",
        buf_str(bf1), buf_str(bf2), buf_str(bf3)
      ));
      return rt1 && rt2;
    }
    buf_add(bf, str_f("if(%s){\n%s\n}\n", buf_str(bf1), buf_str(bf2)));
    return rt1;
  // --------------------------------------------------------------------- while
  } else if (stat_is_while(st)) {
    // [<Exp>, <StatCode>] (Exp can be empty)
    Arr *ps = stat_get_while(st);
    Exp *cond = arr_get(ps, 0);
    StatCode *stat = arr_get(ps, 1);
    if (exp_is_empty(cond)) {
      buf_add(bf, "for (;;){\n");
      int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
      buf_add(bf, "}\n");
      return rt1;
    }
    Buf *bf2 = buf_new();
    Ttype t = tex_read(bf2, th, st_cd, cond);
    tfail_check_etype1(st_cd, cond, t, ttype_BOOL);
    buf_add(bf, str_f("while (%s){\n", buf_str(bf2)));
    int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
    buf_add(bf, "}\n");
    return rt1;
  // ----------------------------------------------------------------------- for
  } else if (stat_is_for(st)) {
    // Arr[<int>, <Exp>, <StatCode>]
    Arr *ps = stat_get_for(st);
    int *vsym = (int *) arr_get(ps, 0);
    char *v = symix_get(*vsym);
    char *ixv = symix_get(symix_generate());
    Exp *ct = arr_get(ps, 1);
    char *ctv = symix_get(symix_generate());
    StatCode *stat = arr_get(ps, 2);
    Buf *bf2 = buf_new();
    Ttype t = tex_read(bf2, th, st_cd, ct);
    tfail_check_etype(st_cd, ct, t, 4, (Ttype[]){
      ttype_AINT, ttype_AFLOAT, ttype_ASTRING, ttype_DIC
    });
    if (t == ttype_DIC) {
      buf_add(bf, str_f("Arrs *%s = dic_values(%s);\n",
        ctv, buf_str(bf2)
      ));
      buf_add(bf, str_f(
        "for (int %s = 0; %s < tarr_sizeS(%s); ++%s){\n"
        "char *%s = tarr_getS(%s, %s);\n",
        ixv, ixv, ctv, ixv, v, ctv, ixv
      ));
      theap_add_separator(th);
      theap_add(th, *vsym, ttype_STRING, st_cd);
      int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
      buf_add(bf, "}\n");
      theap_remove_block(th);
      return rt1;
    } else {
      Ttype te = t == ttype_AINT
        ? ttype_INT
        : t == ttype_AFLOAT
          ? ttype_FLOAT
          : ttype_STRING
      ;
      buf_add(bf, str_f("%s %s = %s;\n",
        ttype_to_c(t), ctv, buf_str(bf2)
      ));
      buf_add(bf, str_f(
        "for (int %s = 0; %s < tarr_size%s(%s); ++%s){\n"
        "%s %s = tarr_get%s(%s, %s);\n",
        ixv, ixv, ttype_to_str(t), ctv, ixv,
        ttype_to_c(te), v, ttype_to_str(t), ctv, ixv
      ));
      theap_add_separator(th);
      theap_add(th, *vsym, te, st_cd);
      int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
      buf_add(bf, "}\n");
      theap_remove_block(th);
      return rt1;
    }
  // -------------------------------------------------------------------- for_ix
  } else if (stat_is_for_ix(st)) {
    // Arr[<int>, <int>, <Exp>, <StatCode>]
    Arr *ps = stat_get_for_ix(st);
    int *vsym_ix = (int *) arr_get(ps, 0);
    char *v_ix = symix_get(*vsym_ix);
    int *vsym_e = (int *) arr_get(ps, 1);
    char *v_e = symix_get(*vsym_e);
    char *ixv = symix_get(symix_generate());
    Exp *ct = arr_get(ps, 2);
    char *ctv = symix_get(symix_generate());
    StatCode *stat = arr_get(ps, 3);
    Buf *bf2 = buf_new();
    Ttype t = tex_read(bf2, th, st_cd, ct);
    tfail_check_etype(st_cd, ct, t, 4, (Ttype[]){
      ttype_AINT, ttype_AFLOAT, ttype_ASTRING, ttype_DIC
    });
    if (t == ttype_DIC) {
      buf_add(bf, str_f("Arrs *%s = %s;\n",
        ctv, buf_str(bf2)
      ));
      buf_add(bf, str_f(
        "for (int %s = 0; %s < tarr_sizeS(%s); %s+=2){\n"
        "char *%s = tarr_getS(%s, %s);\n"
        "char *%s = tarr_getS(%s, %s+1);\n",
        ixv, ixv, ctv, ixv,
        v_ix, ctv, ixv,
        v_e, ctv, ixv
      ));
      theap_add_separator(th);
      theap_add(th, *vsym_ix, ttype_STRING, st_cd);
      theap_add(th, *vsym_e, ttype_STRING, st_cd);
      int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
      buf_add(bf, "}\n");
      theap_remove_block(th);
      return rt1;
    } else {
      Ttype te = t == ttype_AINT
        ? ttype_INT
        : t == ttype_AFLOAT
          ? ttype_FLOAT
          : ttype_STRING
      ;
      buf_add(bf, str_f("%s %s = %s;\n",
        ttype_to_c(t), ctv, buf_str(bf2)
      ));
      buf_add(bf, str_f(
        "for (int64_t %s = 0; %s < tarr_size%s(%s); ++%s){\n"
        "%s %s = tarr_get%s(%s, %s);\n",
        v_ix, v_ix, ttype_to_str(t), ctv, v_ix,
        ttype_to_c(te), v_e, ttype_to_str(t), ctv, v_ix
      ));
      theap_add_separator(th);
      theap_add(th, *vsym_ix, ttype_INT, st_cd);
      theap_add(th, *vsym_e, te, st_cd);
      int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
      buf_add(bf, "}\n");
      theap_remove_block(th);
      return rt1;
    }
  // --------------------------------------------------------------------- for_r
  } else if (stat_is_for_r(st) || stat_is_for_rs(st)) {
    int with_step = stat_is_for_rs(st);
    // [<int>, <Exp>, <Exp>, <Exp>, <StatCode>] or
    // [<int>, <Exp>, <Exp>, <StatCode>]
    Arr *ps = with_step ? stat_get_for_rs(st) : stat_get_for_r(st);
    int *vsym_ix = (int *) arr_get(ps, 0);
    char *v_ix = symix_get(*vsym_ix);
    Exp *start = arr_get(ps, 1);
    Buf *start_bf = buf_new();
    Ttype t = tex_read(start_bf, th, st_cd, start);
    tfail_check_etype1(st_cd, start, t, ttype_INT);
    Exp *end = arr_get(ps, 2);
    Buf *end_bf = buf_new();
    t = tex_read(end_bf, th, st_cd, end);
    tfail_check_etype1(st_cd, end, t, ttype_INT);
    StatCode *stat = arr_get(ps, with_step ? 4 : 3);
    if (with_step) {
      Exp *step = arr_get(ps, 3);
      Buf *step_bf = buf_new();
      t = tex_read(step_bf, th, st_cd, step);
      tfail_check_etype1(st_cd, step, t, ttype_INT);
      buf_add(bf, str_f(
        "if (%s == 0) texc_throw_(%s);\n"
        "for (int64_t %s = %s; "
        "(%s < 0) ? %s >= %s : %s <= %s ; "
        "%s += %s) {\n",
        buf_str(step_bf),
        js_ws(str_f("For step is 0\n  %s", tfail_position(st_cd))),
        v_ix, buf_str(start_bf),
        buf_str(step_bf), v_ix, buf_str(end_bf), v_ix, buf_str(end_bf),
        v_ix, buf_str(step_bf)
      ));
    } else {
      buf_add(bf, str_f(
        "for (int64_t %s = %s; %s < %s; ++%s) {\n",
        v_ix, buf_str(start_bf), v_ix, buf_str(end_bf), v_ix
      ));
    }
    theap_add_separator(th);
    theap_add(th, *vsym_ix, ttype_INT, st_cd);
    int rt1 = read_single_stat(bf, TRUE, n_try_rt, 0, tret, th, stat);
    buf_add(bf, "}\n");
    theap_remove_block(th);
    return rt1;
  // -------------------------------------------------------------------- switch
  } else if (stat_is_switch(st)) {
    // [<Exp>, Arr<Tp<Arr<Exp>, StatCode>>]
    Arr *a = stat_get_switch(st);
    Exp *lcond = arr_get(a, 0);
    // Arr<Tp<Arr<Exp>, StatCode>>
    Arr *entries = arr_get(a, 1);
    if (!arr_size(entries)) tfail_throw(st_cd, "'switch' without entries");
    int rt = TRUE;

    EACH(entries, Tp, e) {
      // <Exp>
      Arr *rconds = tp_e1(e);
      StatCode *stat = tp_e2(e);

      Exp *rcond0 = arr_get(rconds, 0);
      Exp *cond = NULL;
      if (
        exp_is_sym(rcond0) &&
        !strcmp(symix_get(exp_get_sym(rcond0)), "default")
      ) {
        cond = exp_bool(TRUE);
      } else {
        cond = exp_eq(lcond, rcond0);
        RANGE(i, 1, arr_size(rconds)) {
          cond = exp_or(cond, exp_eq(lcond, arr_get(rconds, i)));
        }_RANGE
      }
      Buf *bf1 = buf_new();
      Ttype tcond = tex_read(bf1, th, st_cd, cond);
      tfail_check_etype1(st_cd, cond, tcond, ttype_BOOL);

      Buf *bf2 = buf_new();
      int rt2 = read_single_stat(
        bf2, is_loop, n_try_rt, n_try_loop, tret, th, stat
      );

      buf_add(bf, str_f("%s(%s){\n%s\n}\n",
        _i == 0 ? "if" : "else if", buf_str(bf1), buf_str(bf2)
      ));

      rt = rt && rt2;
    }_EACH
    return rt;
  // ----------------------------------------------------------------------- END
  } else {
    tfail_throw(st_cd,
      "Statement type '%s' is not allowed in typed functions\n  %s",
      stat_type_to_str(st), stat_to_str(st)
    );
  }
  return FALSE;
}

void treader_read (Buf *bf, Ttype tret, Theap *th, StatCode *st) {
  int r = read_single_stat(bf, FALSE, 0, 0, tret, th, st);
  if (!r && tret != ttype_NO_RETURN)
    tfail_throw(st,
      "Function with return of type %s, returns nothing",
      ttype_to_str(tret)
    );

}

