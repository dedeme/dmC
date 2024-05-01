// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "read/token.h"

Token *token_new (
  TokenT type, char *tx, char *rest,
  int ln_start, int ch_start, int ln_end, int ch_end
) {
  Token *this = MALLOC(Token);
  this->type = type;
  this->tx = tx;
  this->rest = rest;
  this->ln_start = ln_start;
  this->ch_start = ch_start;
  this->ln_end = ln_end;
  this->ch_end = ch_end;
  return this;
}
