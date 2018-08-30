// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scan/class_transpilation.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "types/Scanner.h"
#include "types/SkeletonResult.h"
#include "io.h"
#include "scan/skeleton_reading.h"
#include "scan/class_reading.h"

JsClassResult *class_transpilation_run (Lchar *roots, Cpath *class) {
  Scanner *scanner = scanner_new(class, io_read_mini(class));
  SkeletonResult *r = skeleton_reading_run(roots, scanner);

  Fails *fails = skeletonResult_fails(r);
  if (!afail_size(fails_errors(fails))) {
    Skeleton *skeleton = skeletonResult_skeleton(r);
    JsClassResult *r = class_reading_run(skeleton, scanner);

    Fails *cfails = jsClassResult_fails(r);
    if (!afail_size(fails_errors(cfails))) {
      fails_add_warnings(fails, fails_warnings(cfails));
      return jsClassResult_new(fails, jsClassResult_class(r));
    }
    jsClassResult_error(cfails);
  }
  return jsClassResult_error(fails);
}
