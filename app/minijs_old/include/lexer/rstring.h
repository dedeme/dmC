// Copyright 6-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// String reader.<p>
///
/// <b>Template</b>
///   'char'
///   "chars escaped"
///   """\nchars unescaped\n"""
///   `chars interpoled`
/// <b>Examples</b>
///   'ñ'
///   "I see a \"plane\" plane"
///   """
///     It is "timized" with 2
///     blanks
///   """
///   `3 + 2 = ${3 + 2}`
///   `x = $x`
///   `The sign of \"dolar\" is \$`
/// Note: Am user value can be used in interpolation if it has the method
/// .toStr()

#ifndef LEXER_RSTRING_H
  #define LEXER_RSTRING_H

#include "dmc/all.h"
#include "ast/Value.h"
#include "Tx.h"

/// rstring read a string in 'v'.<p>
/// Throws tx_excepton.<p>
Tx *rstring(Value **v, Tx *tx);

#endif
