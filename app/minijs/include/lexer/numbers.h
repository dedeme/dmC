// Copyright 27-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for numbers

#ifndef LEXER_NUMBERS_H
  #define LEXER_NUMBERS_H

#include <dmc/all.h>
#include "lexer/Txpos.h"

///
Txpos *numbers_byte(char **value, Txpos *tx);

///
Txpos *numbers_int(char **value, Txpos *tx);

/// Throws a tx_exception if float take an exponent and it is wrong.
Txpos *numbers_float(char **value, Txpos *tx);

#endif
