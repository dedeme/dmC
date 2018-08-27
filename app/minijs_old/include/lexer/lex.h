// Copyright 06-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Single elements lexer


#ifndef LEXER_LEX_H
  #define LEXER_LEX_H

#include <stdbool.h>

typedef struct list_List List;
typedef struct arr_Arr Arr;
typedef struct tx_Tx Tx;
typedef struct achar_Achar Achar;

/// lex_end returns true if tx is at end of text.
bool  lex_end(Tx *tx);

/// lex_blanks skips blanks and comments
Tx *lex_blanks(Tx *tx);

/// lex_no_blanks read characters until it found one <= ' '
Tx *lex_no_blanks(char **string, Tx *tx);

/// Reads one character. It does not fail. Only advance one character.<br>
/// tx does not change if next character is '0'
Tx *lex_char(char *ch, Tx *tx);

/// Read text until mark. Tx moves after mark.<p>
/// Throws tx_exception if mark is missing.
Tx *lex_native(char **text, Tx *tx, char *mark);

/// lex_const reads a char if is equals to 'ch'. 's' must not be a end of line.
Tx *lex_cconst(Tx *tx, char ch);

/// lex_const reads a sequence of chars if is equals to 's'. 's' must not
/// contain any end of line.
Tx *lex_const(Tx *tx, char *s);

/// Looks if next character is one in 'values'.<br>values does not contains '\n'
Tx *lex_csplit(char *r, Tx *tx, char *values);

/// Looks if next chars are one in 'values'. Values are group of characters
/// separated by one blank.<br>values does not contains '\n'
Tx *lex_split(char **r, Tx *tx, char *values);

/// Reads an identifier.
Tx *lex_id(char **rt, Tx *tx);

/// Read an id.<p>
/// It fails when id is a reserved word and throws a tx_exception;
/// but if there is no id to read does not fail and return 'tx'.<p>
/// Throws tx_exception.
Tx *lex_valid_id(char **id, Tx *tx);

/// Reads a list separated with commas and ending with 'end'.<br>
/// Throws a tx_exception.
Tx *lex_list(
  List **rt, Tx *tx, char end, Tx *(*relement)(void **, Tx *tx)
);

/// Reads an Arr separated with commas and ending with 'end'.<br>
/// Throws a tx_exception.
Tx *lex_arr(
  Arr **rt, Tx *tx, char end, Tx *(*relement)(void **, Tx *tx)
);

/// Creates and reads a function definition parameter list. It can fail if
/// 'read' fails.
Tx *lex_fn_list(Achar **list, Tx *tx, Tx *(*read)(char **, Tx *));

/// lex_test_reserved throws an exception if 'id' is a reserverd word.
void lex_test_reserved(Tx *tx, char *id);

/// lex_test_symbol throws an exception if 'id' is in symbols.
void lex_test_symbol(Tx *tx, Achar *symbols, char *id);

/// Read an id with o without generics. If there are not generics '*generics'
/// is initialized to an empty array.<p>
/// If generics is empty ('<>'), it throws a tx_exception.<p>
/// Throws tx_exception.
Tx *lex_generic_id(char **id, Achar **generics, Tx *tx);

///
Tx *lex_bool(char **value, Tx *tx);

///
Tx *lex_lunary(char **op, Tx *tx);

///
Tx *lex_runary(char **op, Tx *tx);

///
Tx *lex_binary(char **op, Tx *tx);

///
Tx *lex_assign(char **op, Tx *tx);

#endif
