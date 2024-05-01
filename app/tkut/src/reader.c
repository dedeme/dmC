// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader.h"
#include "kut/file.h"
#include "read/module.h"
#include "read/block_reader.h"
#include "run/rn_code.h"

RnCode *reader_read_main_file (char *module_name) {
  module_clear_stock();
  Module *md = reader_read_file(module_name);
  return rn_code_new(module_name, module_stock_indexes(), module_stats(md));
}

Module *reader_read_file (char *module_name) {
  char *fpath = str_f("%s.tkut", module_name);
  if (!file_exists(fpath))
    EXC_KUT(str_f("Module '%s' not found.", module_name));
  char *kt_code = file_read(fpath);

  Module *md = module_new(module_name);
  module_add_to_stock(md);
  // <Symbol>
  Arr *syms = arr_new();
  // <Stat>
  Arr *stats = block_reader_read_main(kt_code, module_name, syms);

  module_update(md, syms, stats);
  return md;
}

RnCode *reader_read (char *kt_code) {
  char *module_name = "<dynamic>";
  module_clear_stock();
  Module *md = module_new(module_name);
  // <Symbol>
  Arr *syms = arr_new();
  // <Stat>
  Arr *stats = block_reader_read_main(kt_code, module_name, syms);

  module_update(md, syms, stats);
  return rn_code_new(module_name, module_stock_indexes(), module_stats(md));
}
