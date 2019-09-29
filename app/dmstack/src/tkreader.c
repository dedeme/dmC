// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tkreader.h"
#include <errno.h>
#include <locale.h>
#include <ctype.h>

static int is_blank (char ch) {
  return ((unsigned char)ch) <= ' ' || ch == ';' || ch == ':' || ch == ',';
}

static void to_unicode(Buf *bf, char *hexdigits) {
  char hexvalue (char ch) {
    return (ch <= '9') ? ch - '0' : toupper(ch) - 'A' + 10;
  }
  unsigned int codepoint =
    hexvalue(*hexdigits) * 4096 +
    hexvalue(*(hexdigits + 1)) * 256 +
    hexvalue(*(hexdigits + 2)) * 16 +
    hexvalue(*(hexdigits + 3));
  if (codepoint < 0x80) {
    buf_cadd(bf, (codepoint>>0  & 0x7F) | 0x00);
    return;
  }
  if (codepoint < 0x0800) {
    buf_cadd(bf, (codepoint>>6  & 0x1F) | 0xC0);
    buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
    return;
  }
  buf_cadd(bf, (codepoint>>12 & 0x0F) | 0xE0);
  buf_cadd(bf, (codepoint>>6  & 0x3F) | 0x80);
  buf_cadd(bf, (codepoint>>0  & 0x3F) | 0x80);
}

Opt *tkreader_next(Reader *reader) {
  if (reader_is_file(reader)) {
    if (reader_nline(reader) == 1 && reader_prg_ix(reader) == 0) {
      char *prg = reader_prg(reader);
      if (*prg && *prg == '#' && prg[1] == '!') {
        int ix = str_cindex(prg, '\n');
        if (ix == -1) ix = strlen(prg);
        reader_set_nline(reader, 2);
        reader_set_prg_ix(reader, ix + 1);
      }
    }
  }
  if (reader_next_tk(reader)) {
    Token *r = reader_next_tk(reader);
    reader_set_next_tk(reader, NULL);
    return opt_new(r);
  }
  int nline = reader_nline(reader);
  int prg_ix = reader_prg_ix(reader);
  char *prg = reader_prg(reader) + prg_ix;
  char *p = prg;

  char ch = *p;
  while (ch) {
    if (is_blank(ch)) {
      if (ch == '\n') ++nline;
      ch = *++p;
      continue;
    }

    if (ch == '/' && *(p + 1) == '/') {
      p += 2;
      ch = *p;
      while (ch && ch != '\n')
        ch = *++p;
      if (ch) {
        ++nline;
        ch = *++p;
      }
      continue;
    }

    if (ch == '/' && *(p + 1) == '*') {
      reader_set_nline(reader, nline);
      p += 2;
      ch = *p;
      while (ch && (ch != '*' || *(p + 1) != '/')) {
        if (ch == '\n') ++nline;
        ch = *++p;
      }
      if (ch) {
        ++p;
        ch = *++p;
      } else {
        reader_fail(reader, "Unclosed commentary");
      }
      continue;
    }

    break;
  }
  reader_set_nline(reader, nline);

  char *prgd = p; // no blank start.
  if (ch) {
    // Minus sign --------------------------------------------------------------
    if (ch == '-') {
      ch = *++p;
      if (ch < '0' || ch > '9') {
        if (is_blank(ch)) {
          reader_set_prg_ix(reader, prg_ix + p - prg);
          return opt_new(token_new_symbol(nline, symbol_new("-")));
        }
      }
    }

    // Number ------------------------------------------------------------------
    if (ch >= '0' && ch <= '9') {
      while (!is_blank(*++p));
      char *n = str_sub(prgd, 0, p - prgd);
      int ix = str_cindex(n, '.');
      if (ix != -1) {
        n[ix] = *(localeconv()->decimal_point);
        errno = 0;
        char *tail;
        double d = strtod(n, &tail);
        if (errno) reader_fail(reader, str_f("Float overflow in '%s'", n));
        if (*tail) reader_fail(reader, str_f("Bad number '%s'", n));

        reader_set_prg_ix(reader, prg_ix + p - prg);
        return opt_new(token_new_float(nline, d));
      }

      errno = 0;
      char *tail;
      int i = strtol (n, &tail, 0);
      if (errno) reader_fail(reader, str_f("Integer overflow in '%s'", n));
      if (*tail) reader_fail(reader, str_f("Bad number '%s'", n));

      reader_set_prg_ix(reader, prg_ix + p - prg);
      return opt_new(token_new_int(nline, i));
    }

    // String ------------------------------------------------------------------
    if (ch == '"') {
      int is_hex (char ch) {
        return (ch >= '0' && ch <= '9') ||
          (ch >= 'a' && ch <= 'f') ||
          (ch >= 'A' && ch <= 'F');
      }

      ++p;
      Buf *bf = buf_new();
      while (((unsigned char)*p) >= ' ' && *p != '"') {
        if (*p == '\\') {
          ++p;
          switch (*p) {
            case '"' :
            case '\\' :
            case '/' :
              buf_cadd(bf, *p);
              break;
            case 'b' :
              buf_cadd(bf, '\b');
              break;
            case 'f' :
              buf_cadd(bf, '\f');
              break;
            case 'n' :
              buf_cadd(bf, '\n');
              break;
            case 'r' :
              buf_cadd(bf, '\r');
              break;
            case 't' :
              buf_cadd(bf, '\t');
              break;
            case 'u' : {
              ++p;
              int c = 5;
              while (--c) {
                if (!is_hex(*p++))
                  reader_fail(reader, str_f(
                    "Bad hexadecimal unicode in '%s'",
                    str_sub(prgd, 0, p - prgd)
                  ));
              }
              to_unicode(bf, p - 4);
              continue;
            }
            default :
              reader_fail(reader, str_f(
                    "Bad escape sequence in '%s'",
                    str_sub(prgd, 0, p - prgd + 1)
              ));
          }
          ++p;
        } else {
          buf_cadd(bf, *p++);
        }
      }
      if (*p != '"') {
        reader_fail(reader, str_f(
          "String does not end with \" in %s (%d)",
          str_sub(prgd, 0, p - prgd), (int) *p
        ));
      }

      reader_set_prg_ix(reader, prg_ix + p - prg + 1);
      return opt_new(token_new_string(nline, buf_to_str(bf)));
    }

    // List --------------------------------------------------------------------
    if (ch == '(' || ch == '[' || ch == '{') {
      char sign = ch;
      int sum0 = sign == '(' ? 1 : 0;
      int sum1 = sign == '[' ? 1 : 0;
      int sum2 = sign == '{' ? 1 : 0;
      char *lstart = p + 1;
      int nline_start = nline;

      while (sum0 + sum1 + sum2 && *++p) {
        char ch = *p;
        switch (ch) {
          case '(':
            ++sum0;
            break;
          case '[':
            ++sum1;
            break;
          case '{':
            ++sum2;
            break;
          case ')':
            --sum0;
            break;
          case ']':
            --sum1;
            break;
          case '}':
            --sum2;
            break;
        }

        if (ch == '"') {
          char ch = *++p;
          while (ch && ch != '"') {
            if (ch == '\\' && p[1] == '"') ++p;
            ch = *++p;
          }
          if (!ch) --p;
          continue;
        }

        char e = ' ';
        if (sum0 < 0) e = '(';
        if (sum1 < 0) e = '[';
        if (sum2 < 0) e = '{';
        if (e != ' ')
          reader_fail(reader, str_f(
            "Extra '%c' in '%s'", e, str_sub(prgd, 0, p - prgd + 1)
          ));

        e = ' ';
        if (sign == '(' && !sum0 && sum1) e = '[';
        if (sign == '(' && !sum0 && sum2) e = '{';
        if (sign == '[' && !sum1 && sum0) e = '(';
        if (sign == '[' && !sum1 && sum2) e = '{';
        if (sign == '{' && !sum2 && sum0) e = '(';
        if (sign == '{' && !sum2 && sum1) e = '[';
        if (e != ' ')
          reader_fail(reader, str_f(
            "Internal '%c' open when '%c' was closed in '%s'",
            e, sign, str_sub(prgd, 0, p - prgd + 1)
          ));
      }
      if (!*p) {
        reader_fail(reader, str_f(
          "'%c' without close in '%s'", sign, str_sub(prgd, 0, p - prgd)
        ));
      }

      Reader *subr = reader_new_from_reader(
        reader, str_sub(lstart, 0, p - lstart), nline
      );

      // Arr<Token>
      Arr *a = arr_new();
      Token *tk = opt_nget(tkreader_next(subr));
      while (tk) {
        if (token_type(tk) == token_SYMBOL)
          tk = reader_symbol_id(subr, a, tk);
        arr_push(a, tk);
        tk = opt_nget(tkreader_next(subr));
      }

      reader_set_nline(reader, reader_nline(subr));
      reader_set_prg_ix(reader, prg_ix + p - prg + 1);
      return opt_new(token_new_list(nline_start, a));
    }

    // Symbol ------------------------------------------------------------------
    while (!is_blank(*++p));
    reader_set_prg_ix(reader, prg_ix + p - prg);
    char *name = str_sub(prgd, 0, p - prgd);
    char *id = str_eq(name, "this") ? reader_source(reader) : name;
    return opt_new(token_new_symbol(nline, symbol_new(id)));
  }

  return opt_empty();
}
