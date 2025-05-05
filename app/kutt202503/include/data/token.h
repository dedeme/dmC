// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token data.
/// NOTE: There are token_error (+ message) and token_eof.

#ifndef DATA_TOKEN_H
  #define DATA_TOKEN_H

/// Token types
typedef enum {
  token_error, token_eof, token_bool, token_int, token_float,
  token_string, token_lineComment, token_comment, token_symbol,
  token_operator, token_ktype
} TokenT;

/// Record structure.
typedef struct {
  int ln;
  TokenT tp;
  char *value;
} Token;

/// Constructor
///   ln   : Number of line.
///   tp   : One of end, bool, int, float, string, lineComment, comment, symbol
///          or operator.
///   value: Token value.
Token *token_new (int ln, TokenT tp, char *value);

/// Returns 'true' if 'tk' is binary operator.
int token_is_binary (Token *tk);

/// Returns 'true' if 'tk' is binary operator "*", "/" or "%".
int token_is_binary1 (Token *tk);

/// Returns 'true' if 'tk' is binary operator "+" or "-".
int token_is_binary2 (Token *tk);


/// Returns 'true' if 'tk' is binary operator "==", "!=", ">", ">=", "<" or "<=".
int token_is_binary3 (Token *tk);

/// Returns 'true' if 'tk' is binary operator "&" or "|".
int token_is_binary4 (Token *tk);

/// Returns 'true' if 'tk' is symbol with value 'v'.
int token_is_sym (Token *tk, char *v);

/// Returns 'true' if 'tk' is operator with value 'v'.
int token_is_op (Token *tk, char *v);

///
char *token_type_to_str (Token *tk);

///
char *token_to_str (Token *tk);

#endif
