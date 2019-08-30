// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Reader.h"
#include "DEFS.h"
#include "tkreader.h"

struct reader_Reader {
  char *source;
  int nline;
  char *prg;
  int prg_ix;
};

Reader *reader_new (char *source, char *prg, int nline, int prg_ix) {
  Reader *this = MALLOC(Reader);
  this->source = source;
  this->nline = nline;
  this->prg = prg;
  this->prg_ix = prg_ix;
  return this;
}

Token *reader_process (Reader *this) {
  // Arr<Token>
  Arr *r = arr_new();
  Token *tk = opt_nget(tkreader_next(this));
  while (tk) {
    arr_push(r, tk);
    tk = opt_nget(tkreader_next(this));
  }
  return token_new_list(r);
}

char *reader_source (Reader *this) {
  return this->source;
}

char *reader_prg (Reader *this) {
  return this->prg;
}

int reader_prg_ix (Reader *this) {
  return this->prg_ix;
}

void reader_set_prg_ix (Reader *this, int value) {
  this->prg_ix = value;
}

int reader_nline (Reader *this) {
  return this->nline;
}

void reader_set_nline (Reader *this, int value) {
  this->nline = value;
}

void reader_fail (Reader *this, char *msg) {
  THROW(ST_EXC) str_f("%s:%d: %s", this->source, this->nline, msg) _THROW
}

void reader_fail_resend (Reader *this, char *msg) {
  THROW(ST_EXC) str_f("%s: from %s:%d", msg, this->source, this->nline) _THROW
}
