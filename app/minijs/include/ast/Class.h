// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Mini class

#ifndef AST_CLASS_H
  # define AST_CLASS_H

#include "Achar.h"
#include "Aachar.h"
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
Achar *class_generics(Class *this);

///
Mchar *class_imports(Class *this);

///
Type *class_super(Class *this);

///
void class_set_super(Class *this, Type *value);

///
Atype *class_implements(Class *this);

///
Aatt *class_statics(Class *this);

///
Aatt *class_instance(Class *this);

///
Achar *class_used(Class *this);

///
Arr/*Json*/ *class_serialize(Class *this);

///
Class *class_restore(Arr/*Json*/ *s);

/*.-.*/

///
Class *class_new(char *id);

/// class_short_id returns the last part of class_id
char *class_short_id(Class *this);

/// class__type returns null if arr_size(generics) is different to this.generics
/// size.
Type *class__type(Class *this, Atype *generics);

/// class_contains_type returns true if id is a generic o an imported class.
bool class_contains_type(Class *this, char *id);

/// class_contains_att returns true if id is an attribute public or private.
bool class_contains_att(Class *this, char *id);

/// class_contains_id is equals to class_contains_type || class_contains_att
bool class_contains_id(Class *this, char *id);

/// class_resolve_type returns actual type of 'tp'.
/// Throws tx_exception.
Type *class_actual_type(Class *this, Type *tp, Pos *pos);

/// class_instance_all returns everay attribute of 'this' and its superclasses.
/// Attributes of 'this' are before those of its superclass.<p>
/// 'this' must be added to Program.
Aatt *class_instance_all(Class *this, Atype *generics);

#endif

