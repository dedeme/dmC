// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/st_reader.h"
#include "kut/path.h"
#include "fileix.h"
#include "symix.h"
#include "reader/reader.h"
#include "reader/ex_reader.h"
#include "reader/pt_sq_pr_reader.h"

static StatCode *st_in_read(Cdr *cdr) {
  int nline = cdr_get_next_nline(cdr);
  StatCode *r = st_reader_read(cdr);
  Stat *st = stat_code_stat(r);
  if (stat_is_block_close(st))
    EXC_KUT(cdr_fail_line(cdr, "Unexpected '}'", nline));
  if (stat_is_import(st))
    EXC_KUT(cdr_fail_line(cdr, "'import' out of main block", nline));
  if (stat_is_end(st))
    EXC_KUT(cdr_fail(cdr, "Unexpected end of text"));

  return r;
}

static Stat *read_symbol(int sym, Cdr *cdr) {
  if (sym == symix_BREAK) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_break();
  }

  if (sym == symix_CONTINUE) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    return stat_continue();
  }

  if (sym == symix_TRACE) {
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

  if (sym == symix_RETURN) {
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

  if (sym == symix_IMPORT) {
    Token *tk = cdr_read_token(cdr);
    if (tk->type != TOKEN_STRING)
      EXC_KUT(cdr_fail_expect(cdr, "string", token_to_str(tk)));

    char *mod = tk->value;
    if (*mod == '/')
      EXC_KUT(cdr_fail(cdr, "Module path starts with '/'"));
    if (str_ends(mod, "/"))
      EXC_KUT(cdr_fail(cdr, "Module path ends with '/'"));
    if (str_index(mod, ".") != -1)
      EXC_KUT(cdr_fail(cdr, "Module path contains '.'"));
    if (!*mod)
      EXC_KUT(cdr_fail(cdr, "Module path is empty"));

    tk = cdr_read_token(cdr);
    int id;
    if (token_is_colon(tk)) {
      tk = cdr_read_token(cdr);
      if (tk->type != TOKEN_SYMBOL)
        EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
      id = tk->b;
      tk = cdr_read_token(cdr);
    } else {
      id = symix_add(path_base(mod));
    }

    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));

    return stat_import(mod, id);
  }

  if (sym == symix_TRY) {
    StatCode *st1 = st_in_read(cdr);
    Token *tk = cdr_read_token(cdr);
    if (!token_is_catch(tk))
      EXC_KUT(cdr_fail_expect(cdr, "catch", token_to_str(tk)));
    tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Token *var = cdr_read_token(cdr);
    if (var->type != TOKEN_SYMBOL)
      EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(var)));
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
    StatCode *st2 = st_in_read(cdr);
    // <StatCode>
    Opt *st3 = opt_none();
    if (cdr_next_token_is_finally(cdr)) {
      cdr_read_token(cdr);
      st3 = opt_some(st_in_read(cdr));
    }
    return stat_try(st1, var->b, st2, st3);
  }

  if (sym == symix_CATCH) {
    EXC_KUT(cdr_fail(cdr, "'catch' without 'try'"));
  }

  if (sym == symix_FINALLY) {
    EXC_KUT(cdr_fail(cdr, "'finally' without 'try'"));
  }

  if (sym == symix_WHILE) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    if (cdr_next_token_is_close_parenthesis(cdr)) {
      cdr_read_token(cdr);
      return stat_while(exp_empty(), st_in_read(cdr));
    }

    Exp *cond = ex_reader_read(cdr);
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
    return stat_while(cond, st_in_read(cdr));
  }

  if (sym == symix_IF) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Exp *cond = ex_reader_read(cdr);
    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
    StatCode *st1 = st_in_read(cdr);

    // <StatCode>
    Opt *st2 = opt_none();
    if (cdr_next_token_is_else(cdr)) {
      cdr_read_token(cdr);
      st2 = opt_some(st_in_read(cdr));
    }

    return stat_if(cond, st1, st2);
  }

  if (sym == symix_ELSE) {
    EXC_KUT(cdr_fail(cdr, "'else' without 'if'"));
  }

  if (sym == symix_FOR) {
    Token *tk = cdr_read_token(cdr);
    if (!token_is_open_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));
    Token *var1 = cdr_read_token(cdr);
    if (var1->type != TOKEN_SYMBOL)
      EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(var1)));
    int v1 = var1->b;

    int v2 = -1;
    if (cdr_next_token_is_comma(cdr)){
      cdr_read_token(cdr);
      Token *var2 = cdr_read_token(cdr);
      if (var2->type != TOKEN_SYMBOL)
        EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(var2)));
      v2 = var2->b;
      if (v1 == v2)
        EXC_KUT(str_f("For variables are equals (%s)", symix_get(v1)));
    }

    tk = cdr_read_token(cdr);
    if (!token_is_equals(tk))
      EXC_KUT(cdr_fail_expect(cdr, "=", token_to_str(tk)));

    Exp *exp1 = ex_reader_read(cdr);
    if (cdr_next_token_is_colon(cdr)){ // Range
      cdr_read_token(cdr);
      if (v2 != -1)
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
        ? stat_for_rs(v1, exp1, exp2, exp3, st_in_read(cdr))
        : stat_for_r(v1, exp1, exp2, st_in_read(cdr))
      ;
    }

    // For collection

    tk = cdr_read_token(cdr);
    if (!token_is_close_par(tk))
      EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));

    return v2 != -1
      ? stat_for_ix(v1, v2, exp1, st_in_read(cdr))
      : stat_for(v1, exp1, st_in_read(cdr))
    ;
  }

  if (sym == symix_SWITCH) {
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
    // <Tp<Arr<Exp>, StatCode>>
    Arr *cases = arr_new();
    if (!cdr_next_token_is_close_bracket(cdr)) {
      for (;;) {
        Exp *e = ex_reader_read(cdr);
        // Exp
        Arr *conds = arr_new_from(e, NULL);
        if (!exp_is_sym(e) || exp_get_sym(e) != symix_DEFAULT) {
          while (cdr_next_token_is_comma(cdr)) {
            cdr_read_token(cdr);
            Exp *exp = ex_reader_read(cdr);
            if (exp_is_sym(exp) && exp_get_sym(exp) == symix_DEFAULT)
              EXC_KUT(cdr_fail(cdr, "Unexpected 'default'"));
            arr_push(conds, exp);
          }
        }

        tk = cdr_read_token(cdr);
        if (!token_is_colon(tk))
          EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));

        arr_push(cases, tp_new(conds, st_in_read(cdr)));
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
    if (exp_is_pr(exp))
      return stat_func(exp);
    EXC_KUT(cdr_fail_expect(cdr, "Function calling", exp_to_js(exp)));
  }
  if (token_is_assign(tk)) {
    if (token_is_equals(tk)) {
      if (
        !(exp_is_sym(exp) || exp_is_pt(exp) || exp_is_sq(exp))
      ) {
        EXC_KUT(cdr_fail(cdr, "Unexpected '='"));
      }
    } else if (
      !(exp_is_pt(exp) || exp_is_sq(exp))
    ) {
      EXC_KUT(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
    }

    Exp *exp2 = ex_reader_read(cdr);

    Token *tk2 = cdr_read_token(cdr);
    if (!token_is_semicolon(tk2))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk2)));

    return token_is_equals(tk) ? stat_assign(exp, exp2)
      : !strcmp(tk->value, "+=") ? stat_add_as(exp, exp2)
      : !strcmp(tk->value, "-=") ? stat_sub_as(exp, exp2)
      : !strcmp(tk->value, "*=") ? stat_mul_as(exp, exp2)
      : !strcmp(tk->value, "/=") ? stat_div_as(exp, exp2)
      : !strcmp(tk->value, "%=") ? stat_mod_as(exp, exp2)
      : !strcmp(tk->value, "&=") ? stat_and_as(exp, exp2)
      : stat_or_as(exp, exp2)
    ;
  }

  EXC_KUT(cdr_fail(cdr, str_f("Unexpected '%s'", token_to_str(tk))));
  return 0; // Unreachable
}

static Stat *read(Cdr *cdr, Token *tk) {
  if (tk->type == TOKEN_SYMBOL)
    return read_symbol(tk->b, cdr);
  if (tk->type == TOKEN_OPERATOR) {
    char *op = tk->value;
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
    if (exp_is_pr(exp)) {
      Token *tk = cdr_read_token(cdr);
      if (!token_is_semicolon(tk))
        EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
      return stat_code_new(cdr_get_file(cdr), nline, stat_func(exp));
    }
    EXC_KUT(cdr_fail(cdr, "Bad statement"));
  }
  Token *tk = opt_get(cdr_read_token_op(cdr));
  if (tk) {
    if (tk->type == TOKEN_LINE_COMMENT) {
      if (str_starts(tk->value, "///")) {
        int fix = cdr_get_file(cdr);
        int nline = cdr_get_nline(cdr) - 1;
        return stat_code_new(fix, nline, stat_export());
      }
      return st_reader_read(cdr); // Skip comment
    }
    if (tk->type == TOKEN_COMMENT)
      return st_reader_read(cdr); // Skip comment
    int fix = cdr_get_file(cdr);
    int nline = cdr_get_nline(cdr);
    return stat_code_new(fix, nline, read(cdr, tk));
  }
  return stat_code_new(cdr_get_file(cdr), cdr_get_nline(cdr), stat_end());
}
