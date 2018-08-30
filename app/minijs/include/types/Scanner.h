// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Text scanner
/// <p>
/// Fields: Opaque.<p>
/// Scanner reads a text character by character.<p>
/// Every new character scanned returns a new scanner.
#ifndef TYPES_SCANNER_H
  # define TYPES_SCANNER_H

#include "Cpath.h"
#include "Fail.h"
#include "stdbool.h"

/*.-.*/

///
typedef struct scanner_Scanner Scanner;

///
Cpath *scanner_file(Scanner *this);

///
char *scanner_tx(Scanner *this);

///
size_t scanner_ix(Scanner *this);

///
int scanner_nline(Scanner *this);

///
int scanner_nchar(Scanner *this);

/*.-.*/

/// Creates a new Scanner.
Scanner *scanner_new(Cpath *file, char *text);

/// Creates a new scanner from a text with a dummy Cpath.
Scanner *scanner_new_text(char *text);

///
bool scanner_eq(Scanner *this, Scanner *other);

/// Returns a new Scanner advanced one character. At end of text, it returns
/// this.
Scanner *scanner_next(Scanner *this);

/// Returns the current character.
char scanner_char(Scanner *this);

/// Returns 'true' if all the text has been scanned.
bool scanner_is_end(Scanner *this);

/// Make a message in the position of 'this'.
Fail *scanner_mk_fail(Scanner *this, char *msg);

#endif

