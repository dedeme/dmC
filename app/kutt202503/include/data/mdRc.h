// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Module record..

#ifndef DATA_MD_RC_H
  #define DATA_MD_RC_H

#include "kut/map.h"

/// Record structure.
typedef struct {
  char *path;     // Canonical path.
  char *id;       // Identifier.
  Map *imports;   // (Map<Imp>) Imports dictionary {UserName::ImportData}.
  Arr *exports;   // (Arr<TypedSym>) Top symbols to export.
  Map *type_defs; // (Map<Tp<Type, Type>>) Type definitions {identifer::[newType, oldType]}
} MdRc;


/// Constructor.
///   path     : Canonical path.
///   id       : Identifier.
///   imports  : (Map<Imp>) Imports dictionary {UserName::ImportData}.
///   exports  : (Arr<TypedSym>) Top symbols to export.
///   type_defs: (Map<Tp<Type, Type>>) Type definitions {identifer::[newType, oldType]}
MdRc *mdRc_new(
  char *path, char *id, Map *imports, Arr *exports, Map *type_defs
);

///
char *mdRc_to_js (MdRc *this);

///
MdRc *mdRc_from_js (char *js);

#endif
