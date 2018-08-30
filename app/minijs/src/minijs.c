// Copyright 03-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "minijs.h"
#include "stdio.h"
#include "dmc/sys.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/Opt.h"
#include "dmc/DEFS.h"
#include "types/UserData.h"
#include "types/Dep.h"
#include "types/Fails.h"
#include "arguments_reading.h"
#include "scan/transpilation.h"

int main (int argc, char **argv) {
  sys_init("minijs");

  Opt/*UserData*/ *oudata = arguments_reader_run(argc, argv);

  if (opt_is_null(oudata)) {
    return 0;
  }

  UserData *udata = opt_value(oudata);
  Fails *fails = transpilation_run(udata);

  if (afail_size(fails_errors(fails)) == 0) {
    if (afail_size(fails_warnings(fails))) {
      puts ("Transpilation Ok.");
    } else {
      puts ("Warnings:");
      EACH(fails_warnings(fails), Fail, w) {
        puts (fail_fmsg(w));
      }_EACH
    }
  } else {
    puts ("Errors:");
    EACH(fails_errors(fails), Fail, e) {
      puts (fail_fmsg(e));
    }_EACH
  }

  return 0;
}
