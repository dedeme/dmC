// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <stddef.h>
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Ochar.h"
#include "dmc/ct/Hchar.h"
#include "dmc/Buf.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "parser/pclass.h"
#include "lexer/rclass.h"
#include "lexer/Tx.h"
#include "ast/Fclass.h"
#include "ast/Class.h"
#include "ast/Type.h"
#include "ct/Fclasses.h"
#include "ct/Classes.h"
#include "ct/Oclass.h"
#include "ct/Ofclass.h"
#include "ct/Otype.h"
#include "ct/Itype.h"
#include "ct/Ltype.h"
#include "ct/Lfclass.h"
#include "ct/Ifclass.h"
#include "io/Cpath.h"
#include "DEFS.h"

static void test_extends(Lfclass *l, Fclass *c) {
  l = lfclass_cons(l, c);
  Otype *ot = fclass_extends(c);
  if (otype_is_null(ot)) {
    return;
  }
  char *tmp = "";
  Tx *tx = tx_new(str_creplace(fclass_id(c), '_', '/'), tmp, tmp, 0, 0);
  Ochar *oid = hchar_get(fclass_imports(c), type_id(otype_value(ot)));
  if (ochar_is_null(oid))
    THROW(exc_illegal_state_t) exc_illegal_state("'oid' is null") _THROW
  char *id = ochar_value(oid);

  /**/bool contains(Fclass *fc) { return str_eq(fclass_id(fc), id); }
  if (ifclass_containsf(lfclass_to_it(l), contains)) {
    Buf *cycle = buf_new();
    buf_add(cycle, id);
    EACHL(l, Fclass, fc) {
      buf_add(cycle, " -> ");
      buf_add(cycle, fclass_id(fc));
    }_EACH
    Tx *tx = tx_new(str_creplace(id, '_', '/'), tmp, tmp, 0, 0);
    TH(tx) "Extends cycle:\n%s", buf_str(cycle) _TH
  }
  Cpath *path = cpath_from_id(tx, id);
  pclass(path);
  Ofclass *ofc = fclasses_get(path);
  if (ofclass_is_null(ofc))
    THROW(exc_illegal_state_t) exc_illegal_state("'ofc' is null") _THROW
  test_extends(l, ofclass_value(ofc));
}

static void test_implements(Fclass *c) {
  THROW("") "Unimplemented" _THROW
}

static void test_class(Fclass *c) {
  char *tmp = "";
  Tx *tx = tx_new(str_creplace(fclass_id(c), '_', '/'), tmp, tmp, 0, 0);
  Cpath *cmain_path = cpath_from_id(tx, fclass_id(c));

  /**/void test_id(char *id) {
  /**/  Cpath *p = cpath_from_id(tx, id);
  /**/  pclass(p);
  /**/  Fclass *fc = ofclass_value(fclasses_get(p));
  /**/  if (
  /**/    fclass_local(fc) &&
  /**/    str_cmp(cpath_parent(cmain_path), cpath_parent(p))
  /**/  )
  /**/    TH(tx)
  /**/      "Local class '%s' can not be imported from '%s'", id, fclass_id(c)
  /**/    _TH
  /**/}

  Ltype *generics = fclass_generics(c);
  Hchar *imports = fclass_imports(c);
  ichar_each(hchar_values(imports), test_id);

  /**/void test_type(char *source, Type *t) {
  /**/  enum Type_t t_t = type_t(t);
  /**/  if (t_t == type_UNKNOWN)
  /**/    THROW(exc_illegal_state_t)
  /**/      exc_illegal_state("t_t is type_UNKNOWN")
  /**/    _THROW
  /**/  if (t_t == type_ANY || t_t == type_VOID) {
  /**/    return;
  /**/  }
  /**/  if (t_t == type_FN) {
  /**/    EACHL(type_params(t), Type, sub_t) {
  /**/      test_type(source, sub_t);
  /**/    }_EACH
  /**/    return;
  /**/  }
  /**/  char *tid = type_id(t);
  /**/  /**/bool test_gen(Type *gt) { return str_eq(type_id(t), type_id(gt)); }
  /**/  if (itype_containsf(ltype_to_it(generics), test_gen)) {
  /**/    if (!ltype_empty(type_params(t)))
  /**/      TH(tx) "In %s: Generic '%s' with params", source, type_id(t) _TH
  /**/    return;
  /**/  }
  /**/  Ochar *oid = hchar_get(imports, tid);
  /**/  if (ochar_is_null(oid))
  /**/    TH(tx) "In %s: Unknown class '%s'", source, tid _TH
  /**/  char *id = ochar_value(oid);
  /**/  Ofclass *ofc = fclasses_get(cpath_from_id(tx, id));
  /**/  if (ofclass_is_null(ofc))
  /**/    THROW(exc_illegal_state_t) exc_illegal_state("'ofc' is null") _THROW
  /**/  Fclass *fc = ofclass_value(ofc);
  /**/  int ntparams = ltype_count(type_params(t));
  /**/  int ngenerics = ltype_count(fclass_generics(fc));
  /**/  if (ntparams != ngenerics)
  /**/    TH(tx)
  /**/      "In %s: '%s' called with %d parameters when it needs %d",
  /**/      source, tid, ntparams, ngenerics
  /**/    _TH
  /**/  EACHL(type_params(t), Type, sub_t) {
  /**/    test_type(source, sub_t);
  /**/  }_EACH
  /**/}

  Otype *ot = fclass_extends(c);
  if (!otype_is_null(ot)) {
    test_type("_extends", otype_value(ot));
  }

  EACHL(fclass_implements(c), Type, t) {
    test_type("_implements", t);
  }_EACH

  test_extends(lfclass_new(), c);

  test_implements(c);

}

void pclass(Cpath *path) {
  if (!fclasses_contains(path)) {
    Oclass *oc = classes_get(path);
    if (oclass_is_null(oc)) {
      rclass(path);
      oc = classes_get(path);
      if (oclass_is_null(oc))
        THROW(exc_illegal_state_t) exc_illegal_state("'oc' is null") _THROW
    }
    Class *c = oclass_value(oc);

    Fclass *fc = fclass_new(
      class_id(c),
      class_local(c),
      class_generics(c),
      class_imports(c),
      class_extends(c),
      class_implements(c)
    );
    fclasses_add(fc);

    test_class(fc);
  }
}
