// Copyright 12-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statement reader.<p>
///
/// <b>Template</b>
///   { stat* } | reserved | assign | fnValue
/// where
///   reserved :: val | var | native | break | continue | return | throw |
///               while | if | for | try | case
///   assign :: value op value
///   val :: type? val id ; | type? val id = value
///   var :: type? var id ; | type? var id = value
///   native :: native MARK text MARK
///   return :: return | return value
///   throw :: throw value
///   while :: while (value) block
///   if :: if (value) block elif* else?
///   for :: for (for_assign,*;value,*;stat*,) block
///   try :: try block catch (id) block finally?
///   case :: case value ccase+ dcase? |
///   block :: stat | { stat*}
///   elif :: elif (value) block
///   else :: else block
///   for_assign :: val | var | assign
///   finaly :: finaly block
///
/// <b>Examples</b>
///   {}
///   {a = b return z}
///   val x;
///   val x = 12
///   var x;
///   var x = 12
///   native NATIVE console.log(x) NATIVE
///   return;
///   return }
///   return x
///   throw ex
///   while (x) { ... }
///   if (x) { ... }
///   if (x) { ... } else { ... }
///   if (x) { ... } elif (y) { ... } elif (z) { ... } else { ... }
///   for(;;)
///   for(var i = 0; i < x; ++i) { ... }
///   for(val x = 34, var i = 0; i < x, i > z; ++i, print(i + x)) { ... }
///   try { ... } catch (ex) { ... }
///   try { ... } catch (ex) { ... } finally { ... }

#ifndef LEXER_RSTAT_H
  #define LEXER_RSTAT_H

typedef struct tx_Tx Tx;
typedef struct stat_Stat Stat;

/// rstat read a statement<p>
/// Throws tx_excepton.
Tx *rstat(Stat **st, Tx *tx);

#endif
