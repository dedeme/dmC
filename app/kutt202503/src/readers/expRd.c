// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "readers/expRd.h"
#include "readers/stRd.h"
#include "data/tp.h"
#include "data/dicLayers.h"
#include "data/stat.h"

// STATIC HEADER -------------------------------------------

//  Returns an error expression.
static Exp *fail (int ln, char *msg);

//  Fail of type expected - actual
static Exp *faile (int ln, char *expected, char *actual);

// Read next single expression. The token reader is modified.
static Exp *read_single (ExpRd *this);

// Cases when esym is in symMods: Reads one symbol (f) and one token (tk)
// and then if
//    - tk == ( -> Reads a function with 1 or more arguments:
//                 Reads 'pars)' -> return md.fn(sym, pars)
//                                  or md.fn(sym) if pars is empty.
//    - Other   -> Reads a field:
//                 return sym[md.field]
//    this: Expression reader.
//    esym: Expression of type symbol.
static Exp *read_symbol (ExpRd *this, Exp *esym);

// Read single expression continuation. The token reader is modified.
//    e: Previous single expression
static Exp *read_continuation (ExpRd *this, Exp *e);

// Read switch expression.
static Exp *read_switch (ExpRd *this, int ln);

// Solve a binary expression
//    match_fn: Returns TRUE the token is included to be solved.
//    es: A sequence [<exp>,<token>, <exp>, <token> ... <token>, <exp>]
// Returns a sequence [<exp>,<token>, <exp>, <token> ... <token>, <exp>]
static Arr *solve_binary (int (*math_fn)(Token *), Arr *es);

// Read one function parameter, testing that it is not duplicated, and
// updating (this->ctx)->symMods.
//    pars: (Arr<char>) Previous read function paramenters.
//  Returns Tp<char, Exp> ([Parameter, Error]):
//   - Parameter is the parameter read.
//      * If there is an error it is NULL.
//      * If there are no more parameters to red, returns "".
//   - Error is a exp_error if Parameter can not be read. Otherwise it is NULL.
static Tp *read_param (ExpRd *this, Arr *pars);

//  Read a dictionary entry
//  Returns Tp<Tp<Exp, Exp>, Exp> ([Entry, Error]):
//   - Entry is the dictionary entry. If there is an error it is NULL.
//   - Error is a exp_error if Entry can not be read. Otherwise it is NULL.
static Tp *read_dic_entry (ExpRd *this);

// BODY ----------------------------------------------------

ExpRd *expRd_new (RdCtx *ctx, char *tx) {
  ExpRd *this = MALLOC(ExpRd);
  this->ctx = ctx;
  this->tkrd = tokenRd_new(tx);
  return this;
}

Exp *expRd_next (ExpRd *this) {
  TokenRd *tkr = this->tkrd;

  Exp *e = read_single(this);
  if (e->tp == exp_error) return e;
  Exp *e2 = read_continuation(this, e);
  if (e2->tp == exp_error) return e2;
  // Arr with the Sequence [Exp, Token, Exp, Token ... Token, Exp]
  Arr *es = arr_new_from(e2, NULL);
  for (;;) {
    Token *tk = tokenRd_peek(tkr);
    if (token_is_binary(tk)) {
      tokenRd_next(tkr);
      arr_push(es, tk);
      Exp *e1 = read_single(this);
      if (e1->tp == exp_error) return e1;
      Exp *e2 = read_continuation(this, e1);
      if (e2->tp == exp_error) return e2;
      arr_push(es, e2);
    } else {
      break;
    }
  }
  Arr *es1 = solve_binary(token_is_binary1, es);
  Arr *es2 = solve_binary(token_is_binary2, es1);
  Arr *es3 = solve_binary(token_is_binary3, es2);
  Arr *es4 = solve_binary(token_is_binary4, es3);

  Token *tk = tokenRd_peek(tkr);
  if (token_is_op(tk, "?")) {
    tokenRd_next(tkr);
    Exp *eyes = expRd_next(this);
    if (eyes->tp == exp_error) return eyes;
    Token *tk = tokenRd_next(tkr);
    if (token_is_op(tk, ":")) {
      Exp *enot = expRd_next(this);
      if (enot->tp == exp_error) return enot;
      return exp_new_ternary(e->ln, *arr_begin(es4), eyes, enot);
    }
    return faile(tk->ln, ":", token_to_str(tk));
  }

  return *arr_begin(es4);
}

Token *expRd_next_token (ExpRd *this) {
  return tokenRd_next(this->tkrd);
}

Token *expRd_peek_token (ExpRd *this) {
  return tokenRd_peek(this->tkrd);
}

Tp *expRd_read_array (ExpRd *this, char end) {
  TokenRd *tkr = this->tkrd;
  Exp *e = expRd_next(this);
  if (e->tp == exp_error) return tp_mk(arr_new(), e);

  // Arr<Exp>
  Arr *a = arr_new_from(e, NULL);
  for(;;) {
    Token *tk = tokenRd_next(tkr);
    if (tk->tp == token_operator) {
      if (*tk->value == end) return tp_mk(a, NULL);

      if (*tk->value == ',') {
        Exp *e = expRd_next(this);
        if (e->tp == exp_error) return tp_mk(arr_new(), e);
        arr_push(a, e);
        continue;
      }
    }
    return tp_mk(arr_new(), faile(tk->ln, str_f("%c or ,", end), token_to_str(tk)));
  }
}

Tp *expRd_read_def_type (ExpRd *this, int is_left) {
  TokenRd *tkr = this->tkrd;
  Token *openTk = tokenRd_next(tkr);
  int ln = openTk->ln;

  if (is_left) {
    Token *id = tokenRd_next(tkr);
    if (id->tp != token_symbol)
      return tp_mk(NULL, faile(id->ln, "symbol", token_to_str(id)));

    Token *op = tokenRd_next(tkr);
    if (op->tp == token_operator) {
      if (*op->value == ')') {
        Rs *rs = type_read(str_f("(%s)", id->value));
        return tp_mk(rs_get(rs), NULL); // Ok return
      }

      if (*op->value == '|') {
        // Arr<char>
        Arr *bf = arr_new();
        for (;;) {
          Token *tk = tokenRd_next(tkr);
          TokenT tp = tk->tp;
          if (tp == token_eof) {
            return tp_mk(
              NULL, fail(op->ln, "Unexpected end of file reading type")
            );
          } else if (tp == token_operator) {
            if (*tk->value == ')') break;
            else return tp_mk(
                NULL, faile(tk->ln, "Generic type", token_to_str(tk))
              );
          } else if (tp == token_symbol) {
            arr_push(bf, tk->value);
          } else {
            return tp_mk(
              NULL, faile(tk->ln, "Generic type", token_to_str(tk))
            );
          }
        }
        char *gs = arr_join(bf, "");
        for (int i = 0; i < strlen(gs); ++i) {
          char ch = gs[i];
          if (ch < 'A' || ch > 'Z')
            return tp_mk(
              NULL,
              fail(op->ln, str_f("Generics '%c' is not uppercase", ch))
            );
          if (str_cindex(str_left(gs, i), ch) != -1)
            return tp_mk(
              NULL,
              fail(op->ln, str_f("Generic '%c' is duplicated", ch))
            );
        }
        Rs *rs = type_read(str_f("(%s|%s)", id->value, gs));
        return tp_mk(rs_get(rs), NULL); // Ok return
      }
    }
    return tp_mk(NULL, faile(op->ln, "| or )", token_to_str(op)));
  }

  if (!token_is_op(openTk, "["))
    return tp_mk(NULL, faile(ln, "[", token_to_str(openTk)));
  // Arr<chr>
  Arr *bf = arr_new();
  for(;;) {
    Token *tk = tokenRd_next(tkr);
    TokenT tp = tk->tp;
    if (tp == token_eof) {
      return tp_mk(NULL, fail(ln, "Unexpected end of file reading type"));
    } else if (tp == token_operator) {
      if (*tk->value == ';') {
        char *t = str_f("[%s", arr_join(bf, ""));
        Rs *rs = type_read(t);
        Type *t2 = rs_get(rs);
        if (!t2) return tp_mk(NULL, fail(tk->ln, rs_error(rs)));
        if (*t2->id != 't' && *t2->id != '0' && *t2->id != '1')
          return tp_mk(NULL, faile(tk->ln, "Function or tuple type", t));
        return tp_mk(t2, NULL); // Ok return
      }
      arr_push(bf, tk->value);
    } else if (tp == token_symbol) {
      arr_push(bf, tk->value);
    } else {
      return tp_mk(
        NULL, faile(tk->ln, "Element of type definition or ';'", token_to_str(tk))
      );
    }
  }
}

// Tp<Type, Exp>
Tp *expRd_read_empty_type (ExpRd *this) {
  TokenRd *tkr = this->tkrd;
  Token *tk = tokenRd_next(tkr);
  int ln = tk->ln;
  // Arr<char>
  Arr *bf = arr_new();
  for (;;) {
    Token *tk = tokenRd_next(tkr);
    TokenT t = tk->tp;
    if (t == token_eof) {
      return tp_mk(
        NULL, fail(ln, "Unexpected end of file reading type")
      );
    } else if (t == token_operator) {
      if (*tk->value == '/') break;
      else arr_push(bf, tk->value);
    } else if (t == token_symbol)  {
      arr_push(bf, tk->value);
    } else {
      return tp_mk(
        NULL, faile(tk->ln, "Element of type definition", token_to_str(tk))
      );
    }
  }
  char *t = arr_join(bf, "");
  Rs *rs = type_read(t);
  Type *t2 = rs_get(rs);
  return t2 ? tp_mk(t2, NULL) : tp_mk(NULL, fail(ln, rs_error(rs)));
}

// STATIC BODY ---------------------------------------------

static Exp *fail (int ln, char *msg) {
  return exp_new_error(ln, msg);
}

static Exp *faile (int ln, char *expected, char *actual) {
  return fail(ln, str_f("Expected: %s\n  Actual: %s", expected, actual));
}

static Exp *read_single (ExpRd *this) {
  TokenRd *tkr = this->tkrd;
  Token  *tk = tokenRd_next(tkr);
  int ln = tk->ln;
  TokenT tp = tk->tp;
  char *value = tk->value;

  switch (tp) {
    case token_error: return exp_new_error(ln, value);
    case token_eof: return fail(ln, "Unexpected end of file");
    case token_operator: switch (*value) {
      case '!':
      case '-': {
        Exp *e1 = read_single(this);
        if (e1->tp == exp_error) return e1;
        Exp *e2 = read_continuation(this, e1);
        if (e2->tp == exp_error) return e2;
        return exp_new_unary(ln, value, e2);
      }
      case '(': {
        Exp *e = expRd_next(this);
        if (e->tp == exp_error) return e;
        Token *tk2 = tokenRd_next(tkr);
        return token_is_op(tk2, ")")
          ? exp_new_group(ln, e)
          : faile(tk2->ln, ")", token_to_str(tk2))
        ;
      }
      case '{': {
        Token *tk2 = tokenRd_peek(tkr);
        if (token_is_op(tk2, "}")) {
          tokenRd_next(tkr);
          Token *tk = tokenRd_peek(tkr);
          if (token_is_op(tk, "/")) {
            Tp *tp = expRd_read_empty_type(this);
            Type *t = tp->e1;
            if (!t) return tp->e2;
            return exp_new_dictionary0(ln, opt_some(t));
          }
          return exp_new_dictionary0(ln, opt_none());
        }
        // Arr<Exp, Exp>
        Arr *d = arr_new();
        Tp *tp = read_dic_entry(this);
        Tp *e = tp->e1;
        if (!e) return tp->e2;
        arr_push(d, e);
        for (;;) {
          Token *tk3 = tokenRd_next(tkr);
          if (tk3->tp == token_operator) {
            if (*tk3->value == '}') return exp_new_dictionary(ln, d);
            if (*tk3->value == ',') {
              Tp *tp = read_dic_entry(this);
              Tp *e = tp->e1;
              if (!e) return tp->e2;
              arr_push(d, e);
              continue;
            }
          }
          return faile(tk3->ln, "} or ,", token_to_str(tk3));
        }
      }
      case '[': {
        Token *tk2 = tokenRd_peek(tkr);
        TokenT tp = tk2->tp;
        char *v = tk2->value;

        int is_array = TRUE;
        int is_range = FALSE;
        if (token_is_op(tk2, "]")) {
          tokenRd_next(tkr);
          Token *tk = tokenRd_peek(tkr);
          if (token_is_op(tk, "/")) {
            Tp *tp = expRd_read_empty_type(this);
            Type *t = tp->e1;
            if (!t) return tp->e2;
            return exp_new_array0(ln, opt_some(t));
          }
          return exp_new_array0(ln, opt_none());
        } else if (tp == token_operator && (*v == '.' || *v == '!')) {
          if (*v == '.') is_array = FALSE;
          else is_range = TRUE;
          tokenRd_next(tkr);
          Token *tk3 = tokenRd_peek(tkr);
          if (token_is_op(tk3, "]"))
            return fail(tk3->ln, "Unexpected ']'");
        }
        Tp *tuple = expRd_read_array(this, ']');
        // Arr<Exp>
        Arr *a = tuple->e1;
        if (!arr_size(a)) return tuple->e2;
        if (is_range) {
          if (arr_size(a) > 3)
            return fail(ln, "Range with more than 3 values");
          return exp_new_range(ln, a);
        }

        return is_array ? exp_new_array(ln, a) : exp_new_tuple(ln, a);
      }
      case '\\': {
        RdCtx *ctx = this->ctx;
        DicLayers *mds = ctx->symMods;
        dicLayers_add_layer(mds);

        // Arr<char>
        Arr *pars = arr_new();
        for (;;) {
          Tp *tp = read_param(this, pars);
          char *p = tp->e1;
          if (!p) return tp->e2;
          if (!*p) break;
          arr_push(pars, p);
        }

        StRd *strd = stRd_new0(ctx, this);
        Stat *st0 = stRd_next(strd);
        if (st0->tp == stat_error) return fail(st0->ln, st0->value);

        Stat *st = st0->tp == stat_block
          ? st0
          : stat_new_block(st0->ln, arr_new_from(st0, NULL))
        ;
        dicLayers_remove_layer(mds);
        // Fake wrong type
        return exp_new_function(ln, type_new("1", arr_new()), pars, st);
      }
      default: return fail(ln, str_f("Unexpected '%s'", value));
    }
    case token_bool: return exp_new_boolean(ln, value);
    case token_int: return exp_new_integer(ln, value);
    case token_float: return exp_new_float(ln, value);
    case token_string: return exp_new_string(ln, value);
    default: {// symbol
      if (str_eq(value, "switch")) return read_switch(this, ln);
      return read_symbol(this, exp_new_symbol(ln, value));
    }
  }
}

static Exp *read_symbol (ExpRd *this, Exp *esym) {
  int ln = esym->ln;
  RdCtx *ctx= this->ctx;
  TokenRd *tkr = this->tkrd;
  Token *tk = tokenRd_peek(tkr);

  if (!token_is_op(tk, ".")) return esym;
  // Expression 'Point'
  tokenRd_next(tkr);
  Token *tk2 = tokenRd_next(tkr);
  if (tk2->tp != token_symbol)
    return faile(tk->ln, "symbol", token_to_str(tk2));
  DicLayers *sym_mods = ctx->symMods;
  char *sym = esym->value;
  char *fn = tk2->value;
  char *md = opt_get(dicLayers_get_element(sym_mods, sym));
  if (!md || !*md) return exp_new_point(ln, sym, fn);

  Token *tk3 = tokenRd_peek(tkr);
  if (tk3->tp == token_operator) {
    if (*tk3->value == '(') {
      tokenRd_next(tkr);
      Arr *es = arr_new_from(esym, NULL);
      Token *tk4 = tokenRd_peek(tkr);
      if (token_is_op(tk4, ")")) {
        tokenRd_next(tkr);
      } else {
        Tp *tp = expRd_read_array(this, ')');
        Arr *e2s = tp->e1;
        if (!arr_size(e2s)) return tp->e2;
        arr_cat(es, e2s);
      }
      return exp_new_parenthesis(ln, exp_new_point(ln, md, fn), es);
    }
    // else go below
  }
  return exp_new_square(ln, esym, exp_new_point(ln, md, fn));
}

static Exp *read_continuation (ExpRd *this, Exp *e) {
  int ln = e->ln;
  TokenRd *tkr = this->tkrd;
  Token *tk = tokenRd_peek(tkr);
  TokenT tp = tk->tp;
  char *v = tk->value;

  if (tp != token_operator) return e;
  switch (*v) {
    case '!': {
      if (!v[1]) {
        tokenRd_next(tkr);
        return read_continuation(
          this, exp_new_square(ln, e, exp_new_integer(tk->ln, "0"))
        );
      }
      return e;
    }
    case '\\': {
      tokenRd_next(tkr);
      Exp *key = expRd_next(this);
      if (key->tp == exp_error) return key;
      if  (key->tp != exp_symbol) faile(tk->ln, "symbol", exp_to_str(key));
      return read_continuation(
        this,
        exp_new_square(ln, e, exp_new_string(key->ln, key->value))
      );
    }
    case '[': {
      tokenRd_next(tkr);
      Token *tk0 = tokenRd_peek(tkr);
      int start_with_colon = FALSE;
      if (token_is_op(tk0, ":")) {
        start_with_colon = TRUE;
        tokenRd_next(tkr);
      }

      Token *tk1 = tokenRd_peek(tkr);
      if (token_is_op(tk1, "]") && start_with_colon) {
        tokenRd_next(tkr);
        return read_continuation(
          this,
          exp_new_slice(ln, e, opt_none(), opt_none())
        );
      }

      Exp *e1 = expRd_next(this);
      if (e1->tp == exp_error) return e1;
      Token *tk2 = tokenRd_next(tkr);
      TokenT tp = tk2->tp;
      char *v = tk2->value;

      if (start_with_colon) {
        if (tp == token_operator && *v == ']')
          return read_continuation(
            this,
            exp_new_slice(ln, e, opt_none(), opt_some(e1))
          );
        return faile(tk->ln, "]", token_to_str(tk));
      }

      if (tp == token_operator) {
        if (*v == ']')
          return read_continuation(this, exp_new_square(ln, e, e1));

        if (*v == ':') {
          Token *tk1 = tokenRd_peek(tkr);
          if (token_is_op(tk1, "]")) {
            tokenRd_next(tkr);
            return read_continuation(
              this,
              exp_new_slice(ln, e, opt_some(e1), opt_none())
            );
          }

          Exp *e2 = expRd_next(this);
          if (e2->tp == exp_error) return e2;
          Token *tk2 = tokenRd_next(tkr);
          if (token_is_op(tk2, "]"))
            return read_continuation(
              this,
              exp_new_slice(ln, e, opt_some(e1), opt_some(e2))
            );

          return faile(tk2->ln, "]", token_to_str(tk2));
        }
        // continue bellow
      }
      return faile(tk->ln, ": or ]", token_to_str(tk2));
    }
    case '(': {
      tokenRd_next(tkr);
      Token *tk = tokenRd_peek(tkr);
      // Arr<Exp>
      Arr *es = arr_new();
      if (token_is_op(tk, ")")) {
        tokenRd_next(tkr);
      } else {
        Tp *tp = expRd_read_array(this, ')');
        es = tp->e1;
        if (!arr_size(es)) return tp->e2;
      }
      return read_continuation(this, exp_new_parenthesis(ln, e, es));
    }
    default:
      return e;
  }
}

static Exp *read_switch (ExpRd *this, int ln) {
  TokenRd *tkr = this->tkrd;

  Token *tk1 = tokenRd_next(tkr);
  if (!token_is_op(tk1, "("))
    return faile(tk1->ln, "(", token_to_str(tk1));

  Exp *e = expRd_next(this);
  if (e->tp == exp_error) return e;

  Token *tk2 = tokenRd_next(tkr);
  if (!token_is_op(tk2, ")"))
    return faile(tk2->ln, ")", token_to_str(tk2));

  Token *tk3 = tokenRd_next(tkr);
  if (!token_is_op(tk3, "{"))
    return faile(tk3->ln, "{", token_to_str(tk3));

  // Arr<Tp<Arr<Exp>, Exp>>
  Arr *es = arr_new();
  int with_default = FALSE;
  for (;;) {
    Tp *tp = expRd_read_array(this, ':');
    Arr *a = tp->e1;
    if (!arr_size(a)) return tp->e2;
    if (arr_size(a) > 1) {
      int anyf (Exp *e) {
        return e->tp == exp_symbol && str_eq(e->value, "default");
      }
      if (arr_any(a, (FPRED)anyf))
        return fail(
          ((Exp *)*arr_begin(a))->ln,
          "'default' in a multiconditional entry"
        );
    } else {
      Exp *e = *arr_begin(a);
      if (e->tp == exp_symbol && str_eq(e->value, "default"))
        with_default = TRUE;
    }

    Exp *e = expRd_next(this);
    if (e->tp == exp_error) return e;
    arr_push(es, tp_mk(a, e));

    Token *tk2 = tokenRd_next(tkr);
    if (!token_is_op(tk2, ";"))
      return faile(tk2->ln, ";", token_to_str(tk2));

    Token *tk3 = tokenRd_peek(tkr);
    if (token_is_op(tk3, "}")) {
      tokenRd_next(tkr);
      break;
    }

    if (with_default) return fail(tk3->ln, "Conditions after 'default'");
  }

  Arr *cds = ((Tp*)(arr_peek(es)))->e1;
  Exp *laste = arr_peek(cds);
  if (
    arr_size(cds) != 1 ||
    laste->tp != exp_symbol ||
    strcmp(laste->value, "default")
  )
    return fail(ln, "Last condition in switch-expression must be 'default:'");

  return exp_new_switch(ln, e, es);
}

static Arr *solve_binary (int (*match_fn)(Token *), Arr *es) {
  int ix = 1;
  // Arr type 'es'
  Arr *r = arr_new_from(*arr_begin(es), NULL);
  int size = arr_size(es);
  for (;;) {
    int i = ix;
    ix += 2;
    if (i >= size) break;
    Token *tk = arr_begin(es)[i];
    Exp *e2 = arr_begin(es)[i+1];
    if (match_fn(tk)) {
      Exp *e1 = arr_pop(r);
      arr_push(r, exp_new_binary(e1->ln, tk->value, e1, e2));
    } else {
      arr_push(r, tk);
      arr_push(r, e2);
    }
  }
  return r;
}

static Tp *read_param (ExpRd *this, Arr *pars) {
  RdCtx *ctx = this->ctx;
  DicLayers *symMods = ctx->symMods;
  TokenRd *tkr = this->tkrd;
  Token *tk1 = tokenRd_peek(tkr);
  char *md = "";
  if (token_is_op(tk1, ":")) {
    tokenRd_next(tkr);
    Token *tk2 = tokenRd_next(tkr);
    if (tk2->tp != token_symbol)
      return tp_mk(NULL, faile(tk2->ln, "symbol", token_to_str(tk2)));
    md = tk2->value;
  }

  Token *tk2 = tokenRd_next(tkr);
  if (token_is_op(tk2, "->"))
    return tp_mk("", NULL); // Ok return -End of params.
  if (tk2->tp != token_symbol)
    return tp_mk(NULL, faile(tk2->ln, "symbol", token_to_str(tk2)));
  char *v = tk2->value;
  int anyf (char *p) { return str_eq(p, v); }
  if (arr_any(pars, (FPRED) anyf))
    return tp_mk(NULL, fail(tk2->ln, str_f("Symbol '%v' is duplicated", v)));
  dicLayers_put_element(symMods, tk2->value, md);

  Token *tk3 = tokenRd_peek(tkr);
  if (tk3->tp == token_operator) {
    if (*tk3->value == ',' || str_eq(tk3->value, "->")) {
      if (*tk3->value == ',') tokenRd_next(tkr);
      return tp_mk(tk2->value, FALSE); // Ok return -A param
    }
    // else below
  }
  return tp_mk(NULL, faile(tk3->ln, ", or ->", token_to_str(tk3)));
}

static Tp *read_dic_entry (ExpRd *this) {
  TokenRd *tkr = this->tkrd;
  Exp *key = expRd_next(this);
  if (key->tp != exp_symbol && key->tp != exp_string)
    return tp_mk(NULL, faile(key->ln, "symbol or string", exp_to_str(key)));

  Token *tk = tokenRd_peek(tkr);
  if (!token_is_op(tk, ":")) {
    if ((token_is_op(tk, ",") || token_is_op(tk, "}")) && key->tp == exp_symbol)
      return tp_mk(tp_mk(exp_new_string(key->ln, key->value), key), FALSE);

    return tp_mk(NULL, faile(tk->ln, ":", token_to_str(tk)));
  }
  tokenRd_next(tkr);

  Exp *e = expRd_next(this);
  return e->tp == exp_error
    ? tp_mk(NULL, e)
    : tp_mk(
      tp_mk(key->tp == exp_symbol
        ? exp_new_string(key->ln, key->value)
        : key,
        e
      ), FALSE)
  ;
}
