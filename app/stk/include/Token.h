// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TOKEN_H
  #define TOKEN_H

#include "dmc/std.h"

/*--*/

///
typedef struct token_Token Token;

///
Token *token_new(int nline, int nchar, char *value);

///
void token_free(Token *this);

///
int token_nline(Token *this);

///
int token_nchar(Token *this);

///
char *token_value(Token *this);

///
typedef struct tkError_TkError TkError;

///
TkError *tkError_new(Token *tk, char *msg);

///
void tkError_free(TkError *this);

///
Token *tkError_tk(TkError *this);

///
char *tkError_msg(TkError *this);

/*--*/

/// An id is a non empty sequence of characters > ' ' and != " which does not start
/// with:
///   A number
///   One of: ' - ! @ [ ( {
int token_is_id (Token *tk);

/// Is a ' followed with an id.
int token_is_quoted_id (Token *tk);

///
int token_is_bool (Token *tk);

///
int token_is_byte (Token *tk);

///
int token_is_int (Token *tk);

///
int token_is_float (Token *tk);


#endif
