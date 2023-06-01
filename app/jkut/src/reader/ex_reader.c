// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/ex_reader.h"
#include "kut/buf.h"
#include "DEFS.h"
#include "function.h"
#include "reader/token.h"
#include "reader/pt_sq_pr_reader.h"
#include "reader/st_reader.h"

static Exp *mk_exp_bin (
  Exp *(*fn)(Exp*, Exp*, char *), Exp *left, Exp *right, Token *op
) {
  char *js;
  if (!strcmp(op->value, "&") || !strcmp(op->value, "|")) {
    js = str_f("%s)%s%s)",
      exp_js_insert(left, "sys.asBool("),
      str_f("%s%s", op->js, !strcmp(op->value, "&") ? "&" : "|"),
      exp_js_insert(right, "sys.asBool(")
    );
  } else {
    js = str_f("%s%s%s",
      exp_get_js(left),
      op->js,
      exp_get_js(right)
    );
  }
  return fn(left, right, js);
}

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
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_mul, last, arr_get(exps, i + 1), tk
        ));
      else if (*((char *)tk->value) == '/')
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_div, last, arr_get(exps, i + 1), tk
        ));
      else
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_mod, last, arr_get(exps, i + 1), tk
        ));
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
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_add, last, arr_get(exps, i + 1), tk
        ));
      else
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_sub, last, arr_get(exps, i + 1), tk
        ));
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
      if (!strcmp((char *)tk->value, "==")) {
        Exp *exp = arr_get(exps, i + 1);
        char *js = str_f("%s%s%s%s%s",
          exp_js_insert(last, "sys.$eq("),
          str_left(tk->js, -2), ",",
          exp_get_js(exp), ")"
        );
        arr_set(new_exps, sz1, exp_eq(last, exp, js));
      } else if (!strcmp((char *)tk->value, "!=")) {
        Exp *exp = arr_get(exps, i + 1);
        char *js = str_f("%s%s%s%s%s",
          exp_js_insert(last, "sys.$neq("),
          str_left(tk->js, -2), ",",
          exp_get_js(exp), ")"
        );
        arr_set(new_exps, sz1, exp_neq(last, exp, js));
      } else if (!strcmp((char *)tk->value, ">"))
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_greater, last, arr_get(exps, i + 1), tk
        ));
      else if (!strcmp((char *)tk->value, ">="))
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_greater_eq, last, arr_get(exps, i + 1), tk
        ));
      else if (!strcmp((char *)tk->value, "<"))
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_less, last, arr_get(exps, i + 1), tk
        ));
      else
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_less_eq, last, arr_get(exps, i + 1), tk
        ));
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
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_and, last, arr_get(exps, i + 1), tk
        ));
      else
        arr_set(new_exps, sz1, mk_exp_bin(
          exp_or, last, arr_get(exps, i + 1), tk
        ));
    } else {
      arr_push(new_exps, tk);
      arr_push(new_exps, arr_get(exps, i + 1));
    }
  }
  return new_exps;
}

static Exp *read_switch (Cdr *cdr, char *blanks) {
  Token *open_pr = cdr_read_token(cdr);

  if (!token_is_open_par(open_pr))
    EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(open_pr)));

  Exp *cond = ex_reader_read(cdr);
  char *cond_js = str_f("sys.$eq(%s,", str_trim(exp_get_js(cond)));

  Token *close_pr = cdr_read_token(cdr);

  if (!token_is_close_par(close_pr))
    EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(close_pr)));

  Token *open_bk = cdr_read_token(cdr);

  if (!token_is_open_bracket(open_bk))
    EXC_GENERIC(cdr_fail_expect(cdr, "{", token_to_str(open_bk)));

  Buf *bf = buf_new();
  buf_add(bf, str_f("%s%s%s%s",
    blanks, str_left(open_pr->js, -1), exp_js_replace(cond, ""),
    str_left(open_bk->js, -1)
  ));

  // <Tp<Arr<Exp>, Exp>>
  Arr *cases = arr_new();
  for (;;) {
    if (cdr_next_token_is_close_bracket(cdr))
      EXC_GENERIC(cdr_fail(cdr, "'default' case is missing"));

    Exp *c1 = ex_reader_read(cdr);

    if (exp_is_sym(c1) && !strcmp(exp_get_sym(c1), "default")) {
      Token *colon = cdr_read_token(cdr);
      if (!token_is_colon(colon))
        EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(colon)));

      Exp *exp = ex_reader_read(cdr);
      arr_push(cases, tp_new(arr_new_from(c1, NULL), exp));

      Token *tk = cdr_read_token(cdr);
      if (!token_is_semicolon(tk))
        EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));

      tk = cdr_read_token(cdr);
      if (!token_is_close_bracket(tk))
        EXC_GENERIC(cdr_fail_expect(cdr, "}", token_to_str(tk)));

      buf_add(bf, str_f("%s%s%s%s",
        exp_js_replace(c1, ""),
        str_left(colon->js, -1), exp_get_js(exp),
        str_left(tk->js, -1)
      ));

      return exp_switch(cond, cases, buf_str(bf));
    }

    // <Exp>
    Arr *cs = arr_new_from(c1, NULL);
    buf_add(bf, str_f("%s%s", exp_js_insert(c1, cond_js), ")"));
    Token *colon = cdr_read_token(cdr);
    while (token_is_comma(colon)) {
      Exp *c = ex_reader_read(cdr);
      arr_push(cs, c);
      buf_add(bf, str_f("%s||%s%s",
        str_left(colon->js, -1), exp_js_insert(c, cond_js), ")"));
      colon = cdr_read_token(cdr);
    }

    if (!token_is_colon(colon))
      EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(colon)));

    Exp *exp = ex_reader_read(cdr);
    arr_push(cases, tp_new(cs, exp));
    buf_add(bf, str_f("%s?%s",
      str_left(colon->js, -1), exp_get_js(exp)
    ));

    Token *tk = cdr_read_token(cdr);

    if (!token_is_semicolon(tk))
      EXC_GENERIC(cdr_fail_expect(cdr, ";", token_to_str(tk)));
    buf_add(bf, str_f("%s:", str_left(tk->js, -1)));
  }
}

static Exp *read_new (Cdr *cdr, char *blanks) {
  Buf *js = buf_new();
  buf_add(js, blanks);
  buf_add(js, "new ");

  Token *id = cdr_read_token(cdr);
  if (id->type != TOKEN_SYMBOL)
    EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(id)));
  buf_add(js, id->js);

  Token *open_pr = cdr_read_token(cdr);
  if (!token_is_open_par(open_pr))
    EXC_GENERIC(cdr_fail_expect(cdr, "(", token_to_str(open_pr)));
  buf_add(js, open_pr->js);

  // Exp
  Arr *params = arr_new();

  if (cdr_next_token_is_close_parenthesis(cdr)) {
    Token *close_pr = cdr_read_token(cdr);
    buf_add(js, close_pr->js);

    return exp_new(id->value, params, buf_str(js));
  }

  for (;;) {
    Exp *exp = ex_reader_read(cdr);
    arr_push(params, exp);
    buf_add(js, exp_get_js(exp));

    Token *tk = cdr_read_token(cdr);
    buf_add(js, tk->js);
    if (token_is_comma(tk)) continue;
    if (token_is_close_par(tk)) break;
    EXC_GENERIC(cdr_fail_expect(cdr, ", or )", token_to_str(tk)));
  }

  return exp_new(id->value, params, buf_str(js));
}

static Exp *read_class (Cdr *cdr, char *blanks) {
  Token *tk = cdr_read_token(cdr);
  if (tk->type != TOKEN_SYMBOL)
    EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));

  char *js = str_f("%s%s", blanks, tk->js);
  return exp_class(tk->value, js);
}

static Exp *read_null (Cdr *cdr, char *blanks) {
  Exp *exp = ex_reader_read(cdr);

  char *js = str_f("%ssys.$null(%s)",
    blanks, exp_get_js(exp)
  );
  return exp_null(exp, js);
}

static Exp *read_eval (Cdr *cdr, char *blanks) {
  Token *js_inject = cdr_read_token(cdr);
  if (js_inject->type != TOKEN_STRING)
    EXC_GENERIC(cdr_fail_expect(cdr, "string", token_to_str(js_inject)));
  char *js = str_f("%s%s", blanks, token_string_injection(js_inject));
  return exp_eval(js_inject->value, js);
}

static Exp *read_async (Cdr *cdr, char *blanks) {
  Exp *exp = ex_reader_read(cdr);
  if (!exp_is_function(exp))
    EXC_GENERIC(cdr_fail_expect(cdr, "function", exp_to_str(exp)));
  char *js = str_f("%sasync %s", blanks, exp_get_js(exp));
  exp_set_js(exp, js);
  return exp;
}

static Exp *read_await (Cdr *cdr, char *blanks) {
  Exp *exp = ex_reader_read(cdr);
  if (!exp_is_pr(exp))
    EXC_GENERIC(cdr_fail_expect(cdr, "function call", exp_to_str(exp)));
  char *js = str_f("%sawait %s", blanks, exp_get_js(exp));
  exp_set_js(exp, js);
  return exp;
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
    Token *tk0 = cdr_read_token(cdr);
    Exp *exp2 = ex_reader_read(cdr);
    Token *tk = cdr_read_token(cdr);
    if (token_is_colon(tk)) {
      Exp *exp3 = ex_reader_read(cdr);
      char *js = str_f("sys.asBool(%s)%s%s%s%s",
        exp_get_js(exp), tk0->js,
        exp_get_js(exp2), tk->js,
        exp_get_js(exp3)
      );
      exp = exp_ternary(exp, exp2, exp3, js);
    } else {
      EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(tk)));
    }
  }
  return exp;
}

Exp *ex_reader_read1 (Cdr *cdr) {
  Token *tk = cdr_read_token(cdr);
  if (token_is_open_par(tk)) { // ( --------------------------------------------
    Exp *exp = ex_reader_read(cdr);
    Token *tk2 = cdr_read_token(cdr);
    if (token_is_close_par(tk2)) {
      char *js = str_f("%s%s%s",
        tk->js, exp_get_js(exp), tk2->js
      );
      exp_set_js(exp, js);
      return exp;
    }
    EXC_GENERIC(cdr_fail_expect(cdr, ")", token_to_str(tk2)));
  }

  if (token_is_unary(tk)) { // - ! ---------------------------------------------
    Exp *exp = ex_reader_read1(cdr);
    exp = pt_sq_pr_reader_read(exp, cdr);
    char *js = token_is_exclamation(tk)
      ? str_f("%ssys.asBool(%s)", tk->js, exp_get_js(exp))
      : str_f(" %s%s", tk->js, exp_get_js(exp))
    ;
    return token_is_exclamation(tk)
      ? exp_not(exp, js)
      : exp_minus(exp, js)
    ;
  }

  if (token_is_open_square(tk)) { // [ ------------------------------------------
    if (cdr_next_token_is_close_square(cdr)) { // Empty array
      Token *tk2 = cdr_read_token(cdr);
      char *js = str_f("%s%s", tk->js, tk2->js);
      return exp_array(arr_new(), js);
    }

    Exp *exp = ex_reader_read(cdr);


    if (cdr_next_token_is_colon(cdr)) { // Range
      Token *tk2 = cdr_read_token(cdr);
      Token *tk3 = NULL;
      Exp *exp2 = ex_reader_read(cdr);
      // <Exp>
      Opt *exp3 = opt_none();
      if (cdr_next_token_is_colon(cdr)) {
        tk3 = cdr_read_token(cdr);
        exp3 = opt_some(ex_reader_read(cdr));
      }
      Token *close_sq = cdr_read_token(cdr);
      if (token_is_close_square(close_sq)) {
        char *js = tk3
          ? str_f("%s%s%s%s%s%s%s%s%s%s%s",
              str_left(tk->js, -1), "iter.$range_step(", exp_get_js(exp),
              str_left(tk2->js, -1), ",", exp_get_js(exp2),
              str_left(tk3->js, -1), ",", exp_get_js(opt_get(exp3)),
              str_left(close_sq->js, -1), ")"
            )
          : str_f("%s%s%s%s%s%s%s%s",
              str_left(tk->js, -1), "iter.$range(", exp_get_js(exp),
              str_left(tk2->js, -1), ",", exp_get_js(exp2),
              str_left(close_sq->js, -1), ")"
            )
        ;
        return exp_range(exp, exp2, exp3, js);
      }
      EXC_GENERIC(cdr_fail_expect(cdr, "]", token_to_str(close_sq)));
    }
    // Array

    // <Exp>
    Arr *exps = arr_new();
    Buf *js = buf_new();

    arr_push(exps, exp);
    buf_add(js, str_f("%s%s", tk->js, exp_get_js(exp)));

    for (;;) {
      Token *tk2 = cdr_read_token(cdr);

      if (token_is_close_square(tk2)) {
        buf_add(js, tk2->js);
        break;
      } else if (token_is_comma(tk2)) {
        Exp *exp2 = ex_reader_read(cdr);
        arr_push(exps, exp2);
        buf_add(js, str_f("%s%s", tk2->js, exp_get_js(exp2)));
      } else EXC_GENERIC(cdr_fail_expect(cdr, "']' or ','", token_to_str(tk2)));
    }

    return exp_array(exps, buf_str(js));
  }

  if (token_is_open_bracket(tk)) { // { ----------------------------------------
    if (cdr_next_token_is_close_bracket(cdr)) {
      Token *tk2 = cdr_read_token(cdr);
      char *js = str_f("%s%s", tk->js, tk2->js);
      return exp_map(map_new(), js);
    }

    // <Exp>
    Map *mp = map_new();
    Buf *js = buf_new();
    buf_add(js, tk->js);

    for (;;) {
      Exp *exp = ex_reader_read(cdr);
      char *key;
      if (exp_is_sym(exp)) key = exp_get_sym(exp);
      else if (exp_is_string(exp)) key = exp_get_string(exp);
      else EXC_GENERIC(cdr_fail_expect(cdr, "symbol or string", exp_to_str(exp)));
      buf_add(js, exp_get_js(exp));

      if (map_has_key(mp, key))
        EXC_GENERIC(cdr_fail(cdr, str_f("Key '%s' is duplicated", key)));

      Token *tk2 = cdr_read_token(cdr);

      if (token_is_comma(tk2) || token_is_close_bracket(tk2)) {
        if (exp_is_sym(exp)) {
          map_put(mp, key, exp);
          buf_add(js, str_f(":%s%s", exp_get_sym(exp), tk2->js));
        } else if (exp_is_string(exp)) {
          EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(tk)));
        } else {
          EXC_GENERIC(cdr_fail_expect(cdr, "symbol or string", exp_to_str(exp)));
        }
        if (token_is_comma(tk2)) continue;
        else break;
      }

      if (token_is_colon(tk2)) {
        Exp *exp2 = ex_reader_read(cdr);
        map_put(mp, key, exp2);
        buf_add(js, str_f("%s%s", tk2->js, exp_get_js(exp2)));
        Token *tk3 = cdr_read_token(cdr);
        if (token_is_comma(tk3)) {
          buf_add(js, tk3->js);
          continue;
        }
        if (token_is_close_bracket(tk3)) {
          buf_add(js, tk3->js);
          break;
        }
        EXC_GENERIC(cdr_fail_expect(cdr, "':' or ','", token_to_str(tk3)));
      }

      EXC_GENERIC(cdr_fail_expect(cdr, ":", token_to_str(tk2)));
    }

    return exp_map(mp, buf_str(js));
  }

  if (token_is_backslash(tk)) { // \ -------------------------------------------
    // <char>
    Arr *pars = arr_new();
    Buf *js = buf_new();
    buf_add(js, str_f("%sfunction(", str_left(tk->js, -1)));

    if (cdr_next_token_is_arrow(cdr)) {
      buf_add(js, str_f(")%s", str_left(cdr_read_token(cdr)->js, -2)));
    } else {
      for(;;) {
        Token *tk2 = cdr_read_token(cdr);
        if (tk2->type == TOKEN_SYMBOL) {
          char *param = tk2->value;
          EACH(pars, char, p) {
            if (!strcmp(p, param))
              EXC_GENERIC(cdr_fail(
                cdr, str_f("Duplicate parameter '%s'", p)
              ));
          }_EACH
          arr_push(pars, param);
          buf_add(js, tk2->js);
          Token *tk3 = cdr_read_token(cdr);
          if (token_is_comma(tk3)) {
            buf_add(js, tk3->js);
            continue;
          }
          if (token_is_arrow(tk3)) {
            buf_add(js, str_f(")%s", str_left(tk3->js, -2)));
            break;
          }
          EXC_GENERIC(cdr_fail_expect(cdr, "'->' or ','", token_to_str(tk3)));
        }
        EXC_GENERIC(cdr_fail_expect(cdr, "symbol", token_to_str(tk)));
      }
    }
    char *pars_ctrl = str_f("sys.$params(arguments.length, %d);", arr_size(pars));
    StatCode *st_cd = st_reader_read(cdr);
    Stat *st = stat_code_stat(st_cd);
    if (stat_is_block(st)) {
      char *stjs = stat_get_js(st);
      int ix = str_cindex(stjs, '{') + 1;
      buf_add(js, str_f("%s%s%s",
        str_left(stjs, ix), pars_ctrl, str_right(stjs, ix)
      ));
    } else {
      buf_add(js, str_f(" {%s", pars_ctrl));
      buf_add(js, stat_get_js(st));
      buf_add(js, "}");
    }
    return exp_function(function_new(pars, st_cd), buf_str(js));
  }

  // Others --------------------------------------------------------------------

  if (tk->type == TOKEN_LINE_COMMENT || tk->type == TOKEN_COMMENT) {
    char *js = tk->js;
    Exp *r = ex_reader_read1(cdr); // skip comment
    exp_set_js(r, str_f("%s%s", js, exp_get_js(r)));
    return r;
  }

  if (tk -> type == TOKEN_BOOL)
    return exp_bool(tk->b, tk->js);

  if (tk -> type == TOKEN_FLOAT)
    return exp_float(tk->d, tk->js);

  if (tk -> type == TOKEN_STRING)
    return exp_string(tk->value, tk->js);

  if (tk->type == TOKEN_SYMBOL) {
    char *sym = tk->value;

    if (!strcmp(sym, "switch")) return read_switch(cdr, str_left(tk->js, -6));
    else if (!strcmp(sym, "new")) return read_new(cdr, str_left(tk->js, -3));
    else if (!strcmp(sym, "class")) return read_class(cdr, str_left(tk->js, -5));
    else if (!strcmp(sym, "null")) return read_null(cdr, str_left(tk->js, -4));
    else if (!strcmp(sym, "eval")) return read_eval(cdr, str_left(tk->js, -4));
    else if (!strcmp(sym, "async")) return read_async(cdr, str_left(tk->js, -5));
    else if (!strcmp(sym, "await")) return read_await(cdr, str_left(tk->js, -5));
    else if (!strcmp(sym, "default")) return exp_sym(sym, tk->js);
    else if (token_is_reserved(sym))
      EXC_GENERIC(cdr_fail(cdr, str_f("'%s' is a reserverd symbol", sym)));

    return exp_sym(sym, tk->js);
  }

  EXC_GENERIC(cdr_fail_expect(cdr, "Expression", token_to_str(tk)));
  return 0; // Unreachable
}
