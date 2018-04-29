// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native Int

#ifndef JS_NATINT_H
  # define JS_NATINT_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natint_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natint_static(Final_class *class, Pos *pos, char *id);

/// natint_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natint_instance(Final_class *class, Pos *pos, char *id);

#endif
