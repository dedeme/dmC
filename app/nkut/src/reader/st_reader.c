// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/st_reader.h"
#include "kut/path.h"
#include "kut/buf.h"
#include "DEFS.h"
#include "fileix.h"
#include "reader/reader.h"
#include "reader/ex_reader.h"
#include "reader/pt_sq_pr_reader.h"

// Read loop and other internals statements.
static StatCode *st_in_read(Types *tps, Cdr *cdr) {
  int nline = cdr_get_next_nline(cdr);
  StatCode *r = st_reader_read(tps, cdr);
  Stat *st = stat_code_stat(r);
  if (stat_is_block_close(st))
    EXC_GENERIC(cdr_fail_line(cdr, "Unexpected '}'", nline));
  if (stat_is_import(st))
    EXC_GENERIC(cdr_fail_line(cdr, "'import' out of main block", nline));
  if (stat_is_end(st))
    EXC_GENERIC(cdr_fail(cdr, "Unexpected end of text"));

  return r;
}

static Stat *read_multi(Types *tps, char *sym, Cdr *cdr, char *js) {
  // <Exp>
  Arr *syms = arr_new();
  Buf *abf = buf_new();
  Buf *dbf = buf_new();

  arr_push(syms, exp_sym(sym, sym));
  buf_add(abf, js);
  if (*sym) buf_add(dbf, js);
  else buf_add(dbf, str_left(js, -1));

  for (;;) {
    Token *tk = cdr_read_token(cdr);
    if (token_is_comma(tk)) {
      arr_push(syms, exp_sym("", ""));
      buf_add(abf, tk->js);
      continue;
    }

    if (token_is_colon(tk)) {
      Token *tk_mod = cdr_read_token(cdr);
      if (tk_mod->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk_mod)));
      char *js = str_f("%s%s",
        str_left(tk->js, -1), str_left(tk_mod->js, -strlen(tk_mod->value))
      );
      buf_add(abf, js);
      buf_add(dbf, js);

      tk = cdr_read_token(cdr);
      if (tk->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
      types_add(tps, tk->value, tk_mod->value);
    } else if (token_is_equals(tk)) {
      buf_add(abf, "]");
      buf_add(abf, tk->js);
      Exp *exp = ex_reader_read(tps, cdr);
      Token *tk2 = cdr_read_token(cdr);
      if (!token_is_semicolon(tk2))
        EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk2)));
      buf_add(abf, exp_get_js(exp));
      buf_add(abf, tk2->js);
      char *js = exp_js_insert(
        exp_bool(FALSE, buf_str(abf)), // fake Exp.
        "const ["
      );
      return stat_arr_multi(syms, exp, js);

    } else if (tk->type != TOKEN_SYMBOL)
      EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));

    arr_push(syms, exp_sym(tk->value, tk->value));
    buf_add(abf, tk->js);
    buf_add(dbf, tk->js);
    tk = cdr_read_token(cdr);

    if (token_is_equals(tk)) {
      buf_add(abf, "]");
      buf_add(abf, tk->js);
      if (cdr_next_token_is_semicolon(cdr)) {
        buf_add(abf, "[0");
        for (int i = 1; i < arr_size(syms); ++i) {
          buf_add(abf, ", ");
          buf_add(abf, str_f("%d", i));
        }
        buf_add(abf, "]");
        tk = cdr_read_token(cdr);
        buf_add(abf, tk->js);
        char *js = exp_js_insert(
          exp_bool(FALSE, buf_str(abf)), // fake Exp.
          "const ["
        );

        return stat_arr_multi(
          syms, exp_array(arr_new(), "[]"), js
        );
      }
      Exp *exp = ex_reader_read(tps, cdr);
      Token *tk2 = cdr_read_token(cdr);
      if (!token_is_semicolon(tk2))
        EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk2)));
      buf_add(abf, exp_get_js(exp));
      buf_add(abf, tk2->js);
      char *js = exp_js_insert(
        exp_bool(FALSE, buf_str(abf)), // fake Exp.
        "const ["
      );
      return stat_arr_multi(syms, exp, js);
    }
    if (token_is_colon(tk)) {
      buf_add(dbf, "}");
      buf_add(dbf, str_left(tk->js, -1));
      buf_cadd(dbf, '=');
      if (cdr_next_token_is_semicolon(cdr)) {
        buf_cadd(dbf, '{');
        int is_first = TRUE;
        for (int i = 0; i < arr_size(syms); ++i) {
          char *sym_i = exp_get_sym(arr_get(syms, i));
          if (*sym_i) {
            if (is_first) is_first = FALSE;
            else buf_add(dbf, ", ");
            buf_add(dbf, str_f("\"%s\":\"%s\"", sym_i, sym_i));
          }
        }
        buf_add(dbf, "}");
        tk = cdr_read_token(cdr);
        buf_add(dbf, tk->js);
        char *js = exp_js_insert(
          exp_bool(FALSE, buf_str(dbf)), // fake Exp.
          "const {"
        );

        return stat_dic_multi(
          syms, exp_array(arr_new(), "{}"), js
        );
      }
      Exp *exp = ex_reader_read(tps, cdr);
      Token *tk2 = cdr_read_token(cdr);
      if (!token_is_semicolon(tk2))
        EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk2)));
      buf_add(dbf, exp_get_js(exp));
      buf_add(dbf, tk2->js);

      char *js = exp_js_insert(
        exp_bool(FALSE, buf_str(dbf)), // fake Exp.
        "const {"
      );
      return stat_dic_multi(syms, exp, js);
    }
    if (!token_is_comma(tk))
       EXC_GENERIC(cdr_fail_expect(cdr, "',', '=' or ':'", token_to_str(tk)));

    buf_add(abf, tk->js);
    if (*exp_get_sym(arr_peek(syms))) buf_add(dbf, tk->js);
  }
}

static Stat *read_symbol(Types *tps, Token *token, Cdr *cdr) {
  char *sym = token->value;
  char *js = token->js;
  if (!strcmp(sym, "break")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_break(str_f("%s%s", js, tk->js));
  }

  if (!strcmp(sym, "continue")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_continue(str_f("%s%s", js, tk->js));
  }

  if (!strcmp(sym, "trace")) {
    js = str_f("%s%s", str_left(js, -5), "sys.$trace");
    Exp *exp = ex_reader_read(tps, cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_trace(exp, str_f("%s(%s)%s", js, exp_get_js(exp), tk->js));
  }

  if (!strcmp(sym, "throw")) {
    js = str_f("%s%s", str_left(js, -5), "throw Error");
    Exp *exp = ex_reader_read(tps, cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_throw(exp, str_f("%s(%s)%s", js, exp_get_js(exp), tk->js));
  }

  if (!strcmp(sym, "return")) {
    if (cdr_next_token_is_semicolon(cdr)) {
      Token *tk = cdr_read_token(cdr);
      return stat_return(opt_none(), str_f("%s%s", js, tk->js));
    }
    Exp *exp = ex_reader_read(tps, cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_return(
      opt_some(exp), str_f("%s%s%s",
        str_left(js, -6),
        exp_js_insert(exp, "return "),
        tk->js
      )
    );
  }

  if (!strcmp(sym, "do")) {
    Token *js_inject = cdr_read_token(cdr);
    if (js_inject->type != TOKEN_STRING)
      EXC_GENERIC(cdr_fail_expect(cdr, "string", token_to_str(js_inject)));
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));

    js = str_f("%s%s%s",
      str_left(js, -2), token_string_injection(js_inject), tk->js
    );
    return stat_do(js_inject->value, js);
  }

  if (!strcmp(sym, "await")) {
    Exp *exp = ex_reader_read(tps, cdr);
    if (!exp_is_pr(exp))
      EXC_GENERIC(cdr_fail_expect(cdr, "function call", exp_to_str(exp)));
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_await(exp, str_f("%s%s%s", js, exp_get_js(exp), tk->js));
  }

  if (!strcmp(sym, "import")) {
    Token *tk = cdr_read_token(cdr);
    if (tk->type != TOKEN_STRING)
      EXC_GENERIC(cdr_fail_expect(cdr, "string", token_to_str(tk)));


    char *mod = tk->value;
    if (*mod == '/')
      EXC_GENERIC(cdr_fail(cdr, "Module path starts with '/'"));
    if (str_ends(mod, "/"))
      EXC_GENERIC(cdr_fail(cdr, "Module path ends with '/'"));
    if (str_index(path_base(mod), ".") != -1)
      EXC_GENERIC(cdr_fail(cdr, "Module name contains '.'"));
    if (!*mod)
      EXC_GENERIC(cdr_fail(cdr, "Module path is empty"));

    Token *tk3 = cdr_read_token(cdr);
    char *id;
    char *js_as = "";
    if (token_is_colon(tk3)) {
      Token *tk2 = cdr_read_token(cdr);
      if (tk2->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk2)));
      id = tk2->value;
      js_as = str_f("%s%s%s", str_left(tk3->js, -1), " as ", tk2->js);
      tk3 = cdr_read_token(cdr);
    } else {
      id = path_base(mod);
      js_as = str_f(" as %s", id);
    }

    if (!token_is_semicolon(tk3))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk3)));

    int ix = str_cindex(tk->js, '"');
    return stat_import(
      mod, id, str_f(
        "%s *%s from %s##%s", js, js_as, str_left(tk->js, ix), tk3->js
      )
    );
  }

  if (!strcmp(sym, "try")) {
    StatCode *st1 = st_in_read(tps, cdr);
    Token *tk_catch = cdr_read_token(cdr);
    if (!token_is_catch(tk_catch))
      EXC_GENERIC(cdr_fail_expect(cdr, "catch", token_to_str(tk_catch)));
    Token *open_pr = cdr_read_token(cdr);
    if (!token_is_open_par(open_pr))
      EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(open_pr)));
    Token *var = cdr_read_token(cdr);
    if (var->type != TOKEN_SYMBOL)
      EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var)));
    Token *close_pr = cdr_read_token(cdr);
    if (!token_is_close_par(close_pr))
      EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));
    StatCode *st2 = st_in_read(tps, cdr);
    js = str_f("%s{%s}%s%s%s%s{%s}",
      js, stat_get_js(stat_code_stat(st1)),
      tk_catch->js, open_pr->js, var->js, close_pr->js,
      stat_get_js(stat_code_stat(st2))
    );
    // <StatCode>
    Opt *st3 = opt_none();
    if (cdr_next_token_is_finally(cdr)) {
      Token *tk_finally = cdr_read_token(cdr);
      StatCode *st3s = st_in_read(tps, cdr);
      st3 = opt_some(st3s);
      js = str_f(
        "%s%s{%s}", js, tk_finally->js, stat_get_js(stat_code_stat(st3s))
      );
    }
    return stat_try(st1, var->value, st2, st3, js);
  }

  if (!strcmp(sym, "catch")) {
    EXC_GENERIC(cdr_fail(cdr, "'catch' without 'try'"));
  }

  if (!strcmp(sym, "finally")) {
    EXC_GENERIC(cdr_fail(cdr, "'finally' without 'try'"));
  }

  if (!strcmp(sym, "while")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    if (cdr_next_token_is_close_parenthesis(cdr)) {
      Token *tk2 = cdr_read_token(cdr);
      StatCode *st_cd = st_in_read(tps, cdr);
      return stat_while(
        opt_none(), st_cd,
        str_f("%s%s%s%s%s",
          js, tk->js, "true", tk2->js, stat_get_js(stat_code_stat(st_cd))
        )
      );
    }

    Exp *cond = ex_reader_read(tps, cdr);
    Token *tk2 = cdr_read_token(cdr);
    if (!token_is_close_par(tk2))
      EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(tk2)));
    StatCode *st_cd = st_in_read(tps, cdr);
    return stat_while(
      opt_some(cond), st_cd,
      str_f("%s%s%s%s%s",
        js, tk->js, exp_get_js(cond), tk2->js, stat_get_js(stat_code_stat(st_cd))
      )
    );
  }

  if (!strcmp(sym, "if")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Exp *cond = ex_reader_read(tps, cdr);
    Token *tk2 = cdr_read_token(cdr);
    if (!token_is_close_par(tk2))
      EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(tk2)));
    StatCode *st1 = st_in_read(tps, cdr);
    js = str_f("%s%s%s%s%s",
      js, tk->js, exp_get_js(cond), tk2->js, stat_get_js(stat_code_stat(st1))
    );

    // <StatCode>
    Opt *st2 = opt_none();
    if (cdr_next_token_is_else(cdr)) {
      Token *tk = cdr_read_token(cdr);
      StatCode *st = st_in_read(tps, cdr);
      st2 = opt_some(st);
      js = str_f("%s%s%s", js, tk->js, stat_get_js(stat_code_stat(st)));
    }

    return stat_if(cond, st1, st2, js);
  }

  if (!strcmp(sym, "else")) {
    EXC_GENERIC(cdr_fail(cdr, "'else' without 'if'"));
  }

  if (!strcmp(sym, "for")) {
    tps = types_new_block(tps);

    Token *open_pr = cdr_read_token(cdr);
    if (!token_is_open_par(open_pr))
      EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(open_pr)));
    Token *var1 = cdr_read_token(cdr);

    if (token_is_colon(var1)) { // Typed For first variable.
      char *colon_js = str_left(var1->js, -1);
      var1 = cdr_read_token(cdr);
      if (var1->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var1)));
      char *module = var1->value;
      char *module_js = str_left(var1->js, -strlen(module));
      Token *var2 = cdr_read_token(cdr);
      if (var2->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var2)));
      char *v = var2->value;
      types_add(tps, v, module);
      Token *tk_eq = cdr_read_token(cdr);
      if (!token_is_equals(tk_eq))
        EXC_GENERIC(cdr_fail_expect(cdr, "=", token_to_str(tk_eq)));

      Exp *exp = ex_reader_read(tps, cdr);
      Token *close_pr = cdr_read_token(cdr);
      if (!token_is_close_par(close_pr))
        EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));

      StatCode *st_cd = st_in_read(tps, cdr);

      js = str_f("%s%s%s%sconst %s%s of sys.$forObject(%s)%s%s",
        js, open_pr->js, colon_js, module_js,
        var2->js, str_left(tk_eq->js, -1),
        exp_get_js(exp), close_pr->js, stat_get_js(stat_code_stat(st_cd))
      );

      return stat_for(v, exp, st_cd, js);
    }

    if (var1->type != TOKEN_SYMBOL)
      EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var1)));
    char *v1 = var1->value;

    Token *tk_eq = cdr_read_token(cdr);
    if (!token_is_equals(tk_eq)) {
      if (!token_is_comma(tk_eq))
        EXC_GENERIC(cdr_fail_expect(cdr, "= or ,", token_to_str(tk_eq)));

      // For collection 2

      Token *var2 = cdr_read_token(cdr);

      if (token_is_colon(var2)) { // Typed For second variable.
        char *colon_js = str_left(var2->js, -1);
        var2 = cdr_read_token(cdr);
        if (var2->type != TOKEN_SYMBOL)
          EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var2)));
        char *module = var2->value;
        char *module_js = str_left(var2->js, -strlen(module));
        Token *var3 = cdr_read_token(cdr);
        if (var3->type != TOKEN_SYMBOL)
          EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var3)));
        char *v3 = var3->value;
        types_add(tps, v3, module);
        Token *tk_eq2 = cdr_read_token(cdr);
        if (!token_is_equals(tk_eq2))
          EXC_GENERIC(cdr_fail_expect(cdr, "=", token_to_str(tk_eq2)));

        Exp *exp = ex_reader_read(tps, cdr);
        Token *close_pr = cdr_read_token(cdr);
        if (!token_is_close_par(close_pr))
          EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));

        StatCode *st_cd = st_in_read(tps, cdr);

        js = str_f("%s%s%s%sconst [%s,%s]%s%s of sys.$forObject2(%s)%s%s",
          js, open_pr->js, colon_js, module_js,
          var1->js, var3->js,
          str_left(tk_eq->js, -1), str_left(tk_eq2->js, -1),
          exp_get_js(exp), close_pr->js, stat_get_js(stat_code_stat(st_cd))
        );

        return stat_for_ix(v1, v3, exp, st_cd, js);
      }

      if (var2->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(var2)));
      char *v2 = var2->value;

      Token *tk_eq2 = cdr_read_token(cdr);
      if (!token_is_equals(tk_eq2))
        EXC_GENERIC(cdr_fail_expect(cdr, "=", token_to_str(tk_eq2)));

      Exp *exp1 = ex_reader_read(tps, cdr);

      Token *close_pr = cdr_read_token(cdr);
      if (!token_is_close_par(close_pr))
        EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));

      StatCode *st_cd = st_in_read(tps, cdr);

      js = str_f("%s%sconst [%s,%s]%s%s of sys.$forObject2(%s)%s%s",
        js, open_pr->js, var1->js, var2->js,
        str_left(tk_eq->js, -1), str_left(tk_eq2->js, -1),
        exp_get_js(exp1), close_pr->js, stat_get_js(stat_code_stat(st_cd))
      );

      return stat_for_ix(v2, v1, exp1, st_cd, js);
    }

    Exp *exp1 = ex_reader_read(tps, cdr);
    if (cdr_next_token_is_colon(cdr)){ // Range
      Token *colon1 = cdr_read_token(cdr);
      Exp *exp2 = ex_reader_read(tps, cdr);
      Exp *exp3 = NULL;
      Token *colon2 = NULL;
      if (cdr_next_token_is_colon(cdr)){
        colon2 = cdr_read_token(cdr);
        exp3 = ex_reader_read(tps, cdr);
      }

      Token *close_pr = cdr_read_token(cdr);
      if (!token_is_close_par(close_pr))
        EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));

      StatCode *st_cd = st_in_read(tps, cdr);

      if (exp3) {
        js = str_f(
          "%s{const _s=%s;const _f=sys.$forCmp(_s);for",
          str_left(js, -3), str_trim(exp_get_js(exp3))
        );
        js = str_f("%s%slet %s%s%s%s;_f(%s, %s)%s;%s += _s%s%s%s}",
          js, open_pr->js, var1->js, tk_eq->js, exp_get_js(exp1),
          str_left(colon1->js, -1), var1->value,
          exp_get_js(exp2), str_left(colon2->js, -1),
          var1->value, exp_js_replace(exp3, ""), close_pr->js,
          stat_get_js(stat_code_stat(st_cd))
        );
        return stat_for_rs(v1, exp1, exp2, exp3, st_cd, js);
      }

      js = str_f("%s%slet %s%s%s%s;%s < %s; ++%s%s%s",
        js, open_pr->js, var1->js, tk_eq->js, exp_get_js(exp1),
        str_left(colon1->js, -1), var1->value, exp_get_js(exp2),
        var1->value, close_pr->js, stat_get_js(stat_code_stat(st_cd))
      );
      return stat_for_r(v1, exp1, exp2, st_cd, js);

    }

    // For collection

    Token *close_pr = cdr_read_token(cdr);
    if (!token_is_close_par(close_pr))
      EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));

    StatCode *st_cd = st_in_read(tps, cdr);

    js = str_f("%s%sconst %s%s of sys.$forObject(%s)%s%s",
      js, open_pr->js, var1->js, str_left(tk_eq->js, -1),
      exp_get_js(exp1), close_pr->js, stat_get_js(stat_code_stat(st_cd))
    );

    return stat_for(v1, exp1, st_cd, js);
  }

  if (!strcmp(sym, "switch")) {
    Token *open_pr = cdr_read_token(cdr);
    if (!token_is_open_par(open_pr))
      EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(open_pr)));
    Exp *cond = ex_reader_read(tps, cdr);
    Token *close_par = cdr_read_token(cdr);
    if (!token_is_close_par(close_par))
      EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_par)));

    Token *open_br = cdr_read_token(cdr);
    if (!token_is_open_bracket(open_br))
      EXC_GENERIC(cdr_fail_expect(cdr, "{", token_to_str(open_br)));

    Buf *bf = buf_new();
    buf_add(bf, str_f("%s%s%s%s%s",
      js, open_pr->js, exp_get_js(cond), close_par->js, open_br->js
    ));

    // <Tp<Arr<Exp>, StatCode>>
    Arr *cases = arr_new();
    if (!cdr_next_token_is_close_bracket(cdr)) {
      for (;;) {
        Exp *c1 = ex_reader_read(tps, cdr);

        if (exp_is_sym(c1) && !strcmp(exp_get_sym(c1), "default")) {
          Token *colon = cdr_read_token(cdr);
          if (!token_is_colon(colon))
            EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(colon)));
          StatCode *st_cd = st_in_read(tps, cdr);
          arr_push(cases, tp_new(arr_new_from(c1, NULL), st_cd));

          Token *close_br = cdr_read_token(cdr);
          if (!token_is_close_bracket(close_br))
            EXC_GENERIC(cdr_fail_expect(cdr, "}", token_to_str(close_br)));

          buf_add(bf, str_f("%s%s{%s}%s",
            exp_get_js(c1), colon->js, stat_get_js(stat_code_stat(st_cd)),
            close_br->js
          ));

          return stat_switch(cond, cases, buf_str(bf));
        }

        // <Exp>
        Arr *cs = arr_new_from(c1, NULL);
        buf_add(bf, exp_js_insert(c1, "case "));

        Token *colon = cdr_read_token(cdr);
        while (token_is_comma(colon)) {
          buf_add(bf, str_f("%s:", str_left(colon->js, -1)));
          Exp *c = ex_reader_read(tps, cdr);
          arr_push(cs, c);
          buf_add(bf, exp_js_insert(c, "case "));
          colon = cdr_read_token(cdr);
        }

        if (!token_is_colon(colon))
          EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(colon)));

        StatCode *st_cd = st_in_read(tps, cdr);
        arr_push(cases, tp_new(cs, st_cd));

        buf_add(bf, str_f("%s{%sbreak;}",
          colon->js, stat_get_js(stat_code_stat(st_cd))
        ));

        if (cdr_next_token_is_close_bracket(cdr)) break;
      }
    }
    Token *close_br = cdr_read_token(cdr);
    buf_add(bf, close_br->js);

    return stat_switch(cond, cases, buf_str(bf));
  }

  if (token_is_reserved(sym))
    EXC_GENERIC(cdr_fail(cdr, str_f("'%s' is a reserverd symbol", sym)));

  // No resereved symbol
  Exp *exp = pt_sq_pr_reader_read(tps, exp_sym(sym, js), cdr);
  Token *tk = cdr_read_token(cdr);

  if (token_is_semicolon(tk)) {
    if (exp_is_pr(exp))
      return stat_func(exp, str_f("%s%s", exp_get_js(exp), tk->js));
    EXC_GENERIC(cdr_fail_expect(cdr, "Function calling", exp_to_str(exp)));
  }
  if (token_is_colon(tk)) {
    if (!exp_is_sym(exp))
      EXC_GENERIC(cdr_fail_expect(cdr, "symbol", exp_to_str(exp)));
    Buf *bf0 = buf_new();
    buf_add(bf0, str_f("%s%s(",
      exp_js_insert(exp, "function "),
      str_left(tk->js, -1)
    ));

    // <Exp>
    Arr *syms = arr_new();
    arr_push(syms, exp);
    Buf *bf1 = buf_new();
    for (;;) {
      Token *tk = cdr_read_token(cdr);
      if (tk->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
      arr_push(syms, exp_sym(tk->value, tk->js));
      buf_add(bf0, str_trim(tk->js));
      buf_add(bf1, tk->js);

      tk = cdr_read_token(cdr);
      if (token_is_semicolon(tk)) {  // Return <---------------
        return stat_indexed(
          syms,
          str_f("%s) { sys.$params(arguments.length, %d); return [%s]%s}",
            buf_str(bf0), arr_size(syms) - 1, buf_str(bf1), tk->js
          )
        );
      }
      if (!token_is_comma(tk))
         EXC_GENERIC(cdr_fail_expect(cdr, ", or ;", token_to_str(tk)));

      buf_cadd(bf0, ',');
      buf_add(bf1, tk->js);
    }
  }
  if (token_is_comma(tk)) {
    if (!exp_is_sym(exp))
      EXC_GENERIC(cdr_fail_expect(cdr, "symbol", exp_to_str(exp)));
    char *js = str_f("%s%s", exp_get_js(exp), tk->js);
    return read_multi(tps, exp_get_sym(exp), cdr, js);
  }
  if (token_is_assign(tk)) {
    if (token_is_equals(tk)) {
      if (
        !(exp_is_sym(exp) || exp_is_pt(exp) || exp_is_sq(exp))
      ) {
        EXC_GENERIC(cdr_fail(cdr, "Unexpected '='"));
      }
    } else if (
      !(exp_is_pt(exp) || exp_is_sq(exp))
    ) {
      EXC_GENERIC(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
    }

    Exp *exp2 = ex_reader_read(tps, cdr);

    if (token_is_equals(tk) && exp_is_sym(exp)) {
      char *sym = exp_get_sym(exp);
      if (!types_in_block(tps, sym)){ // sym is not typed in the current block
        if (exp_is_string(exp2)) types_add(tps, sym, "str");
        else if (exp_is_array(exp2)) types_add(tps, sym, "arr");
        else if (exp_is_map(exp2)) types_add(tps, sym, "dic");
      }
    }

    Token *tk2 = cdr_read_token(cdr);
    if (!token_is_semicolon(tk2))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk2)));

    char *tkjs = tk->js;
    if (str_ends(tkjs, "&=")) tkjs = str_f("%s&=", str_left(tkjs, -1));
    else if (str_ends(tkjs, "|=")) tkjs = str_f("%s|=", str_left(tkjs, -1));

    char *exp2_js = exp_get_js(exp2);
    char *js;
    if (exp_is_sym(exp)) {
      if (exp_is_function(exp2)) {
        if (!token_is_equals(tk))
          EXC_ILLEGAL_ARGUMENT("Bad operator", "=", token_to_str(tk));
        int ix = str_index(exp2_js, "function") + 8;
        js = str_f("%s%s %s%s%s",
          exp_js_replace(exp, ""),
          str_left(exp2_js, ix),
          exp_get_sym(exp),
          str_right(exp2_js, ix),
          tk2->js
        );
      } else if (exp_can_be_null(exp2)) {
        js = str_f("%s%ssys.$checkNull(%s)%s",
          exp_js_insert(exp, "const "),
          tkjs,
          exp2_js,
          tk2->js
        );
      } else {
        js = str_f("%s%s%s%s",
          exp_js_insert(exp, "const "),
          tkjs,
          exp2_js,
          tk2->js
        );
      }
    } else {
      js = str_f("%s%s%s%s",
        exp_get_js(exp),
        tkjs,
        exp_can_be_null(exp2)
          ? str_f("sys.$checkNull(%s)", exp2_js)
          : exp2_js,
        tk2->js
      );
    }

    return token_is_equals(tk) ? stat_assign(exp, exp2, js)
      : !strcmp(tk->value, "+=") ? stat_add_as(exp, exp2, js)
      : !strcmp(tk->value, "-=") ? stat_sub_as(exp, exp2, js)
      : !strcmp(tk->value, "*=") ? stat_mul_as(exp, exp2, js)
      : !strcmp(tk->value, "/=") ? stat_div_as(exp, exp2, js)
      : !strcmp(tk->value, "%=") ? stat_mod_as(exp, exp2, js)
      : !strcmp(tk->value, "&=") ? stat_and_as(exp, exp2, js)
      : stat_or_as(exp, exp2, js)
    ;
  }

  EXC_GENERIC(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
  return 0; // Unreachable
}

static Stat *read(Types *tps, Cdr *cdr, Token *tk) {
  if (tk->type == TOKEN_SYMBOL)
    return read_symbol(tps, tk, cdr);
  if (tk->type == TOKEN_OPERATOR) {
    char *op = tk->value;
    if (!strcmp(op, "{")) {
      // StatCode
      Arr *sts = reader_read_block(tps, cdr);
      Buf *js = buf_new();
      buf_add(js, tk->js);
      char *close_js = stat_get_js(stat_code_stat(arr_pop(sts))); // Not in kut
      EACH(sts, StatCode, st_cd) {
        buf_add(js, stat_get_js(stat_code_stat(st_cd)));
      }_EACH
      buf_add(js, close_js); // Not in kut
      return stat_block(sts, buf_str(js));
    }
    if (!strcmp(op, "}"))
      return stat_block_close(tk->js);
  }
  EXC_GENERIC(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
  return 0; // Unreachable
}

StatCode *st_reader_read(Types *tps, Cdr *cdr) {
  Token *tk = opt_get(cdr_read_token_op(cdr));
  if (tk) {
    int fix = cdr_get_file(cdr);
    if (tk->type == TOKEN_LINE_COMMENT) {
      if (str_starts(tk->value, "///")) {
        int nline = cdr_get_nline(cdr) - 1;
        return stat_code_new(fix, nline, stat_export(tk->js));
      }
      StatCode *st_cd = st_reader_read(tps, cdr); // Skip comment
      Stat *st = stat_code_stat(st_cd);
      stat_set_js(st, str_f("%s%s", tk->js, stat_get_js(st)));
      return st_cd;
    }
    if (tk->type == TOKEN_COMMENT) {
      StatCode *st_cd = st_reader_read(tps, cdr); // Skip comment
      Stat *st = stat_code_stat(st_cd);
      stat_set_js(st, str_f("%s%s", tk->js, stat_get_js(st)));
      return st_cd;
    }

    if (token_is_colon(tk)) {
      char *js = str_left(tk->js, -1);
      Token *tk_mod = cdr_read_token(cdr);
      if (tk_mod->type != TOKEN_SYMBOL && !token_is_colon(tk_mod))
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol or ':'", token_to_str(tk_mod)));
      js = str_f("%s%s", js, str_left(tk_mod->js, -strlen(tk_mod->value)));
      Token *tk3 = cdr_read_token(cdr);
      if (tk3->type != TOKEN_SYMBOL)
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk3)));
      tk3 = token_symbol(tk3->value, str_f("%s%s", js, tk3->js));
      types_add(tps, tk3->value, token_is_colon(tk_mod) ? "" : tk_mod->value);

      int nline = cdr_get_nline(cdr);
      Stat *st = read(tps, cdr, tk3);
      if (stat_is_assign(st) && !exp_is_sym(tp_e1(stat_get_assign(st))))
        EXC_GENERIC(cdr_fail_expect_line(
          cdr, "assignation to symbol", stat_to_str(st), nline
        ));
      if (
        !stat_is_assign(st) &&
        !stat_is_arr_multi(st) && !stat_is_dic_multi(st)
      )
        EXC_GENERIC(cdr_fail_expect_line(
          cdr, "assignation", stat_to_str(st), nline
        ));

      return stat_code_new(fix, nline, st);
    }

    int nline = cdr_get_nline(cdr);
    if (token_is_comma(tk))
      return stat_code_new(fix, nline, read_multi(tps, "", cdr, tk->js));

    return stat_code_new(fix, nline, read(tps, cdr, tk));
  }
  return stat_code_new(cdr_get_file(cdr), cdr_get_nline(cdr), stat_end());
}
