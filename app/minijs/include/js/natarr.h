// Copyright 23-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native array

#ifndef JS_NATARR_H
  # define JS_NATARR_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natarr_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natarr_static(Final_class *class, Pos *pos, char *id);

/// natarr_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natarr_instance(Final_class *class, Pos *pos, char *id);

#endif
