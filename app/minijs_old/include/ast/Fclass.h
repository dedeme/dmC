// Copyright 17-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Class with defined symbols

#ifndef AST_FCLASS_H
  # define AST_FCLASS_H

#include <stdbool.h>

typedef struct ltype_Ltype Ltype;
typedef struct hchar_Hchar Hchar;
typedef struct otype_Otype Otype;
typedef struct lcatt_Lcatt Lcatt;

/*.-.*/

///
typedef struct fclass_Fclass Fclass;

///
char *fclass_id(Fclass *this);

///
bool fclass_local(Fclass *this);

///
Ltype *fclass_generics(Fclass *this);

///
Hchar *fclass_imports(Fclass *this);

///
Otype *fclass_extends(Fclass *this);

///
Ltype *fclass_implements(Fclass *this);

///
Lcatt *fclass_atts(Fclass *this);

/*.-.*/

///
Fclass *fclass_new(
  char *id,
  bool local,
  Ltype *generics,
  Hchar *imports,
  Otype *extends,
  Ltype *implements
);

#endif


