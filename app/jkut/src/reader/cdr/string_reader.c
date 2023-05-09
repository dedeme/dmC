// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/cdr/string_reader.h"
#include "kut/buf.h"
#include "kut/tp.h"
#include "DEFS.h"

// <char, char>
Tp *read_unicode (Cdr *cdr) {
  int from_hex(char *r, char ch) {
    if (ch >= '0' && ch <= '9') *r = ch - '0';
    else if (ch >= 'a' && ch <= 'f') *r = ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F') *r = ch - 'A' + 10;
    else return 1;
    return 0;
  }

  Buf *bf = buf_new();
  buf_add(bf, "\\u");
  unsigned r0 = 0;
  unsigned m = 1 << 12;
  char *tmp = str_new("0");

  char ch;
  REPEAT(4) {
    ch = cdr_read_char(cdr);
    if (from_hex(tmp, ch)) {
      cdr_unread_char(cdr, ch);
      EXC_GENERIC(cdr_fail_line(cdr, str_f(
        "Bad unicode sequence (\\u%s).", buf_str(bf)
      ), cdr_get_next_nline(cdr)));
    }
    buf_cadd(bf,ch);

    r0 += ((unsigned)(*tmp)) * m;
    m >>= 4;
  }_REPEAT

  unsigned *r = ATOMIC(sizeof(unsigned) + sizeof(unsigned));
  r[0] = r0;
  r[1] = 0;
  char *rst = str_from_unicode(r);
  if (!rst)
    EXC_GENERIC(cdr_fail_line(cdr, str_f(
      "Bad unicode sequence (\\u%s).", buf_str(bf)
    ), cdr_get_next_nline(cdr)));
  return tp_new(rst, buf_str(bf));
}

// <char, char>
Tp *read_escape (Cdr *cdr, char close) {
  char ch = cdr_read_char(cdr);
  if (ch == close) {
    char *r = str_new_c(close);
    return tp_new(r, str_f("%c%c", '\\', close));
  }

  switch (ch) {
    case 0:
      EXC_GENERIC(cdr_fail_line(
        cdr, "Unclosed quotes.", cdr_get_next_nline(cdr)
      ));
    case '\\':
      return tp_new("\\", "\\\\");
    case 'n':
      return tp_new("\n", "\\n");;
    case 't':
      return tp_new("\t", "\\t");;
    case 'b':
      return tp_new("\b", "\\b");;
    case 'f':
      return tp_new("\f", "\\f");;
    case 'r':
      return tp_new("\r", "\\r");;
    case 'u':
      return read_unicode(cdr);
    default:
      cdr_unread_char(cdr, ch);
      EXC_GENERIC(cdr_fail_line(cdr, str_f(
        "Bad escape squence '\\' + %d('%c').", ch, ch
      ), cdr_get_next_nline(cdr)));
      return 0; // Unreachable
  }
}

Token *read_single_string(Cdr *cdr, char ch, char *blanks) {
  Buf *row = buf_new();
  Buf *bf = buf_new();
  buf_cadd(row, ch);
  char close = ch;
  for (;;) {
    ch = cdr_read_char(cdr);
    if (ch == 0) EXC_GENERIC(cdr_fail_line(
        cdr, "Unclosed quotes.", cdr_get_next_nline(cdr)
      ));
    if (ch == '\n') EXC_GENERIC(cdr_fail_line(
        cdr, "Unclosed quotes.", cdr_get_next_nline(cdr) - 1
      ));
    if (ch == close) {
      buf_cadd(row, close);
      return token_string(
        str_new(buf_str(bf)),
        str_f("%s%s", blanks, buf_str(row))
      );
    }
    if (ch == '\\') {
      // <char, char>
      Tp *tx_row = read_escape(cdr, close);
      buf_add(bf, tp_e1(tx_row));
      buf_add(row, tp_e2(tx_row));
      continue;
    }

    buf_cadd(bf, ch);
    buf_cadd(row,ch);
  }
}

Token *read_multi_string(Cdr *cdr, int nline, char *blanks) {
  char *format(char *s) {
    if (*s == '\n') s = str_right(s, 1);
    // <char>
    Arr *ss = str_split(s, "\n");
    int cut = -1;
    EACH(ss, char, l) {
      if (!*str_trim(l)) continue;
      char *p = l;
      while (*p == ' ') ++p;
      int n = p - l;
      if (cut < 0 || n < cut) cut = n;
    }_EACH
    if (cut > 0) {
      EACH(ss, char, l) {
        arr_set(ss, _i, str_right(l, cut));
      }_EACH
    }
    return arr_join(ss, "\n");
  }

  Buf *bf = buf_new();
  char ch;
  for (;;) {
    ch = cdr_read_char(cdr);
    switch (ch) {
      case 0:
        EXC_GENERIC(cdr_fail_line(cdr, "Unclosed quotes.", nline));
      case '"': {
        ch = cdr_read_char(cdr);
        switch (ch) {
          case 0:
            EXC_GENERIC(cdr_fail_line(cdr, "Unclosed quotes.", nline));
          case '"': {
            ch = cdr_read_char(cdr);
            switch (ch) {
              case 0:
                EXC_GENERIC(cdr_fail_line(cdr, "Unclosed quotes.", nline));
              case '"': {
                char *s = str_replace(
                  str_replace(buf_str(bf), "$", "${\"$\"}"),
                  "`", "${\"`\"}"
                );
                char *fmt = format(s);
                char *js = *s == '\n'
                  ? str_f("%sString.raw\n`%s`", blanks, fmt)
                  : str_f("%sString.raw`%s`", blanks, fmt)
                ;
                return token_string(fmt, js);
              }
              default:
                buf_cadd(bf, '"');
                buf_cadd(bf, '"');
                buf_cadd(bf, ch);
            }
            break;
          }
          default:
            buf_cadd(bf, '"');
            buf_cadd(bf, ch);
        }
        break;
      }
      default:
        buf_cadd(bf, ch);
    }
  }
}

Token *string_reader_read(Cdr *cdr, char ch, char *blanks) {
  if (ch == '\'') return read_single_string(cdr, ch, blanks);

  ch = cdr_read_char(cdr);
  if (ch != '"') {
    cdr_unread_char(cdr, ch);
    return read_single_string(cdr, '"', blanks);
  }

  ch = cdr_read_char(cdr);
  if (ch == '"') return read_multi_string(cdr, cdr_get_next_nline(cdr), blanks);

  cdr_unread_char(cdr, ch);
  return token_string("", str_f("%s\"\"", blanks));
}
