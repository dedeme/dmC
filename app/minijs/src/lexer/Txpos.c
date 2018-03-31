// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/Txpos.h"

/*.
-struct: Txpos
  cpath: Cpath *
  text: char *
  start: char *
  end: char *
  nline: size_t
  nchar: size_t
*/

/*.-.*/
struct txpos_Txpos {
  Cpath *cpath;
  char *text;
  char *start;
  char *end;
  size_t nline;
  size_t nchar;
};

Txpos *txpos_new(
  Cpath *cpath,
  char *text,
  char *start,
  char *end,
  size_t nline,
  size_t nchar
) {
  Txpos *this = MALLOC(Txpos);
  this->cpath = cpath;
  this->text = text;
  this->start = start;
  this->end = end;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

inline
Cpath *txpos_cpath(Txpos *this) {
  return this->cpath;
}

inline
char *txpos_text(Txpos *this) {
  return this->text;
}

inline
char *txpos_start(Txpos *this) {
  return this->start;
}

inline
char *txpos_end(Txpos *this) {
  return this->end;
}

inline
size_t txpos_nline(Txpos *this) {
  return this->nline;
}

inline
size_t txpos_nchar(Txpos *this) {
  return this->nchar;
}
/*.-.*/

inline
Txpos *txpos_move(Txpos *this, char *pos, size_t nline, size_t nchar) {
  return txpos_new(this->cpath, this->text, pos, this->end, nline, nchar);
}

void txpos_printf(Txpos *this, char *format, ...) {
  va_list args;
  va_start(args, format);
  char *r = str_vprintf(format, args);
  va_end(args);

  printf(
    "%s:%zu[%zu]: %s\n",
    cpath_fpath(this->cpath), this->nline, this->nchar, r
  );
}

inline
bool txpos_at_end(Txpos *this) {
  return this->start == this->end;
}

inline
bool txpos_eq(Txpos *this, Txpos *other) {
  return this->start == other->start;
}

inline
bool txpos_neq(Txpos *this, Txpos *other) {
  return this->start != other->start;
}

inline
Pos *txpos_pos(Txpos *this) {
  return pos_new(this->nline, this->nchar);
}
