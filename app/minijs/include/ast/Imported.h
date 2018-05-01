// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array with imported classes

#ifndef AST_IMPORTED_H
  # define AST_IMPORTED_H

#include <dmc/all.h>
#include "Class.h"

/*.-.*/

///
typedef struct imported_Imported Imported;

/*.-.*/

///
Imported *imported_get(void);

///
void imported_add(Imported *this, Class *class);

/// imported_class returns the corresponding class or NULL
Class *imported__class(Imported *this, char *id);

#endif


