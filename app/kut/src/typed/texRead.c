// Copyright 23-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/texRead.h"
#include "typed/tfail.h"
#include "typed/tfunction.h"
#include "typed/tbmodules.h"
#include "kut/math.h"
#include "kut/js.h"
#include "symix.h"
#include "module.h"
#include "modules.h"

Ttype tex_read (Buf *bf, Theap *th, StatCode *stat, Exp *exp) {
  Ttype binary (int is_bool, char *sym, Tp *vals) {
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    if (is_bool)
      tfail_check_etype1(stat, e1, t1, ttype_BOOL);
    else
      tfail_check_etype(stat, e1, t1, 2, (Ttype[]){ttype_INT, ttype_FLOAT});
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    buf_add(bf, str_f("(%s%s%s)", buf_str(bf1), sym, buf_str(bf2)));
    return t1;
  }

  if (exp_is_bool(exp)) {
    int v = exp_get_bool(exp);
    buf_add(bf, v == 0 ? " 0 " : " 1 ");
    return ttype_BOOL;
  }
  if (exp_is_int(exp)) {
    buf_add(bf, str_f(" %s ", math_itos(exp_get_int(exp))));
    return ttype_INT;
  }
  if (exp_is_float(exp)) {
    char *n = math_ftos(exp_get_float(exp), 9);
    if (str_cindex(n, '.') == -1) n = str_f("%s.0", n);
    buf_add(bf, str_f(" %s ", n));
    return ttype_FLOAT;
  }
  if (exp_is_string(exp)) {
    buf_add(bf, js_ws(exp_get_string(exp)));
    return ttype_STRING;
  }
  if (exp_is_array(exp)) {
    // <Exp>
    Arr *a = exp_get_array(exp);
    if (!arr_size(a)) {
      buf_add(bf, " tarr_new_s()");
      return ttype_ASTRING;
    } else if (arr_size(a) == 1) {
      Exp *e0 = arr_get(a, 0);
      if (exp_is_not(e0)) {
        Exp *e00 = exp_get_not(e0);
        if (exp_is_int(e00)) {
          buf_add(bf, " tarr_new_i()");
          return ttype_AINT;
        }
        if (exp_is_float(e00)) {
          buf_add(bf, " tarr_new_f()");
          return ttype_AFLOAT;
        }
      }
    }
    Exp *e0 = arr_get(a, 0);
    Buf *tmp = buf_new();
    Ttype e0t = tex_read(tmp, th, stat, e0);
    tfail_check_etype(stat, e0, e0t, 3, (Ttype[]){
      ttype_INT, ttype_FLOAT, ttype_STRING
    });
    // <char>
    Arr *a2 = arr_new();
    EACH(a, Exp, e2) {
      buf_reset(tmp);
      Ttype e2t = tex_read(tmp, th, stat, e2);
      tfail_check_etype1(stat, e2, e2t, e0t);
      arr_push(a2, str_new(buf_str(tmp)));
    }_EACH
    buf_add(bf, str_f(
      " tarr_new_from%s((%s []){%s}, %d)",
      ttype_to_str(e0t), ttype_to_c(e0t), arr_join(a2, ","), arr_size(a2)
    ));
    return e0t == ttype_INT
      ? ttype_AINT
      : e0t == ttype_FLOAT
        ? ttype_AFLOAT
        : ttype_ASTRING
    ;
  }
  if (exp_is_dic(exp)) {
    // <Exp>
    Map *a = exp_get_dic(exp);
    Arr *a2 = arr_new();
    Buf *tmp = buf_new();
    EACH(a, Kv, kv) {
      arr_push(a2, js_ws(kv_key(kv)));
      buf_reset(tmp);
      Exp *e = kv_value(kv);
      Ttype et = tex_read(tmp, th, stat, e);
      tfail_check_etype1(stat, e, et, ttype_STRING);
      arr_push(a2, str_new(buf_str(tmp)));
    }_EACH
    buf_add(bf, str_f(
      " tarr_new_froms((char *[]){%s}, %d)",
      arr_join(a2, ","), arr_size(a2)
    ));
    return ttype_DIC;
  }
  if (exp_is_sym(exp)) {
    int sym = exp_get_sym(exp);
    char *ssym = symix_get(sym);
    TheapEntry *e = opt_get(theap_get(th, sym));
    if (!e){
      Module *md = opt_eget(modules_get_ok(stat_code_file_ix(stat)));
      Heap0Entry *e = opt_get(heap0_get(module_get_heap0(md), sym));
      if (e) {
        Exp *ex = heap0_entry_exp(e);
        if (
          exp_is_bool(ex) || exp_is_int(ex) ||
          exp_is_float(ex) || exp_is_string(ex)
        ) {
          return tex_read(bf, th, stat, ex);
        } else {
          tfail_throw(stat, str_f(
            "Reading symbol '%s':\n"
            "Typed function only has access to constants b, i, f or s.",
            ssym
          ));
        }
      }
      tfail_throw(stat, "Local symbol '%s' not found", ssym);
    }
    buf_add(bf, str_f(" %s ", ssym));
    return e->type;
  }
  if (exp_is_pt(exp)) {
    // <Exp, Exp>
    Tp *tp = exp_get_pt(exp);
    Exp *left = tp_e1(tp);
    Exp *right = tp_e2(tp);
    if (
      exp_is_sym(left) &&
      !opt_get(theap_get(th, exp_get_sym(left))) &&
      exp_is_sym(right)
    ) {
      int mod_sym = exp_get_sym(left);
      char *mod_ssym = symix_get(mod_sym);
      Module *md0 = opt_eget(modules_get_ok(stat_code_file_ix(stat)));
      int fix = imports_get_fix(module_get_imports(md0), mod_sym);
      if (fix != -1) {
        int sym = exp_get_sym(right);
        char *ssym = symix_get(sym);
        Module *md = opt_eget(modules_get_ok(fix));
        Heap0Entry *e = opt_get(heap0_get(module_get_heap0(md), sym));
        if (e) {
          Exp *ex = heap0_entry_exp(e);
          if (
            exp_is_bool(ex) || exp_is_int(ex) ||
            exp_is_float(ex) || exp_is_string(ex)
          ) {
            return tex_read(bf, th, stat, ex);
          } else {
            tfail_throw(stat, str_f(
              "Reading symbol '%s.%s':\n"
              "Typed function only has access to constants b, i, f or s.",
              mod_ssym, ssym
            ));
          }
        }
        tfail_throw(stat, "Symbol %s.%s not found", mod_ssym, ssym);
      }
      // do nothing -> continue with Buf *bf1 = buf_new(); ...etc.
    }
    Buf *bf1 = buf_new();
    Ttype tc = tex_read(bf1, th, stat, left);
    tfail_check_etype1(stat, exp, tc, ttype_DIC);
    char *key = symix_get(exp_get_sym(right));
    buf_add(bf, str_f(" dic_get_or_exc(%s, \"%s\")", buf_str(bf1), key));
    return ttype_STRING;
  }
  if (exp_is_sq(exp)) {
    // <Exp, Exp>
    Tp *tp = exp_get_sq(exp);
    Buf *bf1 = buf_new();
    Ttype tc = tex_read(bf1, th, stat, tp_e1(tp));
    tfail_check_etype(stat, exp, tc, 5, (Ttype[]){
      ttype_STRING, ttype_AINT, ttype_AFLOAT, ttype_ASTRING, ttype_DIC
    });
    Exp *ix = tp_e2(tp);
    Buf *bf2 = buf_new();
    Ttype te = tex_read(bf2, th, stat, ix);
    if (tc == ttype_DIC) {
      tfail_check_etype1(stat, ix, te, ttype_STRING);
      buf_add(bf, str_f(" dic_get_or_exc(%s,%s)", buf_str(bf1), buf_str(bf2)));
      return ttype_STRING;
    } else {
      tfail_check_etype1(stat, ix, te, ttype_INT);
      if (tc == ttype_STRING) {
        buf_add(bf, str_f(" str_get(%s,%s)", buf_str(bf1), buf_str(bf2)));
        return ttype_STRING;
      } else {
        buf_add(bf, str_f(" tarr_get%s(%s,%s)",
          ttype_to_str(tc), buf_str(bf1), buf_str(bf2)
        ));
        return tc == ttype_AINT
          ? ttype_INT
          : tc == ttype_AFLOAT
            ? ttype_FLOAT
            : ttype_STRING
        ;
      }
    }
  }
  if (exp_is_slice(exp)) {
    // <Exp, Exp, Exp>
    Tp3 *tp = exp_get_slice(exp);
    Buf *bf1 = buf_new();
    Ttype tc = tex_read(bf1, th, stat, tp3_e1(tp));
    tfail_check_etype(stat, exp, tc, 4, (Ttype[]){
      ttype_STRING, ttype_AINT, ttype_AFLOAT, ttype_ASTRING
    });
    Exp *ixl = tp3_e2(tp);
    Exp *ixr = tp3_e3(tp);
    if (exp_is_empty(ixl)) {
      if (exp_is_empty(ixr)) {
        if (tc == ttype_STRING)
          buf_add(bf, str_f(" str_new(%s)", buf_str(bf1)));
        else
          buf_add(bf, str_f(" tarr_copy%s(%s)", ttype_to_str(tc), buf_str(bf1)));
      } else {
        Buf *bf2 = buf_new();
        Ttype tix = tex_read(bf2, th, stat, ixr);
        tfail_check_etype1(stat, ixr, tix, ttype_INT);
        if (tc == ttype_STRING)
          buf_add(bf, str_f(" str_left(%s,%s)", buf_str(bf1), buf_str(bf2)));
        else
          buf_add(bf, str_f(" tarr_take%s(%s,tarr_ix_correct%s(%s,%s))",
            ttype_to_str(tc), buf_str(bf1),
            ttype_to_str(tc), buf_str(bf1), buf_str(bf2)
          ));
      }
    } else {
      if (exp_is_empty(ixr)) {
        Buf *bf2 = buf_new();
        Ttype tix = tex_read(bf2, th, stat, ixl);
        tfail_check_etype1(stat, ixl, tix, ttype_INT);
        if (tc == ttype_STRING)
          buf_add(bf, str_f(" str_right(%s,%s)", buf_str(bf1), buf_str(bf2)));
        else
          buf_add(bf, str_f(" tarr_drop%s(%s,tarr_ix_correct%s(%s,%s))",
            ttype_to_str(tc), buf_str(bf1),
            ttype_to_str(tc), buf_str(bf1), buf_str(bf2)
          ));
      } else {
        Buf *bf2 = buf_new();
        Ttype tix = tex_read(bf2, th, stat, ixl);
        tfail_check_etype1(stat, ixl, tix, ttype_INT);
        Buf *bf3 = buf_new();
        Ttype tix2 = tex_read(bf3, th, stat, ixr);
        tfail_check_etype1(stat, ixr, tix2, ttype_INT);
        if (tc == ttype_STRING)
          buf_add(bf, str_f(" str_sub(%s,%s,%s)",
            buf_str(bf1), buf_str(bf2), buf_str(bf3)
          ));
        else
          buf_add(bf, str_f(
            " tarr_drop%s(tarr_take%s(%s,"
            "tarr_ix_correct%s(%s,%s)),"
            " tarr_ix_correct%s(%s,%s))",
            ttype_to_str(tc), ttype_to_str(tc), buf_str(bf1),
            ttype_to_str(tc), buf_str(bf1), buf_str(bf3),
            ttype_to_str(tc), buf_str(bf1), buf_str(bf2)
          ));
      }
    }
    return tc;
  }
  if (exp_is_pr(exp)) {
    // <Exp, Arr<Exp>>
    Tp *tp = exp_get_pr(exp);
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
      Ttype t = tex_read(bf2, th, stat, e);
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

    Module *md = opt_eget(modules_get_ok(stat_code_file_ix(stat)));
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
            tfail_throw(stat,
              "Tfunction type '%s' called with arguments of type '%s'",
              fn_type, call_type
            );
          Ttype tret = tfunction_get_tret(tfn);
          buf_add(bf, str_f(" %s(%s).%s",
            tfunction_get_cid(tfn), pars_code, ttype_to_str(tret)
          ));
          return tret;
        } else {
          tfail_throw(stat, "'%s' is not a typed function", fn_str);
        }
      } else {
        tfail_throw(stat, "Symbol '%s' not found", fn_str);
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
                tfail_throw(stat,
                  "Tfunction type '%s' called with arguments of type '%s'",
                  fn_type, call_type
                );
              Ttype tret = tfunction_get_tret(tfn);
              buf_add(bf, str_f(" %s(%s).%s",
                tfunction_get_cid(tfn), pars_code, ttype_to_str(tret)
              ));
              return tret;
            } else {
              tfail_throw(stat, "'%s' is not a typed function", fn_str);
            }
          } else {
            tfail_throw(stat, "Symbol '%s' not found", fn_str);
          }
        } else {
          TbmodRs *md_rs = tbmodules_get(
            mod_id, fn_id, TRUE, call_type, pars_bcode
          );
          if (md_rs->tret == ttype_ERROR) tfail_throw(stat, md_rs->code);
          buf_add(bf, str_f(" %s", md_rs->code));
          return md_rs->tret;
        }
      } else {
        tfail_throw(stat,
          "Expected expression type fn(...) or module.fn(...), "
          "but it was found:\n  %s", exp_to_js(fn)
        );
      }
    } else {
      tfail_throw(stat,
        "Expected expression type fn(...) or module.fn(...), "
        "but it was found:\n  %s", exp_to_js(fn)
      );
    }
  }
  if (exp_is_switch(exp)) {
    // <Exp, Arr<Tp<Arr<Exp>, Exp>>>
    Tp *tp = exp_get_switch(exp);
    Exp *lcond = tp_e1(tp);
    // Arr<Tp<Arr<Exp>, Exp>>
    Arr *es = tp_e2(tp);
    Ttype tret = ttype_ERROR;
    Buf *bf_lcond = buf_new();
    Ttype t_lcond = tex_read(bf_lcond, th, stat, lcond);
    // <char>
    Arr *ops = arr_new();
    EACH(es, Tp, tp) {
      // Arr<Exp>
      Arr *rconds = tp_e1(tp);
      Exp *val = tp_e2(tp);
      //<char>
      Arr *aconds = arr_new();
      EACH(rconds, Exp, rcond) {
        if (exp_is_sym(rcond)) {
          char *sym_s = symix_get(exp_get_sym(rcond));
          if (!strcmp(sym_s, "default")) {
            arr_push(aconds, "");
            continue;
          }
        }
        Buf *bf4 = buf_new();
        Ttype t2 = tex_read(bf4, th, stat, rcond);
        tfail_check_etype1(stat, rcond, t2, t_lcond);
        arr_push(aconds, str_f("built_eq%s(%s,%s)",
          ttype_to_str(t_lcond), buf_str(bf_lcond), buf_str(bf4)
        ));
      }_EACH
      Buf *bf5 = buf_new();
      Ttype t5 = tex_read(bf5, th, stat, val);
      if (tret == ttype_ERROR) tret = t5;
      else tfail_check_etype1(stat, val, t5, tret);
      if (!*(char *)arr_get(aconds, 0))
        arr_push(ops, str_f("%s\n", buf_str(bf5)));
      else
        arr_push(ops, str_f("%s\n  ? %s\n",
          arr_join(aconds, " || "), buf_str(bf5)
        ));
    }_EACH
    buf_add(bf, str_f("(%s)", arr_join(ops, "  : ")));

    return tret;
  }
  if (exp_is_not(exp)) {
    Exp *e = exp_get_not(exp);
    Buf *bf2 = buf_new();
    Ttype t = tex_read(bf2, th, stat, e);
    tfail_check_etype(stat, e, t, 4, (Ttype[]){
      ttype_BOOL, ttype_AINT, ttype_AFLOAT, ttype_ASTRING
    });
    if (t == ttype_BOOL)
      buf_add(bf, str_f("!(%s)", buf_str(bf2)));
    else
      buf_add(bf, str_f("tarr_empty%s(%s)", ttype_to_str(t), buf_str(bf2)));

    return ttype_BOOL;
  }
  if (exp_is_minus(exp)) {
    Exp *e = exp_get_minus(exp);
    Buf *bf2 = buf_new();
    Ttype t = tex_read(bf2, th, stat, e);
    tfail_check_etype(stat, e, t, 2, (Ttype[]){ttype_INT, ttype_FLOAT});
    buf_add(bf, str_f("(-(%s))", buf_str(bf2)));
    return t;
  }
  if (exp_is_add(exp)) {
    Tp *vals = exp_get_add(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    tfail_check_etype(stat, e1, t1, 6, (Ttype[]){
      ttype_INT, ttype_FLOAT, ttype_STRING,
      ttype_AINT, ttype_AFLOAT, ttype_ASTRING
    });
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_INT || t2 == ttype_FLOAT)
      buf_add(bf, str_f("(%s+%s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_STRING)
      buf_add(bf, str_f(" str_f(\"%%s%%s\", %s, %s)",
        buf_str(bf1), buf_str(bf2)
      ));
    else
      buf_add(bf, str_f(" tarr_concat%s(%s, %s)",
        ttype_to_str(t1), buf_str(bf1), buf_str(bf2)
      ));
    return t1;
  }
  if (exp_is_sub(exp)) {
    return binary(FALSE, "-", exp_get_sub(exp));
  }
  if (exp_is_mul(exp)) {
    return binary(FALSE, "*", exp_get_mul(exp));
  }
  if (exp_is_div(exp)) {
    return binary(FALSE, "/", exp_get_div(exp));
  }
  if (exp_is_mod(exp)) {
    Tp *vals = exp_get_mod(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    tfail_check_etype1(stat, e1, t1, ttype_INT);
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    buf_add(bf, str_f("(%s%%%s)", buf_str(bf1), buf_str(bf2)));
    return t1;
  }
  if (exp_is_and(exp)) {
    return binary(TRUE, "&&", exp_get_and(exp));
  }
  if (exp_is_or(exp)) {
    return binary(TRUE, "||", exp_get_or(exp));
  }
  if (exp_is_eq(exp)) {
    Tp *vals = exp_get_eq(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_BOOL || t1 == ttype_INT)
      buf_add(bf, str_f("(%s == %s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_FLOAT)
      buf_add(bf, str_f(" math_eq(%s, %s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_STRING)
      buf_add(bf, str_f(" !strcmp(%s, %s)", buf_str(bf1), buf_str(bf2)));
    else
      buf_add(bf, str_f(" built_eq%s(%s,%s)",
        ttype_to_str(t1), buf_str(bf1), buf_str(bf2))
      );
    return ttype_BOOL;
  }
  if (exp_is_neq(exp)) {
    Tp *vals = exp_get_neq(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_BOOL || t1 == ttype_INT)
      buf_add(bf, str_f("(%s != %s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_FLOAT)
      buf_add(bf, str_f(" !math_eq(%s, %s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_STRING)
      buf_add(bf, str_f(" strcmp(%s, %s)", buf_str(bf1), buf_str(bf2)));
    else
      buf_add(bf, str_f(" built_neq%s(%s,%s)",
        ttype_to_str(t1), buf_str(bf1), buf_str(bf2))
      );
    return ttype_BOOL;
  }
  if (exp_is_greater(exp)) {
    Tp *vals = exp_get_greater(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    tfail_check_etype(stat, e1, t1, 3, (Ttype[]){
      ttype_INT, ttype_FLOAT, ttype_STRING
    });
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_STRING)
      buf_add(bf, str_f("(strcmp(%s, %s) > 0)", buf_str(bf1), buf_str(bf2)));
    else
      buf_add(bf, str_f("(%s > %s)", buf_str(bf1), buf_str(bf2)));
    return ttype_BOOL;
  }
  if (exp_is_greater_eq(exp)) {
    Tp *vals = exp_get_greater_eq(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    tfail_check_etype(stat, e1, t1, 3, (Ttype[]){
      ttype_INT, ttype_FLOAT, ttype_STRING
    });
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_INT)
      buf_add(bf, str_f("(%s >= %s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_FLOAT)
      buf_add(bf, str_f("(%s > %s || math_eq(%s, %s))",
        buf_str(bf1), buf_str(bf2), buf_str(bf1), buf_str(bf2)
      ));
    else
      buf_add(bf, str_f("(strcmp(%s, %s) >= 0)",
        buf_str(bf1), buf_str(bf2), buf_str(bf1), buf_str(bf2)
      ));
    return ttype_BOOL;
  }
  if (exp_is_less(exp)) {
    Tp *vals = exp_get_less(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    tfail_check_etype(stat, e1, t1, 3, (Ttype[]){
      ttype_INT, ttype_FLOAT, ttype_STRING
    });
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_STRING)
      buf_add(bf, str_f("(strcmp(%s, %s) < 0)", buf_str(bf1), buf_str(bf2)));
    else
      buf_add(bf, str_f("(%s < %s)", buf_str(bf1), buf_str(bf2)));
    return ttype_BOOL;
  }
  if (exp_is_less_eq(exp)) {
    Tp *vals = exp_get_less_eq(exp);
    Exp *e1 = tp_e1(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    tfail_check_etype(stat, e1, t1, 3, (Ttype[]){
      ttype_INT, ttype_FLOAT, ttype_STRING
    });
    Exp *e2 = tp_e2(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    if (t1 == ttype_INT)
      buf_add(bf, str_f("(%s <= %s)", buf_str(bf1), buf_str(bf2)));
    else if (t1 == ttype_FLOAT)
      buf_add(bf, str_f("(%s < %s || math_eq(%s, %s))",
        buf_str(bf1), buf_str(bf2), buf_str(bf1), buf_str(bf2)
      ));
    else
      buf_add(bf, str_f("(strcmp(%s, %s) <= 0)",
        buf_str(bf1), buf_str(bf2)
      ));
    return ttype_BOOL;
  }
  if (exp_is_ternary(exp)) {
    Tp3 *vals = exp_get_ternary(exp);
    Exp *e0 = tp3_e1(vals);
    Buf *bf0 = buf_new();
    Ttype t0 = tex_read(bf0, th, stat, e0);
    tfail_check_etype1(stat, e0, t0, ttype_BOOL);
    Exp *e1 = tp3_e2(vals);
    Buf *bf1 = buf_new();
    Ttype t1 = tex_read(bf1, th, stat, e1);
    Exp *e2 = tp3_e3(vals);
    Buf *bf2 = buf_new();
    Ttype t2 = tex_read(bf2, th, stat, e2);
    tfail_check_etype1(stat, e2, t2, t1);
    buf_add(bf, str_f("(%s ? %s : %s)",
      buf_str(bf0), buf_str(bf1), buf_str(bf2)
    ));
    return t1;
  }
  tfail_throw(stat,
    "Expresion not allowed in typed functions\n  %s", exp_to_js(exp)
  );
  return ttype_ERROR;  // Unreachable
}
