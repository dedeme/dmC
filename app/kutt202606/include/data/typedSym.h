// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Typed symbol.

#ifndef DATA_TYPED_SYM_H
  #define DATA_TYPED_SYM_H

#include "kut/opt.h"
#include "data/type.h"

/// Structure.
typedef struct {
  int ln;    // Line number.
  char *id;  // Symbol name.
  Type *tp;  // Symbol type.
  Opt *cval; // (Opt<char>) Literal C value for types b, i, f and s.
  int used;  // TRUE if the symbol was used.
} TypedSym;

/// Constructor.
///   ln  : Line number.
///   id  : Symbol name.
///   tp  : Symbol type.
///   cval: (Opt<char>) Literal C value for types b, i, f and s.
///   used: TRUE if the symbol was used.
TypedSym *typedSym_new (
  int ln, char *id, Type *tp, Opt *cval, int used
);

///
char *typedSym_to_js (TypedSym *this);

///
TypedSym *typedSym_from_js (char *js);


#endif
