// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Token.h"

struct token_Pos {
  Symbol source;
  int line;
};

struct token_Native {
  Symbol sym;
  void *pointer;
};

union token_Value {
  Int i;
  double f;
  char *s;
  Arr *a; // Arr<Token>
  Symbol sym;
  struct token_Native *native;
};

struct token_Token {
  enum token_Type type;
  union token_Value value;
  // Opt<TokenPos>
  Opt *pos;
};

static Token *new (enum token_Type type, union token_Value value, Opt *pos) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->value = value;
  this->pos = pos;
  return this;
}

static  Token *new_pos(
  enum token_Type type, union token_Value value, Symbol source, int line
) {
  TokenPos *pos = MALLOC(TokenPos);
  pos->source = source;
  pos->line = line;
  return new(type, value, opt_new(pos));
}

char *tokenPos_source (TokenPos *this) {
  return symbol_to_str(this->source);
}

int tokenPos_line (TokenPos *this) {
  return this->line;
}

Token *token_new_int (Int value) {
  union token_Value v;
  v.i = value;
  return new(token_INT, v, opt_empty());
}

Token *token_new_float (double value) {
  union token_Value v;
  v.f = value;
  return new(token_FLOAT, v, opt_empty());
}

Token *token_new_string (char *value) {
  union token_Value v;
  v.s = value;
  return new(token_STRING, v, opt_empty());
}

// value is Arr<Token>
Token *token_new_list (Arr *value) {
  union token_Value v;
  v.a = value;
  return new(token_LIST, v, opt_empty());
}

Token *token_new_symbol (Symbol value) {
  union token_Value v;
  v.sym = value;
  return new(token_SYMBOL, v, opt_empty());
}

Token *token_from_pointer (Symbol sym, void *pointer) {
  struct token_Native *nt = MALLOC(struct token_Native);
  nt -> sym = sym;
  nt -> pointer = pointer;

  union token_Value v;
  v.native = nt;
  return new(token_NATIVE, v, opt_empty());
}

Token *token_new_int_pos (Int value, Symbol source, int line) {
  union token_Value v;
  v.i = value;
  return new_pos(token_INT, v, source, line);
}

Token *token_new_float_pos (double value, Symbol source, int line) {
  union token_Value v;
  v.f = value;
  return new_pos(token_FLOAT, v, source, line);
}

Token *token_new_string_pos (char *value, Symbol source, int line) {
  union token_Value v;
  v.s = value;
  return new_pos(token_STRING, v, source, line);
}

// value is Arr<Token>
Token *token_new_list_pos (Arr *value, Symbol source, int line) {
  union token_Value v;
  v.a = value;
  return new_pos(token_LIST, v, source, line);
}

Token *token_new_symbol_pos (Symbol value, Symbol source, int line) {
  union token_Value v;
  v.sym = value;
  return new_pos(token_SYMBOL, v, source, line);
}

enum token_Type token_type (Token *this) {
  return this->type;
}

// Opt<TokenPos>
Opt *token_pos (Token *this) {
  return this->pos;
}

Int token_int (Token *this) {
  return this->value.i;
}

double token_float (Token *this) {
  return this->value.f;
}

char *token_string (Token *this) {
  return this->value.s;
}

// Returns Arr<Token>
Arr *token_list (Token *this) {
  return this->value.a;
}

Symbol token_symbol (Token *this) {
  return this->value.sym;
}

Symbol token_native_symbol (Token *this) {
  return this->value.native->sym;
}

void *token_native_pointer (Token *this) {
  return this->value.native->pointer;
}

Token *token_clone (Token *this) {
  switch (this->type) {
    case token_INT: return token_new_int(this->value.i);
    case token_FLOAT: return token_new_float(this->value.f);
    case token_STRING: return token_new_string(str_new(this->value.s));
    case token_SYMBOL: return token_new_symbol(this->value.sym);
    case token_LIST: {
      // Arr<Token>
      Arr *r = arr_new();
      EACH(this->value.a, Token, tk){
        arr_push(r, token_clone(tk));
      }_EACH
      return token_new_list(r);
    }
    case token_NATIVE: {
      if (token_native_symbol(this) == symbol_MAP_) {
        // Map<Token>
        Map *r = map_new();
        EACH(token_native_pointer(this), Kv, kv){
          map_put(r, str_new(kv_key(kv)), token_clone(kv_value(kv)));
        }_EACH
        return token_from_pointer(symbol_MAP_, r);
      } else {
        struct token_Native *nt = this->value.native;
        return token_from_pointer(nt->sym, nt->pointer);
      }
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
    case token_INT: return this->value.i == other->value.i;
    case token_NATIVE:
      return this->value.native->sym == other->value.native->sym &&
             this->value.native->pointer == other->value.native->pointer;
    case token_FLOAT: return this->value.f == other->value.f;
    case token_STRING: return str_eq(this->value.s, other->value.s);
    case token_SYMBOL: return symbol_eq(this->value.sym, other->value.sym);
    case token_LIST: return arreq(this->value.a, other->value.a);
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return 0; // not reachable.
}

char *token_to_str (Token *this) {
  switch (this->type) {
    case token_INT: return (char *)js_wl(this->value.i);
    case token_FLOAT: return (char *)js_wd(this->value.f);
    case token_STRING: return this->value.s;
    case token_SYMBOL:
      return str_f("'%s'", symbol_to_str(this->value.sym));
    case token_LIST: {
      // Arr<char>
      Arr *a = arr_map(this->value.a, (FCOPY)token_to_str);
      return str_f("(%s)", str_join(a, ", "));
    }
    case token_NATIVE: {
      if (token_native_symbol(this) == symbol_MAP_) {
        // Arr<char>
        Arr *a = arr_new();
        EACH(token_native_pointer(this), Kv, kv) {
          arr_push(a, str_f(
            "%s: %s", kv_key(kv), token_to_str(kv_value(kv))
          ));
        }_EACH
        return str_f("{%s}", str_join(a, ", "));
      } else {
        return str_f(
          "<%s, %ld>",
          symbol_to_str(this->value.native->sym),
          this->value.native->pointer
        );
      }
    }
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

char *token_to_str_draft (Token *this) {
  switch (this->type) {
    case token_STRING: return (char*)js_ws(this->value.s);
    case token_LIST: {
      // Arr<char>
      Arr *a = arr_map(arr_take(this->value.a, 5), (FCOPY)token_to_str_draft);
      if (arr_size(this->value.a) > 5) {
        arr_push(a, "...");
      }
      return str_f("(%s)", str_join(a, ", "));
    }
    case token_NATIVE: {
      if (token_native_symbol(this) == symbol_MAP_) {
        // Arr<char>
        Arr *a = arr_new();
        EACH_IX(token_native_pointer(this), Kv, kv, ix) {
          if (ix > 5) {
            arr_push(a, "...");
            break;
          }
          arr_push(a, str_f(
            "%s: %s", (char *)js_ws(kv_key(kv)),
            token_to_str_draft(kv_value(kv))
          ));
        }_EACH
        return str_f("{%s}", str_join(a, ", "));
      }
    }
    default: return token_to_str(this);
  }
}

char *token_type_to_str (enum token_Type type) {
  switch (type) {
    case token_INT: return "Int";
    case token_FLOAT: return "Float";
    case token_STRING: return "String";
    case token_SYMBOL: return "Symbol";
    case token_LIST: return "List";
    case token_NATIVE: return "CPointer";
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}
