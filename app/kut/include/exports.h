// Copyright 18-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Export symbols array.

#ifndef EXPORTS_H
  #define EXPORTS_H

///
typedef struct exports_Exports Exports;

/// Constructor.
Exports *exports_new (void);

/// Adds a new symbol.
void exports_add(Exports *this, int symbol);

/// Returns TRUE if 'this' contains 'symbol'.
int exports_contains (Exports *this, int symbol);

#endif
