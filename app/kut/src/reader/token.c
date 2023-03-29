// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/token.h"
#include "DEFS.h"
#include "kut/js.h"

enum token_Token_t {
  TOKEN_BOOL, TOKEN_INT, TOKEN_FLOAT, TOKEN_STRING,
  TOKEN_LINE_COMMENT, TOKEN_COMMENT, TOKEN_SYMBOL, TOKEN_OPERATOR
};

typedef enum token_Token_t Token_t;


struct token_Token {
  Token_t type;
  void *value;
};

static Token *new(Token_t type, void *value) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->value = value;
  return this;
}

Token *token_bool (int value) {
  int *val = ATOMIC(sizeof(int));
  *val = value;
  return new(TOKEN_BOOL, val);
}

int token_get_bool (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_bool, "Bool", this);
  return *((int *)this->value);
}

/// Returns TRUE if 'this' match the type.
int token_is_bool (Token *this) {
  return this->type == TOKEN_BOOL;
}

Token *token_int (int64_t value) {
  int64_t *val = ATOMIC(sizeof(int64_t));
  *val = value;
  return new(TOKEN_INT, val);
}

int64_t token_get_int (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_int, "Int", this);
  return *((int64_t *)this->value);
}

int token_is_int(Token *this) {
  return this->type == TOKEN_INT;
}

Token *token_float (double value) {
  double *val = ATOMIC(sizeof(double));
  *val = value;
  return new(TOKEN_FLOAT, val);
}

double token_get_float (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_float, "Float", this);
  return *((double *)this->value);
}

/// Returns TRUE if 'this' match the type.
int token_is_float (Token *this) {
  return this->type == TOKEN_FLOAT;
}

Token *token_string (char *value) {
  return new(TOKEN_STRING, value);
}

char *token_get_string (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_string, "String", this);
  return this->value;
}

int token_is_string (Token *this) {
  return this->type == TOKEN_STRING;
}

Token *token_line_comment (char *value) {
  return new(TOKEN_LINE_COMMENT, value);
}

char *token_get_line_comment (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_line_comment, "Line_comment", this);
  return this->value;
}

int token_is_line_comment (Token *this) {
  return this->type == TOKEN_LINE_COMMENT;
}

Token *token_comment (char *value) {
  return new(TOKEN_COMMENT, value);
}

char *token_get_comment (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_comment, "Comment", this);
  return this->value;
}

int token_is_comment (Token *this) {
  return this->type == TOKEN_COMMENT;
}

Token *token_symbol (char *value) {
  return new(TOKEN_SYMBOL, value);
}

char *token_get_symbol (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_symbol, "Symbol", this);
  return this->value;
}

int token_is_symbol (Token *this) {
  return this->type == TOKEN_SYMBOL;
}

Token *token_operator (char *value) {
  return new(TOKEN_OPERATOR, value);
}

char *token_get_operator (Token *this) {
  TEST_TOKEN_TYPE_ERROR(token_is_operator, "Operator", this);
  return this->value;
}

int token_is_operator (Token *this) {
  return this->type == TOKEN_OPERATOR;
}

int token_is_unary (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    return !strcmp(v, "!") || !strcmp(v, "-");
  }
  return FALSE;
}

int token_is_binary (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    return !strcmp(v, "*") || !strcmp(v, "/") || !strcmp(v, "%") ||
      !strcmp(v, "+") || !strcmp(v, "-") || !strcmp(v, "==") ||
      !strcmp(v, "!=") || !strcmp(v, ">") || !strcmp(v, ">=") ||
      !strcmp(v, "<") || !strcmp(v, "<=") || !strcmp(v, "&") ||
      !strcmp(v, "|");
  }
  return FALSE;
}

int token_is_binary1 (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    return !strcmp(v, "*") || !strcmp(v, "/") || !strcmp(v, "%");
  }
  return FALSE;
}

int token_is_binary2 (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    return !strcmp(v, "+") || !strcmp(v, "-");
  }
  return FALSE;
}

int token_is_binary3 (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    return !strcmp(v, "==") || !strcmp(v, "!=") || !strcmp(v, ">") ||
      !strcmp(v, ">=") || !strcmp(v, "<") || !strcmp(v, "<=");
  }
  return FALSE;
}

int token_is_binary4 (Token *this) {
  if (this->type == TOKEN_OPERATOR) {
    char *v = this->value;
    return !strcmp(v, "&") || !strcmp(v, "|");
  }
  return FALSE;
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
  return this->type == TOKEN_SYMBOL && !strcmp(this->value, "else");
}

int token_is_catch (Token *this) {
  return this->type == TOKEN_SYMBOL && !strcmp(this->value, "catch");
}

int token_is_finally (Token *this) {
  return this->type == TOKEN_SYMBOL && !strcmp(this->value, "finally");
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
    case TOKEN_BOOL: return str_f("Bool: %s", js_wb(*((int *)this->value)));
    case TOKEN_INT: return str_f("Int: %s", js_wl(*((int64_t *)this->value)));
    case TOKEN_FLOAT: return str_f("Float: %s", js_wf(*((double *)this->value), 6));
    case TOKEN_STRING: return str_f("String: %s", js_ws(this->value));
    case TOKEN_LINE_COMMENT: return str_f("Line_comment: %s", this->value);
    case TOKEN_COMMENT: return str_f("Comment: %s", this->value);
    case TOKEN_SYMBOL: return str_f("Symbol: %s", this->value);
    case TOKEN_OPERATOR: return str_f("Operator: %s", this->value);
  }
  EXC_ILLEGAL_ARGUMENT("Bad token type identifier",
    str_f("(0 to %d)", TOKEN_OPERATOR), str_f("%d", this->type)
  );
  return NULL;
}
