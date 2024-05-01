// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/token.h"
#include "kut/DEFS.h"
#include "kut/dec.h"
#include "kut/js.h"
#include "kut/buf.h"

static char *reserved[] = {
  "arguments", "case", "class", "const", "debugger", "delete",
  "enum", "export", "extends", "function", "hasOwnProperty", "implements",
  "in", "Infinity", "instanceof", "interface",
  "isFinite", "isNaN", "isPrototypeOf",
  "let", "NaN", "package", "private", "protected",
  "public", "static", "super", "this", "undefined", "var", "void",
  "with", "yield",
  NULL
};

static Token *newb(Token_t type, int value, char *js) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->b = value;
  this->js = js;
  return this;
}

static Token *newd(Token_t type, double value, char *js) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->d = value;
  this->js = js;
  return this;
}

static Token *new(Token_t type, void *value, char *js) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->value = value;
  this->js = js;
  return this;
}

Token *token_bool (int value, char *js) {
  return newb(TOKEN_BOOL, value, js);
}

Token *token_float (double value, char *js) {
  return newd(TOKEN_FLOAT, value, js);
}

Token *token_string (char *value, char *js) {
  return new(TOKEN_STRING, value, js);
}

char *token_string_injection (Token *this) {
  if (this->type != TOKEN_STRING)
    EXC_ILLEGAL_ARGUMENT("Bad token type", "string", token_type_to_str(this));
  Buf *bf = buf_new();
  char *p = this->js;
  while (*p && *p <= ' ') buf_cadd(bf, *p++);
  int js_lines = 0;
  while (*p) {
    char ch = *p++;
    if (ch == '\n') ++js_lines;
  }
  int v_lines = 0;
  p = this->value;
  while (*p) {
    char ch = *p++;
    if (ch == '\n') ++v_lines;
  }

  buf_add(bf, this->value);
  for (int n = js_lines - v_lines; n > 0; --n) buf_cadd(bf, '\n');
  return buf_str(bf);
}

Token *token_line_comment (char *value, char *js) {
  return new(TOKEN_LINE_COMMENT, value, js);
}

Token *token_comment (char *value, char *js) {
  return new(TOKEN_COMMENT, value, js);
}

Token *token_symbol (char *value, char *js) {
  return new(TOKEN_SYMBOL, value, js);
}

Token *token_operator (char *value, char *js) {
  return new(TOKEN_OPERATOR, value, js);
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
  return this->type == TOKEN_SYMBOL && !strcmp(this->value, "else");
}

int token_is_catch (Token *this) {
  return this->type == TOKEN_SYMBOL && !strcmp(this->value, "catch");
}

int token_is_finally (Token *this) {
  return this->type == TOKEN_SYMBOL && !strcmp(this->value, "finally");
}
int token_is_reserved (char *word) {
  char **p = reserved;
  while (*p) if (!strcmp(*p++, word)) return TRUE;
  return FALSE;
}

char *token_type_to_str (Token *this) {
  switch (this->type) {
    case TOKEN_BOOL: return "Bool";
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
    case TOKEN_FLOAT: return str_f("Float: %s", dec_ftos(this->d, 9));
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

