// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Minimal scanner.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/tokenFunctions'>here</a>.

#ifndef SCAN_TOKEN_H
  # define SCAN_TOKEN_H

#include "dmc/Opt.h"
#include "types/ScanResult.h"

/// Read a char.
///   str: With one characer: The first character of str is used to test.
///        With two characters: Marks a range of characters, between the first
///          and the second one, both inclusive.
///   result: A char * whose first character is the character read.
///   fail: If the current char is not the first character of str or not is
///     in range
ScanResult *token_char(char *str, Scanner *sc);

/// Read Blank. It is "char <= ' '", "/* ... */" or "//...endLine".
///   none: Its value is ignored.
///   result: opt_none()
///   fail: Not blank or \0
ScanResult *token_blank(Opt *none, Scanner *sc);

/// Read Blank*
ScanResult *token_blanks(Opt *none, Scanner *sc);

/// Read Blank+
ScanResult *token_blank1(Opt *none, Scanner *sc);

/// Read a identifier
///   ['a-z' | 'A-Z' | '$' | '_'] [ 'a-z' | 'A-Z' | '$' | '_' | '0-9' ]*
ScanResult *token_id(Opt *none, Scanner *sc);

/// Read the end of text.
///   null: Its value is ignored.
///   result: An empty string.
///   fail: If the current caracter is not '\0'
ScanResult *token_end(Opt *none, Scanner *sc);


#endif
