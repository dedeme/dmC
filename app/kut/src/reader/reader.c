// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "reader/reader.h"
#include "DEFS.h"
#include "kut/opt.h"
#include "modules.h"
#include "reader/st_reader.h"
#include "reader/cdr/cdr.h"

Module *reader_read_main_block (Cdr *cdr) {
  // <int>
  Map *imports = map_new();
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
      // [<int>, <char>]
      Arr *ps = stat_get_import(st);
      char *id = arr_get(ps, 1);

      if (map_has_key(imports, id))
        EXC_KUT(cdr_fail_line(cdr, str_f(
          "Import '%d' already defined in imports", id
        ), stat_code_line(st_cd)));

      if (map_has_key(heap0_get(heap0), id))
        EXC_KUT(cdr_fail_line(cdr, str_f(
          "Import '%d' already defined in code, line %d.",
          id, heap0_entry_nline(opt_get(map_get(heap0_get(heap0), id)))
        ), stat_code_line(st_cd)));

      int *fix = arr_get(ps, 0);
      map_put(imports, id, fix);
      modules_add(*fix);
      continue;
    }

    if (stat_is_assign(st)) { // Adds symbol to heap0
      // <Exp, Exp>
      Tp *ps = stat_get_assign(st);
      Exp *ex = tp_e1(ps);
      if (exp_is_sym(ex)) {
        char *id = exp_get_sym(ex);

        if (map_has_key(imports, id))
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in imports", id
          ), stat_code_line(st_cd)));

        if (!heap0_add(heap0, id, stat_code_line(st_cd), tp_e2(ps)))
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in code (line %d)",
            id, heap0_entry_nline(opt_get(map_get(heap0_get(heap0), id)))
          ), stat_code_line(st_cd)));
      }
    }

    arr_push(stats, st_cd);
  }

  return module_new(imports, heap0, stats);
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

  return stats;
}
