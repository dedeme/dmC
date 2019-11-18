// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Minimal meaning unit of program.

#ifndef TOKEN_H
  #define TOKEN_H

#include "dmc/async.h"
#include "dmc/List.h"
#include "DEFS.h"
#include "Symbol.h"

// typedef struct machine_Machine Machine;

///
enum token_Type {
  token_INT, token_FLOAT, token_STRING, token_LIST, token_SYMBOL,
  token_NATIVE
};

///
typedef struct token_Token Token;

///
typedef struct token_Pos TokenPos;

///
char *tokenPos_source (TokenPos *this);

///
int tokenPos_line (TokenPos *this);

///
Token *token_new_int (Int value);

///
Token *token_new_float (double value);

///
Token *token_new_string (char *value);

/// 'value' is Arr<Token>
Token *token_new_list (Arr *value);

///
Token *token_new_symbol (Symbol value);

///
Token *token_from_pointer (Symbol sym, void *pointer);

///
Token *token_new_int_pos (Int value, Symbol source, int line);

///
Token *token_new_float_pos (double value, Symbol source, int line);

///
Token *token_new_string_pos (char *value, Symbol source, int line);

/// 'value' is Arr<Token>
Token *token_new_list_pos (Arr *value, Symbol source, int line);

///
Token *token_new_symbol_pos (Symbol value, Symbol source, int line);

///
enum token_Type token_type (Token *this);

/// Opt<TokenPos>
Opt *token_pos (Token *this);

///
Int token_int (Token *this);

///
double token_float (Token *this);

///
char *token_string (Token *this);

/// Returns Arr<Token>
Arr *token_list (Token *this);

///
Symbol token_symbol (Token *this);

///
Symbol token_native_symbol (Token *this);

///
void *token_native_pointer (Token *this);

/// Returns a new token equals to 'this'
Token *token_clone (Token *this);

///
int token_eq (Token *this, Token *other);

///
char *token_to_str (Token *this);

///
char *token_to_str_draft (Token *this);

///
char *token_type_to_str (enum token_Type type);

#endif
