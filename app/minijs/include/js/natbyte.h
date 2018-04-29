// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native Byte

#ifndef JS_NATBYTE_H
  # define JS_NATBYTE_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natbyte_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natbyte_static(Final_class *class, Pos *pos, char *id);

/// natbyte_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natbyte_instance(Final_class *class, Pos *pos, char *id);

#endif
