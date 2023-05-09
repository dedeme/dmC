// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/reader.h"
#include "kut/opt.h"
#include "modules.h"
#include "fileix.h"
#include "symix.h"
#include "reader/st_reader.h"
#include "reader/cdr/cdr.h"

Module *reader_read_main_block (Cdr *cdr) {
  Imports *imports = imports_new();
  Exports *exports = exports_new();
  Heap0 *heap0 = heap0_new();
  // <StatCode>
  Arr *stats = arr_new();

  for (;;) {
    StatCode *st_cd = st_reader_read(cdr);
    Stat *st = stat_code_stat(st_cd);
    if (stat_is_end(st)) break;

    if (stat_is_block_close(st))
      EXC_KUT(cdr_fail(cdr, "Unexpected '}'"));

    if (stat_is_import(st)) { // Adds symbol to imports
      // [<int>, <int>]
      Arr *ps = stat_get_import(st);
      int id = *((int *)arr_get(ps, 1));

      if (imports_get_fix(imports, id) != -1)
        EXC_KUT(cdr_fail_line(cdr, str_f(
          "Import '%s' already defined in imports", symix_get(id)
        ), stat_code_line(st_cd)));

      if (opt_get(heap0_get(heap0, id)))
        EXC_KUT(cdr_fail_line(cdr, str_f(
          "Import '%s' already defined in code, line %d.",
          symix_get(id), heap0_entry_nline(opt_get(heap0_get(heap0, id)))
        ), stat_code_line(st_cd)));

      char *mod_path = arr_get(ps, 0);
      int fix = fileix_add(cdr_get_file(cdr), mod_path);
      if (fix == -1)
        EXC_KUT(cdr_fail_line(
          cdr, str_f("Module '%s' not found", mod_path), stat_code_line(st_cd)
        ));
      imports_add(imports, id, fix);
      modules_add(fix);
      continue;
    }

    if (stat_is_assign(st)) { // Adds symbol to heap0
      // <Exp, Exp>
      Tp *ps = stat_get_assign(st);
      Exp *ex = tp_e1(ps);
      if (exp_is_sym(ex)) {
        int id = exp_get_sym(ex);

        if (imports_get_fix(imports, id) != -1)
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in imports", symix_get(id)
          ), stat_code_line(st_cd)));

        if (!heap0_add(heap0, id, stat_code_line(st_cd), tp_e2(ps)))
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in code (line %d)",
            symix_get(id), heap0_entry_nline(opt_get(heap0_get(heap0, id)))
          ), stat_code_line(st_cd)));

        if (
          !arr_empty(stats) &&
          stat_is_export(stat_code_stat(arr_peek(stats))) &&
          stat_code_line(arr_peek(stats)) == stat_code_line(st_cd) - 1
        )
          exports_add(exports, id);
      }
    }
    arr_push(stats, st_cd);
  }

    //--
    int filter (StatCode *st_cd) {
      return !stat_is_export(stat_code_stat(st_cd));
    }
  arr_filter_in(stats, (FPRED)filter);
  return module_new(imports, exports, heap0, stats);
}

// <StatCode>
Arr *reader_read_block (Cdr *cdr) {
  Arr *stats = arr_new();

  for (;;) {
    StatCode *st_cd = st_reader_read(cdr);
    Stat *st = stat_code_stat(st_cd);
    if (stat_is_end(st))
      EXC_KUT(cdr_fail(cdr, "Unexpected end of text"));
    if (stat_is_import(st))
      EXC_KUT(cdr_fail(cdr, "'import' out of main block"));
    if (stat_is_block_close(st))
      break;
    arr_push(stats, st_cd);
  }

    //--
    int filter (StatCode *st_cd) {
      return !stat_is_export(stat_code_stat(st_cd));
    }
  arr_filter_in(stats, (FPRED)filter);
  return stats;
}
