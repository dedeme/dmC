// Copyright 29-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Primitives module.

#ifndef PMODULE_H
  #define PMODULE_H

#include "dmc/async.h"
#include "Machine.h"

///
typedef void (*pmodule_Fn) (Machine *m);

///
typedef struct pmodule_Entry PmoduleEntry;

///
Symbol pmoduleEntry_id (PmoduleEntry *e);

///
pmodule_Fn pmoduleEntry_fn (PmoduleEntry *e);

///
typedef struct pmodule_Pmodule Pmodule;

///
Pmodule *pmodule_new (void);

///
void pmodule_add (Pmodule *pm, Symbol id, pmodule_Fn fn);

/// Opt<pmodule_Fn>
Opt *pmodule_get (Pmodule *pm, Symbol id);

/// Arr<PmoduleEntry>
Arr *pmodule_list (Pmodule *pm);

#endif
