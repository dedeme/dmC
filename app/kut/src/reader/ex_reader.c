// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/ex_reader.h"
#include "function.h"
#include "typed/tfunction.h"
#include "symix.h"
#include "reader/pt_sq_pr_reader.h"
#include "reader/st_reader.h"

/// Return and exps are [Exp, Token, Exp, Token ..., Exp] with at least one element.
static Arr *solve_level1 (Arr *exps) {
  if (arr_size(exps) == 1) return exps;

  // [Exp, Token, Exp, Token ..., Exp] with at least one element.
  Arr *new_exps = arr_new();
  arr_push(new_exps, arr_get(exps, 0));

  for (int i = 1; i < arr_size(exps); i += 2) {
    Token *tk = arr_get(exps, i);
    if (token_is_binary1(tk)) {
      int sz1 = arr_size(new_exps) - 1;
      Exp *last = arr_get(new_exps, sz1);
      if (*((char *)tk->value) == '*')
        arr_set(new_exps, sz1, exp_mul(last, arr_get(exps, i + 1)));
      else if (*((char *)tk->value) == '/')
        arr_set(new_exps, sz1, exp_div(last, arr_get(exps, i + 1)));
      else
        arr_set(new_exps, sz1, exp_mod(last, arr_get(exps, i + 1)));
    } else {
      arr_push(new_exps, tk);
      arr_push(new_exps, arr_get(exps, i + 1));
    }
  }
  return new_exps;
}

/// Return and exps are [Exp, Token, Exp, Token ..., Exp] with at least one element.
static Arr *solve_level2 (Arr *exps) {
  if (arr_size(exps) == 1) return exps;

  // [Exp, Token, Exp, Token ..., Exp] with at least one element.
  Arr *new_exps = arr_new();
  arr_push(new_exps, arr_get(exps, 0));

  for (int i = 1; i < arr_size(exps); i += 2) {
    Token *tk = arr_get(exps, i);
    if (token_is_binary2(tk)) {
      int sz1 = arr_size(new_exps) - 1;
      Exp *last = arr_get(new_exps, sz1);
      if (*((char *)tk->value) == '+')
        arr_set(new_exps, sz1, exp_add(last, arr_get(exps, i + 1)));
      else
        arr_set(new_exps, sz1, exp_sub(last, arr_get(exps, i + 1)));
    } else {
      arr_push(new_exps, tk);
      arr_push(new_exps, arr_get(exps, i + 1));
    }
  }
  return new_exps;
}

/// Return and exps are [Exp, Token, Exp, Token ..., Exp] with at least one element.
static Arr *solve_level3 (Arr *exps) {
  if (arr_size(exps) == 1) return exps;

  // [Exp, Token, Exp, Token ..., Exp] with at least one element.
  Arr *new_exps = arr_new();
  arr_push(new_exps, arr_get(exps, 0));

  for (int i = 1; i < arr_size(exps); i += 2) {
    Token *tk = arr_get(exps, i);
    if (token_is_binary3(tk)) {
      int sz1 = arr_size(new_exps) - 1;
      Exp *last = arr_get(new_exps, sz1);
      if (!strcmp((char *)tk->value, "=="))
        arr_set(new_exps, sz1, exp_eq(last, arr_get(exps, i + 1)));
      else if (!strcmp((char *)tk->value, "!="))
        arr_set(new_exps, sz1, exp_neq(last, arr_get(exps, i + 1)));
      else if (!strcmp((char *)tk->value, ">"))
        arr_set(new_exps, sz1, exp_greater(last, arr_get(exps, i + 1)));
      else if (!strcmp((char *)tk->value, ">="))
        arr_set(new_exps, sz1, exp_greater_eq(last, arr_get(exps, i + 1)));
      else if (!strcmp((char *)tk->value, "<"))
        arr_set(new_exps, sz1, exp_less(last, arr_get(exps, i + 1)));
      else
        arr_set(new_exps, sz1, exp_less_eq(last, arr_get(exps, i + 1)));
    } else {
      arr_push(new_exps, tk);
      arr_push(new_exps, arr_get(exps, i + 1));
    }
  }
  return new_exps;
}

/// Return and exps are [Exp, Token, Exp, Token ..., Exp] with at least one element.
static Arr *solve_level4 (Arr *exps) {
  if (arr_size(exps) == 1) return exps;

  // [Exp, Token, Exp, Token ..., Exp] with at least one element.
  Arr *new_exps = arr_new();
  arr_push(new_exps, arr_get(exps, 0));

  for (int i = 1; i < arr_size(exps); i += 2) {
    Token *tk = arr_get(exps, i);
    if (token_is_binary4(tk)) {
      int sz1 = arr_size(new_exps) - 1;
      Exp *last = arr_get(new_exps, sz1);
      if (*((char *)tk->value) == '&')
        arr_set(new_exps, sz1, exp_and(last, arr_get(exps, i + 1)));
      else
        arr_set(new_exps, sz1, exp_or(last, arr_get(exps, i + 1)));
    } else {
      arr_push(new_exps, tk);
      arr_push(new_exps, arr_get(exps, i + 1));
    }
  }
  return new_exps;
}

static Exp *read_switch (Types *tps, Cdr *cdr) {
  Token *tk = cdr_read_token(cdr);

  if (!token_is_open_par(tk))
    EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));

  Exp *cond = ex_reader_read(tps, cdr);
  tk = cdr_read_token(cdr);

  if (!token_is_close_par(tk))
    EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));

  tk = cdr_read_token(cdr);

  if (!token_is_open_bracket(tk))
    EXC_KUT(cdr_fail_expect(cdr, "{", token_to_str(tk)));

  // <Tp<Arr<Exp>, Exp>>
  Arr *cases = arr_new();
  for (;;) {
    if (cdr_next_token_is_close_bracket(cdr))
      EXC_KUT(cdr_fail(cdr, "'default' case is missing"));

    Exp *c1 = ex_reader_read(tps, cdr);

    if (exp_is_sym(c1) && exp_get_sym(c1) == symix_DEFAULT) {
      Token *tk = cdr_read_token(cdr);

      if (!token_is_colon(tk))
        EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));

      arr_push(cases, tp_new(arr_new_from(c1, NULL), ex_reader_read(tps, cdr)));

      tk = cdr_read_token(cdr);
      if (!token_is_semicolon(tk))
        EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));

      tk = cdr_read_token(cdr);
      if (!token_is_close_bracket(tk))
        EXC_KUT(cdr_fail_expect(cdr, "}", token_to_str(tk)));

      return exp_switch(cond, cases);
    }

    //Exp
    Arr *cs = arr_new_from(c1, NULL);
    while (cdr_next_token_is_comma(cdr)) {
      cdr_read_token(cdr);
      Exp *exp = ex_reader_read(tps, cdr);
      if (exp_is_sym(exp) && exp_get_sym(exp) == symix_DEFAULT)
        EXC_KUT(cdr_fail(cdr, "Unexpected 'default'"));
      arr_push(cs, exp);
    }

    Token *tk = cdr_read_token(cdr);

    if (!token_is_colon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));

    arr_push(cases, tp_new(cs, ex_reader_read(tps, cdr)));

    tk = cdr_read_token(cdr);

    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
  }
}

// exps is [Exp, Token, Exp, Token ..., Exp] -> at least one element is added.
static void add_expressions (Types *tps, Arr *exps, Cdr *cdr) {
  Exp *exp = ex_reader_read1(tps, cdr);
  exp = pt_sq_pr_reader_read(tps, exp, cdr);
  arr_push(exps, exp);
  if (cdr_next_token_is_binary(cdr)) {
    arr_push(exps, cdr_read_token(cdr));
    add_expressions(tps, exps, cdr);
  }
}

Exp *ex_reader_read (Types *tps, Cdr *cdr) {
  // <Exp>
  Arr *exs = arr_new();
  add_expressions(tps, exs, cdr);

  exs = solve_level1(exs); // * / %
  exs = solve_level2(exs); // + -
  exs = solve_level3(exs); // == != > >= < <=
  exs = solve_level4(exs); // & |

  Exp *exp = arr_get(exs, 0);

  if (cdr_next_token_is_ternary(cdr)) {
    cdr_read_token(cdr);
    Exp *exp2 = ex_reader_read(tps, cdr);
    Token *tk = cdr_read_token(cdr);
    if (token_is_colon(tk)) {
      exp = exp_ternary(exp, exp2, ex_reader_read(tps, cdr));
    } else {
      EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));
    }
  }
  return exp;
}

Exp *ex_reader_read1 (Types *tps, Cdr *cdr) {
  Token *tk = cdr_read_token(cdr);
  if (token_is_open_par(tk)) { // ( --------------------------------------------
    Exp *exp = ex_reader_read(tps, cdr);
    tk = cdr_read_token(cdr);
    if (token_is_close_par(tk))
      return exp;
    EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
  }

  if (token_is_unary(tk)) { // - ! ---------------------------------------------
    Exp *exp = ex_reader_read1(tps, cdr);
    exp = pt_sq_pr_reader_read(tps, exp, cdr);
    return token_is_exclamation(tk)
      ? exp_not(exp)
      : exp_minus(exp)
    ;
  }

  if (token_is_open_square(tk)) { // [ ------------------------------------------
    if (cdr_next_token_is_close_square(cdr)) { // Empty array
      cdr_read_token(cdr);
      return exp_array(arr_new());
    }

    Exp *exp = ex_reader_read(tps, cdr);


    if (cdr_next_token_is_colon(cdr)) { // Range
      Token *tk = cdr_read_token(cdr);
      Exp *exp2 = ex_reader_read(tps, cdr);
      Exp *exp3 = exp_empty();
      if (cdr_next_token_is_colon(cdr)) {
        cdr_read_token(cdr);
        exp3 = ex_reader_read(tps, cdr);
      }
      tk = cdr_read_token(cdr);
      if (token_is_close_square(tk))
        return exp_range(exp, exp2, exp3);
      EXC_KUT(cdr_fail_expect(cdr, "]", token_to_str(tk)));
    }
    // Array

    // <Exp>
    Arr *exps = arr_new();
    arr_push(exps, exp);

    for (;;) {
      Token *tk = cdr_read_token(cdr);

      if (token_is_close_square(tk)) break;
      else if (token_is_comma(tk)) arr_push(exps, ex_reader_read(tps, cdr));
      else EXC_KUT(cdr_fail_expect(cdr, "']' or ','", token_to_str(tk)));
    }

    return exp_array(exps);
  }

  if (token_is_open_bracket(tk)) { // { ----------------------------------------
    if (cdr_next_token_is_close_bracket(cdr)) {
      cdr_read_token(cdr);
      return exp_dic(map_new());
    }

    // <Exp>
    Map *mp = map_new();

    for (;;) {
      Exp *exp = ex_reader_read(tps, cdr);
      char *key;
      if (exp_is_sym(exp)) key = symix_get(exp_get_sym(exp));
      else if (exp_is_string(exp)) key = exp_get_string(exp);
      else EXC_KUT(cdr_fail_expect(cdr, "symbol or string", exp_to_js(exp)));

      if (map_has_key(mp, key))
        EXC_KUT(cdr_fail(cdr, str_f("Key '%s' is duplicated", key)));

      tk = cdr_read_token(cdr);

      if (token_is_comma(tk) || token_is_close_bracket(tk)) {
        if (exp_is_sym(exp)) {
          map_put(mp, key, exp);
        } else if (exp_is_string(exp)) {
          EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));
        } else {
          EXC_KUT(cdr_fail_expect(cdr, "symbol or string", exp_to_js(exp)));
        }
        if (token_is_comma(tk)) continue;
        else break;
      }

      if (token_is_colon(tk)) {
        map_put(mp, key, ex_reader_read(tps, cdr));
        tk = cdr_read_token(cdr);
        if (token_is_comma(tk)) continue;
        if (token_is_close_bracket(tk)) break;
        EXC_KUT(cdr_fail_expect(cdr, "':' or ','", token_to_str(tk)));
      }

      EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));
    }

    return exp_dic(mp);
  }

  if (token_is_backslash(tk)) { // \ -------------------------------------------
    tps = types_new_block(tps);
    // <Token>
    Arr *pars = arr_new();
    Iarr *atypes = NULL;
    Ttype rt_tp = ttype_ERROR;
    int isTyped = FALSE;
    if (cdr_next_token_is_arrow(cdr)) {
      cdr_read_token(cdr);
    } else {
      if (cdr_next_token_is_open_parenthesis(cdr)) {
        isTyped = TRUE;
        cdr_read_token(cdr);

        Token *tk = cdr_read_token(cdr);
        if (tk->type == TOKEN_SYMBOL) {
          char *stypes = symix_get(tk->b);
          int len = strlen(stypes);
          int *itypes = ATOMIC(sizeof(int) * len);
          char *ps = stypes;
          int *pi = itypes;
          while (*ps) {
            Ttype tp = ttype_from_sym(*ps++);
            if (tp == ttype_ERROR || tp == ttype_NO_RETURN)
              EXC_KUT(cdr_fail_expect(cdr, "type", stypes));
            *pi++ = tp;
          }
          atypes = iarr_new(len, itypes);
          tk = cdr_read_token(cdr);
        } else {
          atypes = iarr_new(0, ATOMIC(sizeof(int) * 0));
        }

        if (tk->type != TOKEN_OPERATOR || strcmp(tk->value, "|"))
          EXC_KUT(cdr_fail_expect(cdr, "|", token_to_str(tk)));

        tk = cdr_read_token(cdr);
        if (token_is_close_par(tk)) {
          rt_tp = ttype_NO_RETURN;
        } else {
          if (tk->type != TOKEN_SYMBOL)
            EXC_KUT(cdr_fail_expect(
              cdr, "typed return type", token_to_str(tk)
            ));

          char *rtype = symix_get(tk->b);
          if (strlen(rtype) != 1)
            EXC_KUT(cdr_fail_expect(cdr, "typed return type", rtype));
          rt_tp = ttype_from_sym(*rtype);
          if (rt_tp == ttype_ERROR)
            EXC_KUT(cdr_fail_expect(cdr, "typed return type", rtype));

          tk = cdr_read_token(cdr);

          if (!token_is_close_par(tk)) {
            EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
          }
        }
      }
      if (isTyped && cdr_next_token_is_arrow(cdr)) {
        cdr_read_token(cdr);
      } else {
        for(;;) {
          Token *tk = cdr_read_token(cdr);
          int module = -1;
          if (token_is_colon(tk)) {
            tk = cdr_read_token(cdr);
            if (tk->type != TOKEN_SYMBOL)
              EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
            module = tk->b;
            tk = cdr_read_token(cdr);
          }
          if (tk->type == TOKEN_SYMBOL) {
            if (module != -1)
              types_add(tps, tk->b, module);
            arr_push(pars, tk);
            tk = cdr_read_token(cdr);
            if (token_is_comma(tk)) continue;
            if (token_is_arrow(tk)) break;
            EXC_KUT(cdr_fail_expect(cdr, "'->' or ','", token_to_str(tk)));
          }
          EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
        }
      }
    }
    int n = arr_size(pars);
    int *ints = ATOMIC(n * sizeof(int));
    int *p = ints;
    EACH(pars, Token, tk) {
      int s = tk->b;
      int *p2 = ints;
      while (p2 < p) {
        if (s == *p2++)
          EXC_KUT(cdr_fail(
            cdr, str_f("Duplicate parameter '%s'", symix_get(s))
          ));
      }
      *p++ = s;
    }_EACH

    if (isTyped) {
      if (n != iarr_size(atypes))
        EXC_KUT(cdr_fail(cdr, "Number of types and parameters does not match"));
      return exp_tfunction(tfunction_new(
        atypes, rt_tp, iarr_new(n, ints), st_reader_read(tps, cdr)
      ));
    }

    return exp_function(function_new(
      iarr_new(n, ints), st_reader_read(tps, cdr)
    ));
  }

  // Others --------------------------------------------------------------------

  if (tk->type == TOKEN_LINE_COMMENT || tk->type == TOKEN_COMMENT)
    return ex_reader_read1(tps, cdr);

  if (tk -> type == TOKEN_BOOL)
    return exp_bool(tk->b);

  if (tk -> type == TOKEN_INT)
    return exp_int(tk->i);

  if (tk -> type == TOKEN_FLOAT)
    return exp_float(tk->d);

  if (tk -> type == TOKEN_STRING)
    return exp_string(tk->value);

  if (tk->type == TOKEN_SYMBOL) {
    int sym = tk->b;
    return sym == symix_SWITCH
      ? read_switch(tps, cdr)
      : exp_sym(sym)
    ;
  }

  EXC_KUT(cdr_fail_expect(cdr, "Expression", token_to_str(tk)));
  return 0; // Unreachable
}
