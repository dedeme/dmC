// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/reader.h"
#include "kut/opt.h"
#include "modules.h"
#include "fileix.h"
#include "symix.h"
#include "function.h"
#include "reader/st_reader.h"
#include "reader/cdr/cdr.h"

Module *reader_read_main_block (Cdr *cdr) {
  Imports *imports = imports_new();
  Exports *exports = exports_new();
  Heap0 *heap0 = heap0_new();
  Types *tps = types_new();
  EACH(arr_new_from(
      "arr", "b64", "bytes", "cryp",
      "dic", "iter", "js", "file",
      "math", "path", "regex", "str",
      "sys", "tcp", "thread", "time",
      NULL), char, md
    ) {
    types_add(tps, -1, symix_add(md));
  }_EACH
  // <StatCode>
  Arr *stats = arr_new();

  for (;;) {
    StatCode *st_cd = st_reader_read(tps, cdr);
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
      types_add(tps, -1, id);
      continue;
    }

    // Adds symbol to heap0.
    if (stat_is_indexed(st)) {
      int is_export = !arr_empty(stats) &&
        stat_is_export(stat_code_stat(arr_peek(stats))) &&
        stat_code_line(arr_peek(stats)) == stat_code_line(st_cd) - 1
      ;

      // <Exp>
      Arr *syms = stat_get_indexed(st);

      for (int i = 0; i < arr_size(syms); ++i) {
        Exp *left = arr_get(syms, i);
        Exp *right;
        if (i == 0) {
          int psize = arr_size(syms) - 1;
          int *pvalues = ATOMIC(psize * sizeof(int));
          // <Exp>
          Arr *avalues = arr_new_bf(psize);
          for (int j = 0; j < psize; ++j) {
            Exp *sym = arr_get(syms, j + 1);
            pvalues[j] = exp_get_sym(sym);
            arr_push(avalues, sym);
          }
          Iarr *pars = iarr_new(psize, pvalues);
          Stat *fstat = stat_return(exp_array(avalues));
          right = exp_function(function_new(
            pars,
            stat_code_new(
              stat_code_file_ix(st_cd),
              stat_code_line(st_cd),
              fstat
            )
          ));
        } else {
          right = exp_int(i - 1);
        }

        int id = exp_get_sym(left);

        if (imports_get_fix(imports, id) != -1)
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in imports", symix_get(id)
          ), stat_code_line(st_cd)));

        if (!heap0_add(heap0, id, stat_code_line(st_cd), right))
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in code (line %d)",
            symix_get(id), heap0_entry_nline(opt_get(heap0_get(heap0, id)))
          ), stat_code_line(st_cd)));

        if (is_export)
          exports_add(exports, id);

        arr_push(stats, stat_code_new(
          stat_code_file_ix(st_cd),
          stat_code_line(st_cd),
          stat_assign(left, right)
        ));
      }
      continue;
    }

    // Adds symbol to heap0.
    if (stat_is_arr_multi(st) || stat_is_dic_multi(st)) {
      int is_export = !arr_empty(stats) &&
        stat_is_export(stat_code_stat(arr_peek(stats))) &&
        stat_code_line(arr_peek(stats)) == stat_code_line(st_cd) - 1
      ;

      int is_arr = stat_is_arr_multi(st);
      Tp3 *v = is_arr ? stat_get_arr_multi(st) : stat_get_dic_multi(st);

      int new_sym = exp_get_sym(tp3_e1(v));
      // <Exp>
      Arr *syms = tp3_e2(v);
      Exp *exp = tp3_e3(v);

      if (is_arr && exp_is_array(exp) && arr_size(exp_get_array(exp)) == 0) {
        // <Exp>
        Arr *exp_ints = arr_new();
        for (int i = 0; i < arr_size(syms); ++i)
          arr_push(exp_ints, exp_int(i));
        exp = exp_array(exp_ints);
      } else if (!is_arr && exp_is_dic(exp) && map_size(exp_get_dic(exp)) == 0) {
        // <Exp>
        Map *exp_strs = map_new();
        EACH(syms, Exp, sym) {
          if (exp_is_empty(sym)) continue;
          char *s = symix_get(exp_get_sym(sym));
          map_put(exp_strs, s, exp_string(s));
        }_EACH
        exp = exp_dic(exp_strs);
      }

      heap0_add(heap0, new_sym, stat_code_line(st_cd), exp);
      if (is_export) exports_add(exports, new_sym);


      EACH(syms, Exp, sym) {
        if (exp_is_empty(sym)) continue;
        int id = exp_get_sym(sym);

        Exp *right = is_arr
          ? exp_int(_i)
          : exp_string(symix_get(id))
        ;
        Exp *exp = exp_sq(exp_sym(new_sym), right);

        if (imports_get_fix(imports, id) != -1)
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in imports", symix_get(id)
          ), stat_code_line(st_cd)));

        if (!heap0_add(heap0, id, stat_code_line(st_cd), exp))
          EXC_KUT(cdr_fail_line(cdr, str_f(
            "Symbol '%s' already defined in code (line %d)",
            symix_get(id), heap0_entry_nline(opt_get(heap0_get(heap0, id)))
          ), stat_code_line(st_cd)));

        if (is_export)
          exports_add(exports, id);

      }_EACH
    }

    // Adds symbol to heap0.
    if (stat_is_assign(st)) {
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
Arr *reader_read_block (Types *tps, Cdr *cdr) {
  // <StatCode>
  Arr *stats = arr_new();
  tps = types_new_block(tps);

  for (;;) {
    StatCode *st_cd = st_reader_read(tps, cdr);
    Stat *st = stat_code_stat(st_cd);
    if (stat_is_end(st))
      EXC_KUT(cdr_fail(cdr, "Unexpected end of text"));
    if (stat_is_import(st))
      EXC_KUT(cdr_fail(cdr, "'import' out of main block"));
    if (stat_is_export(st))
      continue;
    if (stat_is_block_close(st))
      break;

    arr_push(stats, st_cd);
  }

  return stats;
}
