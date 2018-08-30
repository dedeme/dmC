// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Class skeleton management
/// <p>
/// Fields:
///   entries: Hashtable with top entries.
#ifndef TYPES_SKELETON_H
  # define TYPES_SKELETON_H

#include "types/MskEntry.h"

/*.-.*/

#include "dmc/Json.h"

///
typedef struct skeleton_Skeleton Skeleton;

///
Skeleton *skeleton_new(MskEntry *entries);

///
MskEntry *skeleton_entries(Skeleton *this);

///
Json *skeleton_to_json(Skeleton *this);

///
Skeleton *skeleton_from_json(Json *s);

/*.-.*/

#endif
