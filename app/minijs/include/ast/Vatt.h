// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Value attatchment

#ifndef AST_VATT_H
  # define AST_VATT_H

#include <dmc/all.h>

typedef Arr Avalue;

/*.-.*/

///
typedef struct vatt_Vatt Vatt;

///
bool vatt_is_fn(Vatt *this);

///
char *vatt_id(Vatt *this);

///
Avalue *vatt_params(Vatt *this);

///
Json *vatt_serialize(Vatt *this);

///
Vatt *vatt_restore(Json *s);

/*.-.*/

///
Vatt *vatt_new_id(char *id);

///
Vatt *vatt_new_fn(Avalue *vs);

#endif

