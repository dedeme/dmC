// Copyright 10-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Character functions.

#ifndef CHAR_H
  #define CHAR_H

#include "kut/rs.h"

/// Returns 'true' if 'ch' is a letter [a-zA-Z]
int char_is_letter (char ch);

/// Returns 'true' if 'ch' is a digit [0-9]
int char_is_digit (char ch);

/// Returns 'true' if 'ch' is a letter [a-zA-Z] or a digit.
int char_is_letter_or_digit (char ch);

/// Returns `true' if id is a valid kutt identifier.
///   s: String to check. If it is empty, the function returns 'false'.
int char_is_id (char *id);

/// Returns canonical form of kutt 'module.identifier' or error.
/// Sequences valid are: "module.identifier", ".identifier" and "identifier".
///   s: String to check. If it is empty, the function returns 'false'.
/// Returns Rs<char>
Rs *char_mk_dot_id (char *id);

#endif
