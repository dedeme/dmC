// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Lexeme.h"

union lexeme_Value {
  char bool;
  char *string;
};

static union lexeme_Value *new_value (void) {
  return malloc(sizeof(union lexeme_Value));
}

struct lexeme_Lexeme {
  int nline;
  int nchar;
  Type *type;
  union lexeme_Value *value;
};

static Lexeme *new (
  int nline,
  int nchar,
  Type *type,
  union lexeme_Value *value
) {
  Lexeme *this = malloc(sizeof(Lexeme));
  this->nline = nline;
  this->nchar = nchar;
  this->type = type;
  this->value = value;
  return this;
}

Type *lexeme_type (Lexeme *this) {
  return this->type;
}

void lexeme_free (Lexeme *this) {
  if (this) {
    switch (type_kind(this->type)) {
      case BOOL: break;
    }
    free(this->value);
    type_free(this->type);
    free(this);
  }
}

Lexeme *lexeme_next (TkReader *reader) {
  Token *tk = tkReader_next_new(reader);
  int nl = token_nline(tk);
  int nc = token_nchar(tk);
  char *val = token_value(tk);

  union lexeme_Value *lex = new_value();

  if (token_is_bool(tk)) {
    lex->bool = str_eq(val, "-t-");
    return new(nl, nc, type_bool_new(), lex);
  } else {
    free(lex);
    return NULL;
  }
}
