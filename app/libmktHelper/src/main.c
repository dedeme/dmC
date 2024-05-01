// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "main.h"
#include "kut/DEFS.h"
#include "kut/sys.h"
#include "kut/js.h"
#include "kut/str.h"
#include "kut/buf.h"
#include "kut/file.h"
#include "cts.h"
#include "volumes.h"
#include "quotesReader.h"
#include "modelsHub.h"
#include "strategiesHub.h"
#include "stGroupsHub.h"

char *help (void) {
  return "Usage: kmarketSpeed [test|pathToJSON]";
}

int main(int argc, char *argv[]) {
  sys_init();

  if (argc != 2) {
    fprintf(stderr, "%s\n", help());
    return 0;
  }

  char *arg = argv[1];

  TRY{
    if (str_eq(arg, "constants")) {
      puts(cts_to_js());
      return 0;
    }
    Map *pars = js_ro(file_read(arg));
    char *command = js_rs(opt_eget(map_get(pars, "command")));
    if (str_eq(command, "readQuotes")) puts(quotesReader_run(pars));
    else if (str_eq(command, "volumes")) puts(volumes_read(pars));
    else if (str_eq(command, "mkRefs")) puts(modelsHub_run(pars));
    else if (str_eq(command, "strategy")) puts(strategiesHub_run(pars));
    else if (str_eq(command, "group")) puts(stGroupsHub_run(pars));
    else fprintf(stderr, "Unknown command %s\n", command);
  } CATCH (e) {
    char *msg = exc_msg(e);
    Buf *bf = buf_new();
    buf_add(bf, str_right(msg, str_index(msg, ": ") + 2));
    buf_cadd(bf, '\n');
    EACH(exc_stack(e), char, l)
      buf_add(bf, str_f("  %s\n", l));
    _EACH
    fprintf(stderr, "%s", buf_str(bf));
  }_TRY
}
