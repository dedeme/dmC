// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for strings and chars

#ifndef LEXER_STRINGS_H
  #define LEXER_STRINGS_H

#include <dmc/all.h>
#include "lexer/Txpos.h"
#include "DEFS.h"

/// strings_skip skips a string. It throws a tx_exception if the end of 'tx' is
/// reached.<br>
/// Delimiter must be ("), (`) or ('). 'tx' start at the next character of
/// ther first delimiter.
Txpos *strings_skip(Txpos *tx, char delimiter);

/// strings_skip skips a string. It throws a tx_exception if the end of 'tx' is
/// reached.<br>
/// Delimiter must be ("), (`) or ('). 'tx' start at the delimiter.
Txpos *strings_read(enum Vtype_t *type, char **value, Txpos *tx);

#endif
