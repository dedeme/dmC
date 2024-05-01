// Copyright 01-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "main.h"
#include <signal.h>
#include "kut/sys.h"
#include "kut/path.h"
#include "kut/file.h"
#include "tests/all_tests.h"
#include "run/rn_code.h"
#include "reader.h"

// <char>
static Arr *args;

static char *help =
  "Usage: "
  "\n     tkut -v"
  "\n       Shows Kut version."
  "\n     tkut -c <file>"
  "\n       Compile <file>."
  "\n     tkut -c <file> <code>"
  "\n       Compile <file> in <code>."
  "\n     tkut <file> [args]"
  "\n       Compiles an executes <file> with opational arguments."
  "\n     tkut <file>"
  "\n       Compiles <file> to a .ctkut executable."
  "\n     Examples:"
  "\n       tkut -v"
  "\n       tkut -c myprg"
  "\n       tkut -c myprg bin/myprg"
  "\n       tkut myprg -> execute myprg.ctkut if exists"
  "\n                     Otherwise compiles and executes myprg.tkut"
  "\n       tkut myprg arg1 arg2"
;

static void shandler (int sig) {
  EXC_GENERIC("Arithmetic exception");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts(help);
    return 0;
  }

  signal (SIGFPE, shandler);

  args = arr_new();
  RANGE0(i, argc) {
    arr_push(args, argv[i]);
  }_RANGE

  int compile = FALSE;
  char *p = argv[1];
  char *c = NULL;
  if (str_eq(p, "-v")) {
    puts("Tkut version v2023.08");
    return 0;
  } else if (str_eq(p, "-t")) {
    all_tests_run();
    return 0;
  } else if (argc == 3 && str_eq(p, "-c")) {
    compile = TRUE;
    p = argv[2];
    c = p;
  } else if (argc == 4 && str_eq(p, "-c")) {
    compile = TRUE;
    p = argv[2];
    c = argv[3];
  }

  GC_INIT();
  sys_init();

  int is_rn_code = file_exists(str_f("%s.ctkut", p));

  TRY {
    RnCode *rn_code = is_rn_code && !compile
      ? rn_code_from_js(file_read(str_f("%s.ctkut", p)))
      : rn_code_solve_types(reader_read_main_file(p))
    ;

    if (compile) {
      file_write(
        str_f("%s.ctkut", c),
        rn_code_to_js(rn_code)
      );
    } else {
      rn_code_run(rn_code);
    }
  } CATCH (e) {
    char *msg = exc_msg(e);
    if (str_eq(exc_type(e), exc_kut_t))
      msg = str_right(msg, str_index(msg, ": ") + 2);
    puts(msg);
  } _TRY
}

// <char>
Arr *main_args(void) {
  return args;
}
