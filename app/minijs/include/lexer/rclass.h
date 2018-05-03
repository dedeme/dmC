// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class reader.<p>
///
/// Classes have the following orderded parts, all of them optional:<p>
///
/// <b>Generics declaration</b>
///   &lt;[id1[, id2, ...]]>
/// <b>Local declaration</b>
///   _local
/// <b>Imports declarations</b>
///   _import
///   <a href="?*minijs@lexer/rimport">imports</a>
/// <b>Extend declaration</b>
///   _extend
///   id
/// <b>Public declaration</b>
///   _public + val and var declarations
/// <b>Private declaration</b>
///   _private + val and var declarations

#ifndef LEXER_RCLASS_H
  #define LEXER_RCLASS_H

#include "dmc/all.h"
#include "Cpath.h"
#include "ast/Class.h"

/// rclass reads a class, following the next schema:
///   * if    imported_get() has the class, returns it
///   * else  *if !exists cache or cache date is previous to file .mini date,
///                 reads file .mini, write cache, adds class to imported.get()
///                 and returns it.
///           *else read cache, adds class to imported.get() and returns it
/// Throws tx_exception.
Class *rclass(Cpath *path);

#endif
