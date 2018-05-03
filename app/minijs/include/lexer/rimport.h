// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Import reader.<p>
///
/// <b>Template</b>
///   [path | .] [< id] |
///   [path | .] (name1 [< id1] [, name2 [< is2], ...]) |
///   [path | .] * [(name1 [, name2, ..])]
/// Paths can be duplicates but id's not. If a path is imported without '<'
/// its id is its last part (e.g. dm_It has 'It' as id).<p>
/// <b>Examples</b>
///   dm/It -> Imported as It
///   dm/It < Iterator -> Imported as Iterator
///   dm (It, Dictionary < Dic) -> Imported dm/It as It and dm/Dictionary as Dic
///   dm * -> Imported every class in dm
///   dm * (Dictionary) -> Imported every class in dm except dm/Dictionary
///   . * (Main) -> Imported every class in main directorys except "Main"

#ifndef LEXER_RIMPORT_H
  #define LEXER_RIMPORT_H

#include "dmc/all.h"
#include "ast/Class.h"
#include "Tx.h"

/// rimport reads one import.<br>
/// Throws tx_excepton
Tx *rimport(Tx *tx, Class *c);

#endif
