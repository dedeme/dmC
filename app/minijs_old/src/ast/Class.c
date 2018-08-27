// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "dmc/ct/Hchar.h"
#include "ct/Itype.h"
#include "ct/Ltype.h"
#include "ct/Otype.h"
#include "ct/Oclass.h"
#include "ct/Icatt.h"
#include "ct/Lcatt.h"
#include "ct/Lcsym.h"
#include "ct/Icsym.h"
#include "ct/Classes.h"
#include "ast/Class.h"
#include "ast/Type.h"
#include "ast/Catt.h"
#include "ast/Csym.h"
#include "lexer/Tx.h"
#include "io/Cpath.h"
#include "DEFS.h"

/*.
struct: Class
  id: char *: _string
  local: bool: _bool
  generics: Ltype *: __ltype - type
  imports: Hchar *: __hchar - str
  extends: Otype *: __otype - type
  implements: Ltype *: __ltype - type
  atts: Lcatt *: __lcatt - catt
  -
  -syms: Lcsym * = lcsym_new()
*/

/*.-.*/
struct class_Class {
  char *id;
  bool local;
  Ltype *generics;
  Hchar *imports;
  Otype *extends;
  Ltype *implements;
  Lcatt *atts;

  Lcsym *syms;
};

Class *class_new(
  char *id,
  bool local,
  Ltype *generics,
  Hchar *imports,
  Otype *extends,
  Ltype *implements,
  Lcatt *atts
) {
  Class *this = MALLOC(Class);
  XNULL(id)
  this->id = id;
  this->local = local;
  XNULL(generics)
  this->generics = generics;
  XNULL(imports)
  this->imports = imports;
  XNULL(extends)
  this->extends = extends;
  XNULL(implements)
  this->implements = implements;
  XNULL(atts)
  this->atts = atts;
  this->syms = lcsym_new();
  return this;
}

char *class_id(Class *this) {
  XNULL(this)
  return this->id;
}

bool class_local(Class *this) {
  XNULL(this)
  return this->local;
}

Ltype *class_generics(Class *this) {
  XNULL(this)
  return this->generics;
}

Hchar *class_imports(Class *this) {
  XNULL(this)
  return this->imports;
}

Otype *class_extends(Class *this) {
  XNULL(this)
  return this->extends;
}

Ltype *class_implements(Class *this) {
  XNULL(this)
  return this->implements;
}

Lcatt *class_atts(Class *this) {
  XNULL(this)
  return this->atts;
}

Ajson *class_to_json(Class *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_astring(serial, this->id);
  jarr_abool(serial, this->local);
  ajson_add(serial, json_warray(ltype_to_json(this->generics, type_to_json)));
  ajson_add(serial, json_warray(hchar_to_json(this->imports, str_to_json)));
  ajson_add(serial, json_warray(otype_to_json(this->extends, type_to_json)));
  ajson_add(serial, json_warray(ltype_to_json(this->implements, type_to_json)));
  ajson_add(serial, json_warray(lcatt_to_json(this->atts, catt_to_json)));
  return serial;
}

Class *class_from_json(Ajson *serial) {
  XNULL(serial)
  Class *this = MALLOC(Class);
  size_t i = 0;
  this->id = jarr_gstring(serial, i++);
  this->local = jarr_gbool(serial, i++);
  this->generics = ltype_from_json(json_rarray(ajson_get(serial, i++)), type_from_json);
  this->imports = hchar_from_json(json_rarray(ajson_get(serial, i++)), str_from_json);
  this->extends = otype_from_json(json_rarray(ajson_get(serial, i++)), type_from_json);
  this->implements = ltype_from_json(json_rarray(ajson_get(serial, i++)), type_from_json);
  this->atts = lcatt_from_json(json_rarray(ajson_get(serial, i++)), catt_from_json);
  return this;
}
/*.-.*/

Lcsym *class_syms(Class *this) {
  if (!lcsym_empty(this->syms)) {
    return this->syms;
  }

  char *tmp = "";
  Tx *tx = tx_new(this->id, tmp, tmp, 0, 0);
  Lcsym *ss = lcsym_new();
  Otype *oextends = this->extends;
  if (!otype_is_null(oextends)) {
    Type *extends = otype_value(oextends);
    if (type_t(extends) != type_DATA)
      THROW(exc_illegal_state_t)
        exc_illegal_state("'oextends' is not type 'type_DATA'")
      _THROW
    Cpath *path = cpath_from_id(tx, type_id(extends));
    Oclass *oc = classes_get(path);
    if (!oclass_is_null(oc)) {
      /**/bool filter(Csym *a) {
      /**/  return csym_is_public(a) && !csym_is_static(a);
      /**/}
      ss = lcsym_from_it(
        icsym_filter(lcsym_to_it(class_syms(oclass_value(oc))), filter)
      );
    }
  }

  Lcsym *nss = lcsym_new();
  EACHL(this->atts, Catt, a) {
    Csym *s = csym_new(
      catt_is_public(a),
      catt_is_static(a),
      this->id,
      catt_id(a),
      catt_type(a)
    );
    Lcsym *tmp_ss = lcsym_new();
    EACHL(ss, Csym, s0) {
      if (str_eq(csym_id(s), csym_id(s0))) {
        if (csym_is_static(s))
          TH(tx)
            "Static attribute '%s.%s' overwrite "
            "super instance attribute '%s.%s'",
            this->id, csym_id(s), csym_class_id(s0), csym_id(s0)
          _TH
        if (!type_eq(csym_type(s), csym_type(s0)))
          TH(tx)
            "Type of '%s.%s' is different of parent type '%s.%s'",
            this->id, csym_id(s), csym_class_id(s0), csym_id(s0)
          _TH
        break;
      }
      tmp_ss = lcsym_cons(tmp_ss, s0);
    }_EACH
    ss = tmp_ss;
    nss = lcsym_cons(nss, s);
  }_EACH

  this->syms = lcsym_cat(ss, nss);
  return ss;
}
