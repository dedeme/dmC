// Copyright 31-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "checker/cksym.h"
#include "kut/DEFS.h"

struct cksym_Cksym {
  char *id;
  int fix;
  int nline;
  int used;
};

Cksym *cksym_new (char *id, int fix, int nline) {
  Cksym *this = MALLOC(Cksym);
  this->id = id;
  this->fix = fix;
  this->nline = nline;
  this->used = FALSE;
  return this;
}

char *cksym_get_id (Cksym *this) {
  return this->id;
}

int cksym_get_fix (Cksym *this) {
  return this->fix;
}

int cksym_get_nline (Cksym *this) {
  return this->nline;
}

int cksym_is_used (Cksym *this) {
  return this->used;
}

void cksym_set_used (Cksym *this) {
  this->used = TRUE;
}
