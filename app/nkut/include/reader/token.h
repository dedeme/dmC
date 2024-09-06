// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable classses of Kut tokens.

#ifndef TOKEN_H
  #define TOKEN_H

#include <stdint.h>

enum token_Token_t {
  TOKEN_BOOL, TOKEN_FLOAT, TOKEN_STRING,
  TOKEN_LINE_COMMENT, TOKEN_COMMENT, TOKEN_SYMBOL, TOKEN_OPERATOR
};
typedef enum token_Token_t Token_t;

/// Token structure.
typedef struct token_Token {
  Token_t type;
  union {
    int b;
    double d;
    void *value;
  };
  char *js;
} Token;

/// Creates a token of the indicated type.
Token *token_bool (int value, char *js);

/// Creates a token of the indicated type.
Token *token_float (double value, char *js);

/// Creates a token of the indicated type.
Token *token_string (char *value, char *js);

/// Returns javascript code to inject with 'do' or 'eval'.
char *token_string_injection (Token *this);

/// Creates a token of the indicated type.
Token *token_line_comment (char *value, char *js);

/// Creates a token of the indicated type.
Token *token_comment (char *value, char *js);

/// Creates a token of the indicated type.
Token *token_symbol (char *value, char *js);

/// Creates a token of the indicated type.
Token *token_operator (char *value, char *js);

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
/// ("#")
int token_is_hash (Token *this);

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

/// Return TRUE if 'this' is a reserved word.
int token_is_reserved (char *word);

///
char *token_type_to_str (Token *this);

///
char *token_to_str (Token *this);

#endif
