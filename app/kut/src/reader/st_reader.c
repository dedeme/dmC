// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/st_reader.h"
#include "kut/path.h"
#include "DEFS.h"
#include "fileix.h"
#include "reader/reader.h"
#include "reader/ex_reader.h"
#include "reader/pt_sq_pr_reader.h"

static Stat *read_symbol(char *sym, Cdr *cdr) {
  if (!strcmp(sym, "break")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_break();
  }

  if (!strcmp(sym, "continue")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_continue();
  }

  if (!strcmp(sym, "trace")) {
    int is_complete = FALSE;
    if (cdr_next_token_is_colon(cdr)) {
      cdr_read_token(cdr);
      is_complete = TRUE;
    }
    Exp *exp = ex_reader_read(cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_trace(is_complete, exp);
  }

  if (!strcmp(sym, "return")) {
    if (cdr_next_token_is_semicolon(cdr)) {
      cdr_read_token(cdr);
      return stat_return(exp_empty());
    }
    Exp *exp = ex_reader_read(cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_return(exp);
  }

  if (!strcmp(sym, "import")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_string(tk))
      EXC_KUT(cdr_fail_expect(cdr, "string", token_to_str(tk)));
    char *mod = token_get_string(tk);
    if (*mod == '/')
      EXC_KUT(cdr_fail(cdr, "Module path starts with '/'"));
    if (str_ends(mod, "/"))
      EXC_KUT(cdr_fail(cdr, "Module path ends with '/'"));
    if (str_index(mod, ".") != -1)
      EXC_KUT(cdr_fail(cdr, "Module path contains '.'"));
    if (!*mod)
      EXC_KUT(cdr_fail(cdr, "Module path is empty"));
    int fix = fileix_add(cdr_get_file(cdr), mod);
    if (fix == -1)
      EXC_KUT(cdr_fail(cdr, str_f("Module '%s' not found", mod)));

    tk = cdr_read_token(cdr);
    char *id;
    if (token_is_colon(tk)) {
      tk = cdr_read_token(cdr);
      if (!token_is_symbol(tk))
        EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
      id = token_get_symbol(tk);
      tk = cdr_read_token(cdr);
    } else {
      id = path_base(mod);
    }

    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));

    return stat_import(fix, id);
  }

  if (!strcmp(sym, "try")) {
    StatCode *st1 = st_reader_read(cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_catch(tk))
      EXC_KUT(cdr_fail_expect(cdr, "catch", token_to_str(tk)));
    tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Token *var = cdr_read_token(cdr);
    if (!token_is_symbol(var))
      EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(var)));
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
    StatCode *st2 = st_reader_read(cdr);
    // <StatCode>
    Opt *st3 = opt_none();
    if (cdr_next_token_is_finally(cdr)) {
      cdr_read_token(cdr);
      st3 = opt_some(st_reader_read(cdr));
    }
    return stat_try(st1, token_get_symbol(var), st2, st3);
  }

  if (!strcmp(sym, "catch")) {
    EXC_KUT(cdr_fail(cdr, "'catch' without 'try'"));
  }

  if (!strcmp(sym, "finally")) {
    EXC_KUT(cdr_fail(cdr, "'finally' without 'try'"));
  }

  if (!strcmp(sym, "while")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    if (cdr_next_token_is_close_parenthesis(cdr)) {
      cdr_read_token(cdr);
      return stat_while(exp_empty(), st_reader_read(cdr));
    }

    Exp *cond = ex_reader_read(cdr);
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
    return stat_while(cond, st_reader_read(cdr));
  }

  if (!strcmp(sym, "if")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Exp *cond = ex_reader_read(cdr);
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
    StatCode *st1 = st_reader_read(cdr);

    // <StatCode>
    Opt *st2 = opt_none();
    if (cdr_next_token_is_else(cdr)) {
      cdr_read_token(cdr);
      st2 = opt_some(st_reader_read(cdr));
    }

    return stat_if(cond, st1, st2);
  }

  if (!strcmp(sym, "else")) {
    EXC_KUT(cdr_fail(cdr, "'else' without 'if'"));
  }

  if (!strcmp(sym, "for")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Token *var1 = cdr_read_token(cdr);
    if (!token_is_symbol(var1))
      EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(var1)));
    char *v1 = token_get_symbol(var1);

    char *v2 = "";
    if (cdr_next_token_is_comma(cdr)){
      cdr_read_token(cdr);
      Token *var2 = cdr_read_token(cdr);
      if (!token_is_symbol(var2))
        EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(var2)));
      v2 = token_get_symbol(var2);
      if (!strcmp(v1, v2))
        EXC_KUT(str_f("For variables are equals (%s)", v1));
    }

    tk = cdr_read_token(cdr);
    if (!token_is_equals(tk))
      EXC_KUT(cdr_fail_expect(cdr, "=", token_to_str(tk)));

    Exp *exp1 = ex_reader_read(cdr);

    if (cdr_next_token_is_colon(cdr)){ // Range
      cdr_read_token(cdr);
      if (*v2)
        EXC_KUT(cdr_fail(cdr, "Ranges are not allowed in 'for (i, e = ...)'"));

      Exp *exp2 = ex_reader_read(cdr);
      Exp *exp3 = NULL;
      if (cdr_next_token_is_colon(cdr)){
        cdr_read_token(cdr);
        exp3 = ex_reader_read(cdr);
      }

      tk = cdr_read_token(cdr);
      if (!token_is_close_par(tk))
        EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
      return exp3
        ? stat_for_rs(
            token_get_symbol(var1), exp1, exp2, exp3, st_reader_read(cdr)
          )
        : stat_for_r(
            token_get_symbol(var1), exp1, exp2, st_reader_read(cdr)
          )
      ;
    }

    // For collection

    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));

    return *v2
      ? stat_for_ix(v1, v2, exp1, st_reader_read(cdr))
      : stat_for(v1, exp1, st_reader_read(cdr))
    ;
  }

  if (!strcmp(sym, "switch")) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Exp *exp = ex_reader_read(cdr);
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));

    tk = cdr_read_token(cdr);
    if (!token_is_open_bracket(tk))
      EXC_KUT(cdr_fail_expect(cdr, "{", token_to_str(tk)));
    // <Tp<Exp, StatCode>>
    Arr *cases = arr_new();
    if (!cdr_next_token_is_close_bracket(cdr)) {
      for (;;) {
        Exp *e = ex_reader_read(cdr);
        tk = cdr_read_token(cdr);
        if (!token_is_colon(tk))
          EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));
        arr_push(cases, tp_new(e, st_reader_read(cdr)));
        if (cdr_next_token_is_close_bracket(cdr)) break;
      }
    }
    cdr_read_token(cdr);

    return stat_switch(exp, cases);
  }

  // No resereved symbol

  Exp *exp = pt_sq_pr_reader_read(exp_sym(sym), cdr);
  Token *tk = cdr_read_token(cdr);

  if (token_is_semicolon(tk)) {
    if (exp_is_function_call(exp))
      return stat_func(exp);
    EXC_KUT(cdr_fail_expect(cdr, "Function calling", exp_to_js(exp)));
  }
  if (token_is_assign(tk)) {
    if (token_is_equals(tk)) {
      if (
        !(exp_is_sym(exp) || exp_is_pt(exp) || exp_is_pr(exp) ||
          exp_is_slice(exp) || exp_is_sq(exp))
      ) {
        EXC_KUT(cdr_fail(cdr, "Unexpected '='"));
      }
    } else if (
      !(exp_is_pt(exp) || exp_is_pr(exp) || exp_is_slice(exp) || exp_is_sq(exp))
    ) {
      EXC_KUT(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
    }

    Exp *exp2 = ex_reader_read(cdr);

    Token *tk2 = cdr_read_token(cdr);
    if (!token_is_semicolon(tk2))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk2)));

    return token_is_equals(tk) ? stat_assign(exp, exp2)
      : !strcmp(token_get_operator(tk), "+=") ? stat_add_as(exp, exp2)
      : !strcmp(token_get_operator(tk), "-=") ? stat_sub_as(exp, exp2)
      : !strcmp(token_get_operator(tk), "*=") ? stat_mul_as(exp, exp2)
      : !strcmp(token_get_operator(tk), "/=") ? stat_div_as(exp, exp2)
      : !strcmp(token_get_operator(tk), "%=") ? stat_mod_as(exp, exp2)
      : !strcmp(token_get_operator(tk), "&=") ? stat_and_as(exp, exp2)
      : stat_or_as(exp, exp2)
    ;
  }

  EXC_KUT(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
  return 0; // Unreachable
}

static Stat *read(Cdr *cdr, Token *tk) {
  if (token_is_symbol(tk))
    return read_symbol(token_get_symbol(tk), cdr);
  if (token_is_operator(tk)) {
    char *op = token_get_operator(tk);
    if (!strcmp(op, ";"))
      return stat_empty();
    if (!strcmp(op, "{"))
      return stat_block(reader_read_block(cdr));
    if (!strcmp(op, "}"))
      return stat_block_close();
  }
  EXC_KUT(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
  return 0; // Unreachable
}

StatCode *st_reader_read(Cdr *cdr) {
  if (cdr_next_token_is_backslash(cdr)) {
    int nline = cdr_get_next_nline(cdr);
    Exp *exp = ex_reader_read1(cdr);
    exp = pt_sq_pr_reader_read(exp, cdr);
    if (exp_is_function_call(exp)) {
      Token *tk = cdr_read_token(cdr);
      if (!token_is_semicolon(tk))
        EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
      return stat_code_new(cdr_get_file(cdr), nline, stat_func(exp));
    }
    EXC_KUT(cdr_fail(cdr, "Bad statement"));
  }
  Token *tk = opt_get(cdr_read_token_op(cdr));
  if (tk) {
    if (token_is_comment(tk) || token_is_line_comment(tk))
      return st_reader_read(cdr); // Skip comments
    int fix = cdr_get_file(cdr);
    int nline = cdr_get_nline(cdr);
    return stat_code_new(fix, nline, read(cdr, tk));
  }
  return stat_code_new(cdr_get_file(cdr), cdr_get_nline(cdr), stat_end());
}
