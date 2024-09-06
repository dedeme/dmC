// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/token.h"
#include "kut/math.h"
#include "kut/js.h"
#include "symix.h"


static Token *newb(Token_t type, int value) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->b = value;
  return this;
}

static Token *newi(Token_t type, int64_t value) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->i = value;
  return this;
}

static Token *newd(Token_t type, double value) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->d = value;
  return this;
}

static Token *new(Token_t type, void *value) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->value = value;
  return this;
}

Token *token_bool (int value) {
  return newb(TOKEN_BOOL, value);
}


Token *token_int (int64_t value) {
  return newi(TOKEN_INT, value);
}

Token *token_float (double value) {
  return newd(TOKEN_FLOAT, value);
}

Token *token_string (char *value) {
  return new(TOKEN_STRING, value);
}

Token *token_line_comment (char *value) {
  return new(TOKEN_LINE_COMMENT, value);
}

Token *token_comment (char *value) {
  return new(TOKEN_COMMENT, value);
}

Token *token_symbol (char *value) {
  return newb(TOKEN_SYMBOL, symix_add(value));
}

Token *token_operator (char *value) {
  return new(TOKEN_OPERATOR, value);
}

int token_is_unary (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR &&
    (!strcmp(v, "!") || !strcmp(v, "-"))
  ;
}

int token_is_binary (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR &&
    ( !strcmp(v, "*") || !strcmp(v, "/") || !strcmp(v, "%") ||
      !strcmp(v, "+") || !strcmp(v, "-") || !strcmp(v, "==") ||
      !strcmp(v, "!=") || !strcmp(v, ">") || !strcmp(v, ">=") ||
      !strcmp(v, "<") || !strcmp(v, "<=") || !strcmp(v, "&") ||
      !strcmp(v, "|")
    );
}

int token_is_binary1 (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR &&
    (!strcmp(v, "*") || !strcmp(v, "/") || !strcmp(v, "%"))
  ;
}

int token_is_binary2 (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR &&
    (!strcmp(v, "+") || !strcmp(v, "-"))
  ;
}

int token_is_binary3 (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR &&
    ( !strcmp(v, "==") || !strcmp(v, "!=") || !strcmp(v, ">") ||
      !strcmp(v, ">=") || !strcmp(v, "<") || !strcmp(v, "<=")
    );
}

int token_is_binary4 (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR &&
    (!strcmp(v, "&") || !strcmp(v, "|"))
  ;
}

int token_is_ternary (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "?");
}

int token_is_assign (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    char ch1 = *v;
    char ch2 = *(v + 1);
    return ch1 == '=' ? !ch2
      : ch2 == '=' ? ch1 == '+' || ch1 == '-' || ch1 == '*' || ch1 == '/' ||
                     ch1 == '%' || ch1 == '&' || ch1 == '|'
      : FALSE
    ;
  }
  return FALSE;
}

int token_is_equals (Token *this) {
  char *v = this->value;
  return this->type == TOKEN_OPERATOR && *v == '=' && !(*(v + 1));
}

int token_is_point (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, ".");
}

int token_is_comma (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, ",");
}

int token_is_colon (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, ":");
}

int token_is_semicolon (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, ";");
}

int token_is_exclamation (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "!");
}

int token_is_backslash (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "\\");
}

int token_is_hash (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "#");
}

int token_is_arrow (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "->");
}

int token_is_open_par (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "(");
}

int token_is_close_par (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, ")");
}

int token_is_open_square (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "[");
}

int token_is_close_square (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "]");
}

int token_is_open_bracket (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "{");
}

int token_is_close_bracket (Token *this) {
  return this->type == TOKEN_OPERATOR && !strcmp(this->value, "}");
}

int token_is_else (Token *this) {
  return this->type == TOKEN_SYMBOL && this->b == symix_ELSE;
}

int token_is_catch (Token *this) {
  return this->type == TOKEN_SYMBOL && this->b == symix_CATCH;
}

int token_is_finally (Token *this) {
  return this->type == TOKEN_SYMBOL && this->b == symix_FINALLY;
}

char *token_type_to_str (Token *this) {
  switch (this->type) {
    case TOKEN_BOOL: return "Bool";
    case TOKEN_INT: return "Int";
    case TOKEN_FLOAT: return "Float";
    case TOKEN_STRING: return "String";
    case TOKEN_LINE_COMMENT: return "Line_comment";
    case TOKEN_COMMENT: return "Comment";
    case TOKEN_SYMBOL: return "Symbol";
    case TOKEN_OPERATOR: return "Operator";
  }
  EXC_ILLEGAL_ARGUMENT("Bad token type identifier",
    str_f("(0 to %d)", TOKEN_OPERATOR), str_f("%d", this->type)
  );
  return NULL;
}

char *token_to_str (Token *this) {
  switch (this->type) {
    case TOKEN_BOOL: return str_f("Bool: %s", this->b ? "true" : "false");
    case TOKEN_INT: return str_f("Int: %s", math_itos(this->i));
    case TOKEN_FLOAT: return str_f("Float: %s", math_ftos(this->d, 9));
    case TOKEN_STRING: return str_f("String: %s", js_ws(this->value));
    case TOKEN_LINE_COMMENT: return str_f("Line_comment: %s", this->value);
    case TOKEN_COMMENT: return str_f("Comment: %s", this->value);
    case TOKEN_SYMBOL: return str_f("Symbol: %s", symix_get(this->b));
    case TOKEN_OPERATOR: return str_f("Operator: %s", this->value);
  }
  EXC_ILLEGAL_ARGUMENT("Bad token type identifier",
    str_f("(0 to %d)", TOKEN_OPERATOR), str_f("%d", this->type)
  );
  return NULL;
}
