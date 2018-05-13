// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/Tx.h"

/*.
-struct: Tx
  path: char *
  text: char *
  start: char *
  nline: size_t
  nchar: size_t
*/

/*.-.*/
struct tx_Tx {
  char *path;
  char *text;
  char *start;
  size_t nline;
  size_t nchar;
};

Tx *tx_new(
  char *path,
  char *text,
  char *start,
  size_t nline,
  size_t nchar
) {
  Tx *this = MALLOC(Tx);
  this->path = path;
  this->text = text;
  this->start = start;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

inline
char *tx_path(Tx *this) {
  return this->path;
}

inline
char *tx_text(Tx *this) {
  return this->text;
}

inline
char *tx_start(Tx *this) {
  return this->start;
}

inline
size_t tx_nline(Tx *this) {
  return this->nline;
}

inline
size_t tx_nchar(Tx *this) {
  return this->nchar;
}
/*.-.*/

inline
Tx *tx_move(Tx *this, char *pos, size_t nline, size_t nchar) {
  return tx_new(this->path, this->text, pos, nline, nchar);
}

void tx_printf(Tx *this, char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vprintf(format, args);
  va_end(args);

  printf(
    "%s:%zu[%zu]: %s\n",
    this->path, this->nline, this->nchar, r
  );
}

inline
bool tx_at_end(Tx *this) {
  return !*this->start;
}

inline
bool tx_eq(Tx *this, Tx *other) {
  return this->start == other->start;
}

inline
bool tx_neq(Tx *this, Tx *other) {
  return this->start != other->start;
}

inline
Pos *tx_pos(Tx *this) {
  return pos_new(this->nline, this->nchar);
}
