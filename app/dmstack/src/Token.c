// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Token.h"

union token_Value {
  int i;
  double f;
  char *s;
  Bytes *bs;
  Arr *a; // Arr<Token>
};

struct token_Token {
  enum token_Type type;
  union token_Value *value;
};

Token *token_new_int (int value) {
  union token_Value *v = MALLOC(union token_Value);
  v->i = value;
  Token *this = MALLOC(Token);
  this->type = token_INT;
  this->value = v;
  return this;
}

Token *token_new_float (double value) {
  union token_Value *v = MALLOC(union token_Value);
  v->f = value;
  Token *this = MALLOC(Token);
  this->type = token_FLOAT;
  this->value = v;
  return this;
}

Token *token_new_string (char *value) {
  union token_Value *v = MALLOC(union token_Value);
  v->s = value;
  Token *this = MALLOC(Token);
  this->type = token_STRING;
  this->value = v;
  return this;
}

Token *token_new_blob (int length) {
  union token_Value *v = MALLOC(union token_Value);
  unsigned char *bs = ATOMIC(length);
  v->bs = bytes_from_bytes(bs, length);
  Token *this = MALLOC(Token);
  this->type = token_BLOB;
  this->value = v;
  return this;
}

// value is Arr<Token>
Token *token_new_list (Arr *value) {
  union token_Value *v = MALLOC(union token_Value);
  v->a = value;
  Token *this = MALLOC(Token);
  this->type = token_LIST;
  this->value = v;
  return this;
}

Token *token_new_symbol (char *value) {
  union token_Value *v = MALLOC(union token_Value);
  v->s = value;
  Token *this = MALLOC(Token);
  this->type = token_SYMBOL;
  this->value = v;
  return this;
}

enum token_Type token_type (Token *this) {
  return this->type;
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

char *token_symbol (Token *this) {
  return this->value->s;
}

char *token_to_str (Token *this) {
  switch (this->type) {
    case token_INT: return (char *)js_wi(this->value->i);
    case token_FLOAT: return (char *)js_wd(this->value->f);
    case token_STRING: return (char *)js_ws(this->value->s);
    case token_SYMBOL: return this->value->s;
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
