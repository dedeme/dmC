// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Reader.h"
#include "DEFS.h"
#include "tkreader.h"
#include "imports.h"
#include "args.h"

struct reader_Reader {
  int is_file;
  char *source;
  int nline;
  char *prg;
  int prg_ix;
  Token *next_tk;
  // List<SymbolKv>
  List *syms;
};

Reader *reader_new (char *source, char *prg) {
  Reader *this = MALLOC(Reader);
  this->is_file = 1;
  this->source = source;
  this->nline = 1;
  this->prg = prg;
  this->prg_ix = 0;
  this->next_tk = NULL;
  this->syms = list_new();
  return this;
}

Reader *reader_new_from_reader (Reader *parent, char *prg, int nline) {
  Reader *this = MALLOC(Reader);
  this->is_file = 0;
  this->source = parent->source;
  this->nline = nline;
  this->prg = prg;
  this->prg_ix = 0;
  this->next_tk = NULL;
  this->syms = parent->syms;
  return this;
}

Token *reader_process (Reader *this) {
  int nline = this->nline;

  // Arr<Token>
  Arr *r = arr_new();
  Token *tk = opt_nget(tkreader_next(this));
  while (tk) {
    if (token_type(tk) == token_SYMBOL) tk = reader_symbol_id(this, r, tk);
    if (token_type(tk) == token_STRING) {
      EACH(reader_interpolation(this, tk), Token, t) {
        arr_push(r, t);
      }_EACH
    } else {
      arr_push(r, tk);
    }
    tk = opt_nget(tkreader_next(this));
  }
  return token_new_list(nline, r);
}

int reader_is_file (Reader *this) {
  return this->is_file;
}

char *reader_source (Reader *this) {
  return this->source;
}

char *reader_prg (Reader *this) {
  return this->prg;
}

Token *reader_next_tk (Reader *this) {
  return this->next_tk;
}

void reader_set_next_tk (Reader *this, Token *tk) {
  this->next_tk = tk;
}

int reader_prg_ix (Reader *this) {
  return this->prg_ix;
}

void reader_set_prg_ix (Reader *this, int value) {
  this->prg_ix = value;
}

int reader_nline (Reader *this) {
  return this->nline;
}

void reader_set_nline (Reader *this, int value) {
  this->nline = value;
}

Token *reader_symbol_id (Reader *this, Arr *prg, Token *tk) {
  Symbol sym = token_symbol(tk);
  if (sym == symbol_IMPORT) {
    if (!arr_size(prg)) reader_fail(this, "Import source is missing");

    Token *t = arr_peek(prg);
    // Kv<SymbolKv>
    Kv *esymkv = imports_read_symbol(t);
    if (*kv_key(esymkv)) reader_fail(this, kv_key(esymkv));

    SymbolKv *symkv = kv_value(esymkv);
    Symbol sym = symbolKv_value(symkv);
    char *f = symbol_to_str(sym);
    char *fc = opt_nget(path_canonical(path_cat(
      path_parent(this->source),
      str_cat(f, ".dms", NULL),
      NULL
    )));
    if (!fc)
      reader_fail(this, str_f(
        "Import file '%s' not found", symbol_to_str(sym)
      ));

    Symbol newsym = symbol_new(str_left(fc, -4));
    Symbol key = symbolKv_key(symkv);
    if (key == -1) key = symbol_new(path_name(f));
    this->syms = list_cons(
      this->syms, symbolKv_new(key, newsym)
    );
  } else if (sym == symbol_THIS) {
    return token_new_symbol(token_line(tk), symbol_new(this->source));
  } else if (*symbol_to_str(sym) == '@') {
    int line = token_line(tk);
    char *name = symbol_to_str(sym);
    if (name[strlen(name) - 1] == '?') {
      this->next_tk = token_new_symbol(line, symbol_STACK_CHECK);
      Token *tk = token_new_symbol(line, symbol_new(str_sub(name, 1, -1)));
      return token_new_list(line, arr_new_from(tk, NULL));
    } else {
      if (args_is_production()) {
        return token_new_symbol(line, symbol_NOP);
      } else {
        this->next_tk = token_new_symbol(line, symbol_STACK);
        Token *tk = token_new_symbol(line, symbol_new(str_right(name, 1)));
        return token_new_list(line, arr_new_from(tk, NULL));
      }
    }
  } else {
    EACHL(this->syms, SymbolKv, e) {
      if (symbolKv_key(e) == sym) {
        return token_new_symbol(token_line(tk), symbolKv_value(e));
      }
    }_EACH
  }
  return tk;
}

// Arr<Token>
Arr *reader_interpolation (Reader *this, Token *tk) {
  char *s = token_string(tk);
  int nline = token_line(tk);
  // Arr<Token>
  Arr *r = arr_new();
  int pos = 0;
  int ix = str_index(s, "${");
  while (ix != -1) {
    arr_push(r, token_new_string(nline, str_sub(s, pos, ix)));

    char *p = s + pos;
    char *p_end = s + ix;
    while (p < p_end) if (*p++ == '\n') ++nline;

    int ix2 = str_index_from(s, "}", ix);
    if (ix2 == -1)
      reader_fail(this, "Interpolation '}' not found");

    Reader *subr = reader_new_from_reader(
      this, str_sub(s, ix + 2, ix2), nline
    );

    // Arr<Token>
    Arr *prg = arr_new();
    Token *tk = opt_nget(tkreader_next(subr));
    while (tk) {
      if (token_type(tk) == token_SYMBOL)
        tk = reader_symbol_id(subr, prg, tk);
      arr_push(prg, tk);
      tk = opt_nget(tkreader_next(subr));
    }

    p = s + ix + 2;
    p_end = s + ix2;
    while (p < p_end) if (*p++ == '\n') ++nline;

    if (arr_size(prg)) {
      arr_push(r, token_new_list(nline, prg));
      arr_push(r, token_new_symbol(nline, symbol_DATA));
      arr_push(r, token_new_symbol(nline, symbol_REF_OUT));
      arr_push(r, token_new_symbol(nline, symbol_TO_STR));
      arr_push(r, token_new_symbol(nline, symbol_PLUS));
    } else {
      arr_push(r, token_new_string(nline, ""));
      arr_push(r, token_new_symbol(nline, symbol_PLUS));
    }

    pos = ix2 + 1;
    ix = str_index_from(s, "${", pos);
  }

  arr_push(r, token_new_string(nline, str_right(s, pos)));

  if (arr_size(r) > 1) {
    char *p = s + pos;
    char *p_end = s + strlen(s);
    while (p < p_end) if (*p++ == '\n') ++nline;
    arr_push(r, token_new_symbol(nline, symbol_PLUS));
  }
  return r;
}

void reader_fail (Reader *this, char *msg) {
  printf("%s.dms:%d: %s\n", this->source, this->nline, msg);
  exit(1);
}
