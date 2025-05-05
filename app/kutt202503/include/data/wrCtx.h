// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Writer context.

#ifndef DATA_WR_CTX_H
  #define DATA_WR_CTX_H

#include "kut/map.h"
#include "data/dicLayers.h"
#include "data/module.h"
#include "data/exp.h"
#include "data/tp.h"

/// Record structure.
struct wrCtx_WrCtx {
  char *md_id;          // Module identifier.
  char *md_path;        // Canonical path of module.
  Map *type_defs;       // (Map<Tp<Type, Type>>) Current module type definitions
                        //   {identifer::[newType, oldType]}
  Map *imports;         // (Map<Imp>) Current module imports.
  Arr *top_syms;        // (Arr<TypeSym>Top symbols. (Symbols available in all
                        //   the module)
  DicLayers *sym_types; // DicLayers of Map<typedSym>::{symbol: TypedSymbol}.
};
/// Record structure
typedef struct wrCtx_WrCtx WrCtx;

/// Constructor.
///   mod: Current module.
WrCtx *wrCtx_new (Module *md);

/// Returns TRUE if 'e' is a symbol that is not a top one.
int wrCtx_is_inner_symbol (WrCtx *this, Exp *e);

/// Returns Opt<Tp<TypeSym, int> with:
///   - e1 -> The TypedSym correponding to 'sym'.
///   - e2 -> equals to TRUE if this is a top one or FALSE otherwise.
Opt *wrCtx_get_symbol (WrCtx *this, char *sym);

/// Returns a Rs<char> with the canonical name of a definition type (It is in
/// the form 'realModuleName.sym').
///   ctx: Writer context.
///   mod: Module code name. It can be "" if only the symbol was indicated.
///   sym: Defintion symbol.
Rs *wrCtx_canonical_def (WrCtx *this, char *mod, char *sym);

/// Returns the Rs<Type> corresponding to a definition type 't'.
/// If 't' is not a type definition returns the same 't'.
///   NOTE: Only makes one expasion (new defintion types resulting from the
///         expansion are no expanded).
/// It 't' is not found, it returns rs_error.
Rs *wrCtx_expand (WrCtx *this, Type *t);

#endif
