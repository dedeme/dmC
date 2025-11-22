// Copyright 07-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "main.h"

#include <signal.h>
#include "kut/sys.h"
#include "kut/path.h"
#include "fileix.h"
#include "symix.h"
#include "modules.h"
#include "heaps.h"
#include "reader/reader.h"
/*#include "checker/checker.h"
#include "checker/tpchecker.h"
#include "runner/runner.h"
#include "runner/fail.h"
#include "runner/stack.h"
#include "typed/genc.h"
*/
// <char>
static Arr *args;

static char *help =
  "Usage: "
  "\n     kut -v"
  "\n       Shows Kut version."
  "\n     kut -c <file>"
  "\n       Checks syntaxis of <file>."
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

static void sshandler (int sig) {
  EXC_KUT("Segmentation violation");
}

int main(int argc, char *argv[]) {
  GC_INIT();
  sys_init();

  signal (SIGFPE, shandler);
  signal (SIGSEGV, sshandler);

  if (argc < 2) {
    puts(help);
    return 0;
  }

  args = arr_new();
  RANGE0(i, argc) {
    arr_push(args, argv[i]);
  }_RANGE

  int check = FALSE;
  char *p = argv[1];
  if (str_eq(p, "-v")) {
    puts(str_f("Kut version %s", VERSION));
    return 0;
  } else if (argc == 3 && str_eq(p, "-c")) {
    check = TRUE;
    p = argv[2];
  }

  symix_init();
  fileix_init();
  fileix_set_root(path_parent(p));
  modules_init();

  TRY {
    int fix = fileix_add(-1, path_base(p));
    if (fix < 0) {
      if (str_ends(p, ".kut"))
        EXC_KUT(str_f("Module '%s' not found (modules can not have dots)", p));
      EXC_KUT(str_f("Module '%s' not found", p));
    }
    char *kut_code = fileix_read(fix);
    modules_add(fix);
    Module *mod = reader_read_main_block(cdr_new(fix, kut_code));
    modules_set(fix, mod);
    if (check){
      // READS EVERY MODULE
      // Module
      Arr *imp_mods = arr_new_from(mod, NULL);
      while (arr_size(imp_mods)) {
        Module *md = arr_pop(imp_mods);
        EACH(imports_get_array(module_get_imports(md)), ImportsEntry, sym_fix) {
          int fix2 = imports_entry_fix(sym_fix);
          Module *old_md = opt_get(modules_get_ok(fix2));
          if (!old_md) {
            char *kut_code = fileix_read(fix2);
            Module *new_md = reader_read_main_block(cdr_new(fix2, kut_code));
            modules_set(fix2, new_md);
            arr_push(imp_mods, new_md);
          }
        }_EACH
      }
/*
      // CHEKS MODULES
      // oomd is Opt<Opt<Module>>
      EACH(modules_get_array(), Opt, oomd) {
        Module *md = opt_get(opt_get(oomd));
        checker_run(_i, md);
      }_EACH

      // CHECH TYPES
      tpchecker_run(fix);
*/
    } else {/*
      Exp *rs = runner_run(
        TRUE,
        stack_new(),
        module_get_imports(mod), module_get_heap0(mod),
        heaps_new(module_get_heap(mod)),
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
    */}
  } CATCH (e) {
    char *msg = exc_msg(e);
    int ix = str_index(msg, ": ");
    msg = ix == -1 ? msg : str_right(msg, ix + 2);
    puts(msg);
  } _TRY
}

// <char>
Arr *main_args(void) {
  return args;
}

