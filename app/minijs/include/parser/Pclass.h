// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program class

#ifndef PARSER_CLASS_H
  #define PARSER_CLASS_H

#include "dmc/all.h"

/*.-.*/

///
typedef struct pclass_Pclass Pclass;

///
Pclass *pclass_new(char *path);

///
char *pclass_path(Pclass *this);

/*.-.*/

///
char *pclass_str(Pclass *this);

#endif
