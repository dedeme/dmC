// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Mini class

#ifndef AST_CLASS_H
  # define AST_CLASS_H

#include "Achar.h"
#include "Mchar.h"
#include "Atype.h"
#include "Aatt.h"

/*.-.*/

///
typedef struct class_Class Class;

///
char *class_id(Class *this);

///
bool class_local(Class *this);

///
void class_set_local(Class *this, bool value);

///
char *class_super(Class *this);

///
void class_set_super(Class *this, char *value);

///
Achar *class_generics(Class *this);

///
Mchar *class_imports(Class *this);

///
Aatt *class_statics(Class *this);

///
Aatt *class_instance(Class *this);

///
Json *class_serialize(Class *this);

///
Class *class_restore(Json *s);

/*.-.*/

///
Class *class_new(char *id);

/// class__type returns null if arr_size(generics) is different to this.generics
/// size.
Type *class__type(Class *this, Atype *generics);

#endif

