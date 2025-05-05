// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "readers/tokenRd.h"
#include "DEFS.h"
#include "char.h"
#include "kut/buf.h"
#include "kut/js.h"

static char *op_chs = ";=,.()+-*/!<>[]{}%&|?:\\#_";
static Token *read(TokenRd *this);

TokenRd *tokenRd_new0 (CharRd *chr, Token *tk) {
  TokenRd *this = MALLOC(TokenRd);
  this->chr = chr;
  this->tk = tk;
  return this;
}

TokenRd *tokenRd_new (char *tx) {
  CharRd *chr = charRd_new(tx);
  // Use a fake 'tk' (it must be != token.end)
  TokenRd *r = tokenRd_new0(chr, token_new(0, token_int, "0"));
  Token *tk = read(r);
  return tokenRd_new0(chr, tk);
}

Token *tokenRd_peek (TokenRd *this) {
  return this->tk;
}

Token *tokenRd_next(TokenRd *this) {
  Token *r = this->tk;
  this->tk = read(this);
  return r;
}

//  Returns an error token.
static Token *fail (int ln, char *msg) {
  return token_new(ln, token_error, msg);
}

// Read an operator. 'ch' is operator.
static Token *read_operator (CharRd *chr, char ch) {
  int ln = chr->ln;

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
      if (charRd_peek(chr) == '=') {
        charRd_next(chr);
        return token_new(ln, token_operator, str_f("%c=", ch));
      }
      return token_new(ln, token_operator, str_new_c(ch));
    }
    case '-': {
      char ch2 = charRd_peek(chr);
      if (ch2 == '=' || ch2 == '>') {
        charRd_next(chr);
        return token_new(ln, token_operator, str_f("%c%c", ch, ch2));
      }
      return token_new(ln, token_operator, str_new_c(ch));
    }
    default: return token_new(ln, token_operator, str_new_c(ch));
  }
}

// Read comment or operator starting with '/'.
// NOTE: The first '/' already has been read.
static Token *read_comment_type (CharRd *chr) {
  int ln = chr->ln;

  Token *read_line_comment (void) {
    charRd_next(chr); // skip the second '/'.

    char ch = charRd_peek(chr);
    if (ch == ':') {
      charRd_next(chr);
      Buf *bf = buf_new();
      for (;;) {
        ch = charRd_next(chr);
        switch (ch) {
          case 0: return fail(ln, "Reached unexpected end of line");
          case '\n': {
            for (;;) {
              ch = charRd_peek(chr);
              if (!ch)
                return fail (ln, "Unexpected type declaration at end of file");
              if (ch <= ' ') {
                charRd_next(chr);
                continue;
              }
              break;
            }

            ch = charRd_peek(chr);
            if (ch == '/') {
              charRd_next(chr);
              ch = charRd_next(chr);
              if (ch != '/')
                return fail (chr->ln, str_f(
                  "Expected char '/', but found '%c'", ch
                ));
              continue; // more ktype text.
            }

            return token_new(ln, token_ktype, buf_str(bf));
          }
          default: buf_cadd(bf, ch);
        }
      }
    }

    Buf *bf = buf_new();
    for (;;) {
      ch = charRd_next(chr);
      switch (ch) {
        case 0:
        case '\n': return token_new(
            ln, token_lineComment, buf_str(bf)
          );
        default: buf_cadd(bf, ch);
      }
    }
  }

  Token *read_long_comment (void) {
    charRd_next(chr); // skip '*'.
    Buf *bf = buf_new();
    for (;;) {
      char ch = charRd_next(chr);
      switch (ch) {
        case 0: return fail(ln, "Unclosed commentary.");
        case '*':
          if (charRd_peek(chr) == '/') {
            charRd_next(chr);
            return token_new(ln, token_comment, buf_str(bf));
          } // continue in default
        default: buf_cadd(bf, ch);
      }
    }
  }

  char ch = charRd_peek(chr);

  switch (ch) {
    case '/': return read_line_comment();
    case '*': return read_long_comment();
    default: return read_operator(chr, '/');
  }

};

static Token *read_string (CharRd *chr, char ch) {
  int ln = chr->ln;

  Token *read_single (char ch) {
    // Rs<char>
    Rs *read_unicode (void) {
      int isHex (char ch) {
        return (ch >= '0' && ch <= '9') ||
               (ch >= 'a' && ch <= 'f') ||
               (ch >= 'A' && ch <= 'F')
        ;
      }

      Buf *bf = buf_new();
      for (int i = 0; i < 4; ++i) {
        char ch = charRd_next(chr);
        if (!isHex(ch)) return rs_fail(str_f(
          "Bad unicode sequence '%c'.", ch
        ));
        buf_cadd(bf, ch);
      }
      return rs_ok(js_rs(str_f("\"\\u%s\"", buf_str(bf))));
    };

    // Rs<char>
    Rs *read_escape (char close) {
      char ch = charRd_next(chr);
      switch (ch) {
        case 0: return rs_fail("Unclosed quotes.");
        case '\\': return rs_ok("\\");
        case 'n': return rs_ok("\n");
        case 't': return rs_ok("\t");
        case 'b': return rs_ok("\b");
        case 'f': return rs_ok("\f");
        case 'r': return rs_ok("\r");
        case 'u': return read_unicode();
        default: {
          if (ch == close) return rs_ok(str_new_c(ch));
          return rs_fail(str_f("Bad escape sequence '%c'.", ch));
        }
      };
    };

    Buf *bf = buf_new();
    char close = ch;
    for (;;) {
      ch = charRd_next(chr);
      switch (ch) {
        case 0:
        case '\n': return fail(ln, "Unclosed quotes.");
        default: {
          if (ch == close) return token_new(ln, token_string, buf_str(bf));
          if (ch == '\\') {
            Rs *rs = read_escape(close);
            char *tx = rs_get(rs);
            if (!tx) return token_new(ln, token_error, rs_error(rs));
            buf_add(bf, tx);
          } else {
            buf_cadd(bf, ch);
          }
        }
      }
    }
  };

  Token *read_multi (void) {
    char *format (char *s) {
      if (str_starts(s, "\n")) s = str_right(s, 1);
      // Arr<char>
      Arr *ss = str_csplit(s, '\n');
      int cut = -1;
      EACH(ss, char, l) {
        char *l2 = str_ltrim(l);
        int len2 = strlen(l2);
        if (len2 == 0) continue;
        int n = strlen(l) - len2;
        if (cut < 0 || n < cut) cut = n;
      }_EACH
      if (cut > 0) {
        EACH(ss, char, l) {
          arr_set(ss, _i, str_right(l, cut));
        }_EACH
      }
      return arr_cjoin(ss, '\n');
    }

    Buf *bf = buf_new();
    for (;;) {
      char ch = charRd_next(chr);
      switch (ch) {
        case 0: return fail(ln, "Unclosed quotes.");
        case '"': {
          char ch2 = charRd_next(chr);
          switch (ch2) {
            case 0: return fail(ln, "Unclosed quotes.");
            case '"': {
              char ch3 = charRd_next(chr);
              switch (ch3) {
                case 0: return fail(ln, "Unclosed quotes.");
                case '"':
                  return token_new(ln, token_string, format(buf_str(bf)));
                default: {
                  buf_cadd(bf, '"');
                  buf_cadd(bf, '"');
                  buf_cadd(bf, ch);
                }
              }
            }
            default: {
              buf_cadd(bf, '"');
              buf_cadd(bf, ch);
            }
          }
        }
        default:
          buf_cadd(bf, ch);
      }
    }
  }

  if (ch == '\'') return read_single('\'');

  char ch2 = charRd_peek(chr);
  if (ch2 != '"') return read_single('"');

  charRd_next(chr);
  char ch3 = charRd_peek(chr);
  if (ch3 != '"') return token_new(ln, token_string, "");

  charRd_next(chr);
  return read_multi();
}

static Token *read_symbol (CharRd *chr, char ch) {
  int ln = chr->ln;
  Buf *bf = buf_new();
  buf_cadd(bf, ch);
  for (;;) {
    ch = charRd_peek(chr);
    if (char_is_letter_or_digit(ch)) {
      charRd_next(chr);
      buf_cadd(bf, ch);
    } else {
      break;
    }
  }
  char *sym = buf_str(bf);
  return !strcmp(sym, "true") || !strcmp(sym, "false")
    ? token_new(ln, token_bool, sym)
    : token_new(ln, token_symbol, sym)
  ;
}

static Token *read_number (CharRd *chr, char ch) {
  int ln = chr->ln;
  Buf *bf = buf_new();

  Token *read_exponent (void) {
    char ch = charRd_peek(chr);
    switch (ch){
      case '+':
      case '-': {
        buf_cadd(bf, ch);
        charRd_next(chr);
      }
    }

    char ch2 = charRd_next(chr);
    if (!char_is_digit(ch2))
      return fail(ln, "Exponent has no digits.");
    buf_cadd(bf, ch2);

    for (;;) {
      ch = charRd_peek(chr);
      if (char_is_digit(ch)) {
        buf_cadd(bf, ch);
        charRd_next(chr);
        continue;
      }
      break;
    }
    return token_new(ln, token_float, buf_str(bf));
  };

  Token *read_float (void) {
    for (;;) {
      char ch = charRd_peek(chr);
      switch (ch) {
        case 'e':
        case 'E': {
          buf_cadd(bf, ch);
          charRd_next(chr);
          return read_exponent();
        }
        default: {
          if (char_is_digit(ch)) {
            buf_cadd(bf, ch);
            charRd_next(chr);
          } else {
            return token_new(ln, token_float, buf_str(bf));
          }
        }
      }
    }
  };

  buf_cadd(bf, ch);
  for (;;) {
    char ch = charRd_peek(chr);
    if (ch == '_') {
      charRd_next(chr);
      continue;
    } else if (ch == '.') {
      buf_cadd(bf, ch);
      charRd_next(chr);
      return read_float();
    } else if (char_is_digit(ch)) {
      buf_cadd(bf, ch);
      charRd_next(chr);
    } else {
      return token_new(ln, token_int, buf_str(bf));
    }
  }
}

// Read next token. 'tkr->chr' is modified if it is not at end of text.
static Token *read(TokenRd *this) {
  CharRd *chr = this->chr;

  Token *tk = this->tk;
  if (tk->tp == token_eof) return tk;
  for (;;) {
    char ch = charRd_next(chr);
    switch (ch) {
      case 0: return token_new(chr->ln, token_eof, "");
      case '/': {
        Token *tk = read_comment_type(chr);
        switch (tk->tp) {
          case token_ktype:
          case token_operator: return tk;
          default: return read(this);  // skip comment.
        }
      }
      case '\'':
      case '"': return read_string(chr, ch);
      default: {
        if (ch <= ' ') continue; // Skip blank
        if (char_is_letter(ch))
          return read_symbol(chr, ch);
        if (char_is_digit(ch))
          return read_number(chr, ch);
        if (str_cindex(op_chs, ch) != -1)
          return read_operator(chr, ch);

        return fail(chr->ln, str_f("Unexpected character '%c'.", ch));
      }
    }
  }
}
