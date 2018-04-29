// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native Bool

#ifndef JS_NATBOOL_H
  # define JS_NATBOOL_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natbool_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natbool_static(Final_class *class, Pos *pos, char *id);

/// natbool_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natbool_instance(Final_class *class, Pos *pos, char *id);

#endif
