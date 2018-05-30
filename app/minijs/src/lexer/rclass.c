// Copyright 1-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/rclass.h"
#include <sys/stat.h>
#include "ast/Imported.h"
#include "Cpath.h"
#include "DEFS.h"
#include "lexer/Tx.h"
#include "lexer/token.h"
#include "lexer/rimport.h"
#include "lexer/rtype.h"
#include "lexer/rattribute.h"

static Tx *rgenerics(Tx *tx, Class *c) {
  Achar *generics = class_generics(c);

  // -------------------------------------------------------
  Tx *valid_id(void **id, Tx *tx) {
    Tx *r;
    char *i;
    if (tx_eq(tx, r = token_valid_id(&i, tx))) {
      return tx;
    }
    if (type_reserved_id(i))
      TH(tx) "Identifier '%s' is a reserved type name", i _TH
    if (class_contains_id(c, i))
      TH(tx) "Identifier '%s' is duplicated", i _TH

    *id = i;
    achar_add(generics, i);
    return r;
  }
  // -------------------------------------------------------

  Tx *r;

  if (tx_eq(tx, r = token_cconst(tx, '<'))) {
    return tx;
  }
  tx = r;

  Achar *ids;
  tx = token_list((Arr **)&ids, tx, '>', valid_id);

  return tx;
}

static Tx *rlocal(Tx *tx, Class *c) {
  Tx *r;
  if (tx_neq(tx, r = token_directive(tx, "_local"))) {
    class_set_local(c, true);
    return r;
  }
  return tx;
}

static Tx *rimports(Tx *tx, Class *c) {
  Tx *r;
  if (tx_eq(tx, r = token_directive(tx, "_import"))) {
    return tx;
  }
  tx = r;

  while (tx_neq(tx, r = rimport(tx, c))) {
    tx = r;
  }
  return tx;
}

static Tx *rextend(Tx *tx, Class *c) {
  Tx *r;
  if (tx_eq(tx, r = token_directive(tx, "_extends"))) {
    return tx;
  }
  tx = r;

  Type *tp;
  r = rtype2(&tp, tx);
  if (type_type(tp) != DATA)
    TH(tx) "Expected a class id" _TH
  char *id = type_id(tp);
  if (token_is_reserved(id))
    TH(tx) "'%s' is a reserved word", id _TH
  if (!class_contains_type(c, id) && !type_reserved_id(id))
    TH(r) "Unknown symbol '%s'", id _TH
  EACH(type_params(tp), Type, t) {
    char *id = type_id(t);
    if (!class_contains_type(c, id) && !type_reserved_id(id))
      TH(r) "Unknown symbol '%s'", id _TH
  }_EACH
  tx = r;

  class_set_super(c, tp);

  return tx;
}

static Tx *rimplements(Tx *tx, Class *c) {
  Tx *r;
  if (tx_eq(tx, r = token_directive(tx, "_implements"))) {
    return tx;
  }
  tx = r;

  if (tx_eq(tx, r = token_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = r;

  Atype *imps;
  r = token_list(&imps, tx, ')', (Tx *(*)(void **, Tx *))rtype2);

  Atype *is = class_implements(c);
  EACH(imps, Type, tp) {
    if (type_type(tp) != DATA)
      TH(tx) "Expected class ids" _TH
    char *id = type_id(tp);
    if (token_is_reserved(id))
      TH(tx) "'%s' is a reserved word", id _TH
    if (!class_contains_type(c, id) && !type_reserved_id(id))
      TH(r) "Unknown symbol '%s'", id _TH
    EACH(type_params(tp), Type, t) {
      char *id = type_id(t);
      if (!class_contains_type(c, id) && !type_reserved_id(id))
        TH(r) "Unknown symbol '%s'", id _TH
    }_EACH

    atype_add(is, tp);
  }_EACH

  return r;
}

static Tx *rattributes(Tx *tx, Class *c) {
  Tx *r;

  for(;;) {
    if (tx_eq(tx, r = rattribute(tx, c, true))) {
      break;
    }
    tx = r;
  }

  if (tx_eq(tx, r = token_directive(tx, "_private"))) {
    return tx;
  }
  tx = r;

  for(;;) {
    if (tx_eq(tx, r = rattribute(tx, c, false))) {
      break;
    }
    tx = r;
  }

  if (tx_neq(tx, r = token_directive(tx, "_private")))
    TH(tx) "Duplicate directive '_private'" _TH

  return tx;
}

static Class *read(Cpath *path, char *text) {
  Class *c = class_new(cpath_id(path));

  Tx *tx = tx_new(cpath_fpath(path), text, text, 1, 0);
  tx = token_blanks(tx);

  tx = rgenerics(tx, c);
  tx = rlocal(tx, c);
  tx = rimports(tx, c);
  tx = rextend(tx, c);
  tx = rimplements(tx, c);
  tx = rattributes(tx, c);

  if (!tx_at_end(tx))
    TH(tx) "Unexpected declaration" _TH

  return c;
}

Class *rclass(Cpath *path) {
  Imported *im = imported_get();
  Class *c = imported__class(im, cpath_id(path));

  if (c) {
    return c;
  }

  char *cache = cpath_lib(path);
  char *mini = cpath_file(path);
  time_t tcache = 0;
  if (file_exists(cache)) {
    tcache = file_info(cache)->st_mtime;
  }
  time_t tmini = file_info(mini)->st_mtime;
  if (tcache > tmini) {
    c = class_restore(json_rarray(file_read(cache)));
  } else {
    char *tx = file_read(mini);
    c = read(path, tx);
    if (!file_exists(path_parent(cache))) {
      file_mkdir(path_parent(cache));
    }
    file_write(cache, json_warray(class_serialize(c)));
  }

  imported_add(im, c);

  return c;
}
