// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native Float

#ifndef JS_NATFLOAT_H
  # define JS_NATFLOAT_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natfloat_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natfloat_static(Final_class *class, Pos *pos, char *id);

/// natfloat_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natfloat_instance(Final_class *class, Pos *pos, char *id);

#endif
