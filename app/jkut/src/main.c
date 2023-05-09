// Copyright 15-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "kut/DEFS.h"
#include "kut/file.h"
#include "kut/path.h"
#include "kut/sys.h"
#include "kut/js.h"
#include "fileix.h"
#include "modules.h"
#include "reader/cdr/cdr.h"
#include "reader/reader.h"
#include "checker/checker.h"
#include "builtin/builtin.h"

typedef struct main_PathFix {
  int fix;
} PathFix;
PathFix *new_path_fix(int fix) {
  PathFix *this = MALLOC(PathFix);
  this->fix = fix;
  return this;
}

static char *help =
  "Usage: "
  "\n     jkut -v"
  "\n       Shows JKut version."
  "\n     jkut -c <source>"
  "\n       Check syntaxis and variables of <source-file>"
  "\n     jkut <source> <target>"
  "\n       Transpiles <source-file> to <target-js-directory>."
  "\n     Examples:"
  "\n       jkut -v"
  "\n       jkut -c www/myprg/jskut/main"
  "\n       jkut www/myprg/jskut/main www/myprg"
  "\n         (Transpiles www/myprg/jskut/main.kut to www/myprg/js/main.js)"
;

int main(int argc, char *argv[]) {
  sys_init();

  if (argc < 2) {
    puts(help);
    return 0;
  }

  if (argc == 2 && str_eq(argv[1], "-v")) {
    puts("Kut version v2023.04");
    return 0;
  } else if (argc != 3) {
    puts(help);
    return 0;
  }

  int check = FALSE;
  char *source = argv[1];
  char *target = argv[2];

  if (!strcmp(source, "-c")) {
    check = TRUE;
    source = target;
  }

  char *md_path = path_base(source);

  char *fsource = str_f("%s.jkut", source);
  if (!file_exists(fsource)) {
    if (str_ends(md_path, ".jkut"))
      EXC_IO(str_f(
        "Module '%s' not found (modules can not have dots)", source
      ));
    EXC_IO(str_f("Module '%s' not found", source));
  }

  if (!check && !file_is_directory(target))
    EXC_IO(str_f("'%s' is not a directory", target));
  target = path_cat(target, "jkut", NULL);

  fileix_init(path_parent(source));
  modules_init();

  int fix = -1;
  fix = fileix_add(fix, md_path);
  if (fix < 0)
    EXC_IO(str_f("Fail adding main '%s' module", md_path));

  // ModFix
  Arr *waiting = arr_new();
  arr_push(waiting, new_path_fix(fix));
  // ModFix
  Arr *read = arr_new();

  TRY {
    while (!arr_empty(waiting)) {
      PathFix *md_fix = arr_pop(waiting);
      int fix = md_fix->fix;

      char *kut_code = fileix_read(fix);
      Module *mod = reader_read_main_block(cdr_new(fix, kut_code));
      modules_add(fix, mod);

      arr_push(read, md_fix);

      EACH(imports_get_array(module_get_imports(mod)), ImportsEntry, e) {
        int fix = imports_entry_fix(e);
          //--
          int findex (PathFix *pf) { return pf->fix == fix; }
        if (arr_index(read, (FPRED)findex) == -1) {
          arr_push(waiting, new_path_fix(fix));
        }
      }_EACH
    }

    // omd is Opt<Module>
    EACH(modules_get_array(), Opt, omd) {
      Module *md = opt_get(omd);
      checker_run(_i, md);
    }_EACH

    if (!check) {
      file_del(target);

      char *builtin_tg = path_cat(target, "_js", NULL);
      builtin_generate(builtin_tg);

      // omd is Opt<Module>
      EACH(modules_get_array(), Opt, omd) {
        int fix = _i;
        char *rfile = fileix_relative(fix);
        rfile = str_f("%s%s", str_left(rfile, -4), "js");
        Module *md = opt_get(omd);
        char *tg = path_cat(target, rfile, NULL);
        file_mkdir(path_parent(tg));
        file_write(tg, module_get_js(md));
      }_EACH
    }
  } CATCH (e) {
    char *msg = exc_msg(e);
    msg = str_right(msg, str_index(msg, ": ") + 2);
    puts(msg);
  } _TRY

}
