// Copyright 25-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Process starter.

#include "dmstack.h"
#include "dmc/rnd.h"
#include "args.h"
#include "imports.h"
#include "primitives.h"
#include "modules.h"
#include "Reader.h"
#include "Machine.h"
#include "fails.h"
#include "DEFS.h"

int main (int argc, char *argv[]) {
  exc_init();
  rnd_init();
  primitives_init();
  modules_init();
  imports_init();
  fails_init();

  if (args_init(argc, argv)) return(0);

  char *f = opt_get(args_param("dms"));
  if (!str_ends(f, ".dms")) f = str_cat(f, ".dms", NULL);
  char *fc = opt_nget(path_canonical(f));
  if (!fc)
    EXC_IO(str_f("File '%s' not found.", f));

  char *fid = str_left(fc, -4);
  Symbol *ssource = symbol_new_id(fid, f);
  imports_put_on_way(ssource);

  TRY {
    Reader *r = reader_new_from_file(fid, file_read(fc));
    machine_isolate_process(fc, list_new(), reader_process(r));
  } CATCH (e) {
    fails_from_exception(e);
  }_TRY

}
