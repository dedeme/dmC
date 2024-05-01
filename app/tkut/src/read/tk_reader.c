// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "read/tk_reader.h"

Token *tk_reader_read (char *kt_code, int nline, int nchar) {
  int ln_start = nline;
  int ch_start = nchar;
  if (!*kt_code)
    return token_new(TK_EOF, "", kt_code, ln_start, ch_start, nline, nchar);

  return token_new(TK_EOF, "", kt_code, ln_start, ch_start, nline, nchar);
}
