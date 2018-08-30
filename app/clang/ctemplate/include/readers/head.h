// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Strucuture head reader.<br>
/// Definition:
///   [-]struct: [-|@]Id
/// <tt>[-]struct</tt>
///   '-'     -> struct is not serializable
///   default -> serializable
/// <tt>[-|@]Id</tt>
///   '-' -> struct is public but new() not
///   '@' -> struct and new() are not public
///   default -> struct and new() are public

#ifndef READERS_HEAD_H
  #define READERS_HEAD_H

#include "data.h"
#include "RW.h"

/// Returns a Structure head.
Head *head_read(RW *rw);

#endif

