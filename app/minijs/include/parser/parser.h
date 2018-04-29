// Copyright 13-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Parser

#ifndef PARSER_PARSER_H
  # define PARSER_PARSER_H

#include <dmc/all.h>
#include "Cpath.h"
#include "Obj.h"
#include "Lobj.h"
#include "Final_class.h"
#include "ast/Dvalue.h"

/// parser_class_obj
///   pack: Pack of calling class
///   path: Id of class
///   object: Property or method of path
/// This function fail with a tx_excetion if Obj can not be found.
Obj *parser_class_obj(char *pack, Cpath *path, char *object);

/// parser_value_obj
///   class: Class from which call is made.
///   objs: Registred Lobjs
///   tp: Expected type of 'Obj'. If it is not a function, it can be unknown.
///   v: Value to control.
///   return: Can be null if 'tp' is a function which returns void.
/// This function fail with a tx_exception if Type can not be created or not
/// is a subclass of 'tp'.
Type *parser_value_type(Final_class *class, Lobjs *objs, Type *tp, Value *v);

///  parser_block
///   class: Class from which call is made.
///   objs: Registred Lobjs
///   tp: Expected type of 'Obj'. It can not be unknown, although it can be NULL
///   stats: 'Stat's of block.
/// This function fail with a tx_exception if a Stat is not valid.
void parser_block(
  Final_class *class, Lobjs *objs, Type *tp, Arr/*Stat*/ *stats
);

#endif
