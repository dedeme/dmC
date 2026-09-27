// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Character reader.

#ifndef READERS_CHAR_RD_H
  #define READERS_CHAR_RD_H

/// Record structure.
typedef struct {
  char *tx; // Text to read.
  int i; // Index of current character in 'tx'
  int ln; // Line number
} CharRd;

/// Constructor.
///   tx : Text to read.
CharRd *charRd_new (char *tx);

/// Returns the current character of 'this' or 0 if chr is at end of text.
char charRd_peek (CharRd *this);

/// Returns the current character of 'this' or 0 if chr is at end of text,
/// and advances 'this->i' one position.
/// If the current character is '\n', 'this->ln' advances one line.
char charRd_next (CharRd *this);

#endif
