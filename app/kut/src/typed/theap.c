// Copyright 11-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/theap.h"

struct theap_Theap {
  TheapEntry **es;     // Start elements.
  TheapEntry **end;    //End elements. (Elements are between 'es' (inclusive)
                       //   and 'end' exclusive.
  TheapEntry **endbf; // End buffer.
};

static TheapEntry *newEntry (int sym, Ttype type, StatCode *st) {
  TheapEntry *this = MALLOC(TheapEntry);
  this->sym = sym;
  this->type = type;
  this->st = st;
  this->spare = TRUE;
  return this;
}

Theap *theap_new (void) {
  int buffer = 20;
  Theap *this = MALLOC(Theap);
  TheapEntry **es = GC_MALLOC(buffer * sizeof(void *));
  this->es = es;
  this->end = es;
  this->endbf = es + buffer;
  return this;
}

static void add (Theap *this, TheapEntry *e) {
  if (this->end == this->endbf) {
    int size = this->endbf - this->es;
    int new_size = size + size;
    this->es = GC_REALLOC(this->es, new_size * sizeof(Theap *));
    this->end = this->es + size;
    this->endbf = this->es + new_size;
  }
  *this->end++ = e;
}

int theap_add (Theap *this, int sym, Ttype type, StatCode *st) {
  TheapEntry **p = this->es;
  TheapEntry **p_end = this->end;

  while (p < p_end) {
    --p_end;
    TheapEntry *e = *p_end;
    if (!e) break;
    if (e->sym == sym) return FALSE;
  }

  add(this, newEntry(sym, type, st));
  return TRUE;
}

void theap_add_separator (Theap *this) {
  add(this, NULL);
}

// Returns Opt<TheapEntry>
Opt *theap_get (Theap *this, int sym) {
  TheapEntry **p = this->es;
  TheapEntry **p_end = this->end;

  while (p < p_end) {
    --p_end;
    TheapEntry *e = *p_end;
    if (e && e->sym == sym) {
      e->spare = FALSE;
      return opt_some(e);
    }

  }
  return opt_none();
}

void theap_remove_block (Theap *this) {
  TheapEntry **p = this->es;
  TheapEntry **p_end = this->end;

  while (p < p_end) {
    --p_end;
    TheapEntry *e = *p_end;
    if (!e) {
      this->end = p_end;
      return;
    }
  }

  EXC_KUT("'theap_remove_block' reaches the heap beginning");
};

Opt *theap_check_used(Theap *this) {
  TheapEntry **p = this->es;
  TheapEntry **p_end = this->end;

  while (p < p_end) {
    --p_end;
    TheapEntry *e = *p_end;
    if (!e) break;
    if (e->spare) return opt_some(e);
  }

  return opt_none();
}
