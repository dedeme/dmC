// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Minimal meaning unit of program.

#ifndef TOKEN_H
  #define TOKEN_H

#include "dmc/async.h"
#include "DEFS.h"
#include "Symbol.h"

// typedef struct machine_Machine Machine;

///
enum token_Type {
  token_INT, token_FLOAT, token_STRING, token_LIST, token_SYMBOL,
  token_POINTER
};

///
typedef struct token_Token Token;

///
Token *token_new_int (int line, Int value);

///
Token *token_new_float (int line, double value);

///
Token *token_new_string (int line, char *value);

/// 'value' is Arr<Token>
Token *token_new_list (int line, Arr *value);

///
Token *token_new_symbol (int line, Symbol value);

///
Token *token_from_pointer (Symbol sym, void *pointer);

///
enum token_Type token_type (Token *this);

///
int token_line (Token *this);

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
void *token_pointer (Token *this);

/// Returns a new token equals to 'this'
Token *token_clone (Token *this);

///
int token_eq (Token *this, Token *other);

///
char *token_to_str (Token *this);

///
char *token_type_to_str (enum token_Type type);

#endif
