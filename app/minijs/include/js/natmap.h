// Copyright 23-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Native map

#ifndef JS_NATMAP_H
  # define JS_NATMAP_H

#include "parser/Final_class.h"
#include "parser/Obj.h"

/// natmap_static verifies if id is valid. If it is not, throw a tx_exception.
Obj *natmap_static(Final_class *class, Pos *pos, char *id);

/// natmap_instance verifies if id is valid. If it is not, throw a tx_exception.
Obj *natmap_instance(Final_class *class, Pos *pos, char *id);

#endif
