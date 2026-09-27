// Copyright 11-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "exports.h"

struct exports_Exports {
  int *data;
  int len;
  int buf;
};

Exports *exports_new (void){
  Exports *this = MALLOC(Exports);
  int BUF = 25;
  this->data = ATOMIC(BUF * sizeof(int));
  this->len = 0;
  this->buf = BUF;
  return this;
}

void exports_add(Exports *this, int symbol) {
  int ix = this->len;
  if (ix < this->buf) {
    (this->data)[ix] = symbol;
    this->len = ix + 1;
    return;
  }

  int buf = this->buf + this->buf;
  this->data = GC_REALLOC(this->data, buf * sizeof(int));
  (this->data)[ix] = symbol;
  this->len = ix + 1;
  this->buf = buf;
}

int exports_contains (Exports *this, int symbol) {
  int *p = this->data;
  int *end = this->data + this->len;
  while (p < end)
    if (*p++ == symbol) return TRUE;
  return FALSE;
}
