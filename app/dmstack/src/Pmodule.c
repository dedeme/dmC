// Copyright 29-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Pmodule.h"

typedef void (*pmodule_Fn) (Machine *m);

struct pmodule_Entry {
  Symbol id;
  pmodule_Fn fn;
};

Symbol pmoduleEntry_id (PmoduleEntry *e) {
  return e->id;
}

pmodule_Fn pmoduleEntry_fn (PmoduleEntry *e) {
  return e->fn;
}

Pmodule *pmodule_new (void) {
  return (Pmodule *)arr_new();
}

void pmodule_add (Pmodule *pm, Symbol id, pmodule_Fn fn) {
  struct pmodule_Entry *e = MALLOC(struct pmodule_Entry);
  e->id = id;
  e->fn = fn;
  arr_push((Arr *)pm, e);
}

/// Opt<pmodule_Fn>
Opt *pmodule_get (Pmodule *pm, Symbol id) {
  EACH(pm, struct pmodule_Entry, e) {
    if (symbol_eq(e->id, id)) return opt_new(e->fn);
  }_EACH
  return opt_empty();
}

Arr *pmodule_list (Pmodule *pm) {
  return (Arr *)pm;
}
