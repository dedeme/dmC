// Copyright 06-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <string.h>
#include "dmc/ct/Lchar.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Hchar.h"
#include "dmc/ct/Ikv.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/file.h"
#include "dmc/path.h"
#include "dmc/DEFS.h"
#include "io/Cpath.h"
#include "ct/Asym.h"
#include "lexer/lex.h"
#include "lexer/Tx.h"
#include "lexer/rimports.h"
#include "DEFS.h"

static Tx *rsymbol(char **rt, Tx *tx) {
  Tx *r;

  if (tx_eq(tx, r = lex_cconst(tx, '<'))) {
    return tx;
  }
  tx = lex_blanks(r);

  if (tx_eq(tx, r = lex_id(rt, tx)))
    TH(tx) "Expected a identifier" _TH

  return r;
}

static Tx *rid_symbol(char **rt, Tx *tx) {
  Tx *r;
  char *fid;
  if (tx_eq(tx, r = lex_id(&fid, tx))) {
    return tx;
  }
  tx = lex_blanks(r);

  char *id;
  if (tx_eq(tx, r = rsymbol(&id, tx))) {
    id = fid;
  } else {
    tx = lex_blanks(r);
  }

  *rt = str_printf("%s-%s", fid, id);
  return tx;
}

static Tx *included_dir(Hchar **imports, Tx *tx, Asym *symbols, Cpath *path) {
  Lchar *ids;
  tx = lex_list((List **)&ids, tx, ')', (Tx *(*)(void **, Tx *))rid_symbol);

  if (lchar_empty(ids))
    TH(tx) "Files list can not be empty" _TH

  Hchar *is = hchar_new();
  EACHL(ids, char, fid) {
    Achar *f_id = str_csplit(fid, '-');
    char *f = achar_get(f_id, 0);
    char *id = achar_get(f_id, 1);

    lex_test_reserved(tx, id);
    asym_add_test(symbols, tx, id);

    char *dir = path_parent(cpath_path(path));
    Cpath *p = cpath_new(tx, *dir ? path_cat(dir, f, NULL) : f);
    is = hchar_put(is, id, cpath_id(p));
  }_EACH

  *imports = is;
  return tx;
}

static Tx *massive_dir(Hchar **imports, Tx *tx, Asym *symbols, Cpath *path) {
  Tx *r;

  Lchar *excludes = lchar_new();
  if (tx_neq(tx, r = lex_cconst(tx, '('))) {
    tx = lex_blanks(r);
    tx = lex_list((List **)&excludes, tx, ')', (Tx *(*)(void **, Tx *))lex_id);
  }

  Hchar *is = hchar_new();
  char *fpath = cpath_file(path);
  EACH(file_dir(cpath_parent(path)), char, f) {
    if (str_eq(f, fpath)) {
      continue;
    }
    if (strcmp(path_extension(f), ".mini")) {
      continue;
    }
    char *name = path_only_name(f);
    if (ichar_contains(lchar_to_it(excludes), name)) {
      continue;
    }

    lex_test_reserved(tx, name);
    asym_add_test(symbols, tx, name);

    char *dir = path_parent(cpath_path(path));
    Cpath *p = cpath_new(tx, *dir ? path_cat(dir, name, NULL) : name);
    is = hchar_put(is, name, cpath_id(p));
  }_EACH

  *imports = is;
  return tx;
}

static Tx *included_dir2(Hchar **imports, Tx *tx, Asym *symbols, char *dir) {
  Lchar *ids;
  tx = lex_list((List **)&ids, tx, ')', (Tx *(*)(void **, Tx *))rid_symbol);

  if (lchar_empty(ids))
    TH(tx) "Files list can not be empty" _TH

  Hchar *is = hchar_new();
  EACHL(ids, char, fid) {
    Achar *f_id = str_csplit(fid, '-');
    char *f = achar_get(f_id, 0);
    char *id = achar_get(f_id, 1);

    lex_test_reserved(tx, id);
    asym_add_test(symbols, tx, id);

    Cpath *p = cpath_new(tx, path_cat(dir, f, NULL));
    is = hchar_put(is, id, cpath_id(p));
  }_EACH

  *imports = is;
  return tx;
}

static Tx *massive_dir2(Hchar **imports, Tx *tx, Asym *symbols, char *dir) {
  Tx *r;

  char *adir = cpath_absolute(dir);
  if (!*adir)
    TH(tx) "'%s' is not a valid directory", dir _TH

  Lchar *excludes = lchar_new();
  if (tx_neq(tx, r = lex_cconst(tx, '('))) {
    tx = lex_blanks(r);
    tx = lex_list((List **)&excludes, tx, ')', (Tx *(*)(void **, Tx *))lex_id);
  }

  Hchar *is = hchar_new();
  EACH(file_dir(adir), char, f) {
    if (strcmp(path_extension(f), ".mini")) {
      continue;
    }
    char *name = path_only_name(f);
    if (ichar_contains(lchar_to_it(excludes), name)) {
      continue;
    }

    lex_test_reserved(tx, name);
    asym_add_test(symbols, tx, name);

    Cpath *p = cpath_new(tx, path_cat(dir, name, NULL));
    is = hchar_put(is, name, cpath_id(p));
  }_EACH

  *imports = is;
  return tx;
}

Tx *rimports(Hchar **imports, Tx *tx, Asym *symbols, Cpath *path) {
  Tx *r;

  if (tx_eq(tx, r = lex_const(tx, "_import"))) {
    return tx;
  }
  tx = lex_blanks(r);

  Hchar *bis = hchar_new();
  char *builds = "Arr;Bool;Byte;Char;Float;Int;Map;Str";
  EACH(str_csplit(builds, ';'), char, name) {
    asym_add_test(symbols, tx, name);
    bis = hchar_put(bis, name, name);
  }_EACH

  Hchar *is = hchar_new();
  for (;;) {
    if (!*(tx_start(tx)) || tx_neq(tx, r = lex_cconst(tx, '_'))) {
      if (list_empty((List *)is))
        TH(tx) "Expected at least one import" _TH
      *imports = (Hchar *)lchar_cat((Lchar *)bis, (Lchar *)is);
      return tx;
    }
    if (tx_neq(tx, r = lex_cconst(tx, '/')))
      TH(tx) "Imports must be relative paths" _TH

    Hchar *is_to_add = hchar_new();
    if (tx_neq(tx, r = lex_cconst(tx, '.'))) {
      tx = lex_blanks(r);

      if (tx_neq(tx, r = lex_cconst(tx, '('))) {
        tx = lex_blanks(r);
        tx = included_dir(&is_to_add, tx, symbols, path);
      } else if (tx_neq(tx, r = lex_cconst(tx, '*'))) {
        tx = lex_blanks(r);
        tx = massive_dir(&is_to_add, tx, symbols, path);
      } else
        TH(tx) "Expected '(' or '*'" _TH
    } else {
      Tx *start = tx;
      char *p;
      tx = lex_no_blanks(&p, tx);
      if (!*p)
        THROW(exc_illegal_state_t) exc_illegal_state("'p' is empty") _THROW
      if (p[strlen(p) - 1] == '/')
        TH(start) "Import '%s' finishes with '/'", p _TH
      tx = lex_blanks(tx);

      if (tx_neq(tx, r = lex_cconst(tx, '('))) {
        tx = lex_blanks(r);
        tx = included_dir2(&is_to_add, tx, symbols, p);
      } else if (tx_neq(tx, r = lex_cconst(tx, '*'))) {
        tx = lex_blanks(r);
        tx = massive_dir2(&is_to_add, tx, symbols, p);
      } else {
        Cpath *path = cpath_new(start, p);  // Test if p is a valid name.
        char *id = path_name(p);
        if (tx_neq(tx, r = rsymbol(&id, tx))) {
          tx = r;
        }

        lex_test_reserved(tx, id);
        asym_add_test(symbols, tx, id);

        is = hchar_put(is, id, cpath_id(path));
      }
    }

    is = hchar_from_it(ikv_cat(hchar_to_it(is), hchar_to_it(is_to_add)));
    tx = lex_blanks(tx);
  }
}
