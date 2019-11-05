// Copyright 29-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef EXCEPTION_H
  #define EXCEPTION_H

#include "dmc/async.h"
#include "Machine.h"

///
void exception_throw(Machine *m, char *type, char *msg);

///
char *exception_type(Exc *ex);

///
char *exception_msg(Exc *ex);

///
char *exception_stack(Exc *ex);

#endif
