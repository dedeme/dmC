// Copyright 12-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Imports array.

#ifndef IMPORTS_H
  #define IMPORTS_H

#include "kut/arr.h"

///
typedef struct imports_ImportsEntry ImportsEntry;

///
char *imports_entry_symbol (ImportsEntry *this);

///
int imports_entry_fix (ImportsEntry *this);

///
typedef struct imports_Imports Imports;

/// Constructor.
Imports *imports_new (void);

/// Added a new imports entry.
///   this: Container.
///   symbol: Symbol of import.
///   fix: File index of import.
void imports_add(Imports *this, char *symbol, int fix);

/// Returns the file index of a symbol or -1 if symbol does not exit.
int imports_get_fix (Imports *this, char *symbol);

/// Returs an Arr<ImportsEntry> with the entries of 'this'.
Arr *imports_get_array (Imports *this);

#endif
