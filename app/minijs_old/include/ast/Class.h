// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Ast class model

#ifndef AST_CLASS_H
  #define AST_CLASS_H

#include "dmc/all.h"
#include "Cpath.h"
#include "ast/Import.h"
#include "ast/Implement.h"
#include "ast/Cvalue.h"

/*.-.*/

///
typedef struct class_Class Class;

///
Cpath *class_cpath(Class *this);

///
bool class_public(Class *this);

///
void class_set_public(Class *this, bool value);

///
Arr *class_generics(Class *this);

///
void class_set_generics(Class *this, Arr *value);

///
Arr *class_imports(Class *this);

///
void class_set_imports(Class *this, Arr *value);

///
Arr *class_implements(Class *this);

///
void class_set_implements(Class *this, Arr *value);

///
Json *class_serialize(Class *this);

///
Class *class_restore(Json *s);

/*.-.*/

///
Class *class_mk(Cpath *cpath);

/// If 'this' has not extend, the Implement returned has a blank ("") as
/// 'implement_path()'.
Implement *class_extends(Class *this);

/// If 'value' has not extend, it has a blank ("") as 'implement_path()'.
void class_set_extends(Class *this, Implement *value);

///
Arr/*Cvalue*/ *class_cvalues(Class *this);

///
void class_add_cvalue(Class *this, Cvalue *cval);

#endif
