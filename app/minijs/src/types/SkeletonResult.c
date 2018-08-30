// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/SkeletonResult.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: SkeletonResult
  fails: Fails *
  -skeleton: Skeleton *
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct skeletonResult_SkeletonResult {
  Fails *fails;
  Skeleton *skeleton;
};

SkeletonResult *skeletonResult_new(Fails *fails, Skeleton *skeleton) {
  SkeletonResult *this = MALLOC(SkeletonResult);
  XNULL(fails)
  this->fails = fails;
  XNULL(skeleton)
  this->skeleton = skeleton;
  return this;
}

Fails *skeletonResult_fails(SkeletonResult *this) {
  XNULL(this)
  return this->fails;
}
/*.-.*/

SkeletonResult *skeletonResult_fail(Fails *fails) {
  SkeletonResult *this = MALLOC(SkeletonResult);
  XNULL(fails)
  this->fails = fails;
  this->skeleton = NULL;
  return this;
}

Skeleton *skeletonResult_skeleton(SkeletonResult *this) {
  XNULL(this);
  if (!afail_size(fails_errors(this->fails))) {
    return this->skeleton;
  }
  THROW(exc_illegal_state_t) "The SkeletonResult contains errors" _THROW
  return NULL; // Unreachable statement.
}
