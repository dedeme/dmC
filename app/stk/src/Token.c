// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Token.h"

/* .

Token
  nline: int
  nchar: int
  value: char *

===

TkError
  tk: Token *: token
  msg: char *

*/
/*--*/

struct token_Token {
  int nline;
  int nchar;
  char *value;
};

Token *token_new(int nline, int nchar, char *value) {
  Token *this = malloc(sizeof(Token));
  this->nline = nline;
  this->nchar = nchar;
  this->value = value;
  return this;
}

void token_free(Token *this) {
  if (this) {
    free(this->value);
    free(this);
  }
};

int token_nline(Token *this) {
  return this->nline;
}

int token_nchar(Token *this) {
  return this->nchar;
}

char *token_value(Token *this) {
  return this->value;
}

struct tkError_TkError {
  Token *tk;
  char *msg;
};

TkError *tkError_new(Token *tk, char *msg) {
  TkError *this = malloc(sizeof(TkError));
  this->tk = tk;
  this->msg = msg;
  return this;
}

void tkError_free(TkError *this) {
  if (this) {
    token_free(this->tk);
    free(this->msg);
    free(this);
  }
};

Token *tkError_tk(TkError *this) {
  return this->tk;
}

char *tkError_msg(TkError *this) {
  return this->msg;
}

/*--*/

static int is_id (char *s) {
  char ch = *s;
  if (ch) {
    char c;
    for (;;) {
      c = *s++;
      if (c) {
        if (c <= ' ') {
          return 0;
        }
      }
      break;
    }
    return str_cindex("\"'-!@[({", ch) == -1;
  }
  return 0;
}

int token_is_id (Token *tk) {
  return is_id(tk->value);
}

int token_is_quoted_id (Token *tk) {
  char *p = tk->value;
  if (*p++ == '\'') {
    return is_id(p);
  }
  return 0;
}

int token_is_bool (Token *tk) {
  char *v = token_value(tk);
  return *v == '-' ? str_eq(v, "-f-") || str_eq(v, "-t-") : 0;
}

