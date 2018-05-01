// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array with program classes

#ifndef AST_PROGRAM_H
  # define AST_PROGRAM_H

#include <dmc/all.h>
#include "Class.h"

///
typedef Arr Program;

///
Program *program_get(void);

///
void program_add(Program *this, Class *class);

/// program_class returns the corresponding class or NULL
Class *program__class(Program *this, char *id);

#endif


