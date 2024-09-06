// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/cdr/comment_reader.h"
#include "kut/buf.h"
#include "kut/str.h"
#include "DEFS.h"

static Token *read_line_comment(Cdr *cdr, Buf *bf, char *blanks) {
  char ch;
  for (;;) {
    ch = cdr_read_char(cdr);
    switch (ch) {
      case 0:
      case '\n':
        return token_line_comment(str_new(buf_str(bf)), str_f("%s\n", blanks));
      default:
        buf_cadd(bf, ch);
    }
  }
}

static Token *read_long_comment(Cdr *cdr, Buf *bf, char *blanks) {
  int nline = cdr_get_next_nline(cdr);
  char ch;
  for (;;) {
    ch = cdr_read_char(cdr);
    switch (ch) {
      case 0:
        EXC_GENERIC(cdr_fail_line(cdr, "Unclosed comment.", nline));
      case '*': {
        char ch2 = cdr_read_char(cdr);
        switch (ch2) {
          case 0:
            EXC_GENERIC(cdr_fail_line(cdr, "Unclosed comment.", nline));
          case '/':
            buf_add(bf, "*/");
            int nls = 0;
            char *p = buf_str(bf);
            while (*p) {
              if (*p == '\n') ++nls;
              ++p;
            }
            Buf *js = buf_new();
            buf_add(js, blanks);
            REPEAT(nls) {
              buf_cadd(js, '\n');
            }_REPEAT

            return token_comment(str_new(buf_str(bf)), buf_str(js));
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

Token *comment_reader_read(Cdr *cdr, char *blanks) {
  Buf *bf = buf_new();
  buf_cadd(bf, '/');
  char ch = cdr_read_char(cdr);
  switch (ch) {
    case 0:
      EXC_GENERIC(cdr_fail_line(
        cdr, "Unexpected end of file.", cdr_get_next_nline(cdr)
      ));
    case '/':
      buf_cadd(bf, ch);
      return read_line_comment(cdr, bf, blanks);
    case '*':
      buf_cadd(bf, ch);
      return read_long_comment(cdr, bf, blanks);
    default:
      cdr_unread_char(cdr, ch);
      return token_operator("/", "");
  }
}
