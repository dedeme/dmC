// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Minimal meaning unit of program.

#ifndef TOKEN_H
  #define TOKEN_H

#include "dmc/std.h"

///
enum token_Type {
  token_INT, token_FLOAT, token_STRING, token_BLOB, token_LIST, token_SYMBOL
};

///
typedef struct token_Token Token;

///
Token *token_new_int (int value);

///
Token *token_new_float (double value);

///
Token *token_new_string (char *value);

///
Token *token_new_blob (int length);

/// 'value' is Arr<Token>
Token *token_new_list (Arr *value);

///
Token *token_new_symbol (char *value);

///
enum token_Type token_type (Token *this);

///
int token_int (Token *this);

///
double token_float (Token *this);

///
char *token_string (Token *this);

///
Bytes *token_blob (Token *this);

/// Returns Arr<Token>
Arr *token_list (Token *this);

///
char *token_symbol (Token *this);

///
char *token_to_str (Token *this);

///
char *token_type_to_str (enum token_Type type);

#endif
