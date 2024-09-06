// Copyright 22-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Typed type

#ifndef TYPED_TTYPE_H
  #define TYPED_TTYPE_H

#include "iarr.h"

/// Typed types:
/// b, i, f, s, o, I, F, S, D and 0.
/// Objects ('o') have a Tp<char, void> as value.
typedef enum ttype_Ttype {
    ttype_BOOL, ttype_INT, ttype_FLOAT, ttype_STRING, ttype_OBJECT,
    ttype_AINT, ttype_AFLOAT, ttype_ASTRING, ttype_DIC,
    ttype_NO_RETURN, ttype_ERROR
  } Ttype;

/// Returns type from symbol.
/// 'sym' can have the following values: b, i, f, s, o, I, F, S, D and 0.
Ttype ttype_from_sym (char sym);

///
char *ttype_to_str (Ttype t);

///
char *ttype_group_to_str (Iarr *types);

/// Returns the c repesentation of 't'
char *ttype_to_c (Ttype t);

/// Returns function type (when not return appears an empty string, for
/// example 'ii|' instead of 'ii|0'.
char *ttype_to_func (Iarr *tpars, Ttype tret);

#endif
