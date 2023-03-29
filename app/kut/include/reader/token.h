// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable classses of Kut tokens.

#ifndef TOKEN_H
  #define TOKEN_H

#include <stdint.h>

/// Token structure.
typedef struct token_Token Token;

/// Creates a token of the indicated type.
Token *token_bool (int value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int token_get_bool (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_bool (Token *this);

/// Creates a token of the indicated type.
Token *token_int (int64_t value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
int64_t token_get_int (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_int (Token *this);

/// Creates a token of the indicated type.
Token *token_float (double value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
double token_get_float (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_float (Token *this);

/// Creates a token of the indicated type.
Token *token_string (char *value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *token_get_string (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_string (Token *this);

/// Creates a token of the indicated type.
Token *token_line_comment (char *value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *token_get_line_comment (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_line_comment (Token *this);

/// Creates a token of the indicated type.
Token *token_comment (char *value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *token_get_comment (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_comment (Token *this);

/// Creates a token of the indicated type.
Token *token_symbol (char *value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *token_get_symbol (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_symbol (Token *this);

/// Creates a token of the indicated type.
Token *token_operator (char *value);

/// Read an Token of the indicate type.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *token_get_operator (Token *this);

/// Returns TRUE if 'this' match the type.
int token_is_operator (Token *this);

/// Returns TRUE if 'this' is an unary operator.
int token_is_unary (Token *this);

/// Returns TRUE if 'this' is a binary operator.
int token_is_binary (Token *this);

/// Returns TRUE if 'this' is a binary operator of precedence '1'
/// ("*", "/", "%").
int token_is_binary1 (Token *this);

/// Returns TRUE if 'this' is a binary operator of precedence '2'
/// ("+", "-").
int token_is_binary2 (Token *this);

/// Returns TRUE if 'this' is a binary operator of precedence '3'
/// ("==", "!=", ">", ">=", "<", "<=").
int token_is_binary3 (Token *this);

/// Returns TRUE if 'this' is a binary operator of precedence '4'
/// ("&", "|").
int token_is_binary4 (Token *this);

/// Returns TRUE if 'this' is the ternary operator.
int token_is_ternary (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("=", "+=", "-=", "*=", "/=", "%=", "|=", "&=")
int token_is_assign (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("=")
int token_is_equals (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// (".")
int token_is_point (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// (",")
int token_is_comma (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// (":")
int token_is_colon (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// (";")
int token_is_semicolon (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("!")
int token_is_exclamation (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("\")
int token_is_backslash (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("->")
int token_is_arrow (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("(")
int token_is_open_par (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// (")")
int token_is_close_par (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("[")
int token_is_open_square (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("]")
int token_is_close_square (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("{")
int token_is_open_bracket (Token *this);

/// Returns TRUE if 'this' is an operator of the indicated type.
/// ("}")
int token_is_close_bracket (Token *this);

/// Returns TRUE if 'this' is the symbol 'else'.
int token_is_else (Token *this);

/// Returns TRUE if 'this' is the symbol 'catch'.
int token_is_catch (Token *this);

/// Returns TRUE if 'this' is the symbol 'finally'.
int token_is_finally (Token *this);

char *token_type_to_str (Token *this);

char *token_to_str (Token *this);

#endif
