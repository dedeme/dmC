// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Record to retrieve an result of reading a class skeleton.
/// <p>
/// Fields:
///   errors: Errors in reading
///   skeleton: class skeleton read.
#ifndef TYPES_SKELETONRESULT_H
  # define TYPES_SKELETONRESULT_H

#include "Fails.h"
#include "Skeleton.h"

/*.-.*/

///
typedef struct skeletonResult_SkeletonResult SkeletonResult;

///
SkeletonResult *skeletonResult_new(Fails *fails, Skeleton *skeleton);

///
Fails *skeletonResult_fails(SkeletonResult *this);

/*.-.*/

/// Constructor of errors is known to have fails.
SkeletonResult *skeletonResult_fail(Fails *fails);

/// Returns a skeleton if there is no error, otherwise throws an illegal_state
/// Exception.
Skeleton *skeletonResult_skeleton(SkeletonResult *this);

#endif

