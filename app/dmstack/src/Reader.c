// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Reader.h"
#include "DEFS.h"
#include "tkreader.h"
#include "imports.h"

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
    arr_push(r, tk);
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
  } else {
    EACHL(this->syms, SymbolKv, e) {
      if (symbolKv_key(e) == sym) {
//printf("%ld, %s\n", sym, symbol_to_str(sym));
        return token_new_symbol(token_line(tk), symbolKv_value(e));
      }
    }_EACH
  }
  return tk;
}

void reader_fail (Reader *this, char *msg) {
  printf("%s:%d: %s\n", this->source, this->nline, msg);
  exit(1);
}
