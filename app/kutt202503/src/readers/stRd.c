// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "readers/stRd.h"
#include "data/tp.h"
#include "data/dicLayers.h"
#include "data/stat.h"
#include "data/tp.h"
#include "data/tp3.h"
#include "data/tp4.h"
#include "data/tp5.h"

// STATIC HEADER -------------------------------------------

//  Returns an error expression.
static Stat *fail (int ln, char *msg);
//  Fail of type expected - actual
static Stat *faile (int ln, char *expected, char *actual);
// Checks if 'st' ends in ';', reads ';' and returns 'st'.
static Stat *check_end (StRd *this, Stat *st);
// .......... .......
// Statements reading
// '''''''''' '''''''
static Stat *read_block (StRd *this, int ln);
static Stat *read_module_symbol (StRd *this);
static Stat *read_multiassign (StRd *this, int ln, char *first_sym);
static Stat *read_return (StRd *this, int ln);
static Stat *read_throw (StRd *this, int ln);
static Stat *read_trace (StRd *this, int ln);
static Stat *read_while (StRd *this, int ln);
static Stat *read_if (StRd *this, int ln);
static Stat *read_switch (StRd *this, int ln);
static Stat *read_for (StRd *this, int ln);
static Stat *read_try (StRd *this, int ln);
static Stat *read_import (StRd *this, int ln);
// sym is exp_symbol
static Stat *read_symbol (StRd *this, Exp *sym);
static Stat *read_square (StRd *this, Exp *e);
static Stat *read_function (StRd *this, Exp *e);

// BODY ----------------------------------------------------

StRd *stRd_new0 (RdCtx *ctx, ExpRd *erd) {
  StRd *this = MALLOC(StRd);
  this->ctx = ctx;
  this->erd = erd;
  return this;
}

/// Constructor.
///   ctx: Reader context.
///   tx : Text to read.
StRd *stRd_new (RdCtx *ctx, char *tx) {
  return stRd_new0 (ctx, expRd_new(ctx, tx));
}

/// Returns the current statement and advances the character reader.
Stat *stRd_next (StRd *this) {
  ExpRd *erd = this->erd;
  Stat *next2 () {
    Token *tk = expRd_peek_token(erd);
    int ln = tk->ln;
    switch (tk->tp) {
      case token_eof: return fail(ln, stRd_end_of_file);
      case token_operator: {
        expRd_next_token(erd);
        switch (*tk->value) {
          case '{': return read_block(this, ln);
          case '}': return stat_new_close_block(ln);
          case ':': return read_module_symbol(this);
          case ',': return read_multiassign(this, ln, "");
          case '_': return fail(ln, "Unexpected 'private' symbol");
          default:
            return fail(ln, str_f("Unexpected '%s'", token_to_str(tk)));
        }
      }
      default: {
        if (tk->tp == token_symbol) {
          char *v = tk->value;

          if (!strcmp(v, "break")) {
            expRd_next_token(erd);
            return check_end(this, stat_new_break(ln));
          } else if (!strcmp(v, "continue")) {
            expRd_next_token(erd);
            return check_end(this, stat_new_continue(ln));
          } else if (!strcmp(v, "return")) {
            expRd_next_token(erd);
            return read_return(this, ln);
          } else if (!strcmp(v, "throw")) {
            expRd_next_token(erd);
            return read_throw(this, ln);
          } else if (!strcmp(v, "trace")) {
            expRd_next_token(erd);
            return read_trace(this, ln);
          } else if (!strcmp(v, "while")) {
            expRd_next_token(erd);
            return read_while(this, ln);
          } else if (!strcmp(v, "if")) {
            expRd_next_token(erd);
            return read_if(this, ln);
          } else if (!strcmp(v, "switch")) {
            expRd_next_token(erd);
            return read_switch(this, ln);
          } else if (!strcmp(v, "for")) {
            expRd_next_token(erd);
            return read_for(this, ln);
          } else if (!strcmp(v, "try")) {
            expRd_next_token(erd);
            return read_try(this, ln);
          } else if (!strcmp(v, "import")) {
            expRd_next_token(erd);
            return read_import(this, ln);
          }
          // else continue below
        }

        // Statements starts with an expression
        Exp *e = expRd_next(erd);
        int ln = e->ln;
        switch (e->tp) {
          case exp_error: return fail(ln, e->value);
          case exp_symbol: return read_symbol(this, e);
          case exp_square: return read_square(this, e);
          case exp_parenthesis: return read_function(this, e);
          case exp_binary: {
            Tp3 *tp = e->value;
            return fail(ln, str_f("Unexpected operator '%s'", tp->e1));
          }
          default:
            return fail(ln, str_f("Unexpected expression '%s'", exp_to_str(e)));
        }
      }
    }
  }

  Token *tk = expRd_peek_token(erd);

  Token *ktp = NULL;
  if (tk->tp == token_ktype) {
    ktp = tk;
    expRd_next_token(erd);
  }

  Stat *st = next2();

  int must_have_type = st->tp == stat_indexed &&
    ((Tp3 *)st->value)->e1 != type_bool()
  ;
  if (!must_have_type && st->tp == stat_assign) {
    Tp *t = st->value;
    must_have_type =
      ((Exp *)t->e1)->tp == exp_symbol &&
      ((Exp *)t->e2)->tp == exp_function
    ;

  }

  if (must_have_type) { // Indexed or function assignation.
    if (!ktp) return fail(st->ln, "Type missing in assignation or indexed");
    int ln = ktp->ln;

    Rs *rs = type_read(ktp->value);
    Type *t = rs_get(rs);
    if (!t) return fail(ln, str_f("Wrong type: %s", rs_error(rs)));

    if (st->tp == stat_indexed) { // Indexed
      Tp3 *tp = st->value;
      tp->e1 = t;
      return st;
    }
    // Function assignation
    Tp *tp = st->value;
    Exp *e = tp->e2; // exp_function
    Tp3 *etp = e->value;
    return stat_new_assign(
      st->ln, tp->e1, exp_new_function(e->ln, t, etp->e2, etp->e3)
    );
  }

  // Normal statement.
  if (ktp && st->tp != stat_error) {
    return faile(
      st->ln,
      "'Indexed' or 'assignation of function to symbol' after type defintion",
      stat_to_str(st)
    );
  }
  return st;
}

Token *stRd_peek_token (StRd *this) {
  return expRd_peek_token(this->erd);
}

Token *stRd_next_token (StRd *this) {
  return expRd_next_token(this->erd);
}

Tp *stRd_read_def_type (StRd *this, int is_left) {
  return expRd_read_def_type(this->erd, is_left);
}

// STATIC BODY ---------------------------------------------

static Stat *fail (int ln, char *msg) {
  return stat_new_error(ln, msg);
}

static Stat *faile (int ln, char *expected, char *actual) {
  return fail(ln, str_f("Expected: %s\n  Actual: %s", expected, actual));
}

static Stat *check_end (StRd *this, Stat *st) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_next_token(erd);
  if (token_is_op(tk, ";"))
    return st;
  return faile(tk->ln, ";", token_to_str(tk));
}

static Stat *read_block (StRd *this, int ln) {
  RdCtx *ct = this->ctx;
  ExpRd *erd = this->erd;
  DicLayers *sm = ct->symMods;

  dicLayers_add_layer(sm);
  // Arr<Stat>
  Arr *sts = arr_new();
  for (;;) {
    Token *tk = expRd_peek_token(erd);
    if (tk->tp == token_eof) return fail(ln, "No closed {");
    if (token_is_op(tk, "}")) {
      expRd_next_token(erd);
      break;
    }

    Stat *st = stRd_next(this);
    switch (st->tp) {
      case stat_close_block: break;
      case stat_indexed:
        return fail(st->ln, "'indexed' or 'enumeration' out of main code");
      case stat_import: return fail(st->ln, "'import' out of main code");
      case stat_error: return st;
      default: arr_push(sts, st);
    }
  }
  dicLayers_remove_layer(sm);
  return stat_new_block(ln, sts);
}

static Stat *read_module_symbol (StRd *this) {
  RdCtx *ct = this->ctx;
  ExpRd *erd = this->erd;
  Token *tk = expRd_next_token(erd);
  if (tk->tp != token_symbol) return faile(tk->ln, "symbol", token_to_str(tk));
  char *mod = tk->value;

  Stat *st = stRd_next(this);
  switch (st->tp) {
    case stat_error: return st;
    case stat_assign: {
      Tp *tp = st->value;
      Exp *symSq = tp->e1;
      if (symSq->tp == exp_symbol) {
        DicLayers *sm = ct->symMods;
        dicLayers_put_element(sm, symSq->value, mod);
        return st;
      }
      return faile(symSq->ln,
        str_f("symbol of module %s", mod), stat_to_str(st)
      );
    }
    case stat_multiassign: {
      Tp *tp = st->value;
      // Arr<char>
      Arr *syms = tp->e1;
      char *sym = *arr_begin(syms);
      if (!*sym) return faile(st->ln, str_f("symbol of module %s", mod), ",");
      DicLayers *sm = ct->symMods;
      dicLayers_put_element(sm, sym, mod);
      return st;
    }
    default: return faile(st->ln,
      str_f("symbol of module %s", mod), stat_to_str(st)
      );
  }
}

static Stat *read_multiassign (StRd *this, int ln, char *first_sym) {
  RdCtx *ct = this->ctx;
  ExpRd *erd = this->erd;
  DicLayers *sm = ct->symMods;
  Arr *syms = arr_new_from(first_sym, NULL);
  for (;;) {
    Token *tk = expRd_peek_token(erd);
    if (token_is_op(tk, ",")) {
      expRd_next_token(erd);
      Token *tk = expRd_peek_token(erd);
      if (token_is_op(tk, "="))
        return fail(tk->ln, "Symbol '=' after ','");
      arr_push(syms, "");
      continue;
    }
    char *mod_sym = "";
    if (token_is_op(tk, ":")) {
      expRd_next_token(erd);
      Token *tk = expRd_next_token(erd);
      if (tk->tp == token_symbol) mod_sym = tk->value;
      else return faile(tk->ln, "symbol", token_to_str(tk));
    }
    Token *tk2 = expRd_next_token(erd);
    if (tk2->tp == token_symbol) arr_push(syms, tk2->value);
    else return faile(tk2->ln, "symbol", token_to_str(tk2));
    dicLayers_put_element(sm, tk2->value, mod_sym);

    Token *tk3 = expRd_next_token(erd);
    if (tk3->tp == token_operator) {
      if (*tk3->value == ',') continue;
      else if (*tk3->value == '=') {
        Token *tk = expRd_peek_token(erd);
        if (token_is_op(tk, ";")) {
          expRd_next_token(erd);
          return stat_new_indexed(ln, type_bool(), "", syms);
        }
        Exp *e = expRd_next(erd);
        if (e->tp == exp_error) return fail(e->ln, e->value);
        return check_end(this, stat_new_multiassign(ln, syms, e));
      } // else continue below.
    }
    return faile(tk->ln, "= or ,", token_to_str(tk));
  }
}

static Stat *read_return (StRd *this, int ln) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_peek_token(erd);
  if (token_is_op(tk, ";"))
    return check_end(this, stat_new_return(ln, opt_none()));
  Exp *e = expRd_next(erd);
  if (e->tp == exp_error) return fail(e->ln, e->value);
  return check_end(this, stat_new_return(ln, opt_some(e)));
}

static Stat *read_throw (StRd *this, int ln) {
  ExpRd *erd = this->erd;
  Exp *e = expRd_next(erd);
  if (e->tp == exp_error) return fail(e->ln, e->value);
  return check_end(this, stat_new_throw(ln, e));
}

static Stat *read_trace (StRd *this, int ln) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_peek_token(erd);
  int is_complete = FALSE;
  if (token_is_op(tk, ":")) {
    expRd_next_token(erd);
    is_complete = TRUE;
  }
  Exp *e = expRd_next(erd);
  if (e->tp == exp_error) return fail(e->ln, e->value);
  return check_end(this, stat_new_trace(ln, is_complete, e));

}

static Stat *read_while (StRd *this, int ln) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_next_token(erd);
  if (!token_is_op(tk, "("))
    return faile(tk->ln, "(", token_to_str(tk));

  // Opt<Exp>
  Opt *cond = opt_none();
  Token *tk2 = expRd_peek_token(erd);
  if (!token_is_op(tk2, ")")) {
    Exp *e = expRd_next(erd);
    if (e->tp == exp_error) return fail(e->ln, e->value);
    cond = opt_some(e);
  }

  Token *tk3 = expRd_next_token(erd);
  if (!token_is_op(tk3, ")"))
    return faile(tk3->ln, ")", token_to_str(tk3));

  Stat *st = stRd_next(this);

  switch (st->tp) {
    case stat_error: return st;
    case stat_indexed:
      return fail(st->ln, "'indexed' or 'enumeration' out of main code");
    case stat_import: return fail(st->ln, "'import' out of main code");
    case stat_block: return stat_new_while(ln, cond, st);
    default: return stat_new_while(
        ln, cond, stat_new_block(st->ln, arr_new_from(st, NULL))
      );
  }
}

static Stat *read_if (StRd *this, int ln) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_next_token(erd);
  if (!token_is_op(tk, "("))
    return faile(tk->ln, "(", token_to_str(tk));

  Exp *cond = expRd_next(erd);
  if (cond->tp == exp_error) return fail(cond->ln, cond->value);

  Token *tk2 = expRd_next_token(erd);
  if (!token_is_op(tk2, ")"))
    return faile(tk2->ln, ")", token_to_str(tk2));

  Stat *st = stRd_next(this);

  switch (st->tp) {
    case stat_error: return st;
    case stat_indexed:
      return fail(st->ln, "'indexed' or 'enumeration' out of main code");
    case stat_import: return fail(st->ln, "'import' out of main code");
    default: {
      Stat *stYes = st->tp == stat_block
          ? st
          : stat_new_block(st->ln, arr_new_from(st, NULL))
        ;
      Token *tk = expRd_peek_token(erd);
      if (token_is_sym(tk, "else")) {
        expRd_next_token(erd);
        Stat *st2 = stRd_next(this);
        switch (st2->tp) {
          case stat_error: return st2;
          case stat_indexed:
            return fail(st->ln, "'indexed' or 'enumeration' out of main code");
          case stat_import: return fail(st2->ln, "'import' out of main code");
          default: {
            // Opt<stat>
            Opt *stNo = opt_some(st2->tp == stat_block
                ? st2
                : stat_new_block(st2->ln, arr_new_from(st2, NULL))
              );
            return stat_new_if(ln, cond, stYes, stNo);
          }
        }
      }
      return stat_new_if(ln, cond, stYes, opt_none());
    }
  }
}

static Stat *read_switch (StRd *this, int ln) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_next_token(erd);
  if (!token_is_op(tk, "("))
    return faile(tk->ln, "(", token_to_str(tk));

  Exp *e = expRd_next(erd);
  if (e->tp == exp_error) return fail(e->ln, e->value);

  Token *tk2 = expRd_next_token(erd);
  if (!token_is_op(tk2, ")"))
    return faile(tk2->ln, ")", token_to_str(tk2));

  Token *tk3 = expRd_next_token(erd);
  if (!token_is_op(tk3, "{"))
    return faile(tk3->ln, "{", token_to_str(tk3));

  // Arr<Tp<Arr<Exp>, Stat>>
  Arr *es = arr_new();
  int with_default = FALSE;
  for (;;) {
    // Tp<Arr<Exp>, Exp> ([Array, Error])
    Tp *tp = expRd_read_array(erd, ':');
    Arr *a = tp->e1;
    if (!arr_size(a)) {
      Exp *err = tp->e2;
      return fail(err->ln, err->value);
    }

    if (arr_size(a) > 1) {
      int fany (Exp *e) {
        return e->tp == exp_symbol && str_eq(e->value, "default");
      }
      if (arr_any(a, (FPRED)fany)) {
        Exp *e = *arr_begin(a);
        return fail(e->ln, "'default' in a multiconditional entry");
      }
      // Continue below
    } else {
      Exp *e = *arr_begin(a);
      if (e->tp == exp_symbol && str_eq(e->value, "default"))
        with_default = TRUE;
    }

    Stat *st = stRd_next(this);
    if (st->tp == stat_error) return st;
    arr_push(es, tp_mk(a, st->tp == stat_block
      ? st
      : stat_new_block(st->ln, arr_new_from(st, NULL))
    ));

    Token *tk3 = expRd_peek_token(erd);
    if (token_is_op(tk3, "}")) {
      expRd_next_token(erd);
      break;
    }

    if (with_default) return fail(tk3->ln, "Conditions after 'default'");
  }

  return stat_new_switch(ln, e, es);
}

static Stat *read_for (StRd *this, int ln) {
  RdCtx *ct = this->ctx;
  ExpRd *erd = this->erd;
  DicLayers *sm = ct->symMods;
  dicLayers_add_layer(sm);

  char *v2 = "";

  Token *tk = expRd_next_token(erd);
  if (!token_is_op(tk, "("))
    return faile(tk->ln, "(", token_to_str(tk));

  char *md = "";
  Token *tk2 = expRd_peek_token(erd);
  if (token_is_op(tk2, ":")) {
    expRd_next_token(erd);
    Token *tk = expRd_next_token(erd);
    if (tk->tp == token_symbol) md = tk->value;
    else return faile(tk->ln, "symbol", token_to_str(tk));
  }
  Token *tk3 = expRd_next_token(erd);
  if (tk3->tp != token_symbol)
    return faile(tk3->ln, "symbol", token_to_str(tk3));
  char *v1 = tk3->value;
  dicLayers_put_element(sm, v1, md);

  Token *tk4 = expRd_peek_token(erd);
  if (token_is_op(tk4, ",")) {
    expRd_next_token(erd);
    char *md = "";
    Token *tk =expRd_peek_token(erd);
    if (token_is_op(tk, ":")) {
      expRd_next_token(erd);
      Token *tk = expRd_next_token(erd);
      if (tk->tp == token_symbol) md = tk->value;
      else return faile(tk->ln, "symbol", token_to_str(tk));
    }
    Token *tk2 = expRd_next_token(erd);
    if (tk2->tp != token_symbol)
      return faile(tk2->ln, "symbol", token_to_str(tk2));
    v2 = tk2->value;
    if (str_eq(v2, v1))
      return fail(tk2->ln, str_f("Duplicate 'for' variable '%s'", v1));
    dicLayers_put_element(sm, v2, md);
  }

  Token *tk5 = expRd_next_token(erd);
  if (!token_is_op(tk5, "="))
    return faile(tk5->ln, "= or ,", token_to_str(tk5));

  Exp *e1 = expRd_next(erd);
  Opt *e2 = opt_none(); // Opt<Exp>
  Opt *e3 = opt_none(); // Opt<Exp>
  if (e1->tp == exp_error) return fail(e1->ln, e1->value);

  Token *tk6 = expRd_peek_token(erd);
  if (token_is_op(tk6, ":")) {
    if (*v2) return faile(tk6->ln, ")", token_to_str(tk6));

    expRd_next_token(erd);
    Exp *e = expRd_next(erd);
    if (e->tp == exp_error) return fail(e->ln, e->value);
    e2 = opt_some(e);

    Token *tk = expRd_peek_token(erd);
    if (token_is_op(tk, ":")) {
      expRd_next_token(erd);
      Exp *e = expRd_next(erd);
      if (e->tp == exp_error) return fail(e->ln, e->value);
      e3 = opt_some(e);
    }
  }

  Token *tk7 = expRd_next_token(erd);
  if (!token_is_op(tk7, ")"))
    return faile(tk7->ln, ")", token_to_str(tk7));

  Stat *st0 = stRd_next(this);

  dicLayers_remove_layer(sm);

  switch (st0->tp) {
    case stat_error: return st0;
    case stat_indexed:
      return fail(st0->ln, "'indexed' or 'enumeration' out of main code");
    case stat_import: return fail(st0->ln, "'import' out of main code");
    default: {
      Stat *st = st0->tp == stat_block
        ? st0
        : stat_new_block(st0->ln, arr_new_from(st0, NULL))
      ;
      if (*v2) {
        return stat_new_for(ln, opt_some(v1), v2, e1, st);
      } else {
        Exp *e2_ok = opt_get(e2);
        if (!e2_ok) return stat_new_for(ln, opt_none(), v1, e1, st);
        else return stat_new_for_range(ln, v1, e1, e2_ok, e3, st);
      }
    }
  }
}

static Stat *read_try (StRd *this, int ln) {
  RdCtx *ct = this->ctx;
  ExpRd *erd = this->erd;

  Stat *st = stRd_next(this);

  switch (st->tp) {
    case stat_error: return st;
    case stat_indexed:
      return fail(st->ln, "'indexed' or 'enumeration' out of main code");
    case stat_import: return fail(st->ln, "'import' out of main code");
    default: {} // continue below
  }
  Stat *st1 = st->tp == stat_block
    ? st
    : stat_new_block(st->ln, arr_new_from(st, NULL))
  ;

  Token *tk = expRd_next_token(erd);
  if (tk->tp != token_symbol || strcmp(tk->value, "catch"))
    return faile(tk->ln, "catch", token_to_str(tk));

  Token *tk2 = expRd_next_token(erd);
  if (!token_is_op(tk2, "("))
    return faile(tk2->ln, "(", token_to_str(tk2));

  char *md = "str";
  Token *tk3 = expRd_peek_token(erd);
  if (token_is_op(tk3, ":")) {
    expRd_next_token(erd);
    Token *tk = expRd_next_token(erd);
    if (tk->tp == token_symbol) md = tk->value;
    else return faile(tk->ln, "symbol", token_to_str(tk));
  }
  Token *tk4 = expRd_next_token(erd);
  if (tk4->tp != token_symbol)
    return faile(tk4->ln, "symbol", token_to_str(tk4));
  char *v = tk4->value;

  Token *tk5 = expRd_next_token(erd);
  if (!token_is_op(tk5, ")"))
    return faile(tk5->ln, ")", token_to_str(tk5));

  DicLayers *sm = ct->symMods;
  dicLayers_add_layer(sm);
  dicLayers_put_element(sm, v, md);

  Stat *st02 = stRd_next(this);

  dicLayers_remove_layer(sm);

  switch (st02->tp) {
    case stat_error: return st02;
    case stat_indexed:
      return fail(st->ln, "'indexed' or 'enumeration' out of main code");
    case stat_import: return fail(st02->ln, "'import' out of main code");
    default: {} // continue below
  }
  Stat *st2 = st02->tp == stat_block
    ? st02
    : stat_new_block(st02->ln, arr_new_from(st02, NULL));

  return stat_new_try(ln, st1, v, st2);
}

static Stat *read_import (StRd *this, int ln) {
  ExpRd *erd = this->erd;

  Token *tk = expRd_next_token(erd);
  if (tk->tp != token_string)
    return faile(tk->ln, "string", token_to_str(tk));

  Token *tk2 = expRd_peek_token(erd);
  if (token_is_op(tk2, ":")) {
    expRd_next_token(erd);
    Token *tk2 = expRd_next_token(erd);
    if (tk2->tp != token_symbol)
      return faile(tk2->ln, "symbol", token_to_str(tk2));
    return check_end(this, stat_new_import(ln, tk->value, opt_some(tk2->value)));
  }
  return check_end(this, stat_new_import(ln, tk->value, opt_none()));
}

// sym is exp_symbol
static Stat *read_symbol (StRd *this, Exp *sym) {
  RdCtx *ct = this->ctx;
  ExpRd *erd = this->erd;
  DicLayers *sm = ct->symMods;
  dicLayers_put_element(sm, sym->value, "");
  int ln = sym->ln;

  Token *tk = expRd_next_token(erd);
  if (tk->tp == token_operator) {
    if (*tk->value == ',') return read_multiassign(this, ln, sym->value);
    if (*tk->value == ':') {
      // Arr<char>
      Arr *syms = arr_new();
      for (;;) {
        Token *tk2 = expRd_next_token(erd);
        if (tk2->tp != token_symbol)
          return faile(tk2->ln, "symbol", token_to_str(tk2));
        arr_push(syms, tk2->value);
        Token *tk3 = expRd_peek_token(erd);
        if (token_is_op(tk3, ",")) {
          expRd_next_token(erd);
          continue;
        }
        break;
      }
      // Fake type
      return check_end(this, stat_new_indexed(
        ln, type_new("t", arr_new()), sym->value, syms)
      );
    }
    else if (*tk->value == '=') {
      Exp *e = expRd_next(erd);
      switch (e->tp) {
        case exp_error: return fail(e->ln, e->value);
        case exp_string: {
          dicLayers_put_element(sm, sym->value, "str");
          break;
        }
        case exp_array:
        case exp_array0: {
          dicLayers_put_element(sm, sym->value, "arr");
          break;
        }
        case exp_dictionary:
        case exp_dictionary0: {
          dicLayers_put_element(sm, sym->value, "dic");
          break;
        }
        case exp_range: {
          dicLayers_put_element(sm, sym->value, "iter");
          break;
        }
        case exp_parenthesis: {
          Tp *tp = e->value;
          Exp *fn = tp->e1;
          if (fn->tp == exp_point) {
            Tp *tp2 = fn->value;
            dicLayers_put_element(sm, sym->value, tp2->e1);
          }
          break;
        }
        default: {};
      }
      return check_end(this, stat_new_assign(ln, sym, e));
    } // else continue below.
  }
  return faile(tk->ln, "= or ,", token_to_str(tk));
}

static Stat *read_square (StRd *this, Exp *e) {
  ExpRd *erd = this->erd;
  Token *tk = expRd_next_token(erd);
  if (tk->tp == token_operator) {
    Exp *e2 = expRd_next(erd);
    if (e2->tp == exp_error) return fail(e2->ln, e2->value);
    char *v = tk->value;
    if (*tk->value == '=')
      return check_end(this, stat_new_assign(e->ln, e, e2));
    else if (!strcmp(v, "+="))
      return check_end(this, stat_new_addas(e->ln, e, e2));
    else if (!strcmp(v, "-="))
      return check_end(this, stat_new_subas(e->ln, e, e2));
    else if (!strcmp(v, "*="))
      return check_end(this, stat_new_mulas(e->ln, e, e2));
    else if (!strcmp(v, "/="))
      return check_end(this, stat_new_divas(e->ln, e, e2));
    else if (!strcmp(v, "%="))
      return check_end(this, stat_new_modas(e->ln, e, e2));
    else if (!strcmp(v, "&="))
      return check_end(this, stat_new_andas(e->ln, e, e2));
    else if (!strcmp(v, "|="))
      return check_end(this, stat_new_oras(e->ln, e, e2));
    // else below
  }
  return faile(tk->ln, "assignation", token_to_str(tk));
}

static Stat *read_function (StRd *this, Exp *e) {
  return check_end(this, stat_new_function(e->ln, e));
}
