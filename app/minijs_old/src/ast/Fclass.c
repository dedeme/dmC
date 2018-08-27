// Copyright 17-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "ast/Fclass.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "dmc/ct/Hchar.h"
#include "ct/Itype.h"
#include "ct/Ltype.h"
#include "ct/Otype.h"
#include "ct/Icatt.h"
#include "ct/Lcatt.h"
#include "ast/Type.h"
#include "ast/Catt.h"

/*.
-struct: @Fclass
  id: char *
  local: bool
  generics: Ltype *
  imports: Hchar *
  extends: Otype *
  implements: Ltype *
  atts: Lcatt *
*/

/*.-.*/
struct fclass_Fclass {
  char *id;
  bool local;
  Ltype *generics;
  Hchar *imports;
  Otype *extends;
  Ltype *implements;
  Lcatt *atts;
};

Fclass *_fclass_new(
  char *id,
  bool local,
  Ltype *generics,
  Hchar *imports,
  Otype *extends,
  Ltype *implements,
  Lcatt *atts
) {
  Fclass *this = MALLOC(Fclass);
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
  return this;
}

char *fclass_id(Fclass *this) {
  XNULL(this)
  return this->id;
}

bool fclass_local(Fclass *this) {
  XNULL(this)
  return this->local;
}

Ltype *fclass_generics(Fclass *this) {
  XNULL(this)
  return this->generics;
}

Hchar *fclass_imports(Fclass *this) {
  XNULL(this)
  return this->imports;
}

Otype *fclass_extends(Fclass *this) {
  XNULL(this)
  return this->extends;
}

Ltype *fclass_implements(Fclass *this) {
  XNULL(this)
  return this->implements;
}

Lcatt *fclass_atts(Fclass *this) {
  XNULL(this)
  return this->atts;
}
/*.-.*/

inline
Fclass *fclass_new(
  char *id,
  bool local,
  Ltype *generics,
  Hchar *imports,
  Otype *extends,
  Ltype *implements
) {
  return _fclass_new(
    id, local, generics, imports, extends, implements, lcatt_new()
  );
}
