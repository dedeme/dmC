// Copyright 08-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/cdr/number_reader.h"
#include "kut/buf.h"
#include "kut/js.h"
#include "DEFS.h"

static Token *read_exponent(Cdr *cdr, Buf *bf) {
  char ch = cdr_read_char(cdr);
  switch (ch){
    case '+':
    case '-':
      buf_cadd(bf, ch);
      ch = cdr_read_char(cdr);
  }
  if (ch >= '0' && ch <= '9') {
    while (ch >= '0' && ch <= '9') {
      buf_cadd(bf, ch);
      ch = cdr_read_char(cdr);
    }
    if (ch) cdr_unread_char(cdr, ch);
    return token_float(js_rd(buf_str(bf)));
  }
  if (ch) cdr_unread_char(cdr, ch);
  EXC_KUT(cdr_fail_line(
    cdr, "Exponent has no digits.", cdr_get_next_nline(cdr)
  ));
  return 0; // Unreachable
}

static Token *read_float(Cdr *cdr, Buf *bf) {
  for (;;) {
    char ch = cdr_read_char(cdr);
    switch (ch) {
      case 'e':
      case 'E':
        buf_cadd(bf, ch);
        return read_exponent(cdr, bf);
      default:
        if (ch >= '0' && ch <= '9') {
          buf_cadd(bf, ch);
        } else {
          if (ch) cdr_unread_char(cdr, ch);
          return token_float(js_rd(buf_str(bf)));
        }
    }
  }
}

Token *number_reader_read(Cdr *cdr, char ch) {
  Buf *bf = buf_new();
  buf_cadd(bf, ch);
  for (;;) {
    ch = cdr_read_char(cdr);
    switch (ch) {
      case '_':
        continue;
      case '.':
        buf_cadd(bf, ch);
        return read_float(cdr, bf);
      default:
        if (ch >= '0' && ch <= '9') {
          buf_cadd(bf, ch);
        } else {
          if (ch) cdr_unread_char(cdr, ch);
          return token_int(js_rl(buf_str(bf)));
        }
    }
  }
}
