// Copyright 16-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// case statement reader.<p>
/// This operator translates conditions to a chain of if statements.
///
/// <b>Template</b>
///   value condition* default
/// where
///   condition :: \ value ,+ : block
///   default :: \ : block
/// <b>Examples</b>
///   case x
///   \ 0, y + 2 : return "a"
///   \ 1 : {
///     b = a + "b"
///     return b
///   }
///   \ : return "c"
/// is translated to
///   (x == 0 || x == y + 2) ? "a"
///   : x == 1 ? "b"
///   : "c"
/// NOTE: with x \ : 8 is directly translated to 8.

#ifndef LEXER_RCASE_H
  #define LEXER_RCASE_H

typedef struct tx_Tx Tx;
typedef struct stat_Stat Stat;
typedef struct pos_Pos Pos;

/// rcase read a case statement<p>
/// Throws tx_excepton.<p>
Tx *rcase(Stat **st, Tx *tx, Pos *pos);

#endif
