// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "read/st_reader.h"
#include "read/tk_reader.h"

char *st_reader_read (char *kt_code, RnCode *rn_code) {
  Token *tk = tk_reader_read(kt_code, 0, 0);

  return tk->tx;
}
