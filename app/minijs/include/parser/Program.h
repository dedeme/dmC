// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program model

#ifndef PARSER_PROGRAM_H
  #define PARSER_PROGRAM_H

#include <dmc/all.h>
#include "Cpath.h"

/*.-.*/

///
typedef struct program_Program Program;

///
Program *program_new(Cpath *cpath);

///
Cpath *program_cpath(Program *this);

/*.-.*/

///
void program_add_class(Program *this, char *path);

///
void program_print(Program *this, char *js);

#endif
