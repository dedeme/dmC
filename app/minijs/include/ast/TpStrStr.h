// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Import entry

#ifndef AST_TP_STR_STR_H
  #define AST_TP_STR_STR_H

#include "dmc/all.h"

/*.-.*/

///
typedef struct tp_str_str_Tp_str_str Tp_str_str;

///
Tp_str_str *tp_str_str_new(char *s1, char *s2);

///
char *tp_str_str_s1(Tp_str_str *this);

///
char *tp_str_str_s2(Tp_str_str *this);

///
Json *tp_str_str_serialize(Tp_str_str *this);

///
Tp_str_str *tp_str_str_restore(Json *s);

/*.-.*/

#endif
