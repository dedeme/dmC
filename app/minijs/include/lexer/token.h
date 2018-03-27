// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for little text chuncks

#ifndef LEXER_TOKEN_H
  #define LEXER_TOKEN_H

#include <dmc/all.h>
#include "lexer/Txpos.h"

///
Txpos *token_blanks(Txpos *tx);

/// token_skip_stat jumps until ofter the next end of statement (';') or until
/// the end of text.
Txpos *token_skip_stat(Txpos *tx);

/// Values must not be '\n'. It does not fail.
Txpos *token_cconst(Txpos *tx, char value);

/// Values must not contain '\n'. It does not fail.
Txpos *token_const(Txpos *tx, char *value);

/// Creates and reads a list. If fails it throws a tx_exception.
Txpos *token_list(
  Arr/*char*/ **list, Txpos *tx, char close, Txpos *(*read)(void **, Txpos *)
);

/// Creates and reads a identifier. It does not fail.
Txpos *token_id(char **id, Txpos *tx);


#endif
