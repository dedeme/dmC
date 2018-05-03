// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rimport.h"
#include "lexer/token.h"
#include "Mchar.h"
#include "Cpath.h"
#include "DEFS.h"


Tx *rimport(Tx *tx, Class *c) {
  Mchar *ims = class_imports(c);
  char *path = NULL;
  Cpath *cpath = NULL;
  char *id = NULL;
  Tx *tx_cpath = NULL;
  Tx *tx_id = NULL;
  // -------------------------------------------------------
  void add_import() {
    if (!id)
      THROW exc_null_pointer("id") _THROW
    if (!cpath)
      THROW exc_null_pointer("cpath") _THROW
    if (!tx_id)
      THROW exc_null_pointer("tx_id") _THROW
    if (!tx_cpath)
      THROW exc_null_pointer("tx_cpath") _THROW

    if (mchar_contains(ims, id)) {
      char *new = cpath_id(cpath);
      char *old = mchar_get(ims, id);
      if (!strcmp(new, old)) {
        return;
      }
      TH(tx_id)
        "'%s' can not point to '%s' because already points to '%s'",
        id, new, old
      _TH
    }
    if (class_contains_id(c, id))
      TH(tx_id) "Identifier '%s' is duplicated", id _TH
    if (!file_exists(cpath_file(cpath)))
      TH(tx_cpath) "'%s' not found", cpath_file(cpath) _TH

    mchar_put(ims, id, cpath_id(cpath));
  }

  Tx *ras(void **null, Tx *tx) {
    if (!path)
      THROW exc_null_pointer("path") _THROW

    Tx *r;
    char *file;

    tx_cpath = tx;
    tx_id = tx;
    if (tx_eq(tx, r = token_id(&file, tx)))
      TH(tx) "Expected a identifier" _TH
    cpath = cpath_new(path_cat(path, file, NULL));
    tx = r;

    if (tx_eq(tx, r = token_cconst(tx, '<'))) {
      id = file;
    } else {
      tx = r;
      tx_id = tx;
      if (tx_eq(tx, r = token_id(&id, tx)))
        TH(tx) "Expected a identifier" _TH
      tx = r;
    }

    add_import();
    *null = id;
    return tx;
  }
  // -------------------------------------------------------

  Tx *r;
  path = "";

  tx_cpath = tx;
  tx_id = tx;
  if (tx_eq(tx, r = token_cconst(tx, '.'))) {
    if (tx_eq(tx, r = token_path(&path, tx))) {
      return tx;
    }
  }
  tx = r;

  if (tx_neq(tx, r = token_cconst(tx, '<'))) {
    tx = r;
    cpath = cpath_new(path);
    tx_id = tx;
    if (tx_eq(tx, r = token_id(&id, tx)))
      TH(tx) "Expected an identifier" _TH
    tx = r;

    add_import();
    return tx;
  }

  if (tx_neq(tx, r = token_cconst(tx, '('))) {
    tx = r;
    Achar *nulls;
    tx = token_list(&nulls, tx, ')', ras);
    if (!arr_size(nulls))
      TH(tx) "Expected an identifier" _TH
    return tx;
  }

  if (tx_neq(tx, r = token_cconst(tx, '*'))) {
    tx = r;

    Arr/*Cpath*/ *cpaths = cpath_dirs(path);
    if (!arr_size(cpaths))
      TH(tx_cpath)
        "'%s' is not a directory or does not contain .mini files", path
      _TH
    if (tx_neq(tx, r = token_cconst(tx, '('))) {
      tx = r;
      Achar *ids;
      tx = token_list(&ids, tx, ')', (Tx *(*)(void **, Tx *))token_id);

      Arr/*Cpath*/ *tmp = arr_new();
      bool ok;
      char *name;
      EACH(cpaths, Cpath, cp) {
        ok = true;
        name = path_name(cpath_path(cp));
        EACH(ids, char, id) {
          if (!strcmp(id, name)) {
            ok = false;
          }
        }_EACH
        if (ok) {
          arr_add(tmp, cp);
        }
      }_EACH
      cpaths = tmp;
    }

    EACH(cpaths, Cpath, cp) {
      id = path_name(cpath_path(cp));
      cpath = cp;
      add_import();
    }_EACH

    return tx;
  }

  if (!*path)
    TH(tx_id) "Expected '*', ':' or '('" _TH

  cpath = cpath_new(path);
  id = path_name(path);
  add_import();
  return tx;
}
