// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Token.h"

union token_Value {
  int i;
  double f;
  char *s;
  Bytes *bs;
  Arr *a; // Arr<Token>
  Symbol *sym;
};

struct token_Token {
  enum token_Type type;
  union token_Value *value;
  int line;
};

Token *token_new_int (int line, int value) {
  union token_Value *v = MALLOC(union token_Value);
  v->i = value;
  Token *this = MALLOC(Token);
  this->type = token_INT;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_float (int line, double value) {
  union token_Value *v = MALLOC(union token_Value);
  v->f = value;
  Token *this = MALLOC(Token);
  this->type = token_FLOAT;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_string (int line, char *value) {
  union token_Value *v = MALLOC(union token_Value);
  v->s = value;
  Token *this = MALLOC(Token);
  this->type = token_STRING;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_blob (int line, Bytes *bs) {
  union token_Value *v = MALLOC(union token_Value);
  v->bs = bs;
  Token *this = MALLOC(Token);
  this->type = token_BLOB;
  this->value = v;
  this->line = line;
  return this;
}

// value is Arr<Token>
Token *token_new_list (int line, Arr *value) {
  union token_Value *v = MALLOC(union token_Value);
  v->a = value;
  Token *this = MALLOC(Token);
  this->type = token_LIST;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_symbol (int line, Symbol *value) {
  union token_Value *v = MALLOC(union token_Value);
  v->sym = value;
  Token *this = MALLOC(Token);
  this->type = token_SYMBOL;
  this->value = v;
  this->line = line;
  return this;
}

enum token_Type token_type (Token *this) {
  return this->type;
}

int token_line (Token *this) {
  return this->line;
}

int token_int (Token *this) {
  return this->value->i;
}

double token_float (Token *this) {
  return this->value->f;
}

char *token_string (Token *this) {
  return this->value->s;
}

Bytes *token_blob (Token *this) {
  return this->value->bs;
}

// Returns Arr<Token>
Arr *token_list (Token *this) {
  return this->value->a;
}

Symbol *token_symbol (Token *this) {
  return this->value->sym;
}

Token *token_clone (Token *this) {
  switch (this->type) {
    case token_INT: return token_new_int(0, this->value->i);
    case token_FLOAT: return token_new_float(0, this->value->f);
    case token_STRING: return token_new_string(0, str_new(this->value->s));
    case token_SYMBOL: return token_new_symbol(
      0, symbol_clone(this->value->sym)
    );
    case token_BLOB: {
      Bytes *bs = this->value->bs;
      return token_new_blob(0, bytes_from_bytes(bytes_bs(bs), bytes_len(bs)));
    }
    case token_LIST: {
      int size = arr_size(this->value->a);
      Token **r = GC_MALLOC(size * sizeof(Token *));
      Token **source = (Token **)arr_start(this->value->a);
      Token **target = r;
      int sz = size;
      while (sz--)
        *target++ = token_clone(*source++);
      return token_new_list(0, arr_new_c(size, (void **)r));
    }
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

int token_eq (Token *this, Token *other) {
  int bseq (Bytes *bs1, Bytes *bs2) {
    int len = bytes_len(bs1);
    if (len != bytes_len(bs2)) return 0;
    unsigned char *b1 = bytes_bs(bs1);
    unsigned char *b2 = bytes_bs(bs2);
    unsigned char *end = b1 + len;
    while (b1 < end)
      if (*b1++ != *b2++) return 0;
    return 1;
  }
  // a1 and a2 are Arr<Token>
  int arreq (Arr *a1, Arr *a2) {
    int size = arr_size(a1);
    if (size != arr_size(a2)) return 0;
    Token **ts1 = (Token **)arr_start(a1);
    Token **ts2 = (Token **)arr_start(a2);
    Token **end = ts1 + size;
    while (ts1 < end)
      if (!token_eq(*ts1++, *ts2++)) return 0;
    return 1;
  }

  enum token_Type t = token_type(this);
  if (t != token_type(other)) return 0;

  return
    t == token_INT ? this->value->i == other->value->i
    : t == token_FLOAT ? this->value->f == other->value->f
    : t == token_STRING ? str_eq(this->value->s, other->value->s)
    : t == token_SYMBOL ? symbol_eq(this->value->sym, other->value->sym)
    : t == token_BLOB ? bseq(this->value->bs, other->value->bs)
    : arreq(this->value->a, other->value->a);
  ;
}

char *token_to_str (Token *this) {
  switch (this->type) {
    case token_INT: return (char *)js_wi(this->value->i);
    case token_FLOAT: return (char *)js_wd(this->value->f);
    case token_STRING: return (char *)js_ws(this->value->s);
    case token_SYMBOL: return symbol_to_str(this->value->sym);
    case token_BLOB: return str_f("blob(%d)", bytes_len(this->value->bs));
    case token_LIST: {
      // Arr<char>
      Arr *a = arr_map(this->value->a, (FCOPY)token_to_str);
      if (arr_size(a) > 5) {
        a = arr_take(a, 5);
        arr_push(a, "...");
      }
      return str_f("(%s)", str_join(a, ", "));
    }
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

char *token_type_to_str (enum token_Type type) {
  switch (type) {
    case token_INT: return "Int";
    case token_FLOAT: return "Float";
    case token_STRING: return "String";
    case token_SYMBOL: return "Symbol";
    case token_BLOB: return "Blob";
    case token_LIST: return "List";
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}


