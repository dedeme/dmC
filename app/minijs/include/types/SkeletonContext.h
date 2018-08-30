// Copyright 29-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Context of skeleton context functions
/// <p>
/// Fields:
///   typeNames: Name of known types.
///   entries: Map<name, SkEntry>
#ifndef TYPES_SKELETONCONTEXT_H
  # define TYPES_SKELETONCONTEXT_H

#include "dmc/ct/Achar.h"
#include "MskEntry.h"

/*.-.*/

///
typedef struct skeletonContext_SkeletonContext SkeletonContext;

///
SkeletonContext *skeletonContext_new(Achar *typeNames, MskEntry *entries);

///
Achar *skeletonContext_typeNames(SkeletonContext *this);

///
MskEntry *skeletonContext_entries(SkeletonContext *this);

/*.-.*/


#endif

