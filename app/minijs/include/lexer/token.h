// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader for little text chuncks

#ifndef LEXER_TOKEN_H
  #define LEXER_TOKEN_H

#include <dmc/all.h>
#include "lexer/Tx.h"

///
Tx *token_blanks(Tx *tx);

/// token_skip_block jumps until ofter the next end of block ('}') or until
/// the end of text.
Tx *token_skip_bock(Tx *tx);

/// Reads one character. It does not fail. Only advance one character.<br>
/// tx does not change if next character is '0'
Tx *token_char(char *ch, Tx *tx);

/// Values must not be '\n'. It does not fail. Only advances a character.
Tx *token_cconst0(Tx *tx, char value);

/// Values must not be '\n'. It does not fail.
Tx *token_cconst(Tx *tx, char value);

/// Values must not contain '\n'. It does not fail.
Tx *token_const(Tx *tx, char *value);

/// Looks if next character is one in 'values'.<br>values does not contains '\n'
/// <br>Only advances one character.
Tx *token_csplit0(char *r, Tx *tx, char *values);

/// Looks if next character is one in 'values'.<br>values does not contains '\n'
Tx *token_csplit(char *r, Tx *tx, char *values);

/// Looks if next chars are one in 'values'. Values are group of characters
/// separated by one blank.<br>values does not contains '\n'
Tx *token_split(char **r, Tx *tx, char *values);

/// Creates and reads a list. If fails it throws a tx_exception.<p>
/// Throws tx_exception.
Tx *token_list(Arr **list, Tx *tx, char close, Tx *(*read)(void **, Tx *));

/// Creates and reads a function definition parameter list. It does not fail.
/// Also 'read' must not fail.
Tx *token_fn_list(Arr **list, Tx *tx, Tx *(*read)(void **, Tx *));

/// Creates and reads an identifier. It does not fail.
Tx *token_id(char **id, Tx *tx);

/// Creates and reads a directive. It does not fail.
Tx *token_directive(Tx *tx, char *value);

/// Creates and reads a cpath. It does not fail.
Tx *token_path(char **path, Tx *tx);

///
Tx *token_bool(char **value, Tx *tx);

///
Tx *token_lunary(char **op, Tx *tx);

///
Tx *token_runary(char **op, Tx *tx);

///
Tx *token_binary(char **op, Tx *tx);

///
bool token_is_reserved(char *id);

/// Read an id.<p
/// It fails when id is a reserved word and throws a tx_exception;
/// but if there is not one id to read does not fail and return 'tx'.<p>
/// Throws tx_exception.
Tx *token_valid_id(char **id, Tx *tx);

/// Read an id with o without generics. If there are not generics '*generics'
/// is initialized to an empty array.<p>
/// If genericks is like '<>' throw a tx_exception.<p>
/// Throws tx_exception.
Tx *token_generic_id(char **id, Achar **generics, Tx *tx);

/// Read text until mark. Tx moves after mark.<p>
/// Throws tx_exception if mark is missing.
Tx *token_native(char **text, Tx *tx, char *mark);

#endif
