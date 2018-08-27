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
#include "types/Errors.h"
#include "arguments_reading.h"
#include "scan/transpilation.h"

int main (int argc, char **argv) {
  sys_init("minijs");

  Opt/*UserData*/ *oudata = arguments_reader_run(argc, argv);

  if (opt_is_null(oudata)) {
    return 0;
  }

  UserData *udata = opt_value(oudata);
  Errors *errors = transpilation_run(udata);

  if (lerror_empty(errors_errors(errors))) {
    if (lerror_empty(errors_warnings(errors))) {
      puts ("Transpilation Ok.");
    } else {
      puts ("Warnings:");
      EACHL(lerror_reverse(errors_warnings(errors)), Error, e) {
        puts (error_fmsg(e));
      }_EACH
    }
  } else {
    puts ("Errors:");
    EACHL(lerror_reverse(errors_errors(errors)), Error, e) {
      puts (error_fmsg(e));
    }_EACH
  }

  return 0;
}
