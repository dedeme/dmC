// Copyright 20-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/reader.h"
#include "kut/opt.h"
#include "kut/buf.h"
#include "kut/js.h"
#include "modules.h"
#include "fileix.h"
#include "reader/st_reader.h"
#include "reader/cdr/cdr.h"
#include "builtin/builtin.h"

Module *reader_read_main_block (Cdr *cdr) {
  Imports *imports = imports_new();
  // char
  Arr *exports = arr_new();
  Heap0 *heap0 = heap0_new();
  // <StatCode>
  Arr *stats = arr_new();

  for (;;) {
    StatCode *st_cd = st_reader_read(cdr);
    Stat *st = stat_code_stat(st_cd);
    if (stat_is_end(st)) break;

    if (stat_is_block_close(st))
      EXC_GENERIC(cdr_fail(cdr, "Unexpected '}'"));

    if (stat_is_import(st)) { // Adds symbol to imports
      // [<char>, <char>]
      Arr *ps = stat_get_import(st);
      char *id = arr_get(ps, 1);

      if (imports_get_fix(imports, id) != -1)
        EXC_GENERIC(cdr_fail_line(cdr, str_f(
          "Import '%s' already defined in imports", id
        ), stat_code_line(st_cd)));

      if (opt_get(heap0_get(heap0, id)))
        EXC_GENERIC(cdr_fail_line(cdr, str_f(
          "Import '%s' already defined in code, line %d.",
          id, heap0_entry_nline(opt_get(heap0_get(heap0, id)))
        ), stat_code_line(st_cd)));

      char *cdr_path = fileix_to_root(cdr_get_file(cdr));
      char *mod_path = arr_get(ps, 0);
      int fix = fileix_add(cdr_get_file(cdr), mod_path);
      if (fix == -1)
        EXC_GENERIC(cdr_fail_line(
          cdr, str_f("Module '%s' not found", mod_path), stat_code_line(st_cd)
        ));

      char *rfile = fileix_relative(fix);
      rfile = str_f("%s%s", str_left(rfile, -4), "js");
      char *js_path = str_f("%s%s", cdr_path, rfile);
      stat_set_js(st, str_replace(stat_get_js(st), "##", js_ws(js_path)));
      imports_add(imports, id, fix);
    } else if (stat_is_assign(st)) { // Adds symbol to heap0
      // <Exp, Exp>
      Tp *ps = stat_get_assign(st);
      Exp *ex = tp_e1(ps);
      if (exp_is_sym(ex)) {
        char *id = exp_get_sym(ex);

        if (imports_get_fix(imports, id) != -1)
          EXC_GENERIC(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in imports", id
          ), stat_code_line(st_cd)));

        if (!heap0_add(heap0, id, stat_code_line(st_cd)))
          EXC_GENERIC(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in code (line %d)",
            id, heap0_entry_nline(opt_get(heap0_get(heap0, id)))
          ), stat_code_line(st_cd)));

        if (
          !arr_empty(stats) &&
          stat_is_export(stat_code_stat(arr_peek(stats))) &&
          stat_code_line(arr_peek(stats)) == stat_code_line(st_cd) - 1
        ) {
          char *js = stat_get_js(st);
          js = str_f("export %s", js);
          stat_set_js(st, js);

          arr_push(exports, id);
        }
      }
    }

    arr_push(stats, st_cd);
  }

  Buf *js = buf_new();
  buf_add(js, builtin_module_head(fileix_to_root(cdr_get_file(cdr))));
  EACH(stats, StatCode, st_cd) {
    buf_add(js, stat_get_js(stat_code_stat(st_cd)));
  }_EACH
  buf_cadd(js, '\n');
  return module_new(imports, exports, heap0, stats, buf_str(js));
}

// <StatCode> (The last StatCode is "}" -- difference with kut)
Arr *reader_read_block (Cdr *cdr) {
  Arr *stats = arr_new();

  for (;;) {
    StatCode *st_cd = st_reader_read(cdr);
    Stat *st = stat_code_stat(st_cd);
    if (stat_is_end(st))
      EXC_GENERIC(cdr_fail(cdr, "Unexpected end of text"));
    if (stat_is_import(st))
      EXC_GENERIC(cdr_fail(cdr, "'import' out of main block"));
    if (stat_is_block_close(st)) {
      arr_push(stats, st_cd); // Line not in kut
      break;
    }
    arr_push(stats, st_cd);
  }

  return stats;
}
