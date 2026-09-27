// Copyright 11-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module index entry.

#ifndef DATA_INDEX_ENTRY_H
  #define DATA_INDEX_ENTRY_H

/// Record structure.
typedef struct {
  char *path; // Canonical path.
  char *id;    // Unique module identifier.
} IndexEntry;

/// Constructor.
///   path: Canonical path
///   id  : Unique identifier.
IndexEntry *indexEntry_new (char *path, char *id);

///
char *indexEntry_to_js (IndexEntry *this);

///
IndexEntry *indexEntry_from_js (char *js);

#endif
