// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef READER_H
  #define READER_H

#include "dmc/async.h"
#include "dmc/List.h"
#include "Token.h"

///
typedef struct reader_Reader Reader;

/// Creates a new stack machine
///   source : Identifier (File without extension or "").
///   prg    : Program to process.
Reader *reader_new (char *source, char *prg);

/// Creates a new stack machine
///   parent : Reader parent.
///   prg    : Program to process.
///   nline  : Current line number.
Reader *reader_new_from_reader (Reader *parent, char *prg, int nline);

/// Returns a token type token_LIST.
Token *reader_process (Reader *this);

///
int reader_is_file (Reader *is_file);

///
char *reader_source (Reader *this);

///
char *reader_prg (Reader *this);

///
int reader_prg_ix (Reader *this);

///
void reader_set_prg_ix (Reader *this, int value);

/// Used to pass an extra Token.
/// Its return can be NULL.
Token *reader_next_tk (Reader *this);

///
void reader_set_next_tk (Reader *this, Token *tk);

///
int reader_nline (Reader *this);

///
void reader_set_nline (Reader *this, int value);

/// Calculates symbol id of 'tk'. 'tk' is a token_SYMBOL.
///   this: The reader.
///   prg : Arr<Token> symbols read until now.
///   tk  : Token type token_SYMBOL.
Token *reader_symbol_id (Reader *this, Arr *prg, Token *tk);

/// Reads an interpolation.
///   this: The reader
///   tk  : Token of type token_STRING
///   return: Arr<Token> Tk expanded.
Arr *reader_interpolation (Reader *this, Token *tk);

///
void reader_fail (Reader *this, char *msg);

#endif
