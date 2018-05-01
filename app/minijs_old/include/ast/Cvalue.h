// Copyright 26-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// AST class value declaration model

#ifndef AST_CVALUE_H
  #define AST_CVALUE_H

#include <dmc/all.h>
#include "ast/Dvalue.h"
#include "DEFS.h"

/*.-.*/

///
typedef struct cvalue_Cvalue Cvalue;

///
Pos *cvalue_pos(Cvalue *this);

///
bool cvalue_is_public(Cvalue *this);

///
bool cvalue_is_static(Cvalue *this);

///
enum Cvalue_t cvalue_type(Cvalue *this);

///
Dvalue *cvalue_dvalue(Cvalue *this);

///
Json *cvalue_serialize(Cvalue *this);

///
Cvalue *cvalue_restore(Json *s);

/*.-.*/

///
Cvalue *cvalue_new_val(
  Pos *pos,
  bool is_public,
  bool is_static,
  Dvalue *dvalue
);

///
Cvalue *cvalue_new_var(
  Pos *pos,
  bool is_public,
  bool is_static,
  Dvalue *dvalue
);

///
char *cvalue_id(Cvalue *this);

#endif
