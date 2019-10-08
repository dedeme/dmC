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
  union token_Value value;
  int line;
};

Token *token_new_int (int line, Int value) {
  union token_Value v;
  v.i = value;
  Token *this = MALLOC(Token);
  this->type = token_INT;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_float (int line, double value) {
  union token_Value v;
  v.f = value;
  Token *this = MALLOC(Token);
  this->type = token_FLOAT;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_string (int line, char *value) {
  union token_Value v;
  v.s = value;
  Token *this = MALLOC(Token);
  this->type = token_STRING;
  this->value = v;
  this->line = line;
  return this;
}

// value is Arr<Token>
Token *token_new_list (int line, Arr *value) {
  union token_Value v;
  v.a = value;
  Token *this = MALLOC(Token);
  this->type = token_LIST;
  this->value = v;
  this->line = line;
  return this;
}

Token *token_new_symbol (int line, Symbol value) {
  union token_Value v;
  v.sym = value;
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

Token *token_clone (Token *this) {
  switch (this->type) {
    case token_INT: return token_new_int(0, this->value.i);
    case token_FLOAT: return token_new_float(0, this->value.f);
    case token_STRING: return token_new_string(0, str_new(this->value.s));
    case token_SYMBOL: return token_new_symbol(0, this->value.sym);
    case token_LIST: {
      // Arr<Token>
      Arr *r = arr_new();
      EACH(this->value.a, Token, tk){
        arr_push(r, token_clone(tk));
      }_EACH
      return token_new_list(0, r);
    }
    case token_POINTER: {
      Token *r = token_new_int(0, this->value.i);
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
    case token_INT: return this->value.i == other->value.i;
    case token_POINTER: return this->value.pointer == other->value.pointer;
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
    case token_POINTER: return (char *)js_wl(this->value.i); // Correcto
    case token_FLOAT: return (char *)js_wd(this->value.f);
    case token_STRING: return (char *)js_ws(this->value.s);
    case token_SYMBOL:
      return str_f("'%s'", symbol_to_str(this->value.sym));
    case token_LIST: {
      // Arr<char>
      Arr *a = arr_map(this->value.a, (FCOPY)token_to_str);
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
  p->value.pointer = pointer;
  p->type = token_POINTER;
  return token_new_list(0, arr_new_from(token_new_symbol(0, sym), p, NULL));
}

char *token_check_type (List *tokens, char *type) {
  Token *tk = NULL;
  // List<Token>
  List *tail = NULL;
  if (!list_empty(tokens)) {
    tk = list_head(tokens);
    tail = list_tail(tokens);
  } else {
    return "";
  }

  char *paste (char *code, int len) {
    return str_cat(code, token_check_type(tail, str_right(type, len)), NULL);
  }

  char *tpaste (void) {
    switch (tk->type) {
      case token_INT: return paste("i", 1);
      case token_FLOAT: return paste("f", 1);
      case token_STRING: return paste("s", 1);
      case token_SYMBOL: return paste("y", 1);
      case token_LIST: return paste("l", 1);
      case token_POINTER: return paste("p", 1);
    }
    EXC_ILLEGAL_STATE("switch not exhaustive");
    return NULL; // not reachable.
  }

  char *object (void) {
    // Arr<Token
    Arr *a = tk->type == token_LIST ? tk->value.a : NULL;
    int badformat (void) {
      for (int i = 0; i < arr_size(a); i += 2)
        if (token_type(arr_get(a, i)) != token_STRING) return 1;
      return 0;
    }
    if (!a || arr_size(a) % 2 || badformat()) return tpaste();
    return paste("o", 1);
  }

  char *map (void) {
    // Arr<Token
    Arr *a = tk->type == token_LIST ? tk->value.a : NULL;
    int badformat (void) {
      EACH(a, Token, t) {
        // Arr<Token
        Arr *a2 = t->type == token_LIST ? t->value.a : NULL;
        if (
          !a2 || arr_size(a2) != 2 ||
          token_type(*arr_start(a2)) != token_STRING
        ) return 1;
      }_EACH
      return 0;
    }
    if (!a || badformat()) return tpaste();
    return paste("m", 1);
  }

  char *pointer (void) {
    int ix = str_cindex_from(type, '>', 1);
    if (ix == -1) return paste("<?", 1);
    if (ix == 1) return paste("<>?", 2);
    int len = ix + 1;
    // Arr<Token
    Arr *a = tk->type == token_LIST ? tk->value.a : NULL;
    if (
      !a ||
      arr_size(a) != 2 ||
      ((Token *)*arr_start(a))->type != token_SYMBOL ||
      ((Token *)*(arr_start(a) + 1))->type != token_POINTER
    ) {
      char *r = tpaste();
      return str_f("%c%s", *r, str_right(type, len));
    }
    char *symid = str_f(
      "<%s>", str_right(symbol_to_str(token_symbol(*arr_start(a))), 2)
    );
    return str_cat(symid, token_check_type(tail, str_right(type, len)), NULL);
  }

  char *list (void) {
    int close (void) {
      char *p = type + 2;
      int c = 1;
      while (*p) {
        char ch = *p++;
        if (ch == '>') {
          --c;
          if (!c) return p - type - 1;
        } else if (ch == '<') {
          ++c;
        }
      }
      return -1;
    }

    if (type[1] != '<') return paste("L?", 1);
    int ix = close();
    if (ix == -1) return paste("L<?", 2);
    int len = ix + 1;
    // Arr<Token
    Arr *a = tk->type == token_LIST ? tk->value.a : NULL;

    int badformat (void) {
      char *intype = str_sub(type, 2, ix);
      if (*intype) {
        // List<Token>
        List *lst = list_from_arr(a);
        if (!str_eq(intype, token_check_type(lst, intype))) return 1;
        lst = list_reverse(list_tail(list_reverse(lst)));
        return str_eq(intype, token_check_type(lst, intype));
      }
      return arr_size(a);
    }
    if (!a || badformat()) {
      char *r = tpaste();
      return str_f("%c%s", *r, str_right(type, len));
    }
    return str_cat(
      str_left(type, len), token_check_type(tail, str_right(type, len)), NULL
    );
  }

  // ---------------------------------------------

  char ch = *type;
  switch (ch) {
    case 0 : return "";
    case 'i':
    case 'f':
    case 's':
    case 'y':
    case 'l': return tpaste();
    case 'o': return object();
    case 'm': return map();
    case '<': return pointer();
    case 'L': return list();
    default: return paste(str_f("%c?", ch), 1);
  }
}
