// Copyright 24-Feb-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable list with type definitions.

#ifndef READER_TYPES_H
  #define READER_TYPES_H


///
typedef struct types_Types Types;

/// Constructor with an empty list.
Types *types_new (void);

/// Add a new block.
Types *types_new_block (Types *this);

/// Adds a pair symbol - module at the beginning.
/// When a module is imported a pair '-1, module' is added.
/// If module == -1, symbol has not module (is not typed).
void types_add (Types *this, int symbol, int module);

/// Returns TRUE if 'sym' has been typed in the current block.
int types_in_block (Types *this, int sym);

/// Returns the module of 'sym' or -1 if 'sym' is not typed.
int types_find (Types *this, int sym);

/// Returns TRUE if 'this' contains the module 'module'.
int types_contains(Types *this, int module);

#endif