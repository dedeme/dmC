// Copyright 11-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Typed heap to read typed functions.

#ifndef TYPED_THEAP_H
  #define TYPED_THEAP_H

#include "kut/opt.h"
#include "stat.h"
#include "ttype.h"

/// Heap entry.
typedef struct theap_TheapEntry {
    int sym;
    Ttype type;
    StatCode *st;
    int spare; // 'true' if 'sym' was not used.
  } TheapEntry;

/// Heap structure.
typedef struct theap_Theap Theap;

/// Create an empty heap.
Theap *theap_new (void);

/// Adds a new entry to 'this' and returns TRUE.
/// If 'sym' is duplicated, it is not added and function returns FALSE.
int theap_add (Theap *this, int sym, Ttype type, StatCode *st);

/// Adds block separator
void theap_add_separator (Theap *this);

/// Search an entry with symbol 'sym' and returns Opt<TheapEntry> with
/// value 'opt_none' if it is not found.
Opt *theap_get (Theap *this, int sym);

/// Remove elements added in the last block.
void theap_remove_block (Theap *this);

/// Returns Opt<TheapEntry> with the first entry not used en the last block.
Opt *theap_check_used(Theap *this);

#endif
