// Copyright 29-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/SkeletonContext.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: SkeletonContext
  typeNames: Achar *
  entries: MskEntry *
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct skeletonContext_SkeletonContext {
  Achar *typeNames;
  MskEntry *entries;
};

SkeletonContext *skeletonContext_new(Achar *typeNames, MskEntry *entries) {
  SkeletonContext *this = MALLOC(SkeletonContext);
  XNULL(typeNames)
  this->typeNames = typeNames;
  XNULL(entries)
  this->entries = entries;
  return this;
}

Achar *skeletonContext_typeNames(SkeletonContext *this) {
  XNULL(this)
  return this->typeNames;
}

MskEntry *skeletonContext_entries(SkeletonContext *this) {
  XNULL(this)
  return this->entries;
}
/*.-.*/
