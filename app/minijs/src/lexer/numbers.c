// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/numbers.h"
#include "lexer/token.h"

Txpos *numbers_skip(Txpos *tx) {
  char *p = txpos_start(tx);
  char *start = p;
  char *end = txpos_end(tx);

  if (*p == 'b') {
    return tx;
  }

  int points = 0;
  int es = 0;
  for (;;) {
    if (p >= end) {
      break;
    }
    bool stop = true;
    char ch = *p;
    switch (ch) {
    case '+':
    case '-':
      if (es == 1) {
        ++p;
        ++es;
        stop = false;
      }
      break;
    case '.':
      if (points == 0) {
        ++p;
        ++points;
        stop = false;
      }
      break;
    case 'E':
    case 'e':
      if (es == 0) {
        ++p;
        ++es;
        ++points;
        stop = false;
      }
      break;
    case '_':
      if (points == 0) {
        ++p;
        stop = false;
      }
      break;
    case 'b':
      if (points == 0) {
        ++p;
      }
      break;
    default:
      if (ch >= '0' && ch <= '9') {
        if (es == 1) {
          ++es;
        }
        ++p;
        stop = false;
      }
    }
    if (stop) {
      break;
    }
  }
  if (es == 1) {
    --p;
  }

  return token_blanks(
    txpos_move(tx, p, txpos_nline(tx), txpos_nchar(tx) + p - start)
  );
}
