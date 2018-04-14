// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Final class

#ifndef PARSER_FINAL_CLASS_H
  # define PARSER_FINAL_CLASS_H

#include <dmc/all.h>
#include "Cpath.h"
#include "Obj.h"

/*.-.*/

///
typedef struct final_class_Final_class Final_class;

///
Cpath *final_class_cpath(Final_class *this);

///
bool final_class_public(Final_class *this);

/*.-.*/

/// private true if path can be called although it is local
Final_class *final_class_new(bool local, Cpath *path);

///
Obj *final_class_object_type(Final_class *this, char *object);

#endif
