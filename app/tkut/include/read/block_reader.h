// Copyright 03-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statements block reader.

#ifndef READ_BLOCK_READER_H
  #define READ_BLOCK_READER_H

#include "kut/arr.h"
#include "read/sym_heap.h"

///
typedef struct read_block_return BlockReturn;

/// Reads a main statements block and returns an Arr<Stat> with them.
/// This function modifies the first layer of 'modules'.
///   kt_code: Code to read.
///   module_name: Module name (for debug).
///   syms: Arr<Symbol>. Initially empty, to be populated by the function.
Arr *block_reader_read_main (char *kt_code, char *module_name, Arr *syms);


/// Reads a secondary statements block.
/// This function modifies the last layer of SymHeap.
///   kt_code: Code to read.
///   module_name: Module name (for debug).
///   syms: Symbols heap. 'syms' is created with 'sym_heap_add(NewLayer)'
BlockReturn *block_reader_read (char *kt_code, char *module_name, SymHeap *syms);

#endif
