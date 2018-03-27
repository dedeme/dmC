// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for numbers

#ifndef LEXER_NUMBERS_H
  #define LEXER_NUMBERS_H

#include <dmc/all.h>
#include "lexer/Txpos.h"

/// strings_skip skips a number. It starts at the second character.
Txpos *numbers_skip(Txpos *tx);

/// strings_skip_hex skips a hexadecimal number. It starts at the first
/// character after 0x
Txpos *numbers_skip_hex(Txpos *tx);

#endif
