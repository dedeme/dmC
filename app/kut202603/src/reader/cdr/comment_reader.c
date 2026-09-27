// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/cdr/comment_reader.h"
#include "kut/buf.h"
#include "kut/str.h"

static Token *read_line_comment(Cdr *cdr, Buf *bf) {
  char ch;
  for (;;) {
    ch = cdr_read_char(cdr);
    switch (ch) {
      case 0:
      case '\n':
        return token_line_comment(str_new(buf_str(bf)));
      default:
        buf_cadd(bf, ch);
    }
  }
}

static Token *read_long_comment(Cdr *cdr, Buf *bf) {
  int nline = cdr_get_next_nline(cdr);
  char ch;
  for (;;) {
    ch = cdr_read_char(cdr);
    switch (ch) {
      case 0:
        EXC_KUT(cdr_fail_line(cdr, "Unclosed comment.", nline));
      case '*': {
        char ch2 = cdr_read_char(cdr);
        switch (ch2) {
          case 0:
            EXC_KUT(cdr_fail_line(cdr, "Unclosed comment.", nline));
          case '/':
            buf_add(bf, "*/");
            return token_comment(str_new(buf_str(bf)));
          default:
            buf_cadd(bf, '*');
            ch = ch2;
        }
      }
      default:
        buf_cadd(bf, ch);
    }
  }
}

Token *comment_reader_read(Cdr *cdr) {
  Buf *bf = buf_new();
  buf_cadd(bf, '/');
  char ch = cdr_read_char(cdr);
  switch (ch) {
    case 0:
      EXC_KUT(cdr_fail_line(
        cdr, "Unexpected end of file.", cdr_get_next_nline(cdr)
      ));
    case '/':
      buf_cadd(bf, ch);
      return read_line_comment(cdr, bf);
    case '*':
      buf_cadd(bf, ch);
      return read_long_comment(cdr, bf);
    default:
      cdr_unread_char(cdr);
      return token_operator("/");
  }
}
