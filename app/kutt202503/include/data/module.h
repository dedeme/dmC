// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module data.

#ifndef DATA_MODULE_H
  #define DATA_MODULE_H

#include "kut/map.h"

/// Record structure.
typedef struct {
  int is_main;    // 'true' if the module is the main one.
  char *path;     // Canonical path.
  char *id;       // Identifier.
  Map *imports;   // (Map<Imp>) Imports dictionary {UserName::ImportData}.
  Arr *exports;   // (Arr<TypedSym>) Top symbols to export.
  Arr *top_syms;  // (Arr<TypedSym>) Top symbols. (Symbols available in all the module)
  Map *type_defs; // (Map<Tp<Type, Type>>) Type definitions {identifer::[newType, oldType]}
  Arr *code     ; // (Arr<Stat>) Statement array.
} Module;


/// Constructor.
///   is_main  : 'true' if the module is the main one.
///   path     : Canonical path.
///   id       : Identifier.
///   imports  : (Map<Imp>) Imports dictionary {UserName::ImportData}.
///   exports  : (Arr<TypedSym>) Top symbols to export.
///   top_syms : (Arr<TypedSym>) Top symbols. (Symbols available in all the module)
///   type_defs: (Map<Tp<Type, Type>>) Type definitions {identifer::[newType, oldType]}
///   code     : (Arr<Stat>) Statement array.
Module *module_new(
  int is_main, char *path, char *id, Map *imports, Arr *exports,
  Arr *top_syms, Map *type_defs, Arr *code
);

#endif
