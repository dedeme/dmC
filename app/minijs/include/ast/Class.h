// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Ast class model

#ifndef AST_CLASS_H
  #define AST_CLASS_H

#include "dmc/all.h"
#include "Cpath.h"
#include "ast/Cvalue.h"

/*.-.*/

///
typedef struct class_Class Class;

///
Cpath *class_cpath(Class *this);

///
Arr *class_generics(Class *this);

///
Arr *class_cvalues(Class *this);

///
Json *class_serialize(Class *this);

///
Class *class_restore(Json *s);

/*.-.*/

Class *class_mk(Cpath *cpath);

void class_add_cvalue(Class *this, Cvalue *cval);

#endif
