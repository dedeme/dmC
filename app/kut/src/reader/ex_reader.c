// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/ex_reader.h"
#include "DEFS.h"
#include "function.h"
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
      if (*token_get_operator(tk) == '*')
        arr_set(new_exps, sz1, exp_mul(last, arr_get(exps, i + 1)));
      else if (*token_get_operator(tk) == '/')
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
      if (*token_get_operator(tk) == '+')
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
      if (!strcmp(token_get_operator(tk), "=="))
        arr_set(new_exps, sz1, exp_eq(last, arr_get(exps, i + 1)));
      else if (!strcmp(token_get_operator(tk), "!="))
        arr_set(new_exps, sz1, exp_neq(last, arr_get(exps, i + 1)));
      else if (!strcmp(token_get_operator(tk), ">"))
        arr_set(new_exps, sz1, exp_greater(last, arr_get(exps, i + 1)));
      else if (!strcmp(token_get_operator(tk), ">="))
        arr_set(new_exps, sz1, exp_greater_eq(last, arr_get(exps, i + 1)));
      else if (!strcmp(token_get_operator(tk), "<"))
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
      if (*token_get_operator(tk) == '&')
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

static Exp *read_switch (Cdr *cdr) {
  Token *tk = cdr_read_token(cdr);

  if (!token_is_open_par(tk))
    EXC_KUT(cdr_fail_expect(cdr, "(", token_to_str(tk)));

  Exp *cond = ex_reader_read(cdr);
  tk = cdr_read_token(cdr);

  if (!token_is_close_par(tk))
    EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));

  tk = cdr_read_token(cdr);

  if (!token_is_open_bracket(tk))
    EXC_KUT(cdr_fail_expect(cdr, "{", token_to_str(tk)));

  // <Tp<Exp, Exp>>
  Arr *cases = arr_new();
  for (;;) {
    if (cdr_next_token_is_close_bracket(cdr)) {
      cdr_read_token(cdr);
      break;
    }

    Exp *c1 = ex_reader_read(cdr);

    Token *tk = cdr_read_token(cdr);

    if (!token_is_colon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));

    arr_push(cases, tp_new(c1, ex_reader_read(cdr)));

    tk = cdr_read_token(cdr);

    if (!token_is_semicolon(tk))
      EXC_KUT(cdr_fail_expect(cdr, ";", token_to_str(tk)));
  }

  return exp_switch(cond, cases);
}

// exps is [Exp, Token, Exp, Token ..., Exp] -> at least one element is added.
static void add_expressions (Arr *exps, Cdr *cdr) {
  Exp *exp = ex_reader_read1(cdr);
  exp = pt_sq_pr_reader_read(exp, cdr);
  arr_push(exps, exp);
  if (cdr_next_token_is_binary(cdr)) {
    arr_push(exps, cdr_read_token(cdr));
    add_expressions(exps, cdr);
  }
}

Exp *ex_reader_read (Cdr *cdr) {
  // <Exp>
  Arr *exs = arr_new();
  add_expressions(exs, cdr);

  exs = solve_level1(exs); // * / %
  exs = solve_level2(exs); // + -
  exs = solve_level3(exs); // == != > >= < <=
  exs = solve_level4(exs); // & |

  Exp *exp = arr_get(exs, 0);

  if (cdr_next_token_is_ternary(cdr)) {
    cdr_read_token(cdr);
    Exp *exp2 = ex_reader_read(cdr);
    Token *tk = cdr_read_token(cdr);
    if (token_is_colon(tk)) {
      exp = exp_ternary(exp, exp2, ex_reader_read(cdr));
    } else {
      EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));
    }
  }
  return exp;
}

Exp *ex_reader_read1 (Cdr *cdr) {
  Token *tk = cdr_read_token(cdr);
  if (token_is_open_par(tk)) { // ( --------------------------------------------
    Exp *exp = ex_reader_read(cdr);
    tk = cdr_read_token(cdr);
    if (token_is_close_par(tk))
      return exp;
    EXC_KUT(cdr_fail_expect(cdr, ")", token_to_str(tk)));
  }

  if (token_is_unary(tk)) { // - ! ---------------------------------------------
    Exp *exp = ex_reader_read1(cdr);
    exp = pt_sq_pr_reader_read(exp, cdr);
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

    Exp *exp = ex_reader_read(cdr);


    if (cdr_next_token_is_colon(cdr)) { // Range
      Token *tk = cdr_read_token(cdr);
      Exp *exp2 = ex_reader_read(cdr);
      Exp *exp3 = exp_empty();
      if (cdr_next_token_is_colon(cdr)) {
        cdr_read_token(cdr);
        exp3 = ex_reader_read(cdr);
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
      else if (token_is_comma(tk)) arr_push(exps, ex_reader_read(cdr));
      else EXC_KUT(cdr_fail_expect(cdr, "']' or ','", token_to_str(tk)));
    }

    return exp_array(exps);
  }

  if (token_is_open_bracket(tk)) { // { ----------------------------------------
    if (cdr_next_token_is_close_bracket(cdr)) {
      cdr_read_token(cdr);
      return exp_map(map_new());
    }

    // <Exp>
    Map *mp = map_new();

    for (;;) {
      Exp *exp = ex_reader_read(cdr);
      char *key;
      if (exp_is_sym(exp)) key = exp_get_sym(exp);
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
        map_put(mp, key, ex_reader_read(cdr));
        tk = cdr_read_token(cdr);
        if (token_is_comma(tk)) continue;
        if (token_is_close_bracket(tk)) break;
        EXC_KUT(cdr_fail_expect(cdr, "':' or ','", token_to_str(tk)));
      }

      EXC_KUT(cdr_fail_expect(cdr, ":", token_to_str(tk)));
    }

    return exp_map(mp);
  }

  if (token_is_backslash(tk)) { // \ -------------------------------------------
    // <char>
    Arr *vars = arr_new();
    if (cdr_next_token_is_arrow(cdr)) {
      cdr_read_token(cdr);
    } else {
      for(;;) {
        Token *tk = cdr_read_token(cdr);
        if (token_is_symbol(tk)) {
          arr_push(vars, token_get_symbol(tk));
          tk = cdr_read_token(cdr);
          if (token_is_comma(tk)) continue;
          if (token_is_arrow(tk)) break;
          EXC_KUT(cdr_fail_expect(cdr, "'->' or ','", token_to_str(tk)));
        }
        EXC_KUT(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
      }
    }

    return exp_function(function_new(vars, st_reader_read(cdr)));
  }

  // Others --------------------------------------------------------------------

  if (token_is_line_comment(tk) || token_is_comment(tk))
    return ex_reader_read1(cdr);

  if (token_is_bool(tk))
    return exp_bool(token_get_bool(tk));

  if (token_is_int(tk))
    return exp_int(token_get_int(tk));

  if (token_is_float(tk))
    return exp_float(token_get_float(tk));

  if (token_is_string(tk))
    return exp_string(token_get_string(tk));

  if (token_is_symbol(tk)) {
    char *sym = token_get_symbol(tk);
    return !strcmp(sym, "switch")
      ? read_switch(cdr)
      : exp_sym(sym)
    ;
  }

  EXC_KUT(cdr_fail_expect(cdr, "Expression", token_to_str(tk)));
  return 0; // Unreachable
}
