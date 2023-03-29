// Copyright 24-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constructors of built-in objects.

#ifndef OBJ_H
  #define OBJ_H

#include "kut/it.h"
#include "module.h"
#include "bfunction.h"

/// Creates an expression of the indicated object.
Exp *obj_module (Module *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Module *obj_get_module (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_module (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_bmodule (char *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
char *obj_get_bmodule (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_bmodule (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_bfunction (Bfunction value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
Bfunction obj_get_bfunction (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_bfunction (Exp *this);

/// Creates an expression of the indicated object.
Exp *obj_iter (It *value);

/// Read an Exp of the indicate object.
/// Throws EXC_ILLEGAL_AGUMENT if 'this' is not of such type.
It *obj_get_iter (Exp *this);

/// Returns TRUE if 'this' match the object type.
int obj_is_iter (Exp *this);


#endif
