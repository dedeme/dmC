// Copyright 10-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Cvalue attribute

#ifndef AST_VATT_H
  # define AST_VATT_H

typedef struct pos_Pos Pos;
typedef struct lvalue_Lvalue Lvalue;

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct vatt_Vatt Vatt;

///
bool vatt_is_fn(Vatt *this);

///
Pos *vatt_pos(Vatt *this);

///
char *vatt_id(Vatt *this);

///
Lvalue *vatt_params(Vatt *this);

///
Ajson *vatt_to_json(Vatt *this);

///
Vatt *vatt_from_json(Ajson *s);

/*.-.*/

///
Vatt *vatt_new_id(Pos *pos, char *id);

///
Vatt *vatt_new_fn(Pos *pos, Lvalue *vs);

#endif
