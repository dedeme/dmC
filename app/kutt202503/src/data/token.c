// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "data/token.h"
#include "DEFS.h"
#include "kut/str.h"

Token *token_new (int ln, TokenT tp, char *value) {
  Token *this = MALLOC(Token);
  this->ln = ln;
  this->tp = tp;
  this->value = value;
  return this;
}

/// Returns 'true' if 'tk' is binary operator.
int token_is_binary (Token *tk) {
  if (tk->tp == token_operator) {
    char *v = tk->value;
    return
      !strcmp(v, "+") ||
      !strcmp(v, "-") ||
      !strcmp(v, "*") ||
      !strcmp(v, "/") ||
      !strcmp(v, "%") ||
      !strcmp(v, "==") ||
      !strcmp(v, "!=") ||
      !strcmp(v, ">") ||
      !strcmp(v, ">=") ||
      !strcmp(v, "<") ||
      !strcmp(v, "<=") ||
      !strcmp(v, "&") ||
      !strcmp(v, "|")
      ? TRUE : FALSE;
  }
  return FALSE;
}

int token_is_binary1 (Token *tk) {
  if (tk->tp == token_operator) {
    char *v = tk->value;
    return
      !strcmp(v, "*") ||
      !strcmp(v, "/") ||
      !strcmp(v, "%")
      ? TRUE : FALSE;
  }
  return FALSE;
}

int token_is_binary2 (Token *tk) {
  if (tk->tp == token_operator) {
    char *v = tk->value;
    return
      !strcmp(v, "+") ||
      !strcmp(v, "-")
      ? TRUE : FALSE;
  }
  return FALSE;
}

int token_is_binary3 (Token *tk) {
  if (tk->tp == token_operator) {
    char *v = tk->value;
    return
      !strcmp(v, "==") ||
      !strcmp(v, "!=") ||
      !strcmp(v, ">") ||
      !strcmp(v, ">=") ||
      !strcmp(v, "<") ||
      !strcmp(v, "<=")
      ? TRUE : FALSE;
  }
  return FALSE;
}

int token_is_binary4 (Token *tk) {
  if (tk->tp == token_operator) {
    char *v = tk->value;
    return
      !strcmp(v, "&") ||
      !strcmp(v, "|")
      ? TRUE : FALSE;
  }
  return FALSE;
}

int token_is_sym (Token *tk, char *v) {
  return tk->tp == token_symbol && !strcmp(tk->value, v);
}

int token_is_op (Token *tk, char *v) {
  return tk->tp == token_operator && !strcmp(tk->value, v);
}

char *token_type_to_str (Token *tk) {
  TokenT tp = tk->tp;
  char *r = tp == token_error ? "Error"
    : tp == token_eof ? "End_of_file"
    : tp == token_bool ? "Bool"
    : tp == token_int ? "Int"
    : tp == token_float ? "Float"
    : tp == token_string ? "String"
    : tp == token_lineComment ? "Line_comment"
    : tp == token_comment ? "Comment"
    : tp == token_symbol ? "Symbol"
    : tp == token_operator ? "Operator"
    : tp == token_ktype ? "Type"
    : ""
  ;
  if (!*r)
    EXC_KUTT(str_f("Bad token type identifier %d (%s)", tp, token_to_str(tk)));
  return r;
}

char *token_to_str (Token *tk) {
  TokenT tp = tk->tp;
  char *v = tk->value;
  char *r = tp == token_error ? str_f("Error: %s", v)
    : tp == token_eof ? "End_of_file"
    : tp == token_bool ? str_f("Bool: %s", v)
    : tp == token_int ? str_f("Int: %s", v)
    : tp == token_float ? str_f("Float: %s", v)
    : tp == token_string ? str_f("String: %s", v)
    : tp == token_lineComment ? str_f("Line_comment: %s", v)
    : tp == token_comment ? str_f("Comment: %s", v)
    : tp == token_symbol ? str_f("Symbol: %s", v)
    : tp == token_operator ? str_f("Operator: %s", v)
    : tp == token_ktype ? str_f("Type: %s", v)
    : ""
  ;
  if (!*r)
    EXC_KUTT(str_f("Bad token type identifier %d (%s)", tp, token_to_str(tk)));
  return r;
}

