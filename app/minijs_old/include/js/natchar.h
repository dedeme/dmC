// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native Char

#ifndef JS_NATCHAR_H
  # define JS_NATCHAR_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natchar_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natchar_static(Final_class *class, Pos *pos, char *id);

/// natchar_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natchar_instance(Final_class *class, Pos *pos, char *id);

#endif
