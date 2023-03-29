// Copyright 27-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include <signal.h>
#include "kut/sys.h"
#include "kut/path.h"
#include "fileix.h"
#include "modules.h"
#include "reader/reader.h"
#include "reader/cdr/cdr.h"
#include "checker/checker.h"
#include "runner/runner.h"
#include "runner/fail.h"
#include "DEFS.h"

// <char>
static Arr *args;

static char *help =
  "Usage: "
  "\n     kut -v"
  "\n       Shows Kut version."
  "\n     kut -c <file>"
  "\n       Tests syntaxis of <file>."
  "\n     kut <file> [args]"
  "\n       Executes <file> with opational arguments."
  "\n     Examples:"
  "\n       kut -v"
  "\n       kut -c myprg"
  "\n       kut myprg"
  "\n       kut myprg arg1 arg2"
;

static void shandler (int sig) {
  EXC_KUT("Arithmetic exception");
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

  int check = FALSE;
  char *p = argv[1];
  if (str_eq(p, "-v")) {
    puts("Kut version v2023.02");
    return 0;
  } else if (argc == 3 && str_eq(p, "-c")) {
    check = TRUE;
    p = argv[2];
  }

  sys_init();
  fileix_init();
  modules_init();
  fileix_set_root(path_parent(p));

  TRY {
    int fix = fileix_add(-1, path_base(p));
    if (fix < 0)
      EXC_KUT(str_f("Module '%s' not found", p));
    char *kut_code = fileix_read(fix);
    modules_add(fix);
    Module *mod = reader_read_main_block(cdr_new(fix, kut_code));
    modules_set(fix, mod);
    if (check){
      checker_run();
    } else {
      // <StatCode>
      Arr *stack = arr_new();
      // <Heap>
      Arr *heaps = arr_new();
      arr_push(heaps, module_get_heap(mod));

      Exp *rs = runner_run(
        stack, module_get_imports(mod), module_get_heap0(mod), heaps,
        module_get_code(mod)
      );
      if (exp_is_break(rs))
        EXC_KUT(fail_add_stack(
          "'break' without 'while' or 'for'", exp_get_break(rs)
        ));
      if (exp_is_continue(rs))
        EXC_KUT(fail_add_stack(
          "'continue' without 'while' or 'for'", exp_get_continue(rs)
        ));
    }
  } CATCH (e) {
    char *msg = exc_msg(e);
    msg = str_right(msg, str_index(msg, ": ") + 2);
    msg = str_replace(msg, "\n----", "\n");
    puts(msg);
  } _TRY
}


Arr *main_args(void) {
  return args;
}
