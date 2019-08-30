// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef READER_H
  #define READER_H

#include "dmc/std.h"
#include "dmc/List.h"
#include "Token.h"

///
typedef struct reader_Reader Reader;

/// Creates a new stack machine
///   source: Program source (file name, *string* or other identifier)
///   prg   : Program to process.
///   nline : Current process line.
///   prg_ix: Current char index of prg. Process will start in this char.
Reader *reader_new (char *source, char *prg, int nline, int prg_ix);

/// Returns a token type token_LIST.
Token *reader_process (Reader *this);

///
char *reader_source (Reader *this);

///
char *reader_prg (Reader *this);

///
int reader_prg_ix (Reader *this);

///
void reader_set_prg_ix (Reader *this, int value);

///
int reader_nline (Reader *this);

///
void reader_set_nline (Reader *this, int value);

///
void reader_fail (Reader *this, char *msg);

///
void reader_fail_resend (Reader *this, char *msg);

#endif
