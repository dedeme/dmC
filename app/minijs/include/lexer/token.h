// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for little text chuncks

#ifndef LEXER_TOKEN_H
  #define LEXER_TOKEN_H

#include <dmc/all.h>
#include "lexer/Txpos.h"
#include "Cpath.h"

///
Txpos *token_blanks(Txpos *tx);

/// token_skip_stat jumps until ofter the next end of statement (';') or until
/// the end of text.
Txpos *token_skip_stat(Txpos *tx);

/// Values must not be '\n'. It does not fail. Only advances a character.
Txpos *token_cconst0(Txpos *tx, char value);

/// Values must not be '\n'. It does not fail.
Txpos *token_cconst(Txpos *tx, char value);

/// Values must not contain '\n'. It does not fail.
Txpos *token_const(Txpos *tx, char *value);

/// Looks if next character is one in 'values'.<br>values does not contains '\n'
/// <br>Only advances one character.
Txpos *token_csplit0(char *r, Txpos *tx, char *values);

/// Looks if next character is one in 'values'.<br>values does not contains '\n'
Txpos *token_csplit(char *r, Txpos *tx, char *values);

/// Looks if next chars are one in 'values'. Values are group of characters
/// separated by one blank.<br>values does not contains '\n'
Txpos *token_split(char **r, Txpos *tx, char *values);

/// Creates and reads a list. If fails it throws a tx_exception.
Txpos *token_list(
  Arr **list, Txpos *tx, char close, Txpos *(*read)(void **, Txpos *)
);

/// Creates and reads a list. It does not fail. Also 'read' must not fail.
Txpos *token_fn_list(Arr **list, Txpos *tx, Txpos *(*read)(void **, Txpos *));

/// Creates and reads an identifier. It does not fail.
Txpos *token_id(char **id, Txpos *tx);

/// Creates and reads a directive. It does not fail.
Txpos *token_directive(Txpos *tx, char *value);

/// Creates and reads a cpath. It does not fail.
Txpos *token_path(char **path, Txpos *tx);

/// Creates and reads an identifier with or without point. It does not fail.
Txpos *token_point_id(char **id, Txpos *tx);

///
Txpos *token_bool(char **value, Txpos *tx);

///
Txpos *token_lunary(char **op, Txpos *tx);

///
Txpos *token_runary(char **op, Txpos *tx);

///
Txpos *token_binary(char **op, Txpos *tx);

///
bool token_is_reserved(char *id);

#endif
