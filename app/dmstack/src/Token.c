// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Token.h"


union token_Value {
  Int i;
  double f;
  char *s;
  Arr *a; // Arr<Token>
  Symbol sym;
  void *pointer;
};

struct token_Token {
  enum token_Type type;
  union token_Value *value;
  int line;
};

Token *token_new_int (int line, Int value) {
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

Token *token_new_symbol (int line, Symbol value) {
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

Int token_int (Token *this) {
  return this->value->i;
}

double token_float (Token *this) {
  return this->value->f;
}

char *token_string (Token *this) {
  return this->value->s;
}

// Returns Arr<Token>
Arr *token_list (Token *this) {
  return this->value->a;
}

Symbol token_symbol (Token *this) {
  return this->value->sym;
}

Token *token_clone (Token *this) {
  switch (this->type) {
    case token_INT: return token_new_int(0, this->value->i);
    case token_FLOAT: return token_new_float(0, this->value->f);
    case token_STRING: return token_new_string(0, str_new(this->value->s));
    case token_SYMBOL: return token_new_symbol(0, this->value->sym);
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
    case token_POINTER: {
      Token *r = token_new_int(0, this->value->i);
      r->type = token_POINTER;
      return r;
    }
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

int token_eq (Token *this, Token *other) {
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

  switch (this->type) {
    case token_INT: return this->value->i == other->value->i;
    case token_POINTER: return this->value->pointer == other->value->pointer;
    case token_FLOAT: return this->value->f == other->value->f;
    case token_STRING: return str_eq(this->value->s, other->value->s);
    case token_SYMBOL: return symbol_eq(this->value->sym, other->value->sym);
    case token_LIST: return arreq(this->value->a, other->value->a);
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return 0; // not reachable.
}

char *token_to_str (Token *this) {
  switch (this->type) {
    case token_INT: return (char *)js_wl(this->value->i);
    case token_POINTER: return (char *)js_wl(this->value->i); // Correcto
    case token_FLOAT: return (char *)js_wd(this->value->f);
    case token_STRING: return (char *)js_ws(this->value->s);
    case token_SYMBOL:
      return str_f("'%s'", symbol_to_str(this->value->sym));
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
    case token_LIST: return "List";
    case token_POINTER: return "CPointer";
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

Token *token_from_pointer (Symbol sym, void *pointer) {
  Token *p = token_new_int(0, 0);
  p->value->pointer = pointer;
  p->type = token_POINTER;
  return token_new_list(0, arr_new_from(token_new_symbol(0, sym), p, NULL));
}
