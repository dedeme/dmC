// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Reader.h"
#include "DEFS.h"
#include "tkreader.h"
#include "imports.h"
#include "args.h"

struct reader_Reader {
  int is_file;
  Symbol source;
  int nline;
  char *prg;
  int prg_ix;
  Token *next_tk;
  // List<SymbolKv>
  List *syms;
  int stkcounter;
};

Reader *reader_new (char *source, char *prg) {
  Reader *this = MALLOC(Reader);
  this->is_file = 1;
  this->source = symbol_new(source);
  this->nline = 1;
  this->prg = prg;
  this->prg_ix = 0;
  this->next_tk = NULL;
  this->syms = list_new();
  this->stkcounter = 0;
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
  this->stkcounter = 0;
  return this;
}

Token *reader_process (Reader *this) {
  int nline = this->nline;

  // Arr<Token>
  Arr *r = arr_new();
  Token *tk = opt_nget(tkreader_next(this));
  while (tk) {
    if (token_type(tk) == token_SYMBOL) {
      EACH(reader_symbol_id(this, r, tk), Token, t) {
        arr_push(r, t);
      }_EACH
    } else if (token_type(tk) == token_STRING) {
      EACH(reader_interpolation(this, tk), Token, t) {
        arr_push(r, t);
      }_EACH
    } else {
      arr_push(r, tk);
    }
    tk = opt_nget(tkreader_next(this));
  }
  if (this->stkcounter)
    reader_fail(this, str_f(
      "Expected 0 'stack stops'. Actual %d.", this->stkcounter
  ));
  return token_new_list_pos(r, this->source, nline);
}

int reader_is_file (Reader *this) {
  return this->is_file;
}

Symbol reader_source (Reader *this) {
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

// Return Arr<Token>. prg is Arr<Token>
Arr *reader_symbol_id (Reader *this, Arr *prg, Token *tk) {
  Symbol sym = token_symbol(tk);
  char *symstr = symbol_to_str(sym);
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
      path_parent(symbol_to_str(this->source)),
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
    TokenPos *pos = opt_nget(token_pos(tk));
    if (!pos) EXC_ILLEGAL_STATE("pos is null")

    return arr_new_from(
      token_new_symbol_pos(this->source, this->source, tokenPos_line(pos)), NULL
    );
  } else if (*symstr == '.' && symstr[1]) {
    TokenPos *pos = opt_nget(token_pos(tk));
    if (!pos) EXC_ILLEGAL_STATE("pos is null")

    int line = tokenPos_line(pos);
    return arr_new_from(
      token_new_string_pos(str_right(symstr, 1), this->source, line),
      token_new_symbol_pos(symbol_new("map"), this->source, line),
      token_new_symbol_pos(symbol_new("get"), this->source, line),
      NULL
    );
  } else if (*symstr == '@') {
    TokenPos *pos = opt_nget(token_pos(tk));
    if (!pos) EXC_ILLEGAL_STATE("pos is null")

    int line = tokenPos_line(pos);
    if (symstr[1] == '?') {
      Token *tk = token_new_symbol_pos(
        symbol_new(str_right(symstr, 2)), this->source, line
      );
      return arr_new_from(
        token_new_list_pos(arr_new_from(tk, NULL), this->source, line),
        token_new_symbol_pos(symbol_STACK_CHECK, this->source, line),
        NULL
      );
    } else if (symstr[1] == '+') {
      if (args_is_production()) return arr_new();

      ++this->stkcounter;
      Token *tk = token_new_symbol_pos(
        symbol_new(str_right(symstr, 2)), this->source, line
      );
      return arr_new_from(
        token_new_list_pos(arr_new_from(tk, NULL), this->source, line),
        token_new_symbol_pos(symbol_STACK_OPEN, this->source, line),
        NULL
      );
    } else if (symstr[1] == '-') {
      if (args_is_production()) return arr_new();

      --this->stkcounter;
      Token *tk = token_new_symbol_pos(
        symbol_new(str_right(symstr, 2)), this->source, line
      );
      return arr_new_from(
        token_new_list_pos(arr_new_from(tk, NULL), this->source, line),
        token_new_symbol_pos(symbol_STACK_CLOSE, this->source, line),
        NULL
      );
    } else {
      if (args_is_production()) return arr_new();

      Token *tk = token_new_symbol_pos(
        symbol_new(str_right(symstr, 1)), this->source, line
      );
      return arr_new_from(
        token_new_list_pos(arr_new_from(tk, NULL), this->source, line),
        token_new_symbol_pos(symbol_STACK, this->source, line),
        NULL
      );
    }
  } else if (this->prg[this->prg_ix] == ',') {
    EACHL(this->syms, SymbolKv, e) {
      if (symbolKv_key(e) == sym) {
        TokenPos *pos = opt_nget(token_pos(tk));
        if (!pos) EXC_ILLEGAL_STATE("pos is null")
        return arr_new_from(
          token_new_symbol_pos(
            symbolKv_value(e), this->source, tokenPos_line(pos)
          ), NULL
        );
      }
    }_EACH
  }
  return arr_new_from(tk, NULL);
}

// Arr<Token>
Arr *reader_interpolation (Reader *this, Token *tk) {
  char *s = token_string(tk);
  TokenPos *postk = opt_nget(token_pos(tk));
  if (!postk) EXC_ILLEGAL_STATE("postk is null")
  int nline = tokenPos_line(postk);
  // Arr<Token>
  Arr *r = arr_new();
  int pos = 0;
  int ix = str_index(s, "${");
  while (ix != -1) {
    arr_push(r, token_new_string_pos(
      str_sub(s, pos, ix), this->source, nline
    ));
    if (pos) arr_push(r, token_new_symbol_pos(
      symbol_PLUS, this->source, nline
    ));

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
    Arr *prg = token_list(reader_process(subr));

    p = s + ix + 2;
    p_end = s + ix2;
    while (p < p_end) if (*p++ == '\n') ++nline;

    if (arr_size(prg)) {
      arr_push(r, token_new_list_pos(prg, this->source, nline));
      arr_push(r, token_new_symbol_pos(symbol_DATA, this->source, nline));
      arr_push(r, token_new_symbol_pos(symbol_REF_OUT, this->source, nline));
      arr_push(r, token_new_symbol_pos(symbol_TO_STR, this->source, nline));
      arr_push(r, token_new_symbol_pos(symbol_PLUS, this->source, nline));
    } else {
      arr_push(r, token_new_string_pos("", this->source, nline));
      arr_push(r, token_new_symbol_pos(symbol_PLUS, this->source, nline));
    }

    pos = ix2 + 1;
    ix = str_index_from(s, "${", pos);
  }

  arr_push(r, token_new_string_pos(str_right(s, pos), this->source, nline));

  if (arr_size(r) > 1) {
    char *p = s + pos;
    char *p_end = s + strlen(s);
    while (p < p_end) if (*p++ == '\n') ++nline;
    arr_push(r, token_new_symbol_pos(symbol_PLUS, this->source, nline));
  }
  return r;
}

void reader_fail (Reader *this, char *msg) {
  printf("%s.dms:%d: %s\n", symbol_to_str(this->source), this->nline, msg);
  exit(1);
}
