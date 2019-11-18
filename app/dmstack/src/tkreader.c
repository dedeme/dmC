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

static char *big_msg (char *tx) {
  tx = str_trim(tx);
  int ix = str_cindex(tx, '\n');
  char *left = ix == -1 ? tx : str_left(tx, ix);
  ix = str_last_cindex(tx, '\n');
  char *right = str_right(tx, ix + 1);
  return str_eq(left, right)
    ? left
    : str_f("%s\n...\n%s", left, right)
  ;
}

Opt *tkreader_next(Reader *reader) {
  if (
    reader_is_file(reader) &&
    reader_nline(reader) == 1 &&
    reader_prg_ix(reader) == 0
  ) {
    char *prg = reader_prg(reader);
    if (*prg && *prg == '#' && prg[1] == '!') {
      int ix = str_cindex(prg, '\n');
      if (ix == -1) ix = strlen(prg) - 1;
      reader_set_nline(reader, 2);
      reader_set_prg_ix(reader, ix + 1);
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
      if (is_blank(ch)) {
        reader_set_prg_ix(reader, prg_ix + p - prg);
        return opt_new(token_new_symbol_pos(
          symbol_new("-"), reader_source(reader), nline
        ));
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
        if (errno) reader_fail(reader, str_f("Float overflow in\n'%s'", n));
        if (*tail) reader_fail(reader, str_f("Bad number '%s'", n));

        reader_set_prg_ix(reader, prg_ix + p - prg);
        return opt_new(token_new_float_pos(d, reader_source(reader), nline));
      }

      errno = 0;
      char *tail;
      int i = strtol (n, &tail, 0);
      if (errno) reader_fail(reader, str_f("Integer overflow in\n'%s'", n));
      if (*tail) reader_fail(reader, str_f("Bad number '%s'", n));

      reader_set_prg_ix(reader, prg_ix + p - prg);
      return opt_new(token_new_int_pos(i, reader_source(reader), nline));
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
                    "Bad hexadecimal unicode in\n'%s'",
                    str_sub(prgd, 0, p - prgd)
                  ));
              }
              to_unicode(bf, p - 4);
              continue;
            }
            default :
              reader_fail(reader, str_f(
                    "Bad escape sequence in\n'%s'",
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
          "String does not end with \" in\n%s (%d)",
          str_sub(prgd, 0, p - prgd), (int) *p
        ));
      }

      reader_set_prg_ix(reader, prg_ix + p - prg + 1);
      return opt_new(token_new_string_pos(
        buf_to_str(bf), reader_source(reader), nline
      ));
    }

    // String multiline --------------------------------------------------------
    if (ch == '`') {

      ++p;
      char *start = p;
      int start_line = nline;
      while (*p && !is_blank(*p)) ++p;
      if (!*p) reader_fail(reader, "String multiline not closed");
      char *id = str_cat(str_left(start, p - start), "`", NULL);

      while (*p && is_blank(*p))
        if (*p == '\n') break;
        else ++p;
      if (!*p) reader_fail(reader, "String multiline not closed");
      if (*p != '\n')
        reader_fail(reader, "String multiline open must be at end of line");
      ++p;
      ++nline;
      start = p;
      while (*p && is_blank(*p)) ++p;
      if (!*p) reader_fail(reader, "String multiline not closed");
      int blanks = p - start;

      Buf *bf = buf_new();
      while (*p && !str_starts(p, id)) {
        if(*p == '\n') {
          buf_cadd(bf, *p++);
          ++nline;
          for (int i = 0; i < blanks; ++i) {
            if (*p && *p != '\n' && is_blank(*p)) ++p;
            else break;
          }
        } else {
          buf_cadd(bf, *p++);
        }
      }
      if (!*p) reader_fail(reader, "String multiline not closed");

      reader_set_nline(reader, nline);
      reader_set_prg_ix(reader, prg_ix + p - prg + strlen(id));
      return opt_new(token_new_string_pos(
        buf_to_str(bf), reader_source(reader), start_line + 1
      ));
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
          char *start = p;
          ch = *++p;
          while (ch && ch != '"') {
            if (ch == '\\') ++p;
            ch = *++p;
          }
          if (!ch)
            reader_fail(reader, str_f(
              "Quotes unclosed in\n%s",
              big_msg(str_sub(prgd, start - prgd, p - prgd))
            ));
          continue;
        }

        if (ch == '/') {
          char *start = p;
          ch = *++p;
          if (ch == '/') {
            int ix = str_cindex_from(prgd, '\n', p - prgd);
            if (ix == -1) ix = strlen(prgd) - 1;
            p = prgd + ix;
            continue;
          }
          if (ch == '*') {
            ch = *++p;
            while (ch) {
              if (ch == '*' && p[1] == '/') break;
              ch = *++p;
            }
            if (!ch)
              reader_fail(reader, str_f(
                "'/*' unclosed in\n%s",
                big_msg(str_right(prgd, start - prgd))
              ));
            ++p;
            continue;
          }
        }

        if (ch == '`') {
          char *start = p++;
          int ix = str_cindex_from(prgd, '\n', p - prgd);
          if (ix != -1) {
            char *close = str_f("%s%c", str_sub(prgd, p - prgd, ix), ch);
            int ix2 = str_index_from(prgd, close, ix + 1);
            if (ix2 != -1) {
              p = prgd + ix2 + strlen(close);
              continue;
            }
          }
          reader_fail(reader, str_f(
            "'`' unclosed or not at end of line in\n%s",
            big_msg(str_right(prgd, start - prgd))
          ));
        }

        char e = ' ';
        if (sum0 < 0) e = '(';
        if (sum1 < 0) e = '[';
        if (sum2 < 0) e = '{';
        if (e != ' ')
          reader_fail(reader, str_f(
            "Extra '%c' in\n'%s'", e, big_msg(str_sub(prgd, 0, p - prgd + 1))
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
            "Internal '%c' open when '%c' was closed in\n'%s'",
            e, sign, big_msg(str_sub(prgd, 0, p - prgd + 1))
          ));
      }
      if (!*p) {
        reader_fail(reader, str_f(
          "'%c' without close in\n'%s'",
          sign, big_msg(str_sub(prgd, 0, p - prgd))
        ));
      }

      Reader *subr = reader_new_from_reader(
        reader, str_sub(lstart, 0, p - lstart), nline
      );
      Token *tk = reader_process(subr);

      reader_set_nline(reader, reader_nline(subr));
      reader_set_prg_ix(reader, prg_ix + p - prg + 1);
      return opt_new(token_new_list_pos(
        token_list(tk), reader_source(reader), nline_start
      ));
    }

    // Symbol ------------------------------------------------------------------
    while (!is_blank(*++p));
    reader_set_prg_ix(reader, prg_ix + p - prg);
    char *id = str_sub(prgd, 0, p - prgd);
    return opt_new(token_new_symbol_pos(
      symbol_new(id), reader_source(reader), nline
    ));
  }

  return opt_empty();
}
