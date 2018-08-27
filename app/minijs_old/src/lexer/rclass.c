// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <stddef.h>
#include "dmc/ct/Ochar.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Lchar.h"
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Hchar.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/Json.h"
#include "dmc/Tuples.h"
#include "dmc/DEFS.h"
#include "io/Cpath.h"
#include "io/io.h"
#include "ct/Classes.h"
#include "ct/Ltype.h"
#include "ct/Otype.h"
#include "ct/Lcatt.h"
#include "ct/Asym.h"
#include "ast/Class.h"
#include "ast/Type.h"
#include "lexer/Tx.h"
#include "lexer/lex.h"
#include "lexer/rclass.h"
#include "lexer/rimports.h"
#include "lexer/rcatts.h"
#include "lexer/rtype.h"
#include "DEFS.h"

static Tx *rlocal(bool *rt, Tx *tx) {
  Tx *r;

  if (tx_neq(tx, r = lex_const(tx, "_local"))) {
    tx = r;
    *rt = true;
  }
  return tx;
}

static Tx *rgenerics(Ltype **rt, Tx *tx, Asym *symbols) {
  Tx *rgen(Type **id, Tx *tx) {
    Type *t;
    Tx *r = rtype(&t, tx);
    if (type_t(t) != type_DATA)
      TH(tx) "Generics must be identifiers" _TH
    if (!ltype_empty(type_params(t)))
      TH(tx) "Generic %s is not a identifier", type_id(t) _TH
    char *i = type_id(t);
    lex_test_reserved(tx, i);
    asym_add_test(symbols, tx, i);
    *id = t;
    return r;
  }

  Tx *r;
  if (tx_neq(tx, r = lex_cconst(tx, '<'))) {
    tx = lex_blanks(r);

    Ltype *params = ltype_new();
    tx = lex_list((List **)&params, tx, '>', (Tx *(*)(void **, Tx *))rgen);
    if (ltype_empty(params))
      TH(tx) "Expected at least one parameter" _TH
    *rt = params;
  }
  return tx;
}

static Tx *rextends(Otype **rt, Tx *tx, Asym *symbols) {
  Tx *r;

  if (tx_eq(tx, r = lex_const(tx, "_extends"))) {
    *rt = otype_null();
    return tx;
  }
  tx = lex_blanks(r);

  Type *t;
  if (tx_eq(tx, r = rtype(&t, tx)))
    TH(tx) "Expected a type definition" _TH
  tx = r;

  if (type_t(t) != type_DATA)
    TH(tx) "Type function and type Any can not be extended" _TH
  char *test = type_test_symbols(t, symbols);
  if (*test)
    TH(tx) "Unknown type '%s'", test _TH

  *rt = otype_new(t);
  return tx;
}

static Tx *rimplements(Ltype **rt, Tx *tx, Asym *symbols) {
  Tx *rentry(Tp/*Tx, Type*/ **value, Tx *tx) {
    Type *t;
    Tx *r = rtype(&t, tx);
    *value = tp_new(tx, t);
    return r;
  }

  Tx *r;

  if (tx_eq(tx, r = lex_const(tx, "_implements"))) {
    return tx;
  }
  tx = lex_blanks(r);

  if (tx_eq(tx, r = lex_cconst(tx, '(')))
    TH(tx) "Expected '('" _TH
  tx = lex_blanks(r);

  List/*Tp[Tx, Type]*/ *entries;
  tx = lex_list((List **)&entries, tx, ')', (Tx *(*)(void **, Tx *))rentry);

  if (list_empty(entries))
    TH(tx) "Expected at least one type definition" _TH

  Ltype *ts = ltype_new();
  EACHL(entries, Tp, tp) {
    Tx *tx = tp_e1(tp);
    Type *t = tp_e2(tp);

    if (type_t(t) != type_DATA)
      TH(tx) "Type function and type Any can not be implemented" _TH
    char *test = type_test_symbols(t, symbols);
    if (*test)
      TH(tx) "Unknown type '%s'", test _TH

    ts = ltype_cons(ts, t);
  }_EACH

  *rt = ts;
  return tx;
}

static Class *read(Cpath *path, Tx *tx) {
  Tx *r;

  char *id = cpath_id(path);
  bool local = false;
  Ltype *generics = ltype_new();
  Hchar *imports = hchar_new();
  Otype *extends = otype_null();
  Ltype *implements = ltype_new();
  Lcatt *atts = lcatt_new();

  Asym *symbols = asym_new();

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rlocal(&local, tx))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rgenerics(&generics, tx, symbols))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rimports(&imports, tx, symbols, path))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rextends(&extends, tx, symbols))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rimplements(&implements, tx, symbols))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rcatts(&atts, tx, true, true, symbols))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (tx_neq(tx, r = rcatts(&atts, tx, false, true, symbols))) {
    tx = r;
  }

  tx = lex_blanks(tx);

  if (!lex_end(tx)) {
    TH(tx) "Unexpected characters at end of file" _TH
  }

  return class_new(
    id,
    local,
    generics,
    imports,
    extends,
    implements,
    atts
  );
}

void rclass(Cpath *path) {
  XNULL(path)
  if (!classes_contains(path)) {
    Ochar *serial = io_rclass_serial(path);
    if (ochar_is_null(serial)) {
      char *code = io_rclass_code(path);
      Tx *tx = tx_new(cpath_path(path), code, code, 1, 0);
      Class *c = read(path, tx);
      io_wclass(path, (char *)json_warray(class_to_json(c)));
      classes_add(c);
    } else {
      classes_add(class_from_json(
        json_rarray((Json *)ochar_value(serial))
      ));
    }
  }
}
