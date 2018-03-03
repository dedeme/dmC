// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "RW.h"
#include "dmc/all.h"

struct RW {
  char *fmain;
  LckFile *main;
  LckFile *tmp;
  size_t n;
  char *current;
  char *next;
  Rcode *rc;
};

RW *rw_new(char *main, char *tmp) {
  RW *this = MALLOC(RW);
  this->fmain = path_name(main);
  this->main = file_ropen(main);
  this->tmp = file_wopen(tmp);
  this->n = 0;
  this->current = "";
  this->next = file_read_line(this->main);
  this->rc = NULL;
  return this;
}

void rw_close(RW *this) {
  file_close(this->main);
  file_close(this->tmp);
}

static char *next(RW *this) {
  char *r = this->current = this->next;
  if (*r) {
    ++this->n;
    this->next = file_read_line(this->main);
  }
  return r;
}

void rw_read_end(RW *this) {
  char *l = next(this);
  while (*l) {
    file_write_text(this->tmp, l);
    l = next(this);
  }
}

bool rw_read_until(RW *this, char *end) {
  char *l = next(this);
  while (strcmp(str_trim(l), end)) {
    if (!*l)
      return false;
    file_write_text(this->tmp, l);
    l = next(this);
  }
  file_write_text(this->tmp, l);
  return true;
}

bool rw_read_blank(RW *this, char *end) {
  char *l = next(this);
  while (strcmp(str_trim(l), end)) {
    if (!*l)
      return false;
    l = next(this);
  }
  file_write_text(this->tmp, l);
  return true;
}

void rw_read(RW *this) {
  char *l = NULL;
  char *lt = NULL;
  for (;;) {
    l = next(this);
    if (!*l)
      THROW "Reached end of file" _THROW
    file_write_text(this->tmp, l);
    lt = str_trim(l);
    if (*lt) break;
  }

  Rcode *r = MALLOC(Rcode);
  r->indent = *l <= ' ';
  r->first = *lt;
  r->l = lt;
  r->l1 = lt + 1;

  this->rc = r;
}

inline
Rcode *rw_rc(RW *this) {
  return this->rc;
}

inline
char *rw_peek(RW *this) {
  return this->next;
}

inline
void rw_write(RW *this, char *tx) {
  file_write_text(this->tmp, tx);
}

void rw_writeln(RW *this, char *tx) {
  file_write_text(this->tmp, str_printf("%s\n", tx));
}

inline
char *rw_msg(RW *this, char *m) {
  return str_printf(
    "%s:%d: %s\n%s",
    this->fmain, this->n, this->current, m
  );
}

Tp/*char, char*/ *rw_split(RW *this, char *l, char separator) {
  int ix = str_cindex(l, separator);
  if (ix == -1)
    THROW rw_msg(
      this, str_printf("'%c' is missing", separator)
    ) _THROW

  Tp *tp = tp_new(
    str_trim(str_sub(l, 0, ix)),
    str_trim(str_sub_end(l, ix + 1))
  );
  if (!*((char *)tp->e1))
    THROW rw_msg(
      this, str_printf("Left side of '%c' is empty", separator)
    ) _THROW
  if (!*((char *)tp->e2))
    THROW rw_msg(
      this, str_printf("Right side of '%c' is empty", separator)
    )_THROW

  return tp;
}

