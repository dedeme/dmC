// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Extends / imports entry

#ifndef AST_TP_STR_TYPE_H
  #define AST_TP_STR_TYPE_H

#include "dmc/all.h"
#include "types/Type.h"

/*.-.*/

///
typedef struct tp_str_type_Tp_str_type Tp_str_type;

///
Tp_str_type *tp_str_type_new(char *s, Type *t);

///
char *tp_str_type_s(Tp_str_type *this);

///
Type *tp_str_type_t(Tp_str_type *this);

///
Json *tp_str_type_serialize(Tp_str_type *this);

///
Tp_str_type *tp_str_type_restore(Json *s);

/*.-.*/

#endif
