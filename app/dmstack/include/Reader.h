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
///   source : Program source (file name, *string* or other identifier)
///   prg    : Program to process.
///   nline  : Current line number.
Reader *reader_new (char *source, char *prg, int nline);

/// Creates a new stack machine
///   source : Program source (file name, *string* or other identifier)
///   prg    : Program to process.
Reader *reader_new_from_file (char *source, char *prg);

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

/// Used to pass an extra Tokebn.
/// Its return can be NULL.
Token *reader_next_tk (Reader *this);

///
void reader_set_next_tk (Reader *this, Token *tk);

///
int reader_nline (Reader *this);

///
void reader_set_nline (Reader *this, int value);

///
void reader_fail (Reader *this, char *msg);

#endif
