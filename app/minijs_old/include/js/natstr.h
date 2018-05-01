// Copyright 25-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native Str

#ifndef JS_NATSTR_H
  # define JS_NATSTR_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natstr_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natstr_static(Final_class *class, Pos *pos, char *id);

/// natstr_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natstr_instance(Final_class *class, Pos *pos, char *id);

#endif
