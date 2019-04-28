// Copyright 21-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef LINEREADER_H
  #define LINEREADER_H

#include "dmc/std.h"

///
typedef int (*lineReader_Nline)(void *reader);

///
typedef char *(*lineReader_Next_new)(void *reader);

///
typedef struct lineReader_LineReader LineReader;

/// LineReader interface
struct lineReader_LineReader {
  lineReader_Nline fnline;
  lineReader_Next_new fnext_new;
  FPROC ffree;
};

/// Current line number. Its value before call 'next_new()' is 0
int lineReader_line(LineReader *this);

/// Returns next line finished ever with '\n'.<p>
/// Callings after reading being finished return an empty string.
char *lineReader_next_new(LineReader *this);

///
void lineReader_free(LineReader *this);

#endif
