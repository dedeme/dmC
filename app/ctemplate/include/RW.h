// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// File reader.<br>
/// Reader reads and writes from rd->main to rd->tmp skeeping blanks lines.

#ifndef READERS_RW_H
  #define READERS_RW_H

#include "Tuples.h"
#include "data.h"
#include "file.h"

///
struct Rcode {
  bool indent;
  char first;
  char *l; // Trimmed line
  char *l1; // Trimed sub(line, 1, end)
};

typedef struct Rcode Rcode;

///
typedef struct RW RW;

///
RW *rw_new(char *main, char *tmp);

///
void rw_close(RW *this);

/// Reads rest text from this->main to this->tmp
void rw_read_end(RW *this);

/// Reads text from this->main to this->tmp until trim(line) == 'end'
/// inclusive.<br>
/// If 'this' reachs the end of file returns false.
bool rw_read_until(RW *this, char *end);

/// Reads text from this->main without writing until trim(line) == 'end'
/// inclusive, but writing the last line.
/// If 'this' reachs the end of file returns false.
bool rw_read_blank(RW *this, char *end);

/// Reads a line from this->main to td->tmp and sets this->rc.<br>
/// If next line is empty, this->rc is NULL.
void rw_read(RW *this);

/// Returns this->rc. Its value can be NULL if the last line read was blank.
Rcode *rw_rc(RW *this);

/// Returns next line. It does not read a new line.
char *rw_peek(RW *this);

///
void rw_write(RW *this, char *tx);

///
void rw_writeln(RW *this, char *tx);

///
char *rw_msg(RW *this, char *m);

/// Utility to split lines in two trimezed parts
Tp/*char, char*/ *rw_split(RW *this, char *l, char separator);


#endif

