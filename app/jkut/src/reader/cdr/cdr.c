// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/cdr/cdr.h"
#include "reader/cdr/number_reader.h"
#include "reader/cdr/comment_reader.h"
#include "reader/cdr/string_reader.h"
#include "kut/buf.h"
#include "fileix.h"
#include "DEFS.h"

static char *opChs = ";=,.()+-*/!<>[]{}%&|?:\\";

struct cdr_Cdr {
  char *code;
  char *pcode;
  int fix;
  int nline;
  Token *next_tk; // Can be NULL
  int next_nline;
};

static Token *read_symbol (Cdr *this, char ch, char *blanks) {
  Buf *bf = buf_new();
  buf_cadd(bf, ch);
  for (;;) {
    ch = cdr_read_char(this);
    if (
      (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
      (ch >= '0' && ch <= '9')
    ) {
      buf_cadd(bf, ch);
    } else {
      if (ch) cdr_unread_char(this, ch);
      break;
    }
  }

  char *sym = str_new(buf_str(bf));
  if (!strcmp(sym, "false")) return token_bool(FALSE, str_f("%sfalse", blanks));
  if (!strcmp(sym, "true")) return token_bool(TRUE, str_f("%strue", blanks));
  return token_symbol(sym, str_f("%s%s", blanks, sym));
}

static Token *read_operator (Cdr *this, char ch, char *blanks) {
  char *mk_c2 (char ch1, char ch2) {
    char *r = ATOMIC(3);
    r[0] = ch1;
    r[1] = ch2;
    r[2] = 0;
    return r;
  }

  switch (ch) {
    case '=':
    case '!':
    case '>':
    case '<':
    case '+':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&': {
      char ch2 = cdr_read_char(this);
      if (ch2 == '=') {
        char *op = mk_c2(ch, ch2);
        return token_operator(op, str_f("%s%s", blanks, op));
      }

      cdr_unread_char(this, ch2);
    }
    case '-': {
      char ch2 = cdr_read_char(this);
      if (ch2 == '=' || ch2 == '>') {
        char *op = mk_c2(ch, ch2);
        return token_operator(op, str_f("%s%s", blanks, op));
      }

      cdr_unread_char(this, ch2);
    }
  }
  char *op = str_new_c(ch);
  return token_operator(op, str_f("%s%s", blanks, op));
}

/// Returns NULL at end of text.
static Token *read_token (Cdr *this) {
  Buf *blanks = buf_new();
  for (;;) {
    char ch = cdr_read_char(this);
    switch (ch) {
      case 0:
        return NULL;
      case '/': {
        Token *tk = comment_reader_read(this, buf_str(blanks));
        if (tk->type == TOKEN_OPERATOR)
          return read_operator(this, '/', buf_str(blanks)); // Operator '/'
        if (str_starts(tk->value, "///"))
          return tk;
        // Skip comment
        Token *tk2 = read_token(this);
        if (tk2) tk2->js = str_f("%s%s", tk->js, tk2->js);
        return tk2;
      }
      case '\'':
      case '"':
        return string_reader_read(this, ch, buf_str(blanks));
      default:
        if (ch <= ' ') {
          buf_cadd(blanks, ch);
          continue;
        }
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
          return read_symbol(this, ch, buf_str(blanks));
        if ((ch >= '0' && ch <= '9'))
          return number_reader_read(this, ch, buf_str(blanks));
        if (str_cindex(opChs, ch) == -1)
          EXC_GENERIC(cdr_fail_line(
            this, str_f("Unexpected character %d('%c')", ch, ch), this->next_nline
          ));

        return read_operator(this, ch, buf_str(blanks));
    }
  }
}


Cdr *cdr_new (int fix, char *code) {
  Cdr *this = MALLOC(Cdr);
  this->code = code;
  this->pcode = code;
  this->fix = fix;
  this->nline = 1;
  this->next_nline = 1;
  this->next_tk = read_token(this);
  return this;
}

int cdr_get_file (Cdr *this) {
  return this->fix;
}

int cdr_get_nline (Cdr *this) {
  return this->nline;
}

int cdr_get_next_nline (Cdr *this) {
  return this->next_nline;
}

char *cdr_fail_line (Cdr *this, char *msg, int nline) {
  return str_f("%s\n  %s:%d:", msg, fileix_to_fail(this->fix), nline);
}

char *cdr_fail (Cdr *this, char *msg) {
  return cdr_fail_line(this, msg, this->nline);
}

char *cdr_fail_expect_line (Cdr *this, char *expect, char *found, int nline) {
  char *msg = str_f("Expected: %s\nFound   : %s", expect, found);
  return cdr_fail_line(this, msg, nline);
}

char *cdr_fail_expect (Cdr *this, char *expect, char *found) {
  char *msg = str_f("Expected: %s\nFound   : %s", expect, found);
  return cdr_fail(this, msg);
}

char cdr_read_char(Cdr *this) {
  char ch = *this->pcode;
  if (ch) {
    ++this->pcode;
    if (ch == '\n') ++this->next_nline;
  }
  return ch;
}

void cdr_unread_char(Cdr *this, char ch) {
  if (this->pcode <= this->code)
    EXC_ILLEGAL_STATE("Backing from code start.");
  if (ch == '\n') --this->next_nline;
  --this->pcode;
}

// <Token>
Opt *cdr_read_token_op (Cdr *this) {
  Token *r = this->next_tk;
  this->nline = this->next_nline;
  this->next_tk = read_token(this);
  return r ? opt_some(r) : opt_none();
}

Token *cdr_read_token (Cdr *this) {
  Token *r = this->next_tk;
  if (!r)
    EXC_GENERIC(cdr_fail_line(this, "Unexpected end of file", this->next_nline));
  this->nline = this->next_nline;
  this->next_tk = read_token(this);
  return r;
}

int cdr_next_token_is_point (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_point(tk);
}

int cdr_next_token_is_comma (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_comma(tk);
}

int cdr_next_token_is_colon (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_colon(tk);
}

int cdr_next_token_is_semicolon (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_semicolon(tk);
}

int cdr_next_token_is_exclamation (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_exclamation(tk);
}

int cdr_next_token_is_backslash (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_backslash(tk);
}

int cdr_next_token_is_arrow (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_arrow(tk);
}

int cdr_next_token_is_binary (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_binary(tk);
}

int cdr_next_token_is_ternary (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_ternary(tk);
}

int cdr_next_token_is_open_square (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_open_square(tk);
}

int cdr_next_token_is_close_square (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_close_square(tk);
}

int cdr_next_token_is_open_parenthesis (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_open_par(tk);
}

int cdr_next_token_is_close_parenthesis (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_close_par(tk);
}

int cdr_next_token_is_close_bracket (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_close_bracket(tk);
}

int cdr_next_token_is_else (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_else(tk);
}

int cdr_next_token_is_finally (Cdr *this) {
  Token *tk = this->next_tk;
  return tk && token_is_finally(tk);
}
