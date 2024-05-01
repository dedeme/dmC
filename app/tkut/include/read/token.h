// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token data (immtutable).

#ifndef READ_TOKEN_H
  #define READ_TOKEN_H


///
typedef enum token_type TokenT;
///
typedef struct read_token Token;

///
enum token_type {
  TK_EOF,
  TK_COMMENT,
  TK_DOC_COMMENT
};

///
struct read_token {
  TokenT type;
  char *tx;
  char *rest;
  int ln_start;
  int ch_start; // inclusive
  int ln_end;
  int ch_end; // exclusive
};

/// Constructor
Token *token_new (
  TokenT type, char *tx, char *rest,
  int ln_start, int ch_start, int ln_end, int ch_end
);

#endif
