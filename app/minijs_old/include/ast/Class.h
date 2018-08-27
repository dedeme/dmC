// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Class with no defined symbols

#ifndef AST_CLASS_H
  # define AST_CLASS_H

#include <stdbool.h>

typedef struct type_Type Type;
typedef struct ltype_Ltype Ltype;
typedef struct otype_Otype Otype;
typedef struct hchar_Hchar Hchar;
typedef struct catt_Catt Catt;
typedef struct lcatt_Lcatt Lcatt;
typedef struct lcsym_Lcsym Lcsym;

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct class_Class Class;

///
Class *class_new(
  char *id,
  bool local,
  Ltype *generics,
  Hchar *imports,
  Otype *extends,
  Ltype *implements,
  Lcatt *atts
);

///
char *class_id(Class *this);

///
bool class_local(Class *this);

///
Ltype *class_generics(Class *this);

///
Hchar *class_imports(Class *this);

///
Otype *class_extends(Class *this);

///
Ltype *class_implements(Class *this);

///
Lcatt *class_atts(Class *this);

///
Ajson *class_to_json(Class *this);

///
Class *class_from_json(Ajson *s);

/*.-.*/

/// class_syms must be called after parsing 'this' and testing that super's
/// chain is not cyclic.
Lcsym *class_syms(Class *this);

#endif
