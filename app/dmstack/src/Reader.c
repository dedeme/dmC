// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Reader.h"
#include "DEFS.h"
#include "tkreader.h"
#include "imports.h"

struct reader_Reader {
  char *source;
  int nline;
  char *prg;
  int prg_ix;
  Token *next_tk;
  // Arr<Symbol>
  Arr *syms;
};

Reader *reader_new (char *source, char *prg, int nline, int prg_ix) {
  Reader *this = MALLOC(Reader);
  this->source = source;
  this->nline = nline;
  this->prg = prg;
  this->prg_ix = prg_ix;
  this->next_tk = NULL;
  this->syms = arr_new();
  return this;
}

Token *reader_process (Reader *this) {
  int nline = this->nline;
  // Arr<Token>
  Arr *r = arr_new();
  Token *tk = opt_nget(tkreader_next(this));
  while (tk) {
    if (token_type(tk) == token_SYMBOL) {
      char *id = symbol_id(token_symbol(tk));
      if (str_eq(id, "this")) {
        tk = token_new_symbol(
          token_line(tk), symbol_new_id(this->source, "this")
        );
      } else if (str_eq(id, "import")) {
        if (!arr_size(r)) reader_fail(this, "Import source is missing");

        Token *t = arr_peek(r);
        // Kv<Symbol>
        Kv *esym = imports_read_symbol(t);
        if (*kv_key(esym)) reader_fail(this, kv_key(esym));
        Symbol *sym = kv_value(esym);
        char *fc = opt_nget(path_canonical(path_cat(
          path_parent(this->source),
          str_cat(symbol_id(sym), ".dms", NULL),
          NULL
        )));
        if (!fc)
          reader_fail(this, str_f(
            "Import file '%s' not found %s", str_cat(symbol_id(sym), ".dms", NULL)
            , path_cat(
          this->source, str_cat(symbol_id(sym), ".dms", NULL), NULL
        )
          ));
        arr_push(this->syms, symbol_new_id(
          str_left(fc, -4), symbol_name(sym)
        ));
      } else {
        char *name = symbol_name(token_symbol(tk));
        int fn (Symbol *sym) { return str_eq(name, symbol_name(sym)); }
        Symbol *sym = opt_nget(it_find(it_from(this->syms), (FPRED)fn));
        if (sym) {
          tk = token_new_symbol(token_line(tk), sym);
        }
      }
    }

    arr_push(r, tk);
    tk = opt_nget(tkreader_next(this));
  }
  return token_new_list(nline, r);
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

void reader_fail (Reader *this, char *msg) {
  printf("%s:%d: %s\n", this->source, this->nline, msg);
  exit(1);
}
